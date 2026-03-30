#include "Task_LVGL.h"
#include "Task_common.h"
#include "lvgl.h"
typedef enum {
    UPLOAD_IDLE,
    UPLOAD_RUNNING,
    UPLOAD_DONE
} upload_state_t;

static upload_state_t state_screen =UPLOAD_IDLE;
static upload_state_t state_screen1 =UPLOAD_IDLE;
static uint32_t done_tick = 0;
static void upload_ui_update( upload_state_t *state_upload ,uint8_t screen_num);

void LVGL_Task(void *pvParameters)
{
	for(;;)
	{
		uint8_t cmd;
		
		if (xQueueReceive(Refresh_Button_QueueHandle, &cmd, 0) == pdTRUE) {
            if (in_camera_page && guider_ui.screen_1_btn_4 && guider_ui.screen_1_btn_3) {
                lv_obj_invalidate(guider_ui.screen_1_btn_4);
				lv_obj_invalidate(guider_ui.screen_1_btn_3);
            }
        }
		if(in_camera_page == 0)
		{
			upload_ui_update(&state_screen , 0);
		}
		else
		{
			upload_ui_update(&state_screen1 , 1);
		}
		lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));  
	}
	
}

static void upload_ui_update( upload_state_t *state_upload,uint8_t screen_num )
{
	static uint8_t progress = 0;
	lv_obj_t *label;
	lv_obj_t *bar;
	if(screen_num == 0)
	{
		label=guider_ui.screen_label_1;
		bar=guider_ui.screen_bar_1 ;
	}
	else
	{
		label=guider_ui.screen_1_label_1;
		bar=guider_ui.screen_1_bar_1 ;
	}
	switch(*state_upload)
	{
		case UPLOAD_IDLE:
			if(xQueueReceive(Upload_Progress_QueueHandle,&progress,0)==pdTRUE)
			{
				*state_upload = UPLOAD_RUNNING;

				lv_obj_clear_flag(label , LV_OBJ_FLAG_HIDDEN);
				lv_obj_clear_flag(bar , LV_OBJ_FLAG_HIDDEN);

				lv_label_set_text(label, "Uploading...");
				lv_bar_set_value(bar, progress, LV_ANIM_ON); 
			}
			break;
		case UPLOAD_RUNNING:
			if(xQueueReceive(Upload_Progress_QueueHandle,&progress,0)==pdTRUE)
			{
				lv_bar_set_value(bar, progress, LV_ANIM_ON);
				if(progress >= 100)
				{
					lv_label_set_text(label , "Done!" );
					done_tick = lv_tick_get() ;
					*state_upload = UPLOAD_DONE ;
				}
			}
			break;
		case UPLOAD_DONE:
			if(lv_tick_elaps(done_tick) > 500)
			{
				lv_obj_add_flag(label , LV_OBJ_FLAG_HIDDEN);
				lv_obj_add_flag(bar , LV_OBJ_FLAG_HIDDEN);
				lv_bar_set_value(bar, 0, LV_ANIM_OFF);
				*state_upload = UPLOAD_IDLE ;
			}
			break;
		default:
			break;
		
	}
}
