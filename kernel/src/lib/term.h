#ifndef _HYDRA_LIB_TERM_H
#define _HYDRA_LIB_TERM_H

#include <stddef.h>

void term_init(void);
void term_write(const char *str, size_t len);

#endif /* _HYDRA_LIB_TERM_H */
