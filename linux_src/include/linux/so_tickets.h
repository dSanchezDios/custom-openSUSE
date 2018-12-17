#ifndef _SO_TICKETS_H
#define _SO_TICKETS_H

#include <linux/unistd.h>
#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/syscalls.h>
#include <linux/time.h>
#include <linux/times.h>
#include <linux/timer.h>


#define MAX_TIME 10

#define ADMIN 96
#define PAYMENT 95
#define CANCELATION 94
#define BOOKING 93
#define QUERY 92

#define N 10

int so_count_processes(void);

int so_find_victim(int priority);

void so_count_time(void);

void so_new_process(struct task_struct *pcb);

int so_insert_process(struct task_struct *pcb, int priority);

#endif
