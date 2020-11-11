#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

int getNumber() {
  int num; 
  printf("\nPlease enter a number : ");
  scanf("%d", &num);

  return num;
}
 
int main(void) {
  int fd[2], m=0, flag=0;

  void sigint_handler(int sig);

  if (signal(SIGINT, sigint_handler) == SIG_ERR){
     perror("signal");
     exit(1);
  }
  pipe(fd);
  pid_t pid = fork();
  if(pid > 0) {
    close(0);
    close(fd[1]);
    dup(fd[0]);

    int numParent;
    size_t readBytes = read(fd[0], &numParent, sizeof(numParent));

    wait(NULL);

    m=numParent/2;
    for(int i=2;i<=m;i++)
    {
     if(numParent%i==0)
     {
       printf("Number is not prime\n");
       flag=1;
       break;
     }
    }
    if(flag==0)
      printf("Number is prime\n");
}
  else if(pid == 0) {
    int num = getNumber();
    close(1);
    close(fd[0]);
    dup(fd[1]);

    write(fd[1], &num, sizeof(num));
    exit(EXIT_SUCCESS);
  }

  return EXIT_SUCCESS;
}
void sigint_handler(int sig)
{
  printf("Jangan tekan control C!\n");
}
