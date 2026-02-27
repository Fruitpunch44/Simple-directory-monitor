#ifndef NOTIFICATION_H
#define NOTIFICATION_H
#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#include <stdio.h>
#define TRAY_ICON_ID 1
#define WM_TRAYICON (WM_USER + 1)

extern LPCWSTR global_dir; // declare the global variable to store the directory path

LRESULT CALLBACK Wndproc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam);


#endif