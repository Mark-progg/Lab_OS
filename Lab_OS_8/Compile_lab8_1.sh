g++ -c Lab_OS_8_1.cpp
g++ -o Lab_OS_8_1 Lab_OS_8_1.o -lpthread
sudo setcap cap_sys_resource=eip Lab_OS_8_1
