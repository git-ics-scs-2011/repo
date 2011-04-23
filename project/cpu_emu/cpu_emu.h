#ifndef __CPU_EMU_H__
#define __CPU_EMU_H__

/**
   Iterrupt sources
 */
enum cpu_emu_int_mask {
  CPU_EMU_INT_MASK_TIMER = (1 << 0),
  CPU_EMU_INT_MASK_ADC   = (1 << 1),
  CPU_EMU_INT_MASK_UART  = (1 << 2)
};

/**
   CPU EMU:
   Special function registers
 */
struct cpu_emu_sfr {
  unsigned int INTMASK; /* mask of occured interrupts in system */
  /* ... */  
  unsigned int ADCSAMP;
  /* ... */
  unsigned int UARTREG;
  /* ... */
};

int cpu_emu_run(void);
int cpu_emu_is_finishing(void);

void cpu_emu_int_on(void);
void cpu_emu_int_off(void);

void cpu_emu_entry_assign(int (*p)(int argc, char **argv));
void cpu_emu_int_handler_assign(void (*p)(struct cpu_emu_sfr *regs));

#endif /* __CPU_EMU_H__ */
