#ifndef __CPU_EMU_H__
#define __CPU_EMU_H__

/**
   CPU EMU:
   Special function registers
 */
struct cpu_emu_sfr {
  unsigned int ADCCTRL;
  unsigned int ADCSAMP;
  /* ... */
};

int cpu_emu_run(void);
void cpu_emu_int_on(void);
void cpu_emu_int_off(void);
void cpu_emu_entry_assign(int (*p)(int argc, char **argv));
void cpu_emu_int_handler_assign(void (*p)(struct cpu_emu_sfr *regs));

#endif /* __CPU_EMU_H__ */
