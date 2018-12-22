#include <stdio.h>
#include <unistd.h>

int main(){
  int i;	
  int j;
  printf("Executing admin process %d\n", getpid());
    
  for( i=0;i<2000;i++){
    for( j=0;j<2000000;j++){
      
    }		
  }	
  printf("End of process admin %d\n", getpid());	
  return 0;
}
