#include <kmod.h>
#include <renderer/font_renderer.h>

extern "C" {
	#include <lai/core.h>
	#include <lai/helpers/sci.h>
}

void init() {
	lai_enable_tracing(LAI_TRACE_OP | LAI_TRACE_IO);
	lai_create_namespace();
}

define_module("hello", init);