#include <linux/so_tickets.h>

int so_count_processes(void){
  int count = 0;
  struct task_struct *PCB;
  for_each_process(PCB){
    if(PCB->priority != 0){
      count ++;
    }
  }
  printk("Number of processes: %d.\n",count);
  return count;
}

int so_find_victim(int new_priority){
  struct task_struct *PCB;
  
  printk("Looking for a victim\n");
  
  for_each_process(PCB){
    int target_priority = PCB->priority;
    
    if(target_priority != 0 && target_priority < new_priority){
      printk("Victim found pid: %d, %s, priority: %d\n", PCB->pid, PCB->comm, target_priority);
      kill_pid(find_vpid(PCB->pid), SIGKILL, 1);
      return 0;
    }
  }
  return 1;
}

void so_count_time(void){
  struct task_struct *PCB;
  for_each_process(PCB){
    
    if((PCB->priority != 0) && (cputime_to_secs(PCB->utime + PCB->stime) >= MAX_TIME)){
      printk("Exceeded maximum time for pid %d.\n",PCB->pid);
      printk("Killing pid: %d, kill code :%d, name: %s",PCB->pid ,kill_pid(find_vpid(PCB->pid), SIGKILL, 1), PCB->comm);
    }
  }
}

void so_new_process(struct task_struct *PCB){
  int priority = 0;
  
  if(strcmp(PCB->comm, "admin") == 0){
    priority = 1;
  } else if((strcmp(PCB->comm, "payment") == 0) || (strcmp(PCB->comm, "payment_long") == 0)){
    priority = 2;
  } else if(strcmp(PCB->comm, "annulment") == 0){
    priority = 3;
  } else if(strcmp(PCB->comm, "booking") == 0){
    priority = 4;
  } else if(strcmp(PCB->comm, "query") == 0){
    priority = 5;
  }else{
    return;
  }

  printk("Process request name: %s , pid: %d \n", PCB->comm, PCB->pid);
  if((so_count_processes() >= N) && (so_find_victim(priority) == 1)){

    printk("Rejecting process pid %d ...\n", PCB->pid);
    printk("Sigkill says %d", kill_pid(find_vpid(PCB->pid), SIGKILL, 1));
    printk("Process not added to queue");
    return;

  }else if((so_insert_process(PCB, priority) == 0)){

      printk("Process %d added to queue\n", PCB->pid);
      return;

  }else{

    printk("Something get wrong with incoming process %s %d", PCB->comm, PCB->pid);
    return;
  }

  printk("ERROR");
}

int so_insert_process(struct task_struct *PCB, int priority){    
  struct sched_param queue;
  int queue_alg;
  
  printk("Inserting process name: %s, pid: %d\n", PCB->comm, PCB->pid);
  PCB ->priority = priority;

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
    printk("Something get wrong with priorities");
    return -2;
  }
  
  return sched_setscheduler(PCB, queue_alg, &queue);
}
