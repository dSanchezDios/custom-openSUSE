#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>

void test_rr_queries(void);
void test_long_process(void);
void test_rr_admins(void);
void test_fifo_payments(void);
void test_fifo_annulments(void);
void test_payments_not_query(void);
void test_annulment_kills_query(void);
void test_payment_kills_annulment(void);
void test_admin_kills_payment(void);
void test_annulment_not_kill_payment(void);
void test_generate_every_process(void);
void generatePayment(int number);
void generatePaymentLong(int number);
void generateAnnulment(int number);
void generateQuery(int number);
void generateBooking(int number);
void generateAdmin(int number);
void generateProcess(char name[], int number);

int main(int argc, char *argv[]){
  int test_number = 0;
  struct sched_param node;
    
  test_number=atoi(argv[1]);
  node.sched_priority=99;

  if(sched_setscheduler(0,SCHED_FIFO, &node)!=0){
    printf("Error with testing node\n");
  }

  switch(test_number) {
  case 1:
    test_rr_queries();
    break;
  case 2:
    test_long_process();
    break;
  case 3:
    test_rr_admins();
    break;
  case 4:
    test_fifo_payments();
    break;
  case 5:
    test_fifo_annulments();
    break;
  case 6:
    test_payments_not_query();
    break;
  case 7:
    test_annulment_kills_query();
    break;
  case 8:
    test_payment_kills_annulment();
    break;
  case 9:
    test_admin_kills_payment();
    break;
  case 10:
    test_annulment_not_kill_payment();
    break;
  case 11:
    test_generate_every_process();
    break;
  default:
    printf("Se debe seleccionar el número de prueba\n");
    return 0;
  }
  printf("Fin prueba\n");
  return 0;
}

void test_rr_queries(void){
  int i = 0;
  while(i<5){
    generateQuery(i);
    i++;
  }
  return;
}

void test_long_process(){
  generatePaymentLong(1);
  return;
}

void test_rr_admins(){
  int i = 0;
  while(i<5){
    generateAdmin(i);
    i++;
  }
  return;
}

void test_fifo_payments(){
  int i = 0;
  while(i<5){  
    generatePayment(i);  
    i++;
  }
  return;
}

void test_fifo_annulments(){
  int i = 0;
  while(i<5){
    generateAnnulment(i);
    i++;
  }
  return;
}

void test_payments_not_query(){
  int i = 0;
  while(i<10){
    generatePayment(i);
    i++;
  }
  generateQuery(1);
  return;
}

void test_annulment_kills_query(){
  int i = 0;
  generateQuery(1);
  while(i<9){
    generatePayment(i);
    i++;
  }
  generateAnnulment(1);
  return;
}

void test_payment_kills_annulment(){
  int i = 0;
  generateAnnulment(1);
  while(i<9){
    generatePayment(i);
    i++;
  }
  generatePayment(1);
  return;
}

void test_admin_kills_payment(){
  int i = 0;
  generatePayment(1);
  while(i<9){
    generateAdmin(i);
    i++;
  }
  generateAdmin(1);
  return;
}

void test_annulment_not_kill_payment(){
  int i = 0;
  while(i<10){
    generatePayment(i);
    i++;
  }
  generateAnnulment(1);
  return;
}

void test_generate_every_process(){
  generatePayment(1);
  generateAnnulment(1);
  generateQuery(1);
  generateBooking(1);
  generateAdmin(1);
  return;
}

void generateAdmin(int number){
  generateProcess("admin", number);
  return;
}

void generatePayment(int number){
  generateProcess("payment", number);
  return;
}

void generateAnnulment(int number){
  generateProcess("annulment", number);
  return;
}

void generateBooking(int number){
  generateProcess("booking", number);
  return;
}

void generateQuery(int number){
  generateProcess("query", number);
  return;
}

void generatePaymentLong(int number){
  generateProcess("payment_long", number);
  return;
}

void generateProcess(char name[], int number){
  int error = fork();
  if(error<0) {
    printf("Error creating %s number %d\n", name, number);
    return;
  }
  if((error==0)&&(execve(name,NULL, NULL)<0)) {
    printf("Error executing %s number %d\n",name, number);
    return;
  }
  printf("Created %s number %d\n", name, number);
  return;
}
