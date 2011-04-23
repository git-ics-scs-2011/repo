#include <stdio.h>
#include "prj_core/prj_core.h"
#include "cpu_emu/cpu_emu.h"


int _main(int argc, char **argv)
{
  while (!cpu_emu_is_finishing()) {
    //printf("_main()\n");
  }
  return 0;
}

void _interrupt(struct cpu_emu_sfr *regs)
{
  //printf("_interrupt()\n");
  printf("regs: %X\n", regs->INTMASK);
}
