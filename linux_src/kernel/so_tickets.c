#include <linux/so_tickets.h>

int so_count_processes(void){
  int count = 0;
  struct task_struct *PCB;

  for_each_process(PCB){
    if(PCB->priority != 0){
      count ++;
    }
  }

  logger(concat("Number of processes", count));

  return count;
}

int so_find_victim(int new_priority){
 struct task_struct *PCB;
 int target_priority;
 int kill_code = 1;
 int pid_victim = 0;
 char log[] = "Looking for a victim...\n"
  
 for_each_process(PCB){
    target_priority = PCB->priority;
    
    if(target_priority > new_priority){
      pid_victim = PCB->pid;
      kill_code = kill_pid(find_vpid(pid_victim), SIGKILL, 1);
      break;
    }
  }
     log += concat("Victim found pid:",PCB->pid ,"\n");
     log += concat"Kill code: ", kill_code,"n");
   }

  logger(log);
  return kill_code;
}

void so_count_time(void){
  struct task_struct *PCB;
  int kill_code = 1;

  for_each_process(PCB){    
    if((PCB->priority != 0) && (cputime_to_secs(PCB->utime + PCB->stime) >= MAX_TIME)){
       kill_code = kill_pid(find_vpid(PCB->pid), SIGKILL, 1);
       logger(concat("Exceeded max of time for pid ",PCB->pid));
       logger(concat("Killing pid:",PCB->pid ," kill code : ", kill_code ));
      }
   }
 }
}

void so_new_process(struct task_struct *PCB){
  int priority = 0;
  char log[];
  
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
 
  log = concat("New process request: ", PCB->comm, ", pid: ", PCB->pid);

  if((so_count_processes() >= N) && (so_find_victim(priority) == 1)){
    log += concat("Rejecting process pid ...\n", PCB->pid);
    log += concat("Sigkill says ", kill_pid(find_vpid(PCB->pid), SIGKILL, 1));
    log += concat("Process not added to queue");
  }else if((so_insert_process(PCB, priority) == 0)){
    log += concat("Process %d added to queue\n", PCB->pid);
  }else{
    log += concat("Something get wrong with incoming process ", PCB->comm, PCB->pid);
  }

  logger(log);
}

int so_insert_process(struct task_struct *PCB, int priority){    
  struct sched_param queue;
  char log[] = concat("Inserting process: ", PCB-comm);
  int queue_alg;
  
  PCB->priority = priority;

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
    logger("Problem with priorities\n");	
    return -1;
  }
  
  logger(log);
  return sched_setscheduler(PCB, queue_alg, &queue);
}

void logger(char *log){
  if(DEBUG_MODE == 1){
    printk("\n DEBUG:\n");
    printk(log);
    printk("\n");
  }
}

