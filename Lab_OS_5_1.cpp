#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <cstdio>
#include <fstream>
#include <semaphore.h>
#include <fcntl.h>

using std::cout;
using std::flush;
using std::endl;

int flag;
sem_t *id_sem;
std::ofstream f;
char sem_name[] = "/Semofor";

void *thread_func(void *)
{
  char s = '1';
  while(flag != 0)
  {
    sem_wait(id_sem);
    for (int i = 0; i<5; i++)
    {
      cout << s << flush;
      f << s << flush;
      sleep(1);
    }
    sem_post(id_sem);
    sleep(1);
  }
  pthread_exit((void*)0);
}

int main()
{
  int exit,result;
  flag = 1;

  id_sem = sem_open(sem_name, O_CREAT, 0644, 1);
  if(id_sem == SEM_FAILED || id_sem == NULL)
  {
    cout<<"Error sem_open"<<endl;
    return 1;
  }

  pthread_t thread;
  result = pthread_create(&thread,nullptr,thread_func,nullptr);
  if(result != 0)
  {
    cout<<"Error create pthread"<<endl;
    return 2;
  }

  f.open("text",std::ios::out|std::ios::app);

  getchar();
  flag = 0;

  result = pthread_join(thread,(void **)&exit);
  if(result != 0)
  {
    cout<<"Error pthread_join"<<endl;
    return 3;
  }
  sem_close(id_sem);
  sem_unlink(sem_name);
  f.close();
  return 0;
}
