#include "errors.hpp"

#include <stm32f0xx.h>

jmp_buf g_error_env;
static jmp_buf g_error_env_save;
static bool g_env_saved;

const char* const error_str[error_e_count] = {
    "No Error",
    "Nested Exception",
    "I2C Communication error",
};

__attribute__((weak)) void unhandled_exception(error_e /*err*/) {
  NVIC_SystemReset();
}

void errors_init() {
  g_env_saved = 0;

  int err = setjmp(g_error_env);

  if (err != 0) {
    unhandled_exception((error_e)err);
  }
}

void save_jmp_buf() {
  throw_on(g_env_saved, errExceptionNesting);

  memcpy(&g_error_env_save, &g_error_env, sizeof(jmp_buf));
  g_env_saved = true;
}

void restore_jmp_buf() {
  throw_on(!g_env_saved, errExceptionNesting);

  memcpy(&g_error_env, &g_error_env_save, sizeof(jmp_buf));
  g_env_saved = false;
}
