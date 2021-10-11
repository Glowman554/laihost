#include <kmod.h>
#include <renderer/font_renderer.h>

void init() {
	renderer::global_font_renderer->printf("%fHello, world!%r\n", 0xff00ff00);
}

define_module("hello", init);