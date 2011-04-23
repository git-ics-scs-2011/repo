#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include "cpu_emu.h"

static pthread_t main_thread;
static pthread_t int_thread;
static pthread_mutex_t int_onoff = PTHREAD_MUTEX_INITIALIZER;
static bool done = false;
static int (*_p_main)(int argc, char **argv);
static void (*_p_int)(struct cpu_emu_sfr *regs);
static const uint64_t US = 1000000;
static struct cpu_emu_sfr cpu_regs = {
  .INTMASK = 0,
  .ADCSAMP = 0,
  .UARTREG = 0
};

int cpu_emu_is_finishing(void)
{
  return done;
}

void cpu_emu_entry_assign(int (*p)(int argc, char **argv))
{
  _p_main = p;
}

void cpu_emu_int_handler_assign(void (*p)(struct cpu_emu_sfr *regs))
{
  _p_int = p;
}

void cpu_emu_int_on(void)
{
  int ret;
  ret = pthread_mutex_lock(&int_onoff);
  assert(ret == 0);
}

void cpu_emu_int_off(void)
{
  int ret;
  ret = pthread_mutex_unlock(&int_onoff);
  assert(ret == 0);
}

static void* cpu_emu_main(void* arg)
{
  int ret;

  if (!done) {
    ret = _p_main(0, NULL);
    assert(ret == 0);
  }

  return NULL;
}

uint64_t cpu_emu_get_ticks(void)
{
  struct timeval tv;
  int ret;
  
  ret = gettimeofday(&tv, NULL);
  assert(ret == 0);

  return (tv.tv_sec * US + tv.tv_usec);
}

static void cpu_emu_uart_handle(void)
{
  /* printf("uart!\n"); */
  cpu_regs.INTMASK |= CPU_EMU_INT_MASK_UART;
  _p_int(&cpu_regs);
  cpu_regs.INTMASK &= ~CPU_EMU_INT_MASK_UART;
}

static void cpu_emu_adc_handle(void)
{
  /* printf("adc!\n"); */
  cpu_regs.INTMASK |= CPU_EMU_INT_MASK_ADC;
  _p_int(&cpu_regs);
  cpu_regs.INTMASK &= ~CPU_EMU_INT_MASK_ADC;
}

static void cpu_emu_timer_handle(void)
{
  /* printf("timer!\n"); */
  cpu_regs.INTMASK |= CPU_EMU_INT_MASK_TIMER;
  _p_int(&cpu_regs);
  cpu_regs.INTMASK &= ~CPU_EMU_INT_MASK_TIMER;
}

static void* cpu_emu_int(void* arg)
{
  int ret;
  const uint64_t timer_interval = 1*US; /* in us */
  uint64_t timer_start, timer_end;

  const uint64_t adc_interval   = 3*US; /* in us */
  uint64_t adc_start, adc_end;

  const uint64_t uart_interval  = 15*US; /* in us */
  uint64_t uart_start, uart_end;

  cpu_emu_int_off();

  timer_start = cpu_emu_get_ticks();
  adc_start = cpu_emu_get_ticks();
  uart_start = cpu_emu_get_ticks();

  while (!done) {
    /* timer emulation */
    timer_end = cpu_emu_get_ticks();
    if (timer_end - timer_start > timer_interval) {
      timer_start = cpu_emu_get_ticks();
      cpu_emu_timer_handle();
    }

    /* adc emulation */
    adc_end = cpu_emu_get_ticks();
    if (adc_end - adc_start > adc_interval) {
      adc_start = cpu_emu_get_ticks();
      cpu_emu_adc_handle();
    }

    /* uart emulation */
    uart_end = cpu_emu_get_ticks();
    if (uart_end - uart_start > uart_interval) {
      uart_start = cpu_emu_get_ticks();
      cpu_emu_uart_handle();
    }
  }

  cpu_emu_int_on();
  return NULL;
}

static void menu_print(void)
{
  printf("Menu:\n");
  printf("q: quits emulator\n"
	 "a: does something\n"
	 "b: does another thing\n"
	 "z: ..................\n");
}

static void menu_command_process(char c)
{
  switch(c) {
  case 'q':
    done = true;
    break;
  default:
    printf("menu_command_process: default\n");
  }
}

int cpu_emu_run(void)
{
  int ret;
  char c;

  ret = pthread_create(&main_thread, NULL, cpu_emu_main, NULL);
  assert(ret == 0);

  ret = pthread_create(&int_thread, NULL, cpu_emu_int, NULL);
  assert(ret == 0);

  do {
    menu_print();
    c = getc(stdin);
    menu_command_process(c);
  } while (c != 'q');

  ret = pthread_join(main_thread, NULL);
  assert(ret == 0);

  ret = pthread_join(int_thread, NULL);
  assert(ret == 0);

  ret = pthread_mutex_destroy(&int_onoff);
  assert(ret == 0);

  return ret;
}
