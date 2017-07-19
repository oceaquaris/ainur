#ifndef RND_H
#define RND_H

#include <stdint.h>

extern void     rnd_128_jump (void);
extern uint64_t rnd_128_next (void);
extern void     rnd_init     (void);

#endif
