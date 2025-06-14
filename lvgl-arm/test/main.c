#include <stdio.h>
#include "lv_conf.h"
#include "lvgl/lvgl.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <sys/time.h>

#define  DISP_BUF_SIZE  800*480*2
void lvgl_init_framebuffer_ts();

static lv_obj_t* kb = NULL;
static void ta_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);  //获取事件
    lv_obj_t * ta = lv_event_get_target(e);  //获取焦点组件
    if(code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
        /*Focus on the clicked text area*/
        if(kb != NULL) lv_keyboard_set_textarea(kb, ta);
    }
    else if(code == LV_EVENT_READY) {
        LV_LOG_USER("Ready, current text: %s", lv_textarea_get_text(ta));
    }
}

int main(void)
{

    //初始化
    lvgl_init_framebuffer_ts();
    //设计一个登录界面
    lv_obj_t *mainwin = lv_obj_create(lv_scr_act());
    lv_obj_set_align(mainwin,LV_ALIGN_TOP_MID);
    lv_obj_set_size(mainwin,440,200);

    //在mainwin上创建一个输入框
    lv_obj_t* userEdit = lv_textarea_create(mainwin);
    lv_obj_align(userEdit,LV_ALIGN_CENTER,0,-25);
    lv_obj_set_size(userEdit,200,40);
    lv_obj_set_style_bg_color(userEdit,lv_color_hex(0x123456),LV_STYLE_BG_COLOR);
    lv_textarea_set_one_line(userEdit, true);
    lv_textarea_set_placeholder_text(userEdit,"username");


    lv_obj_t *passEdit = lv_textarea_create(mainwin);
    lv_obj_align(passEdit,LV_ALIGN_CENTER,0,25);
    lv_obj_set_size(passEdit,200,40);
    lv_textarea_set_one_line(passEdit, true);
    lv_textarea_set_placeholder_text(passEdit,"password");
    lv_textarea_set_password_mode(passEdit,true);
    
    kb = lv_keyboard_create(lv_scr_act());
    lv_obj_set_size(kb,  LV_HOR_RES, LV_VER_RES / 2);

    //添加事件
    lv_obj_add_event_cb(userEdit, ta_event_cb, LV_EVENT_ALL,NULL);
    lv_obj_add_event_cb(passEdit, ta_event_cb, LV_EVENT_ALL, NULL);

    while(1) {
        lv_timer_handler();
        usleep(5000);
    }
    return  0;
}

void lvgl_init_framebuffer_ts()
{
    /*LittlevGL init*/
    lv_init();
    //配置显示接口
    fbdev_init();
    //初始化输入接口
    evdev_init();
    //输入接口
    static lv_color_t buf[DISP_BUF_SIZE]; //显示缓冲区
    static lv_color_t buf1[DISP_BUF_SIZE]; //显示缓冲区
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, buf1, DISP_BUF_SIZE);

    //设置显示缓冲区属性
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 800;
    disp_drv.ver_res    = 480;
    lv_disp_drv_register(&disp_drv);


    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;  //设置输入事件类型
    //设置具体采集输入数据接口
    indev_drv_1.read_cb = evdev_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

}
/*Set in lv_conf.h as `LV_TICK_CUSTOM_SYS_TIME_EXPR`*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
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