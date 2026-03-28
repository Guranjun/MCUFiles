#include "Task_LVGL.h"
#include "Task_common.h"
#include "lvgl.h"



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
		lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));  
	}
	
}

