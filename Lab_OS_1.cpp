#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>

using std::cout;
using std::endl;

void* thread_func1(void *arg)
{
  cout<<"In potok 1"<<endl;
  while((*(int *) arg) != 0)
  {
    cout<<"Thread 1 is working "<<endl;
    sleep(2);
  }
  pthread_exit((void *)102);
}

void* thread_func2(void *arg)
{
  cout<<"In potok 2"<<endl;
  while((*(int *) arg) != 0)
  {
    cout<<"Thread 2 is working "<<endl;
    sleep(2);
  }
  pthread_exit((void *)202);
}

int main()
{
  int flag1,flag2, exit1, exit2, result;
  pthread_t thread1, thread2;

  flag1 = 2;
  flag2 = 3;

  result = pthread_create(&thread1, NULL, thread_func1, &flag1);
  if(result==0)
  {
    cout<<"thread1 created"<<endl;
  }
  else
  {
    cout<<"Error thread1 created"<<endl;
  }

  result = pthread_create(&thread2, NULL, thread_func2, &flag2);
  if(result==0)
  {
    cout<<"thread2 created"<<endl;
  }
  else
  {
    cout<<"Error thread2 created"<<endl;
  }

  getchar();
  //sleep(6);

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


  return 0;
}
