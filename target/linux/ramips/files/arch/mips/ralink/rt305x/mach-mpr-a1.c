/*
 *  HAME MPR-A1 board support by Michel Stempin <michel.stempin@wanadoo.fr>
 *  Based on previous work by arpunk, arteq, Heffer & p1vo from OpenWrt forum.
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
#include <asm/mach-ralink/dev-gpio-leds.h>
#include <asm/mach-ralink/rt305x.h>
#include <asm/mach-ralink/rt305x_regs.h>

#include "devices.h"

#define MPR_A1_GPIO_BUTTON_RESET	0
#define MPR_A1_GPIO_USB_POWER		7
#define MPR_A1_GPIO_ROOT_HUB_POWER	12
#define MPR_A1_GPIO_LED_POWER		17
#define MPR_A1_GPIO_LED_SYSTEM		20

#define MPR_A1_KEYS_POLL_INTERVAL	20
#define MPR_A1_KEYS_DEBOUNCE_INTERVAL	(3 * MPR_A1_KEYS_POLL_INTERVAL)

const struct flash_platform_data mpr_a1_flash = {
        .type           = "pm25lq032",
};

struct spi_board_info mpr_a1_spi_slave_info[] __initdata = {
        {
                .modalias       = "m25p80",
                .platform_data  = &mpr_a1_flash,
                .irq            = -1,
                .max_speed_hz   = 10000000,
                .bus_num        = 0,
                .chip_select    = 0,
        }
};

static struct gpio_led mpr_a1_leds_gpio[] __initdata = {
        {
                .name           = "mpr-a1:blue:system",
                .gpio           = MPR_A1_GPIO_LED_SYSTEM,
                .active_low     = 1,
        },
        {
                .name           = "mpr-a1:red:power",
                .gpio           = MPR_A1_GPIO_LED_POWER,
                .active_low     = 1,
                .default_state  = LEDS_GPIO_DEFSTATE_ON,
        }
};

static struct gpio_keys_button mpr_a1_gpio_buttons[] __initdata = {
        {
                .desc           = "reset",
                .type           = EV_KEY,
                .code           = KEY_RESTART,
                .debounce_interval = MPR_A1_KEYS_DEBOUNCE_INTERVAL,
                .gpio           = MPR_A1_GPIO_BUTTON_RESET,
                .active_low     = 1,
        }
};

static void __init mpr_a1_init(void)
{
        // JTAG is required for LED control
        rt305x_gpio_init((RT305X_GPIO_MODE_GPIO <<
                          RT305X_GPIO_MODE_UART0_SHIFT) |
                         RT305X_GPIO_MODE_JTAG);

        rt305x_register_spi(mpr_a1_spi_slave_info,
                            ARRAY_SIZE(mpr_a1_spi_slave_info));

        ramips_register_gpio_leds(-1, ARRAY_SIZE(mpr_a1_leds_gpio),
                                  mpr_a1_leds_gpio);

        ramips_register_gpio_buttons(-1, MPR_A1_KEYS_POLL_INTERVAL,
                                     ARRAY_SIZE(mpr_a1_gpio_buttons),
                                     mpr_a1_gpio_buttons);

        rt305x_esw_data.vlan_config = RT305X_ESW_VLAN_CONFIG_LLLLW;
        rt305x_register_ethernet();

        rt305x_register_wifi();

        rt305x_register_wdt();

        gpio_request_one(MPR_A1_GPIO_USB_POWER,
                         GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_FIXED,
                         "USB power");
        gpio_request_one(MPR_A1_GPIO_ROOT_HUB_POWER,
                         GPIOF_OUT_INIT_HIGH | GPIOF_EXPORT_DIR_FIXED,
                         "USB root hub power");
        rt305x_register_usb();
}

MIPS_MACHINE(RAMIPS_MACH_MPR_A1, "MPR-A1", "HAME MPR-A1",
             mpr_a1_init);
