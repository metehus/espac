/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include <esp_log.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"
#include "esp_flash.h"
#include "display.h"

void measure(void *pvParameters) {

    while (true) {
        int raw0 = gpio_get_level(GPIO_NUM_15);
        int raw1 = gpio_get_level(GPIO_NUM_16);

        printf("S0 %d, S1: %d\n", raw0, raw1);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

extern "C" void app_main(void) {
    printf("Hello world!\n");
    ESP_LOGI("MAIN", "test log");

    gpio_set_direction(GPIO_NUM_15, GPIO_MODE_INPUT);
    gpio_set_direction(GPIO_NUM_16, GPIO_MODE_INPUT);


    xTaskCreatePinnedToCore(measure, "sensor_task", configMINIMAL_STACK_SIZE * 8, NULL, 2, NULL, 0);

    xTaskCreatePinnedToCore(gui_task, "gui", 4096*2, NULL, 0, NULL, 1);

}
