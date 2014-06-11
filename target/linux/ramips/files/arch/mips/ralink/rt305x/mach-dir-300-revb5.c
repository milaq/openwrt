/*
 *  DIR-300B5 board support by Mikael Frykholm <mikael@frykholm.com>
 *
 *  Based on previous work by Michel Stempin <michel.stempin@wanadoo.fr>, arpunk, arteq, Heffer & p1vo from OpenWrt forum.
 * 
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/gpio.h>

#include <asm/mach-ralink/machine.h>
#include <asm/mach-ralink/dev-gpio-buttons.h>
#include <asm/mach-ralink/dev-gpio-leds.h>*/
#include <asm/mach-ralink/rt305x.h>
#include <asm/mach-ralink/rt305x_regs.h>

#include "devices.h"

#define DIR_300B5_GPIO_BUTTON_RESET	0
#define DIR_300B5_GPIO_USB_POWER		7
#define DIR_300B5_GPIO_ROOT_HUB_POWER	12
#define DIR_300B5_GPIO_LED_POWER		17
#define DIR_300B5_GPIO_LED_SYSTEM		20

#define DIR_300B5_KEYS_POLL_INTERVAL	20
#define DIR_300B5_KEYS_DEBOUNCE_INTERVAL	(3 * DIR_300B5_KEYS_POLL_INTERVAL)

const struct flash_platform_data dir_300b5_flash = {
        .type           = "w25q32bv",
};

struct spi_board_info dir_300b5_spi_slave_info[] __initdata = {
        {
                .modalias       = "m25p80",
                .platform_data  = &dir_300b5_flash,
                .irq            = -1,
                .max_speed_hz   = 10000000,
                .bus_num        = 0,
                .chip_select    = 0,
        }
};
static void __init dir_300b5_init(void)
{
        // JTAG is required for LED control
        rt305x_gpio_init((RT305X_GPIO_MODE_GPIO <<
                          RT305X_GPIO_MODE_UART0_SHIFT) |
                         RT305X_GPIO_MODE_JTAG);

        rt305x_register_spi(dir_300b5_spi_slave_info,
                            ARRAY_SIZE(dir_300b5_spi_slave_info));

        rt305x_esw_data.vlan_config = RT305X_ESW_VLAN_CONFIG_LLLLW;
        rt305x_register_ethernet();

        rt305x_register_wifi();

        rt305x_register_wdt();

        gpio_request_one(DIR_300B5_GPIO_USB_POWER,
                         GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_FIXED,
                         "USB power");
        gpio_request_one(DIR_300B5_GPIO_ROOT_HUB_POWER,
                         GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_FIXED,
                         "USB root hub power");
        rt305x_register_usb();
}

MIPS_MACHINE(RAMIPS_MACH_DIR_300_B5, "DIR-300-B5", "D-Link DIR-300 B5",
             dir_300b5_init);
