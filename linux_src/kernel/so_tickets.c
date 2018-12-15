#include <linux/so_tickets.h>

int so_count_processes(void){
  int count = 0;
  struct task_struct *PCB;
  
  for_each_process(PCB){
    char process_name = PCB->comm[0];

    if(process_name == 'a' ||
       process_name == 'p' ||
       process_name == 'c' ||
       process_name == 'b' ||
       process_name == 'q' ){
      count ++;
    }
  }
  printk("Number of processes: %d.\n",count);
  return count; 
}

int so_find_victim(int priority){
  struct task_struct *PCB;

  printk("Looking for a victim\n");

  for_each_process(PCB){ 
    char process_name = PCB->comm [0];

    if(process_name == 'a' ||
       process_name == 'p' ||
       process_name == 'c' ||
       process_name == 'b' ||
       process_name == 'q' ){

      if(PCB->priority > priority){
	printk("Victim found pid: %d, priority: %d", PCB->pid, PCB->priority);
	kill_pid(find_vpid(PCB->pid), SIGKILL, 1);
	return 0;
      }
    }
  }
  return 1;
}

void so_count_time(void){
  struct task_struct *PCB;

   for_each_process(PCB){

    char process_name = PCB->comm[0];

    if(process_name == 'a' ||
       process_name == 'p' ||
       process_name == 'c' ||
       process_name == 'b' ||
       process_name == 'q' ||
       process_name == 'l' ){

       if(cputime_to_secs( PCB->utime + PCB->stime) >= MAX_TIME){
	printk("Exceeded maximum time for pid %d.",PCB->pid);
	kill_pid(find_vpid(PCB->pid), SIGKILL, 1);
       }
     }
   }
}

void so_new_process(struct task_struct *PCB){
  int priority = 0;
  char name = PCB->comm[0];
  
  switch(name){    
  case 'a':
    priority = 1;
    break;
  case 'l':
  case 'p':
    priority = 2;
    break;
  case 'c':
    priority = 3;
    break;
  case 'b':
    priority = 4;
    break;
  case 'q':
    priority = 5;
    break;
  default :
    priority = 0;
    break;
  }
  
  if((so_count_processes() > N) && (so_find_victim(priority) == 1)){
      kill_pid(find_vpid(PCB->pid), SIGKILL, 1);
      return;
  }

  if((priority != 0) && (so_insert_process(PCB, priority) == 0)){
    printk("Process %d added to queue", PCB->pid);
    return;
  }
  
  printk("Process %d not added to queue", PCB->pid);
  
}

int so_insert_process(struct task_struct *PCB, int priority){
  
  struct sched_param queue;
  int queue_alg;

  switch(priority){
  case 1:
    queue.sched_priority = ADMIN;
    queue_alg = SCHED_RR;
    break;
  case 2:
    queue.sched_priority = PAYMENT;
    queue_alg = SCHED_FIFO;
    break;
  case 3:
    queue.sched_priority = CANCELATION;
    queue_alg = SCHED_FIFO;
    break;
  case 4:
    queue.sched_priority = BOOKING;
    queue_alg = SCHED_FIFO;
    break;
  case 5:
    queue.sched_priority = QUERY;
    queue_alg = SCHED_RR;
    break;
  default:
    return 0;
  }

  return sched_setscheduler(PCB, queue_alg, &queue);

}
