
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>

using std::cout;
using std::endl;

int flag1, flag2;
int filedes[2];

void* thread_func1(void *arg)
{
  cout<<"potok 1 start"<<endl;
  char bufer[32];
  struct hostent *hostaddr;
  while(flag1 != 0)
  {
    hostaddr = gethostbyname("etu.ru");
    if(hostaddr == nullptr)
    {
      herror("gethostbyname()");
    }
    else
    {
      sprintf(bufer, "%s", inet_ntoa(*(struct in_addr *)hostaddr->h_addr_list[0]));
      write(filedes[1], bufer, sizeof(bufer));
    }
    sleep(2);
  }
  cout<<"potok 1 end"<<endl;
  pthread_exit((void *)102);
}

void* thread_func2(void *arg)
{
  cout<<"potok 2 start"<<endl;
  char bufer[32];
  while(flag1 != 0)
  {
    memset(bufer, 0, sizeof(bufer));
    read(filedes[0], bufer, sizeof(bufer));
    cout << bufer << endl;
  }
  cout<<"potok 2 end"<<endl;
  pthread_exit((void *)202);
}

int main()
{
  cout<<"Programm start"<<endl;

  int exit1, exit2, result;
  pthread_t thread1, thread2;

  result = pipe(filedes);
  if(result == 0)
  {
    cout << "pipe() worked" << endl;
  }
  else
  {
    perror("Error pipe()");
  }

  flag1 = 2;
  flag2 = 3;

  result = pthread_create(&thread1, NULL, thread_func1, NULL);
  if(result == 0)
  {
    cout<<"thread1 created"<<endl;
  }
  else
  {
    cout<<"Error thread1 created"<<endl;
  }

  result = pthread_create(&thread2, NULL, thread_func2, NULL);
  if(result == 0)
  {
    cout<<"thread2 created"<<endl;
  }
  else
  {
    cout<<"Error thread2 created"<<endl;
  }

  cout<<"Waiting getchar()"<<endl;
  getchar();
  cout<<"getchar() worked"<<endl;

  flag1 = 0;
  flag2 = 0;

  result = pthread_join(thread1, (void**)&exit1);
  if(result != 0)
  {
    cout<<"Error join thread1"<<endl;
    return 1;
  }

  result = pthread_join(thread2, (void**)&exit2);
  if(result != 0)
  {
    cout<<"Error join thread2"<<endl;
    return 2;
  }

  close(filedes[0]);
  close(filedes[1]);

  cout<<"exit code thread1 is "<<exit1<<endl;
  cout<<"exit code thread2 is "<<exit2<<endl;

  cout<<"Programm end"<<endl;
  return 0;
}
