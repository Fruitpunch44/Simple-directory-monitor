#ifndef TEST_H
#define TEST_H

#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<wchar.h>
#include<stddef.h>
#include<time.h>
#include<unistd.h>
#define MAX_PATH_SIZE 70
#define max_buffer_size 16384 //why this i don't know random size


BOOL WINAPI console_handler(DWORD signal);
void handle_signal(int signal);
char *return_current_time();
const char *file_actions(DWORD Action);
DWORD WINAPI watch_directory_thread(LPVOID lpParam);

#endif