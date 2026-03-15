#ifndef NOTIFICATION_H
#define NOTIFICATION_H
#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#include <stdio.h>
#define TRAY_ICON_ID 1
#define WM_TRAYICON (WM_USER + 1)
#define WM_TRAY_MENU (WM_USER + 2)
#define ID_EXIT 1
#define ID_OPEN_FILE 2

extern LPCWSTR global_dir; // declare the global variable to store the directory path

LRESULT CALLBACK Wndproc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam);
void handle_icons_popup(WCHAR *buffer);
BOOL set_hotkey_to_terminate(HWND hwnd);
void save_to_file(HANDLE file,WCHAR  *buffer,DWORD written_bytes);


#endif