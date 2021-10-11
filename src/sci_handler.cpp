#include <sci_handler.h>
#include <driver/serial.h>

extern "C" {
	#include <lai/helpers/sci.h>
}


SciHandler::SciHandler(int intr) : InterruptHandler(intr) {
	// Nothing to do
}

void SciHandler::handle() {
	// Nothing to do

	uint16_t ev = lai_get_sci_event();

	const char *ev_name = "?";
	if (ev & ACPI_POWER_BUTTON) {
		ev_name = "power button";
	}

	if (ev & ACPI_SLEEP_BUTTON) {
		ev_name = "sleep button";
	}

	if (ev & ACPI_WAKE) {
		ev_name = "sleep wake up";
	}

    driver::global_serial_driver->printf("acpi: a SCI event has occured: %x (%s)\n", ev, ev_name);
}