#ifndef __PRJ_CORE_H__
#define __PRJ_CORE_H__

struct cpu_emu_sfr;
int _main(int argc, char **argv);
void _interrupt(struct cpu_emu_sfr *regs);

#endif /* __PRJ_CORE_H__ */
