#pragma once

#include <csetjmp>
#include <cstddef>
#include <cstdint>
#include <cstring>

#if defined(NDEBUG)
#define BREAKPOINT
#else
#define BREAKPOINT __BKPT(0)
#endif

typedef enum {
  errNone = 0,
  errExceptionNesting,
  errI2CCommError,
  error_e_count,
} error_e;

typedef void (*reinit_func_t)(void);

extern const char* const error_str[];
extern jmp_buf g_error_env;

void errors_init();
__attribute__((weak)) void unhandled_exception(error_e err);

void save_jmp_buf();
void restore_jmp_buf();

#define throw_on(cond, err)    \
  if ((cond)) {                \
    BREAKPOINT;                \
    longjmp(g_error_env, err); \
  }

#define local_eh_begin(max_retry, reinit_f) \
  do {                                      \
    volatile uint32_t retry = (max_retry);  \
    int err;                                \
    save_jmp_buf();                         \
    if ((err = setjmp(g_error_env))) {      \
      retry--;                              \
      reinit_func_t reinit = reinit_f;      \
      if (reinit)                           \
        reinit();                           \
    }                                       \
    if (!retry) {                           \
      restore_jmp_buf();                    \
      BREAKPOINT;                           \
      longjmp(g_error_env, err);            \
    }

#define local_eh_end() \
  restore_jmp_buf();   \
  }                    \
  while (0)
