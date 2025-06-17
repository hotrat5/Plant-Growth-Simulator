#include <stdio.h>
#include "lv_conf.h"
#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include "lv_drivers/sdl/sdl.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <demos/lv_demos.h>
#include "UI/ui.h"
#include "UI/plant_simulation.h"
#include <signal.h>

#define DISP_BUF_SIZE 800 * 480 * 8

void lvgl_init_framebuffer_ts();
uint32_t custom_tick_get(void);
static void plant_update_timer(lv_timer_t *timer);
// 信号处理函数
static void handleSignal(int signal);
static void plant_own();

// 注册信号处理函数
static void registerSignalHandlers();
static void load_info();

static const uint32_t UPDATE_INTERVAL = 5000;  // 5秒
static const uint32_t TIMEOUT_WAIT_PLANT = 60000;  // 30秒超时

Environment* env = NULL;
PlantState* plant = NULL;  // 向日葵
PlantState* tomato = NULL; 
PlantState* cactus = NULL;
PlantState* cannibal_flower = NULL;
User* user = NULL;
Commodity* commodity = NULL;
extern bool exist_plant;
bool day_change = true;
uint8_t plantstage = 0;
static bool own_plant[4];

int main(void)
{
    //信号处理
    registerSignalHandlers();

    lvgl_init_framebuffer_ts();

    // 初始化UI
    ui_init();

    
    

    //初始化用户和商店
    user = (User*)malloc(sizeof(User));
    if (user == NULL) {
        printf("内存分配失败！\n");
        return -1;
    }

    commodity = (Commodity*)malloc(sizeof(Commodity));
    if (commodity == NULL) {
        printf("内存分配失败！\n");
        return -1;
    }

    // 初始化环境和植物
    env = (Environment*)malloc(sizeof(Environment));
    if (env == NULL) {
        printf("内存分配失败！\n");
        return -1;
    }
    own_plant[0] = true;
    plant = (PlantState*)malloc(sizeof(PlantState));
    if (plant == NULL) {
        printf("内存分配失败！\n");
        return -1;
    }

    
        tomato = (PlantState*)malloc(sizeof(PlantState));
        if (tomato == NULL) {
            printf("内存分配失败！\n");
            return -1;
        }
    
    
        cactus = (PlantState*)malloc(sizeof(PlantState));
        if (cactus == NULL) {
            printf("内存分配失败！\n");
            return -1;
        }
   
    
        cannibal_flower = (PlantState*)malloc(sizeof(PlantState));
        if (cannibal_flower == NULL) {
            printf("内存分配失败！\n");
            return -1;
        }
   
    

    load_info();
    
    // 等待植物初始化（添加超时机制）
    uint32_t start_time = custom_tick_get();
    printf("等待植物初始化...\n");
    
    while(exist_plant == false) {
        // 处理LVGL事件，保持界面响应
        lv_timer_handler();
        usleep(100000);  // 100ms检查一次
        
        if (custom_tick_get() - start_time > TIMEOUT_WAIT_PLANT) {
            printf("等待植物初始化超时！\n");
            if (env != NULL) free(env);
            return -1;
        }
    }
    
    printf("植物初始化成功！\n");
    //判断是否拥有植物
    plant_own();
    printf("%d\n", own_plant[1]);
    // 创建LVGL定时器用于定期更新植物状态
    lv_timer_create(plant_update_timer, UPDATE_INTERVAL, NULL);
    

    // 主循环
    while (1) {
        // 处理LVGL事件
        lv_timer_handler();
        
        // 短暂延时，避免CPU占用过高
        usleep(5000);
    }
    
    // 释放资源
    if (plant != NULL) {
        free(plant);
    }
    
    if (env != NULL) {
        free(env);
    }
    
    return 0;
}

/* 植物状态更新定时器回调函数 */
static void plant_update_timer(lv_timer_t *timer) {
    

    if (plant == NULL || env == NULL || plant->health == 0) {
        return;
    }
    printf("enter plant_update\n");
    day_change = true;
    simulate_day(plant, env);
    printf("out simulate_day\n");
    print_environment(env);
    print_plant_state(plant);
    
    if(day_change == true){
        lv_event_send(ui_healthlabel1, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_statelabel1, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_humiditylabel1, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_temperaturelabel1, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_lightlabel1, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_weatherlabel1, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_seasonlabel1, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_growthstagelabel1, LV_EVENT_REFRESH, NULL);
        lv_event_send(ui_Label4, LV_EVENT_REFRESH, NULL);
    }

    if(plantstage != plant->stage){
        printf("%d\n", plant->stage);
        switch (plant->stage)
        {
        case 1:
            lv_event_send(ui_sunflower2, LV_EVENT_REFRESH, NULL);
            if(own_plant[1]) lv_event_send(ui_tomato2, LV_EVENT_REFRESH, NULL);
            break;
        case 2:
            lv_event_send(ui_sunflower4, LV_EVENT_REFRESH, NULL);
            if(own_plant[1]) lv_event_send(ui_tomato4, LV_EVENT_REFRESH, NULL);
            break;
        case 3:
            lv_event_send(ui_sunflower5, LV_EVENT_REFRESH, NULL);
            if(own_plant[1]) lv_event_send(ui_tomato5, LV_EVENT_REFRESH, NULL);      
            break;
        case 4:
            lv_event_send(ui_sunflower6, LV_EVENT_REFRESH, NULL);
            if(own_plant[1]) lv_event_send(ui_tomato6, LV_EVENT_REFRESH, NULL);
            break;
        case 5:
            lv_event_send(ui_sunflower7, LV_EVENT_REFRESH, NULL);  
            if(own_plant[1]) lv_event_send(ui_tomato7, LV_EVENT_REFRESH, NULL); 
            break;
        case 6:
            lv_event_send(ui_sunflower7, LV_EVENT_REFRESH, NULL);
            if(own_plant[1]) lv_event_send(ui_tomato7, LV_EVENT_REFRESH, NULL);
            break;
        default:
            break;
        }
        lv_event_send(ui_plantstagetextarea1, LV_EVENT_REFRESH, NULL);
       
        lv_event_send(ui_moneymessage, LV_EVENT_REFRESH, NULL);
    }

    user->coins += 500;

    day_change = false;
    //ui_event_temperaturelabel1(NULL);
    plantstage = plant->stage;
    // 如果植物死亡，停止定时器
    if (plant->health <= 0) {
        printf("你的植物已经死亡，模拟结束。\n");
        lv_timer_del(timer);  
    }
}

static void load_info(){
    load_commodity(commodity);
    load_user(user);
    load_environment(env);
    load_plant(plant, 0);
    load_plant(tomato, 1);
    load_plant(cactus, 2);
    load_plant(cannibal_flower, 3);
    if(user->plant_num!=0) exist_plant = true;  
}

// 信号处理函数
static void handleSignal(int signal) {
    save_user(user);
    save_commodity(commodity);
    save_environment(env);
    save_plant(plant);
    if(own_plant[1]) save_plant(tomato);
    if(own_plant[2]) save_plant(cactus);
    if(own_plant[3]) save_plant(cannibal_flower);    
    printf("程序接收到信号，即将退出\n");
    exit(EXIT_SUCCESS);
}

// 注册信号处理函数
static void registerSignalHandlers() {
    //SDL_EventType event;
    signal(SIGINT, handleSignal);  // 处理Ctrl+C
    signal(SIGTERM, handleSignal); // 处理终止信号
    signal(SIGQUIT, handleSignal); // 处理退出信号
    // if (event.type == SDL_QUIT){
    //     handleSignal();
    // }
}


static void plant_own(){
  
    if(!commodity->ishave[1]) own_plant[1] = true;
    if(!commodity->ishave[2]) own_plant[2] = true;
    if(!commodity->ishave[3]) own_plant[3] = true;
    
}



void lvgl_init_framebuffer_ts()
{
    /*LittlevGL init*/
    lv_init();

    sdl_init();

    // 输入接口
    static lv_color_t buf[DISP_BUF_SIZE];  // 显示缓冲区
    static lv_color_t buf1[DISP_BUF_SIZE]; // 显示缓冲区
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, buf1, DISP_BUF_SIZE);

    // 设置显示缓冲区属性
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;

    disp_drv.flush_cb = sdl_display_flush;

    disp_drv.hor_res = 800;
    disp_drv.ver_res = 480;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);          /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER; // 设置输入事件类型
    // 设置具体采集输入数据接口

    indev_drv_1.read_cb = sdl_mouse_read;

    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);
}

/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if (start_ms == 0)
    {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}

/*

*/