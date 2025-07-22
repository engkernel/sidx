#include "task.h"
#include "mm/buddy.h"
#include "utils/log.h"

static task_t* running_task;
static task_t task_main;
static task_t task_other;

extern void task_switch(registers_t* from, registers_t* to); 
void task_yield();
void task_new(task_t *task, void (*main)(), uint32_t flags, uint32_t* vm_dir);

static void other_main()
{
	kinfo("Hello multi tasking!!!");
	task_yield();
}

void task_init()
{
	asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(task_main.regs.cr3)::"%eax");
	asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m" (task_main.regs.eflags)::"%eax");
	task_new(&task_other, other_main, task_main.regs.eflags, (uint32_t*)task_main.regs.cr3);
	task_main.next = &task_other;
	task_other.next = &task_main;

	running_task = &task_main;
}

void task_new(task_t *task, void (*main)(), uint32_t flags, uint32_t* vm_dir)
{
	task->regs.eax = 0;
	task->regs.ebx = 0;
	task->regs.ecx = 0;
	task->regs.edx = 0;
	task->regs.esi = 0;
	task->regs.edi = 0;
	task->regs.eflags = flags;
	task->regs.eip = (uint32_t) main;
	task->regs.cr3 = (uint32_t) vm_dir;	
	task->regs.esp = (uint32_t) buddy_alloc(0);
	task->next = 0;
}

void task_yield()
{
	task_t* last = running_task;
	running_task = running_task->next;
	kinfo("running task %x", running_task->regs.esp);
	task_switch(&last->regs, &running_task->regs);
}

void do_it()
{
	kinfo("Switching to other task...");
	task_yield();
	kinfo("Returned to main task!");
}
