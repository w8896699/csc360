Assignment 1
Csc 360
Haohang Li V00828818

This program works by accepting line input from the user, the input cmd is seperated by " "(space) and put into an array 
for futher internal command or external command determination. When bg is called a new Process is made in linklist and 
 Then when a process is finished, stopped, killed, or started, the program will response to these command. When bglist is 
 called, the program scan the linklist and shows the status, and the command and the address of process.

To test the program
gcc -o inf inf.c

 Using this program:
		1   make
		2   ./rsi
in cmd

then in PMan 
bg cat foo.txt
bg ./inf hahaha 20
bglist
bgstop pid
bgstart pid 