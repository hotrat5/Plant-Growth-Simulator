#include <stdio.h>
#include "lv_conf.h"
#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <sys/time.h>

#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <demos/lv_demos.h>
#include "UI/ui.h"

#define DISP_BUF_SIZE 800 * 480 * 4
void lvgl_init_framebuffer_ts();

int main(void)
{
    lvgl_init_framebuffer_ts();

    ui_init();

    while (1)
    {
        lv_timer_handler();
        usleep(5000);
    }
    return 0;
}

void lvgl_init_framebuffer_ts()
{
    /*LittlevGL init*/
    lv_init();
    // 配置显示接口
    fbdev_init();
    // 初始化输入接口
    evdev_init();
    // 输入接口
    static lv_color_t buf[DISP_BUF_SIZE];  // 显示缓冲区
    static lv_color_t buf1[DISP_BUF_SIZE]; // 显示缓冲区
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, buf1, DISP_BUF_SIZE);

    // 设置显示缓冲区属性
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf = &disp_buf;
    disp_drv.flush_cb = fbdev_flush;
    disp_drv.hor_res = 800;
    disp_drv.ver_res = 480;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1);          /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER; // 设置输入事件类型
    // 设置具体采集输入数据接口
    indev_drv_1.read_cb = evdev_read;
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
