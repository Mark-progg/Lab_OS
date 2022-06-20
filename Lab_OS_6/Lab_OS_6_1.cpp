#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdio>
#include <fstream>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

using std::cout;
using std::flush;
using std::endl;

int flag;
sem_t *id_sem_record;
sem_t *id_sem_reading;
std::ofstream f;
void *adr;

char sem_record_name[] = "/Semofor_record";
char sem_reading_name[] = "/Semofor_write";

char bufer[32];

void *thread_func(void *)
{
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
      cout<<bufer<<endl;
      memcpy(adr, bufer, sizeof(bufer));
      result = sem_post(id_sem_record);
      if(result == -1)
      {
        cout<<"Error sem_post()"<<endl;
      }

      result = sem_wait(id_sem_reading);
      if(result == -1)
      {
        cout<<"Error sem_post()"<<endl;
      }
      sleep(2);
    }
  }
  pthread_exit((void*)0);
}

int main()
{
  int exit,result, proj_id, shmid;
  pthread_t thread;
  key_t key;
  proj_id = 1;
  flag = 1;

  f.open("Memory", std::ios::out|std::ios::app);
  sleep(1);
  key = ftok("Memory", proj_id);
  if(key == -1)
  {
    cout<<"Error ftok()"<<endl;
    return 1;
  }

  shmid = shmget(key, sizeof(bufer), 0644|IPC_CREAT);
  if(shmid == -1)
  {
    cout<<"Error shmget()"<<endl;
    return 2;
  }

  adr = shmat(shmid, NULL, 0644|IPC_CREAT);
  if(adr == (void *) -1)
  {
    cout<<"Error shmat()"<<endl;
    return 3;
  }

  id_sem_record = sem_open(sem_record_name, O_CREAT, 0644, 0);
  if(id_sem_record == SEM_FAILED || id_sem_record == NULL)
  {
    cout<<"Error sem_record_open"<<endl;
    return 4;
  }

  id_sem_reading = sem_open(sem_reading_name, O_CREAT, 0644, 0);
  if(id_sem_reading == SEM_FAILED || id_sem_reading == NULL)
  {
    cout<<"Error sem_reading_open"<<endl;
    return 5;
  }

  result = pthread_create(&thread,nullptr,thread_func,nullptr);
  if(result != 0)
  {
    cout<<"Error create pthread"<<endl;
    return 6;
  }

  getchar();
  flag = 0;

  result = pthread_join(thread,(void **)&exit);
  if(result != 0)
  {
    cout<<"Error pthread_join"<<endl;
    return 7;
  }
  result = sem_close(id_sem_record);
  if(result != 0)
  {
    cout<<"Error pthread_join"<<endl;
    return 7;
  }
  result = sem_unlink(sem_record_name);
  if(result != 0)
  {
    cout<<"Error pthread_join"<<endl;
    return 7;
  }
  result = sem_close(id_sem_reading);
  if(result != 0)
  {
    cout<<"Error pthread_join"<<endl;
    return 7;
  }
  result = sem_unlink(sem_reading_name);
  if(result != 0)
  {
    cout<<"Error pthread_join"<<endl;
    return 7;
  }
  shmdt(adr);
  shmctl(shmid, IPC_RMID, NULL);
  f.close();
  return 0;
}
