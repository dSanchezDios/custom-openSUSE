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

#define DEBUG_MODE 1
#define MAX_OF_PROCESSES 10
#define MAX_TIME 15
#define PRIORITY_QUEUE 97
#define ADMIN "admin"
#define PAYMENT "payment"
#define PAYMENT "payment_long"
#define CANCELATION "cancelation"
#define BOOKING "booking"
#define QUERY "query"

int so_count_processes(void);

int so_find_victim(int priority);

void so_count_time(void);

void so_new_process(struct task_struct *pcb);

int so_insert_process(struct task_struct *pcb, int priority);

void logger(char *log);

int so_get_priority(char *comm);

#endif
