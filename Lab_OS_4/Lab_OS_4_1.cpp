#include <iostream>
#include <unistd.h>
#include <stdlib.h>
//#include <sys/types>

using std::cout;
using std::endl;

int main(int argc, char const *argv[])
{
  cout<<"Prog1 current id: "<<getpid()<<endl;
  cout<<"Prog1 parrent id: "<<getppid()<<endl;

    if(argc > 1)
    {
      for(int i=0;i<argc;i++)
      {
        cout<<argv[i]<<endl;
        sleep(1);
      }
    }

    exit(123);
}
