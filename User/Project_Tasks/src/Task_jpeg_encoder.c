#include "Task_jpeg_encoder.h"
#include "Task_common.h"
#define NDEBUG
#define TJE_IMPLEMENTATION
#include "tiny_jpeg.h"
#include <string.h>



static void my_output_callback(void *context, void *data, int size) {
    if (jpeg_chunk_len + size <= MAX_CHUNK_SIZE) {
        memcpy(jpeg_chunk_buffer + jpeg_chunk_len, data, size);
        jpeg_chunk_len += size;
    }
}

void JPEG_Task(void *pvParameters) {
    const int width = 320;
    const int height = 240;
    const int num_comp = 3;
    const int quality = 2;
    uint8_t count = 0;
    const uint8_t success = 1;
    struct TJEEncoderState enc;

    for (;;) {
        xQueueReceive(JPEG_FRAME_QueueHandle, &count, portMAX_DELAY);

        if (count == 1) {
            while (!tje_encode_init(&enc, my_output_callback, NULL, quality,
                                    width, height, num_comp,
                                    camera_data, FIFO_INPUT_BUF_SIZE));
            tje_encode_row_block(&enc);
        } else if (count > 1 && count <= 30) {
            tje_encode_row_block(&enc);
        } else if (count == 0) {
            tje_encode_row_block(&enc);
            tje_encode_finish(&enc);
        }

        xQueueSend(JPEG_DMA_QueueHandle, &success, portMAX_DELAY);
    }
}
