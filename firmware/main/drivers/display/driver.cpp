#include <soc/gpio_num.h>
#include <hal/gpio_types.h>
#include <driver/gpio.h>
#include <esp_task_wdt.h>
#include "driver.h"
#include "esp_log.h"
#include "esp_lvgl_port.h"
#include "esp_lcd_gc9a01.h"
#include "ui_main.h"

#define TAG "display_driver"

DisplayDriver::DisplayDriver() {
    ESP_LOGI(TAG, "Starting display driver");
}

void DisplayDriver::init(UiMain* ui) {

    // TODO: better backlight config
    gpio_set_direction(GPIO_NUM_8, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_8, 1);

    const spi_bus_config_t bus_config = {
            .mosi_io_num = TFT_MOSI,
            .miso_io_num = -1,
            .sclk_io_num = TFT_SCLK,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = DISP_WIDTH * 240 * sizeof(uint16_t)
    };

    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &bus_config, SPI_DMA_CH_AUTO));

    esp_lcd_panel_io_handle_t io_handle = NULL;
    const esp_lcd_panel_io_spi_config_t io_config = GC9A01_PANEL_IO_SPI_CONFIG(TFT_CS, TFT_DC, NULL, NULL);

    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t) LCD_HOST, &io_config, &io_handle));

    esp_lcd_panel_handle_t panel_handle = NULL;
    const esp_lcd_panel_dev_config_t panel_config = {
            .reset_gpio_num = -1,
            .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
            .data_endian = LCD_RGB_DATA_ENDIAN_BIG,
            .bits_per_pixel = 16,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_gc9a01(io_handle, &panel_config, &panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    ESP_ERROR_CHECK(lvgl_port_init(&lvgl_cfg));

    const lvgl_port_display_cfg_t disp_cfg = {
            .io_handle = io_handle,
            .panel_handle = panel_handle,
            .buffer_size = DISP_WIDTH * DISP_HEIGHT,
            .double_buffer = true,
            .hres = DISP_WIDTH,
            .vres = DISP_HEIGHT,
            .monochrome = false,
            .rotation = {
                    .swap_xy = false,
                    .mirror_x = true,
                    .mirror_y = false,
            },
            .color_format = LV_COLOR_FORMAT_RGB565,
            .flags = {
                    .buff_dma = true,
                    .buff_spiram = true,
                    .swap_bytes = true,
            },

    };

    lvgl_port_add_disp(&disp_cfg);

    lvgl_port_lock(0);

    ui->create();

    lvgl_port_unlock();
}
