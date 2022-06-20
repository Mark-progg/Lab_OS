#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <csignal>

using std::cout;
using std::endl;

int flag;
pthread_t thread, thread_open;
int fd;
char pathname[]="/tmp/Path";

void sig_handler(int signo)
{
  cout<< "signal SIGPIPE received" <<endl;
}

void *thread_func(void *)
{
  char bufer[32];
  struct hostent *hostaddr;
  int result;

  while(flag != 0)
  {
    hostaddr = gethostbyname("etu.ru");
    if(hostaddr == nullptr)
    {
      herror("gethostbyname()");
    }
    else
    {
      sprintf(bufer, "%s", inet_ntoa(*(struct in_addr *)hostaddr->h_addr_list[0]));
      result = write(fd, bufer, sizeof(bufer));
      cout<<bufer<<endl;
      if(result == -1)
      {
        cout<<"Error write()"<<endl;
      }
    }
    sleep(2);
  }
  pthread_exit((void *)0);
}

void *func_fopen(void *)
{
  int result;
  while(flag != 0)
  {
    if ((fd = open(pathname,O_WRONLY|O_NONBLOCK))==-1)
    {
      cout<<"Error open()"<<endl;
      sleep(2);
    }
    else
    {
        result  = pthread_create(&thread,nullptr,thread_func,nullptr);
        if(result != 0)
        {
          cout<<"Error pthread_create(&thread,,);"<<endl;
        }
        pthread_exit((void*)0);
    }

  }
  pthread_exit((void*)1);

}

int main()
{
  int result, exit_code;
  signal(SIGPIPE,sig_handler);
  flag = 1;
  result = pthread_create(&thread_open,nullptr,func_fopen,nullptr);
  if(result != 0)
  {
    cout<<"Error pthread_create(&thread_open,,)"<<endl;
    return 1;
  }
  getchar();
  flag = 0;
  result = pthread_join(thread,(void**)&exit_code);
  if(result != 0)
  {
    cout<<"Error pthread_join(thread,)"<<endl;
    return 2;
  }
  cout<<"exit_code = "<<exit_code<<endl;
  close(fd);
  unlink(pathname);

  return 0;
}
