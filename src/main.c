#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "rp2040-map.h"
#include "helpers.h"
#include "gpio.h"
#include "spi.h"
#include "resets.h"
#include "clocks.h"
#include "uart.h"
#include "rtc.h"
#include "i2c.h"
#include "tm1637.h"

#define LED_PIN (25)

static inline void set_uart0_pinmux(void);
static inline void set_i2c0_pinmux(void);

static inline void set_time(void);
static inline void print_time(void);

static uint32_t div10(uint32_t val);
static uint8_t mod10(uint32_t val);
static uint8_t divmod10(uint32_t val, uint32_t *div);
static inline void print_dec(uint32_t val);

int main(void) {
	uint16_t i = 0;

	init_xosc();
	set_perif_clock_xosc();
	set_rtc_clock_xosc();

	set_reset(RESETS_PLL_USB, 0);
	set_reset(RESETS_PLL_SYS, 0);
	set_usb_pll();
	set_sys_pll();

	set_sys_clock_pll_sys();

	set_reset(RESETS_IO_BANK0, 0);
	set_reset(RESETS_UART0, 0);
	set_reset(RESETS_RTC, 0);
	set_reset(RESETS_I2C0, 0);

	set_uart0_pinmux();
	set_i2c0_pinmux();

	set_pinfunc(LED_PIN, 5);

	SIO_BASE[9] = 1 << LED_PIN; // OE_SET

	uart_init();
	uart_tx_rx_int_en();
	tm1637_init();

	rtc_init();

	//set_time();

	while(1) {
		delay(10000000);
		print_time();
		tm1637_write_bch(i++);
		//uart_puts("Hello, world!\n\r");
		SIO_BASE[7] = 1 << LED_PIN; // OUT_XOR
	}

	return 0;
}

static inline void set_uart0_pinmux(void) {
	set_pinfunc(0, GPIO_FUNC_UART); /* TX, PICO PIN 1 */
	set_pinfunc(1, GPIO_FUNC_UART); /* RX, PICO PIN 2 */
}

static inline void set_i2c0_pinmux(void) {
	set_pinfunc(4, GPIO_FUNC_I2C); /* SDA, PICO PIN 6 */
	set_pinfunc(5, GPIO_FUNC_I2C); /* SCL, PICO PIN 7 */

	set_pulldown(4, false);
	set_pulldown(5, false);
	set_pullup(4, true);
	set_pullup(5, true);
}

static inline void set_time(void) {
	rtc_set_second(0);
	rtc_set_minute(0);
	rtc_set_hour(0);
	rtc_set_day(9);
	rtc_set_month(4);
	rtc_set_year(2025);
}

static inline void print_time(void) {
	uint32_t tmp;

	tmp = rtc_get_year();
	print_dec(tmp);
	uart_putc('-');

	tmp = rtc_get_month();
	print_dec(tmp);
	uart_putc('-');

	tmp = rtc_get_day();
	print_dec(tmp);
	uart_putc(' ');

	/* uart broken... */
	delay(100000);

	tmp = rtc_get_hour();
	print_dec(tmp);
	uart_putc(':');

	tmp = rtc_get_minute();
	print_dec(tmp);
	uart_putc(':');

	tmp = rtc_get_second();
	print_dec(tmp);
	uart_puts("\n\r");
}


static uint32_t div10(uint32_t val) {
	uint32_t cnt = 0;

	while (val >= 10) {
		cnt++;
		val -= 10;
	}

	return cnt;
}

static uint8_t mod10(uint32_t val) {
	uint32_t ret;

	ret = div10(val) * 10;

	return val - ret;
}

static uint8_t divmod10(uint32_t val, uint32_t *div) {
	*div = div10(val);

	return val - (*div * 10);
}

static inline void print_dec(uint32_t val) {
	uint32_t rev = 0;
	uint8_t count = 0;

	do {
		rev = (rev * 10) + divmod10(val, &val);
		count++;
	} while (val);

	while(count--)
		uart_putc(divmod10(rev, &rev) + '0');
}
