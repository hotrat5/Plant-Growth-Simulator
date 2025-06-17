#ifndef PLANT_SIMULATION_H
#define PLANT_SIMULATION_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// 天气类型枚举
typedef enum {
    WEATHER_CLEAR,      // 晴朗
    WEATHER_CLOUDY,     // 多云
    WEATHER_RAINY,      // 下雨
    WEATHER_SNOWY,      // 下雪
    WEATHER_WINDY,      // 大风
    WEATHER_TYPES_COUNT // 天气类型总数
} WeatherType;

// 植物类型定义
typedef enum {
    SUNFLOWER,  // 向日葵
    TOMATO,     // 番茄
    CACTUS,     // 仙人掌
    CANNIBAL_FLOWERS,  // 食人花
    PLANT_TYPES_COUNT
} PlantType;

// 植物生长阶段
typedef enum {
    SEED,         // 种子
    SPROUT,       // 发芽
    SEEDLING,     // 幼苗
    MATURE,       // 成熟
    FLOWERING,    // 开花
    FRUITING,     // 结果
    STAGE_COUNT
} GrowthStage;

// 环境参数结构
typedef struct {
    uint8_t temperature;  // 温度 (℃)
    uint8_t humidity;     // 水分含量 (0-100)
    uint8_t light;        // 光照强度 (0-100)
    uint8_t season;       // 季节（0:春, 1:夏, 2:秋, 3:冬）
    uint32_t day;         // 当前天数
    WeatherType weather;  // 天气类型
} Environment;

// 植物状态结构
typedef struct {
    PlantType type;         // 植物种类
    uint8_t rarity;         // 植物稀有度[1->3]
    GrowthStage stage;      // 生长阶段
    uint8_t health;         // 健康状态 (0-100)
    uint32_t age;           // 植物年龄 (天)
    uint8_t growth;         // 当前阶段生长进度 (0-100)
    bool needs_water;       // 需要浇水
    bool needs_fertilizer;  // 需要施肥
    bool needs_pest_control;// 需要除虫
    uint8_t water_count;      // 当天浇水次数
    uint8_t fertilize_count;  // 当天施肥次数
    uint8_t pest_control_count; // 当天除虫次数
} PlantState;

//用户
typedef struct{
    uint32_t coins;
    uint8_t plant_num;
    PlantState plant[4];
}User;

//商品
typedef struct{
    PlantType plant_type[4];
    uint16_t price[4];
    bool ishave[4];
}Commodity;

// 常量定义
extern const uint8_t stage_durations[STAGE_COUNT];
extern const uint8_t plant_prefs[PLANT_TYPES_COUNT][3][2];
extern const uint8_t season_weather_probs[4][WEATHER_TYPES_COUNT];
extern const char* weather_str[];
extern const char* season_str[];
extern const char* type_str[];
extern const char* stage_str[];

// 环境相关函数
void apply_weather_effects(Environment* env);
void generate_environment(Environment* env);

// 植物相关函数
void init_plant(PlantState* plant, PlantType type, uint8_t rarity);
void update_plant_health(PlantState* plant, const Environment* env);
void update_plant_growth(PlantState* plant);
void pest_control(PlantState* plant);
void fertilize(PlantState* plant);
void watering(PlantState* plant);
const char* get_plant_needs(const PlantState* plant);

// 模拟函数
void simulate_day(PlantState* plant, Environment* env);

//购买植物
void buy_plant(User* user, Commodity* commodity, PlantType plant_type);

//从文件中读取信息
void load_user(User* user);
void load_commodity(Commodity* commodity);
void load_environment(Environment* env);
void load_plant(PlantState* plant);

//保存信息到文件
void save_user(User* user);
void save_commodity(Commodity* commodity);
void save_environment(Environment* env);
void save_plant(PlantState* plant);

// 打印函数
void print_environment(const Environment* env);
void print_plant_state(const PlantState* plant);

#endif // PLANT_SIMULATION_H