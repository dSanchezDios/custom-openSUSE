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
    char log[];
    
    log = ("Looking for a victim...\n");

    for_each_process(PCB){
        if(PCB->priority > new_priority){
            log += concat("Victim found pid:",PCB->pid);
            pid_victim = PCB->pid;
            kill_code = kill_pid(find_vpid(pid_victim), SIGKILL, 1);
            break;
        }
    }
    log += concat("Kill code :", kill_code);
    
    if(kill_code != 0 && kill_code != 1){
        log += ("Problem killing victim");
        kill_code = 1;
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
            logger(concat("Exceeded max of time for pid ", PCB->pid));
            logger(concat("Killing pid:", PCB->pid, " kill code : ", kill_code ));
        }
    }
}

void so_new_process(struct task_struct *PCB){
    char log[];
    int priority = so_get_priority(PCB->comm);
    
    if(priority == 0){
        return;
    }
    
    log = concat("New process request: ", PCB->comm, ", pid: ", PCB->pid);

    if((so_count_processes() >= MAX_OF_PROCESSES) && (so_find_victim(priority) == 1)){
        log += concat("Rejecting process with pid: ", PCB->pid);
        log += concat("Kill code: ", kill_pid(find_vpid(PCB->pid), SIGKILL, 1));
        log += concat("Process not added to queue");
    }else if((so_insert_process(PCB, priority) == 0)){
        log += concat("Process added to queue pid:", PCB->pid);
    }else{
        log += concat("Not added to queue pid: ", PCB->pid);
        log += concat("Kill code: ", kill_pid(find_vpid(PCB->pid), SIGKILL, 1));
    }

    logger(log);
}

int so_get_priority(char *comm){
    if(strcmp(comm, ADMIN) == 0){
        return 1;
    } else if((strcmp(comm, PAYMENT) == 0) || (strcmp(comm, PAYMENT_LONG) == 0)){
        return 2;
    } else if(strcmp(comm, ANNULMENT) == 0){
        return 3;
    } else if(strcmp(comm, BOOKING) == 0){
        return 4;
    } else if(strcmp(comm, QUERY) == 0){
        return 5;
    } else{
        return 0;
    }
}

int so_insert_process(struct task_struct *PCB, int priority){    
    struct sched_param queue;
    int queue_alg;
  
    PCB->priority = priority;

    switch(priority){
        case 1:
        case 5:
            queue_alg = SCHED_RR;
            break;
        case 2:
        case 3:
        case 4:
            queue_alg = SCHED_FIFO;
            break;
        default:
            return -1;
    }

    queue.sched_priority = (PRIORITY_QUEUE - priority);
    logger(concat("Inserting process: ", PCB-comm));
    return sched_setscheduler(PCB, queue_alg, &queue);
}

void logger(char *log){
    if(DEBUG_MODE == 1){
        printk("\n DEBUG:\n");
        printk(log);
        printk("\n");
    }
}
