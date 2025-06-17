#include "plant_simulation.h"


// 季节对天气的影响概率[季节][天气]
const uint8_t season_weather_probs[4][WEATHER_TYPES_COUNT] = {
    // 春季：多云为主，有雨
    {20, 40, 30, 0, 10},   // 晴朗, 多云, 下雨, 下雪, 大风
    
    // 夏季：晴朗为主，有雨
    {40, 20, 30, 0, 10},   // 晴朗, 多云, 下雨, 下雪, 大风
    
    // 秋季：晴朗/多云，大风
    {30, 30, 10, 0, 30},   // 晴朗, 多云, 下雨, 下雪, 大风
    
    // 冬季：多云/下雪
    {10, 30, 0, 50, 10}    // 晴朗, 多云, 下雨, 下雪, 大风
};



// 植物生长阶段的基础持续时间（天）
const uint8_t stage_durations[STAGE_COUNT] = {
    [SEED] = 3,      // 种子阶段
    [SPROUT] = 5,    // 发芽阶段
    [SEEDLING] = 7,  // 幼苗阶段
    [MATURE] = 10,   // 成熟阶段
    [FLOWERING] = 8, // 开花阶段
    [FRUITING] = 15  // 结果阶段
};

// 植物环境偏好（最小/最大适宜值）
const uint8_t plant_prefs[PLANT_TYPES_COUNT][3][2] = {
    // 温度, 湿度, 光照
    [SUNFLOWER] = {{20, 30}, {40, 70}, {70, 90}},   // 向日葵
    [TOMATO]    = {{18, 28}, {60, 80}, {60, 85}},   // 番茄
    [CACTUS]    = {{25, 40}, {10, 30}, {80, 100}},  // 仙人掌
    [CANNIBAL_FLOWERS] = {{22, 28}, {75, 95}, {40, 60}} // 食人花
};

// 字符串数组
const char* weather_str[] = {"Clear", "Cloudy", "Rainy", "Snowy", "Windy"};
const char* season_str[] = {"Spring", "Summer", "Autumn", "Winter"};
const char* type_str[] = {"Sunflower", "Tomato", "Cactus", "Cannibal Flowers"};
const char* stage_str[] = {"Seed", "Sprout", "Seedling", "Mature", "Flowering", "Fruiting"};


// 天气对环境的影响
void apply_weather_effects(Environment* env) {
    switch (env->weather) {
        case WEATHER_CLEAR:
            env->light = (env->light + 20 > 100) ? 100 : env->light + 20;
            env->humidity = (env->humidity < 20) ? 0 : env->humidity - 15;
            break;
        case WEATHER_CLOUDY:
            env->light = (env->light < 30) ? 0 : env->light - 20;
            break;
        case WEATHER_RAINY:
            env->humidity = (env->humidity + 30 > 100) ? 100 : env->humidity + 30;
            env->light = (env->light < 40) ? 0 : env->light - 30;
            env->temperature = (env->temperature < 5) ? 0 : env->temperature - 3;
            break;
        case WEATHER_SNOWY:
            env->temperature = (env->temperature < 10) ? 0 : env->temperature - 8;
            env->humidity = (env->humidity + 10 > 100) ? 100 : env->humidity + 10;
            break;
        case WEATHER_WINDY:
            env->humidity = (env->humidity < 25) ? 0 : env->humidity - 20;
            env->temperature = (env->temperature < 3) ? 0 : env->temperature - 2;
            break;
        default:
            break;
    }
}

// 生成当天的环境
void generate_environment(Environment* env) {
    // 季节影响天气
    uint8_t r = rand() % 100;
    uint8_t acc = 0;
    for (int i = 0; i < WEATHER_TYPES_COUNT; i++) {
        acc += season_weather_probs[env->season][i];
        if (r < acc) {
            env->weather = (WeatherType)i;
            break;
        }
    }
    
    // 基础环境值（受季节影响）
    switch (env->season) {
        case 0: // 春
            env->temperature = 15 + rand() % 15;
            env->humidity = 50 + rand() % 40;
            env->light = 60 + rand() % 30;
            break;
        case 1: // 夏
            env->temperature = 25 + rand() % 20;
            env->humidity = 40 + rand() % 40;
            env->light = 70 + rand() % 30;
            break;
        case 2: // 秋
            env->temperature = 10 + rand() % 15;
            env->humidity = 30 + rand() % 40;
            env->light = 50 + rand() % 30;
            break;
        case 3: // 冬
            env->temperature = rand() % 10;
            env->humidity = 40 + rand() % 30;
            env->light = 40 + rand() % 30;
            break;
    }
    
    // 应用天气效果
    apply_weather_effects(env);
    
    // 确保值在合理范围内
    env->temperature = (env->temperature > 50) ? 50 : env->temperature;
    env->humidity = (env->humidity > 100) ? 100 : env->humidity;
    env->light = (env->light > 100) ? 100 : env->light;
    
    env->day++;
}

// 打印环境信息
void print_environment(const Environment* env) {
    const char* weather_str[] = {"Clear", "Cloudy", "Rainy", "Snowy", "Windy"};
    const char* season_str[] = {"Spring", "Summer", "Autumn", "Winter"};
    
    printf("Day %u [%s]\n", env->day, season_str[env->season]);
    printf("Weather: %s\n", weather_str[env->weather]);
    printf("Temp: %d°C, Humidity: %d%%, Light: %d%%\n", 
           env->temperature, env->humidity, env->light);
}

// 初始化植物
void init_plant(PlantState* plant, PlantType type, uint8_t rarity) {
    plant->type = type;
    plant->rarity = rarity;
    plant->stage = SEED;
    plant->health = 80;
    plant->age = 0;
    plant->growth = 0;
    plant->needs_water = false;
    plant->needs_fertilizer = false;
    plant->needs_pest_control = false;
    plant->water_count = 0;
    plant->fertilize_count = 0;
    plant->pest_control_count = 0;
}

// 更新植物健康状态（环境因素）
void update_plant_health(PlantState* plant, const Environment* env) {
    int8_t health_change = 0;
    
    // 检查温度适宜度
    if (env->temperature < plant_prefs[plant->type][0][0]) {
        health_change -= 2; // 太冷
    } else if (env->temperature > plant_prefs[plant->type][0][1]) {
        health_change -= 2; // 太热
    } else {
        health_change += 1; // 适宜
    }
    
    // 检查湿度适宜度
    if (env->humidity < plant_prefs[plant->type][1][0]) {
        health_change -= 2; // 太干
        plant->needs_water = true;
    } else if (env->humidity > plant_prefs[plant->type][1][1]) {
        health_change -= 1; // 太湿
    } else {
        health_change += 1; // 适宜
    }
    
    // 检查光照适宜度
    if (env->light < plant_prefs[plant->type][2][0]) {
        health_change -= 2; // 太暗
    } else if (env->light > plant_prefs[plant->type][2][1]) {
        health_change -= 1; // 太亮
    } else {
        health_change += 1; // 适宜
    }
    
    // 应用健康变化（用户操作影响更大）
    plant->health = (int)plant->health + health_change < 0 ? 0 : 
                   (plant->health + health_change > 100 ? 100 : plant->health + health_change);
    
    // 健康状态低于阈值时设置提醒
    if (plant->health < 40) {
        plant->needs_fertilizer = true;
    }
    if (plant->health < 30) {
        plant->needs_pest_control = true;
    }
}

// 更新植物生长阶段
void update_plant_growth(PlantState* plant) {
    // 健康状态影响生长速度
    uint8_t growth_rate;
    if (plant->health > 70) {
        growth_rate = 120; // 健康状态良好，生长加速
    } else if (plant->health > 40) {
        growth_rate = 100; // 正常生长
    } else if (plant->health > 20) {
        growth_rate = 60;  // 生长减缓
    } else {
        growth_rate = 30;  // 严重减缓
    }
    
    // 计算生长增量（基于阶段持续时间和健康状态）
    uint8_t stage_duration = stage_durations[plant->stage];
    uint8_t growth_increment = (100 * growth_rate) / (stage_duration * 100);
    
    // 更新生长进度
    plant->growth += growth_increment;
    plant->age++;
    
    // 确保生长进度不超过100%
    if (plant->growth > 100) {
        plant->growth = 100;
    }
    
    // 检查阶段升级
    if (plant->growth >= 100 && plant->stage < FRUITING) {
        plant->stage++;
        plant->growth = 0;
        printf("\n>>> Plant advanced to %s stage! <<<\n", stage_str[plant->stage]);
    }
}

// 用户操作：除虫
void pest_control(PlantState* plant) {
    if (plant->pest_control_count >= 1) {
        plant->health = (plant->health < 8) ? 0 : plant->health - 8;
        printf("Warning: Excessive pest control! Health -8\n");
        return;
    }
    
    plant->health = (plant->health + 15 > 100) ? 100 : plant->health + 15;
    plant->needs_pest_control = false;
    plant->pest_control_count++;
    printf("Applied pest control. Health +15\n");
}

// 用户操作：施肥
void fertilize(PlantState* plant) {
    if (plant->fertilize_count >= 1) {
        plant->health = (plant->health < 8) ? 0 : plant->health - 8;
        printf("Warning: Excessive fertilization! Health -8\n");
        return;
    }
    
    plant->health = (plant->health + 20 > 100) ? 100 : plant->health + 20;
    plant->growth += 10;
    if (plant->growth > 100) plant->growth = 100;
    plant->needs_fertilizer = false;
    plant->fertilize_count++;
    printf("Applied fertilizer. Health +20, Growth +10%%\n");
}

// 用户操作：浇水
void watering(PlantState* plant) {
    if (plant->water_count >= 2) {
        plant->health = (plant->health < 5) ? 0 : plant->health - 5;
        printf("Warning: Excessive watering! Health -5\n");
        return;
    }
    
    plant->health = (plant->health + 15 > 100) ? 100 : plant->health + 15;
    plant->needs_water = false;
    plant->water_count++;
    printf("Watered plant. Health +15\n");
}

// 获取提醒信息
const char* get_plant_needs(const PlantState* plant) {
    if (plant->needs_pest_control) return "Needs pest control!";
    if (plant->needs_fertilizer) return "Needs fertilizer!";
    if (plant->needs_water) return "Needs watering!";
    return "No urgent needs";
}

// 打印植物状态
void print_plant_state(const PlantState* plant) {
    printf("\nPlant: %s [Rarity: %u]\n", type_str[plant->type], plant->rarity);
    printf("Stage: %s (%u%%)\n", stage_str[plant->stage], plant->growth);
    printf("Health: %u/100, Age: %u days\n", plant->health, plant->age);
    printf("Status: %s\n", get_plant_needs(plant));
    printf("Actions today: Water(%d/2), Fertilize(%d/1), Pest Control(%d/1)\n\n",
           plant->water_count, plant->fertilize_count, plant->pest_control_count);
}

// 模拟一天的变化
void simulate_day(PlantState* plant, Environment* env) {
    // 重置操作计数
    plant->water_count = 0;
    plant->fertilize_count = 0;
    plant->pest_control_count = 0;
    
    // 生成新环境
    generate_environment(env);
    
    // 更新植物健康（环境因素）
    update_plant_health(plant, env);
    
    // 更新植物生长
    if (plant->health > 10) { // 健康状态过低时停止生长
        update_plant_growth(plant);
    }
    
    // 季节循环（每90天换季）
    if (env->day % 90 == 0 && env->day > 0) {
        env->season = (env->season + 1) % 4;
        printf("\n=== Season changed to %s! ===\n", season_str[env->season]);
    }
    //ui_event_temperaturelabel1(NULL);
}

#include <stdbool.h>

void buy_plant(User* user, Commodity* commodity, PlantType plant_type) {
    // 1. 检查用户植物数量是否已达上限
    if (user->plant_num >= 4) {
        // 植物栏位已满，无法购买
        return;
    }
    
    // 2. 在商品中查找匹配的植物类型
    int found_index = -1;
    for (int i = 0; i < 4; i++) {
        if (commodity->plant_type[i] == plant_type && commodity->ishave[i]) {
            found_index = i;
            break;
        }
    }
    
    // 3. 检查是否找到可购买的商品
    if (found_index == -1) {
        // 未找到可购买的指定植物
        printf("未找到可购买的指定植物\n");
        return;
    }
    
    // 4. 检查用户金币是否足够
    uint16_t price = commodity->price[found_index];
    if (user->coins < price) {
        // 金币不足，无法购买
        printf("金币不足，无法购买\n");
        return;
    }
    
    // 5. 执行购买操作
    // 扣除金币
    user->coins -= price;
    
    // 标记商品为已购买
    commodity->ishave[found_index] = false;
    
    // 添加新植物到用户植物栏
    int plant_index = user->plant_num;
    user->plant[plant_index].type = plant_type;
    user->plant[plant_index].rarity = 1;  // 默认稀有度为1
    user->plant[plant_index].stage = SEEDLING; // 初始为幼苗阶段
    user->plant[plant_index].health = 100; // 初始健康值100
    user->plant[plant_index].age = 0;      // 初始年龄0天
    user->plant[plant_index].growth = 0;   // 初始生长进度0%
    
    // 增加用户植物数量
    user->plant_num++;
}

// 从文件加载用户数据
void load_user(User* user) {
    FILE* file = fopen("user.txt", "r");
    if (!file) return;

    fscanf(file, "%u", &user->coins);
    fscanf(file, "%hhu", &user->plant_num);
    
    // for (int i = 0; i < user->plant_num; i++) {
    //     int type, stage;
    //     fscanf(file, "%d", &type);
    //     fscanf(file, "%hhu", &user->plant[i].rarity);
    //     fscanf(file, "%d", &stage);
    //     fscanf(file, "%hhu", &user->plant[i].health);
    //     fscanf(file, "%u", &user->plant[i].age);
    //     fscanf(file, "%hhu", &user->plant[i].growth);
        
    //     user->plant[i].type = (PlantType)type;
    //     user->plant[i].stage = (GrowthStage)stage;
    // }
    fclose(file);
}

// 从文件加载商品数据
void load_commodity(Commodity* commodity) {
    FILE* file = fopen("commodity.txt", "r");
    if (!file) return;

    for (int i = 0; i < 4; i++) {
        int type, have;
        fscanf(file, "%d", &type);
        fscanf(file, "%hu", &commodity->price[i]);
        fscanf(file, "%d", &have);
        
        commodity->plant_type[i] = (PlantType)type;
        commodity->ishave[i] = (bool)have;
    }
    fclose(file);
}

// 从文件加载环境数据
void load_environment(Environment* env) {
    FILE* file = fopen("environment.txt", "r");
    if (!file) return;

    int weather;
    fscanf(file, "%hhu", &env->temperature);
    fscanf(file, "%hhu", &env->humidity);
    fscanf(file, "%hhu", &env->light);
    fscanf(file, "%hhu", &env->season);
    fscanf(file, "%u", &env->day);
    fscanf(file, "%d", &weather);
    
    env->weather = (WeatherType)weather;
    fclose(file);
}

// 从文件加载单个植物数据
void load_plant(PlantState* plant) {
    FILE* file;
    
    switch(plant->type){
        case 0: file = fopen("plant.txt", "w");break;
        case 1: file = fopen("tomato.txt", "w");break;
        case 2: file = fopen("cactus.txt", "w");break;
        case 3: file = fopen("cannibal.txt", "w");break;
        default: break;
    }
    if (!file) return;

    int type, stage;
    fscanf(file, "%d", &type);
    fscanf(file, "%hhu", &plant->rarity);
    fscanf(file, "%d", &stage);
    fscanf(file, "%hhu", &plant->health);
    fscanf(file, "%u", &plant->age);
    fscanf(file, "%hhu", &plant->growth);
    
    plant->type = (PlantType)type;
    plant->stage = (GrowthStage)stage;
    fclose(file);
}

//保存信息到文件
void save_user(User* user){
    
    // 打开文件以写入文本数据
    FILE* file = fopen("user.txt", "w");
    if (file == NULL) {
        // 若文件打开失败，输出错误信息并终止程序
        perror("无法打开文件");
        return;
    }

    if (fprintf(file, "%d\n", user->coins) < 0 ||
        fprintf(file, "%d\n", user->plant_num) < 0) {
        // 若写入失败，输出错误信息
        perror("写入文件失败");
        fclose(file);
        return;
    }
    

    // 关闭文件
    if (fclose(file) != 0) {
        // 若关闭文件失败，输出错误信息
        perror("关闭文件失败");
        return;
    }
    printf("user finish\n");
}
void save_commodity(Commodity* commodity){
    
    // 打开文件以写入文本数据
    FILE* file = fopen("commodity.txt", "w");
    if (file == NULL) {
        // 若文件打开失败，输出错误信息并终止程序
        perror("无法打开文件");
        return;
    }

    for(int i = 0; i < 4; i++) {
        fprintf(file, "%d\n", commodity->plant_type[i]);
        fprintf(file, "%d\n", commodity->price[i]);
        fprintf(file, "%d\n", commodity->ishave[i]);

    }
    
    

    // 关闭文件
    if (fclose(file) != 0) {
        // 若关闭文件失败，输出错误信息
        perror("关闭文件失败");
        return;
    }
}
void save_environment(Environment* env){
    // 打开文件以写入文本数据
    FILE* file = fopen("environment.txt", "w");
    if (file == NULL) {
        // 若文件打开失败，输出错误信息并终止程序
        perror("无法打开文件");
        return;
    }

    if(
        fprintf(file, "%d\n", env->temperature)<0 ||
        fprintf(file, "%d\n", env->humidity)<0 ||
        fprintf(file, "%d\n", env->light)<0 ||
        fprintf(file, "%d\n", env->season)<0 ||
        fprintf(file, "%d\n", env->day)<0 ||
        fprintf(file, "%d\n", env->weather)<0
    )
    // 关闭文件
    if (fclose(file) != 0) {
        // 若关闭文件失败，输出错误信息
        perror("关闭文件失败");
        return;
    }
}


void save_plant(PlantState* plant){
    // 打开文件以写入文本数据
    FILE* file;
    
    switch(plant->type){
        case 0: file = fopen("plant.txt", "w");break;
        case 1: file = fopen("tomato.txt", "w");break;
        case 2: file = fopen("cactus.txt", "w");break;
        case 3: file = fopen("cannibal.txt", "w");break;
        default: break;
    }

    if (file == NULL) {
        // 若文件打开失败，输出错误信息并终止程序
        perror("无法打开文件");
        return;
    }

    if(
        fprintf(file, "%d\n", plant->type)<0 ||
        fprintf(file, "%d\n", plant->rarity)<0 ||
        fprintf(file, "%d\n", plant->stage)<0 ||
        fprintf(file, "%d\n", plant->health)<0 ||
        fprintf(file, "%d\n", plant->age)<0 ||
        fprintf(file, "%d\n", plant->growth)<0
    )
    // 关闭文件
    if (fclose(file) != 0) {
        // 若关闭文件失败，输出错误信息
        perror("关闭文件失败");
        return;
    }
}