/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"
#include "Task_common.h"
#include "./usart/bsp_usart.h"
#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

volatile uint8_t in_camera_page = 0;//标记当前处在哪个页面
//volatile uint8_t Upload_Pressed = 0;
/*按键dispaly事件处理*/
static void screen_btn_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
		in_camera_page = 1;
		printf("\r\n incarmerapage\r\n");
		lv_scr_load(guider_ui.screen_1);       // 加载相机页面
        lv_obj_invalidate(guider_ui.screen_1); // 触发首次刷新
        break;
    }
    default:
        break;
    }
}
/*按键upload事件处理*/
static void screen_btn_2_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
//		Upload_Pressed = 1 ;
		xSemaphoreGive(Upload_SemaphoreHandle);
        break;
    }
    default:
        break;
    }
}

void events_init_screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_btn_1, screen_btn_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_btn_2, screen_btn_2_event_handler, LV_EVENT_ALL, ui);
}
/*按键exti事件处理*/
static void screen_1_btn_4_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
		in_camera_page = 0;
		lv_scr_load(guider_ui.screen);
        lv_obj_invalidate(guider_ui.screen);
        break;
    }
    default:
        break;
    }
}
/*按键shot事件处理*/
static void screen_1_btn_3_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
//		Upload_Pressed = 1 ;
		xSemaphoreGive(Upload_SemaphoreHandle);
        break;
    }
    default:
        break;
    }
}

void events_init_screen_1 (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->screen_1_btn_4, screen_1_btn_4_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->screen_1_btn_3, screen_1_btn_3_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}