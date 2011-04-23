#include "prj_core/prj_core.h"
#include "cpu_emu/cpu_emu.h"

int main(void)
{
  int ret;

  cpu_emu_entry_assign(_main);
  cpu_emu_int_handler_assign(_interrupt);
  ret = cpu_emu_run();

  return ret;
}
