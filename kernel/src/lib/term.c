#include <lib/term.h>
#include <flanterm/flanterm.h>
#include <flanterm/backends/fb.h>
#include <limine.h>
#include <stdbool.h>

/* TODO: move framebuffer request to something like dev/fb.* */
__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

typedef struct terminal {
    struct flanterm_context *ft_ctx;
    /* spinlock_t lock; */
} terminal_t;

/* TODO: support multiple terminals? */
static terminal_t term;
static bool term_init_done = false;

void term_init(void) {
    if (term_init_done == true)
        return;

    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1) {
        /* TODO: report failure */
        return;
    }


    // Extract the framebuffer
    struct limine_framebuffer *fb =
        framebuffer_request.response->framebuffers[0];

    void *(*alloc_func)(size_t) = NULL;
    void (*free_func)(void *, size_t) = NULL;


    term.ft_ctx = flanterm_fb_init(
            alloc_func, free_func,
            fb->address, fb->width, fb->height, fb->pitch,
            fb->red_mask_size, fb->red_mask_shift,
            fb->green_mask_size, fb->green_mask_shift,
            fb->blue_mask_size, fb->blue_mask_shift,
            NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            NULL, 0, 0, 1, 1, 1, 0);
    if (term.ft_ctx == NULL)
        return;

    term_init_done = true;
}

void term_write(const char *str, size_t len) {
    if (!term_init_done)
        return;

    /* TODO: lock & unlock */
    flanterm_write(term.ft_ctx, str, len);
}
