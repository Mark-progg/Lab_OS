#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <semaphore.h>

using std::cout;
using std::endl;
using std::flush;

int flag1, flag2;
sem_t semofor;

void* thread_func1(void *arg)
{
  cout<<"potok 1 start"<<endl;
  int tmp;
  while(flag1 != 0)
  {
    tmp = sem_trywait(&semofor);
    if(tmp != 0)
    {
      perror(" Error sem_trywait_1()");
      sleep(1);
    }
    else
    {
    for(int i=1;i<4;i++)
      {
      cout<<"1"<<flush;
      sleep(1);
      }
    sem_post(&semofor);
    sleep(2);
    }
  }
  sleep(2);
  cout<<"potok 1 end"<<endl;
  pthread_exit((void *)102);
}

void* thread_func2(void *arg)
{
  sleep(1); //чтобы первый поток начал первым
  cout<<"potok 2 start"<<endl;
  int tmp;
  while(flag1 != 0)
  {
    tmp = sem_trywait(&semofor);
    if(tmp != 0)
    {
      perror(" Error sem_trywait_2()");
      sleep(1);
    }
    else
    {
    for(int i=1;i<4;i++)
      {
      cout<<"2"<<flush;
      sleep(1);
      }
    sem_post(&semofor);
    sleep(2);
    }
  }
  sleep(2);
  cout<<"potok 2 end"<<endl;
  pthread_exit((void *)202);
}

int main()
{
  cout<<"Programm start"<<endl;

  int exit1, exit2, result;
  pthread_t thread1, thread2;

  flag1 = 2;
  flag2 = 3;

  result = sem_init(&semofor, 0, 1);
  if(result==0)
  {
    cout<<"semofor created"<<endl;
  }
  else
  {
    cout<<"Error semofor created"<<endl;
  }

  result = pthread_create(&thread1, NULL, thread_func1, NULL);
  if(result==0)
  {
    cout<<"thread1 created"<<endl;
  }
  else
  {
    cout<<"Error thread1 created"<<endl;
  }

  result = pthread_create(&thread2, NULL, thread_func2, NULL);
  if(result==0)
  {
    cout<<"thread2 created"<<endl;
  }
  else
  {
    cout<<"Error thread2 created"<<endl;
  }
  //sleep(1);
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

  cout<<"exit code thread1 is "<<exit1<<endl;
  cout<<"exit code thread2 is "<<exit2<<endl;

  sem_destroy(&semofor);
  cout<<"Programm end"<<endl;
  return 0;
}
