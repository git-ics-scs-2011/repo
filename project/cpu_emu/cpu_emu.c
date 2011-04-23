#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <pthread.h>
#include "cpu_emu.h"

static pthread_t main_thread;
static pthread_t int_thread;
static pthread_mutex_t int_onoff = PTHREAD_MUTEX_INITIALIZER;
static bool done = false;
static int (*_p_main)(int argc, char **argv);
static void (*_p_int)(struct cpu_emu_sfr *regs);

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

  while (!done) {
    ret = _p_main(0, NULL);
    assert(ret == 0);
  }

  return NULL;
}

static void* cpu_emu_int(void* arg)
{
  cpu_emu_int_off();

  while (!done) {
    
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

  ret = pthread_create(&int_thread, NULL, cpu_emu_main, NULL);
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
