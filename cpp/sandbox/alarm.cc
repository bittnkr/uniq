// #include <stdio.h>
#include "std.h"
// #include<signal.h>
#include<unistd.h>
 
int COUNTER = 0;
bool STOP = false;

void sig_handler(int signum){ 
  
  if (signum==SIGALRM) {
    COUNTER++;
    // if(COUNTER%10000==0) 
    cout<<"SIGALRM: "<< COUNTER <<"\n";
  }

  if(signum == SIGINT){ // signal handler for SIGINT
    cout<<"SIGALRM!\n";;
    STOP = true;
  }
 
  if(COUNTER==10) alarm(0);  // Stop the alarm

  // ualarm(1,0);  // Scheduled alarm for the next 1us
}
  
int main(){
 
  signal(SIGALRM,sig_handler); // Register signal handler
  signal(SIGINT,sig_handler); // Register signal handler for SIGINT
  // sig_handler(0);
  ualarm(1,200000);  // Scheduled alarm after 1us and each 0,1s

  for(int i=1;!STOP;i++){ 
    printf("MAIN: %d\n",i);
    sleep(1);  // Delay for 1 second, or until a signal happens
  }
  return 0;
}