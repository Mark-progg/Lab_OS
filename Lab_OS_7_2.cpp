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

using std::cout;
using std::endl;

int flag;
int fd;
char pathname[]="/tmp/Path";

void *thread_func(void *)
{
  char bufer[32];
  int result;
  while(flag != 0)
  {
    sleep(2);
    memset(bufer, 0, sizeof(bufer));
    result = read(fd, bufer, sizeof(bufer));
    if(result == -1)
    {
      cout<<"Error read()"<<endl;
    }
    else if(result == 0)
    {
      cout<<"buffer empty"<<endl;
    }
    else
    {
      cout << bufer << endl;
    }
  }
  pthread_exit((void *)0);
}

int main()
{
  int result, exit_code;
  pthread_t thread;
  flag = 1;
  result = mkfifo(pathname, 0644);
  if(result != 0)
  {
    cout<<"Error mkfifo()"<<endl;
    return 1;
  }
  fd = open(pathname, O_RDONLY|O_NONBLOCK);
  if (fd == -1)
  {
    cout<<"Error open()"<<endl;
    return 2;
  }
  result = pthread_create(&thread,nullptr,thread_func,nullptr);
  if(result != 0)
  {
    cout<<"Error pthread_create(&thread)"<<endl;
    return 3;
  }
  getchar();
  flag = 0;
  result = pthread_join(thread,(void**)&exit_code);
  if(result != 0)
  {
    cout<<"Error pthread_join(thread,)"<<endl;
    return 4;
  }
  cout<<"exit_code = "<<exit_code<<endl;
  close(fd);
  unlink(pathname);

  return 0;
}
