# Multithreading in C
This is a project assigned in my CSCI 459: Foundations of Computer Networks class that introduced the idea of multithreading to send messages from a server. **Unfortunately, the following usage instructions likely won't work on your system**, but the final code and commands are available for those curious.

## To compile in LINUX:
```
	server: gcc server.c -o server.exe -lpthread
	client: gcc client.c -o client.exe
```
## To run in LINUX:
```
	server: ./server.exe (39000-39099)
	client (on a separate computer): ./client.exe (lab computer used (lab00-lab20) to run server).cs.ndsu.nodak.edu (number used to run server (39000-39099))
```
## Example of running statements:
```
	server (lab00.cs.ndsu.nodak.edu): ./server.exe 39000
	client (lab01.cs.ndsu.nodak.edu): ./client.exe lab00.cs.ndsu.nodak.edu 39000
```
## To exit functions:
```
	server: CTRL + C
	client: EXIT (then hit enter)
```
