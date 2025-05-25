#include <stdbool.h>
#include <limine.h>
#include <arch/cpu.h>
#include <lib/term.h>
#include <lib/log.h>

extern bool is_limine_base_revision_supported(void);

_Noreturn void _kernel_entry(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (is_limine_base_revision_supported() == false) {
        for (;;)
            halt();
    }

    term_init();

    log_write(LOG_LEVEL_ERR, __FILE__, __LINE__, __FUNCTION__, "%s", "hello, world!");

    for (;;)
        halt();
}
