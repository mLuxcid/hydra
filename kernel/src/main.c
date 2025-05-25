#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <arch/cpu.h>

extern bool is_limine_base_revision_supported();

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

_Noreturn void _kernel_entry(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (is_limine_base_revision_supported() == false) {
        for (;;)
            halt();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        for (;;)
            halt();
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // Note: we assume the framebuffer model is RGB with 32-bit pixels.
    for (size_t i = 0; i < 100; i++) {
        volatile uint32_t *fb_ptr = framebuffer->address;
        fb_ptr[i * (framebuffer->pitch / 4) + i] = 0xffffff;
    }

    for (;;)
        halt();
}
