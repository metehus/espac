//
// Created by methi on 22/02/2024.
//

#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>
#include <esp_log.h>
#include "display.h"
#include "sdkconfig.h"
#include "lvgl.h"
extern "C" {
    #include "gc9a01/gc9a01.h"
}

#include "event/lv_example_event.h"

#define DISPLAY_W 240
#define DISPLAY_H 240

#define LV_HOR_RES_MAX DISPLAY_W
#define LV_HOR_RES_MAX DISPLAY_H
#define LV_TICK_PERIOD_MS 1

static const char* TAG = "DISPLAY";


static void lv_tick_task(void *arg);
static void flush_pixel(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);

SemaphoreHandle_t xGuiSemaphore;

void gui_task(void *pvParameter) {
    xGuiSemaphore = xSemaphoreCreateMutex();

    lv_init();
    lv_display_t * display = lv_display_create(DISPLAY_W, DISPLAY_H);

    GC9A01_Init();

//    GC9A01_DrawPixel(20, 20, 100);
//    GC9A01_DrawPixel(70, 70, 4369);
//    GC9A01_Update();

    ESP_LOGI(TAG, "starting drivers");
//

//    lvgl_driver_init();

    lv_display_set_flush_cb(display, flush_pixel);

//    void* buf1 = (lv_color_t*)heap_caps_malloc(DISPLAY_W * DISPLAY_H * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    static lv_color_t* buf1[DISPLAY_W * DISPLAY_H / 4];                        /*Declare a buffer for 1/10 screen size*/
//    static lv_color_t* buf2[DISPLAY_W * DISPLAY_H / 4];                        /*Declare a buffer for 1/10 screen size*/
//    static lv_color_t * buf1 =  (lv_color_t *)heap_caps_malloc((DISPLAY_W * DISPLAY_H * sizeof(lv_color_t)), MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM);
    assert(buf1);
    lv_display_set_buffers(display, buf1, nullptr, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);


    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &lv_tick_task,
            .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    lv_example_event_4();
    ESP_LOGI(TAG, "starting thing\n");

    while (1) {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));

//        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
        }
    }


    vTaskDelete(NULL);
}

void flush_pixel(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    ESP_LOGI(TAG, "flashing pixel\n");

    uint16_t * buf16 = (uint16_t *)px_map; /*Let's say it's a 16 bit (RGB565) display*/
    int16_t x, y;

    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            // subtracting from display size to rotate 180 deg
            GC9A01_DrawPixel(DISPLAY_W - x, DISPLAY_H - y, *buf16);
            buf16++;
        }
    }

    GC9A01_Update();

    /* IMPORTANT!!!
     * Inform LVGL that you are ready with the flushing and buf is not used anymore*/
    lv_display_flush_ready(disp);
}

static void lv_tick_task(void *arg) {
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}