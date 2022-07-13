#include <stdint.h>


#include <boot/stivale2.h>
// #include <debug/debug.h>
#include <libk/debug/debug.h>
// #include <io/mmio.h>
#include <libk/io/io.h>
// #include <mm/vmm.h>
#include <memory/vmm.h>
// #include <sys/hpet.h>
#include <devices/hpet/hpet.h>
// #include <sys/timer.h>

#include <firmware/acpi/acpi.h>

static struct hpet_table *hpet_table;
static struct hpet *hpet;
static uint32_t clk = 0;
static bool timer_installed_b = false;

void hpet_init(void) {
	hpet_table = (void *)acpi_find_sdt_table("HPET");
	if (!hpet_table) {
		debug("HPET: Polaris requires a HPET to be installed\n");
		__builtin_unreachable();
	}
	hpet = (struct hpet *)(hpet_table->address.base + HIGHER_HALF_DATA_LV4);

	debug("HPET: HPET at %p\n", (void *)hpet);

	clk = hpet->general_capabilities >> 32;

	/*
	  General Configuration Register
		0 - main counter is halted, timer interrupts are disabled
		1 - main counter is running, timer interrupts are allowed if enabled
	 */
	mmoutq(&hpet->general_configuration, 0);
	mmoutq(&hpet->main_counter_value, 0);
	mmoutq(&hpet->general_configuration, 1);
	timer_installed_b = true;
}

uint64_t hpet_counter_value(void) {
	return mminq(&hpet->main_counter_value);
}

void hpet_sleep(uint64_t us) {
	uint64_t target = hpet_counter_value() + (us * 1000000000) / clk;
	while (hpet_counter_value() < target)
		;
}

bool timer_installed(void) {
	return timer_installed_b;
}

void timer_sleep(uint64_t ms) {
	hpet_sleep(ms * 1000);
}

uint64_t timer_count(void) {
	return hpet_counter_value() / 1000;
}
