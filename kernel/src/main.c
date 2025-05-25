#include <stdbool.h>
#include <limine.h>
#include <arch/cpu.h>
#include <lib/term.h>

extern bool is_limine_base_revision_supported(void);

_Noreturn void _kernel_entry(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (is_limine_base_revision_supported() == false) {
        for (;;)
            halt();
    }

    term_init();

    const char buf[] = "Hello, world!\n";
    term_write(buf, sizeof(buf));

    for (;;)
        halt();
}
