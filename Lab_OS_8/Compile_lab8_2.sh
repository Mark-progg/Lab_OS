g++ -c Lab_OS_8_2.cpp
g++ -o Lab_OS_8_2 Lab_OS_8_2.o -lpthread
sudo setcap cap_sys_resource=eip Lab_OS_8_2
