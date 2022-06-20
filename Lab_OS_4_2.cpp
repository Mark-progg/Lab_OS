#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
//#include <sys/types>

using std::cout;
using std::endl;

int main(int argc, char const *argv[])
{
  pid_t pid = fork();
  if(pid == 0)
  {
    execlp("Lab_OS_4_1", "1", "2", "3", nullptr);
    //дочерний
  }
  else if(pid == -1)
  {
    cout<<"Eror create child process"<<endl;//ошибка
  }
  else
  {
    int exit_code;
    cout<<"Prog2 parent id: "<<getppid()<<endl;
    cout<<"Prog2 current id: "<<getpid()<<endl;
    cout<<"Prog2 child id: "<<pid<<endl;

    while(waitpid(pid, &exit_code, WNOHANG) == 0)
    {
      usleep(500000);
      cout<<"Wait"<<endl;
    }
      if(WIFEXITED(exit_code))
      {
        cout<<"Exit code child process: "<<WEXITSTATUS(exit_code)<<endl;
      }
      else
      {
        cout<<"Error child process"<<endl;
      }
  }
    //родительский
}
