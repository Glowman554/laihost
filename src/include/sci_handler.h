#pragma once

#include <interrupts/interrupt_handler.h>

class SciHandler : public interrupts::InterruptHandler {
	public:
		SciHandler(int intr);

		virtual void handle();
};