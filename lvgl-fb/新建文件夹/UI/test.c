#include "plant_simulation.h"

int main() {
    srand(time(NULL));
    
    // 初始化环境
    Environment env = {
        .season = 1, // 夏季开始
        .day = 0
    };
    
    // 初始化植物（向日葵）
    PlantState plant;
    init_plant(&plant, SUNFLOWER, 2);
    
    // 模拟30天
    for (int i = 0; i < 30; i++) {
        printf("\n==================== Day %d ====================\n", i+1);
        
        // 模拟一天
        simulate_day(&plant, &env);
        print_environment(&env);
        print_plant_state(&plant);
        
        // 用户干预（根据提醒操作）
        if (plant.needs_water) {
            watering(&plant);
        }
        if (plant.needs_fertilizer) {
            fertilize(&plant);
        }
        if (plant.needs_pest_control) {
            pest_control(&plant);
        }
        
        // 随机用户操作（模拟玩家行为）
        if (rand() % 3 == 0 && !plant.needs_water) {
            watering(&plant);
        }
        
        // 检查植物是否死亡
        if (plant.health == 0) {
            printf("\n!!! PLANT HAS DIED !!!\n");
            break;
        }
    }
    
    return 0;
}