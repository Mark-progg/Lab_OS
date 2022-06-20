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
#include <fstream>
//#include <linux/msg.h>
#include <sys/ipc.h>
#include <sys/msg.h>

using std::cout;
using std::endl;

struct msg_t {
long mtype;
char body[32];
};

struct msg_t message;

int flag;
int msqid;
std::ofstream f;

void *thread_func(void *)
{
  int result;

  while(flag != 0)
    {
      memset( message.body, 0, sizeof(message.body) );
      result = msgrcv(msqid, &message, sizeof(message), 1, 0);
      cout<<message.body<<endl;
      if(result == -1)
      {
        cout<<"Error msgrcv()"<<endl;
        perror("msgrcv");
      }
      sleep(1);
    }
    sleep(1);
  pthread_exit((void *)0);
}

int main()
{
  int result, exit_code, proj_id;
  pthread_t thread;
  key_t key;
  struct msqid_ds msqid_ds1;
  proj_id = 1;
  flag = 1;
  f.open("name", std::ios::out|std::ios::app);
  key = ftok("name", proj_id);
  if(key == -1)
  {
    cout<<"Error ftok()"<<endl;
    return 1;
  }
  msqid = msgget(key, IPC_CREAT|0644);
  if(msqid == -1)
  {
    cout<<"Error msgget()"<<endl;
    return 2;
  }
  result = msgctl(msqid, IPC_STAT, &msqid_ds1);
  if(result != 0)
  {
    cout<<"Error msgctl(IPC_STAT_1)"<<endl;
    return 322;
  }
  cout<<msqid_ds1.msg_qbytes<<endl;
  msqid_ds1.msg_qbytes = 17000;

  result = msgctl(msqid, IPC_SET, &msqid_ds1);
  if(result != 0)
  {
    cout<<"Error msgctl(IPC_SET)"<<endl;
    return 322;
  }
  result = msgctl(msqid, IPC_STAT, &msqid_ds1);
  if(result != 0)
  {
    cout<<"Error msgctl(IPC_STAT_2)"<<endl;
    return 322;
  }
  cout<<msqid_ds1.msg_qbytes<<endl;
  result = pthread_create(&thread,nullptr,thread_func,nullptr);
  if(result != 0)
  {
    cout<<"Error pthread_create()"<<endl;
    return 3;
  }
  getchar();
  flag = 0;

  result = pthread_join(thread,(void**)&exit_code);
  if(result != 0)
  {
    cout<<"Error pthread_join()"<<endl;
    return 4;
  }
  cout<<"exit_code = "<<exit_code<<endl;
  result = msgctl(msqid, IPC_RMID, &msqid_ds1);
  if(result != 0)
  {
    cout<<"Error msgctl()"<<endl;
    return 5;
  }
  return 0;
}
