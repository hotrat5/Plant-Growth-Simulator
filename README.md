# Plant-Growth-Simulator
### 植物生长模拟系统说明文档

#### 一、系统概述
这是一个基于LVGL图形库的植物生长模拟系统，主要功能包括：
1. 模拟植物生长过程（向日葵、番茄、仙人掌、食人花）
2. 动态环境生成（温度/湿度/光照/季节/天气）
3. 用户交互系统（购买/照料植物）
4. 数据持久化（保存/加载游戏状态）
5. 响应式UI界面

#### 二、核心模块

##### 1. 环境模拟系统
```c
typedef struct {
    uint8_t temperature;  // 温度(℃)
    uint8_t humidity;     // 湿度(0-100%)
    uint8_t light;        // 光照(0-100%)
    uint8_t season;       // 季节(0-3: 春夏秋冬)
    uint32_t day;         // 当前天数
    WeatherType weather;  // 天气类型
} Environment;
```

**关键函数：**
- `generate_environment()`：生成每日环境参数
- `apply_weather_effects()`：应用天气效果
- `print_environment()`：打印环境信息

##### 2. 植物生长模型
```c
typedef struct {
    PlantType type;         // 植物种类
    uint8_t rarity;         // 稀有度(1-3)
    GrowthStage stage;      // 生长阶段
    uint8_t health;         // 健康值(0-100)
    uint32_t age;           // 年龄(天)
    uint8_t growth;         // 生长进度(0-100%)
    // ...照料状态标记和计数器
} PlantState;
```

**生长阶段：**
```c
typedef enum {
    SEED,         // 种子
    SPROUT,       // 发芽
    SEEDLING,     // 幼苗
    MATURE,       // 成熟
    FLOWERING,    // 开花
    FRUITING,     // 结果
    STAGE_COUNT
} GrowthStage;
```

**关键函数：**
- `init_plant()`：初始化植物
- `update_plant_health()`：根据环境更新健康值
- `update_plant_growth()`：更新生长进度
- `watering()/fertilize()/pest_control()`：照料操作

##### 3. 用户系统
```c
typedef struct {
    uint32_t coins;        // 金币数量
    uint8_t plant_num;     // 拥有植物数量
    uint8_t plant_type[4]; // 拥有的植物类型
} User;
```

##### 4. 商店系统
```c
typedef struct {
    PlantType plant_type[4]; // 出售的植物类型
    uint16_t price[4];       // 价格
    bool ishave[4];          // 是否有货
} Commodity;
```

**关键函数：**
- `buy_plant()`：购买植物

#### 三、数据持久化

**文件存储结构：**
1. `user.txt`：用户数据（金币/植物）
2. `commodity.txt`：商品数据
3. `environment.txt`：环境数据
4. `plant.txt`/`tomato.txt`/`cactus.txt`/`cannibal.txt`：植物状态

**关键函数：**
- `load_user()/save_user()`
- `load_plant()/save_plant()`
- `load_environment()/save_environment()`

#### 四、系统工作流程

1. **初始化阶段**
   - 初始化LVGL图形系统
   - 加载用户/环境/植物数据
   - 创建植物状态更新定时器(5秒/次)

2. **主循环**
   ```c
   while (1) {
       lv_timer_handler();  // 处理UI事件
       usleep(5000);        // 5ms延迟
   }
   ```

3. **植物更新流程**
   ```c
   static void plant_update_timer(lv_timer_t *timer) {
       generate_environment(env);          // 生成新环境
       simulate_day(sunflower, env);       // 更新植物状态
       // ...更新其他植物
       lv_event_send(ui_healthlabel1...); // 刷新UI
       user->coins += 50;                 // 每日金币奖励
   }
   ```

#### 五、环境生成逻辑

1. **季节影响**
   | 季节   | 温度范围 | 湿度范围 | 光照范围 |
   |--------|----------|----------|----------|
   | 春季   | 15-30℃  | 50-90%   | 60-90%   |
   | 夏季   | 25-45℃  | 40-80%   | 70-100%  |
   | 秋季   | 10-25℃  | 30-70%   | 50-80%   |
   | 冬季   | 0-10℃   | 40-70%   | 40-70%   |

2. **天气概率**
   | 季节   | 晴朗 | 多云 | 下雨 | 下雪 | 大风 |
   |--------|------|------|------|------|------|
   | 春季   | 20%  | 40%  | 30%  | 0%   | 10%  |
   | 夏季   | 40%  | 20%  | 30%  | 0%   | 10%  |
   | 秋季   | 30%  | 30%  | 10%  | 0%   | 30%  |
   | 冬季   | 10%  | 30%  | 0%   | 50%  | 10%  |

#### 六、植物需求参数

| 植物类型       | 温度范围 | 湿度范围 | 光照范围 |
|----------------|----------|----------|----------|
| 向日葵 (SUNFLOWER) | 20-30℃  | 40-70%   | 70-90%   |
| 番茄 (TOMATO)     | 18-28℃  | 60-80%   | 60-85%   |
| 仙人掌 (CACTUS)   | 25-40℃  | 10-30%   | 80-100%  |
| 食人花 (CANNIBAL) | 22-28℃  | 75-95%   | 40-60%   |

#### 七、信号处理
```c
static void handleSignal(int signal) {
    save_user(user);                // 保存用户数据
    save_plant(sunflower);          // 保存植物状态
    // ...保存其他数据
    exit(EXIT_SUCCESS);             // 安全退出
}
```
处理信号：`SIGINT`(Ctrl+C), `SIGTERM`, `SIGQUIT`

#### 八、编译与运行
1. 依赖库：LVGL图形库
2. 编译命令示例：
   ```bash
   gcc main.c plant_simulation.c -llvgl -o plant_sim
   ```
3. 运行：
   ```bash
   ./plant_sim
   ```

#### 九、注意事项
1. UI界面元素在`ui.h`中定义
2. 植物图像路径需正确配置
3. 数据文件需有读写权限
4. 使用信号机制保证数据安全保存

> 该系统模拟了完整的植物生命周期，通过环境参数影响植物生长状态，配合直观的UI界面，提供沉浸式的植物养护体验。