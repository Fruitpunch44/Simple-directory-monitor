#ifndef NOTIFICATION_H
#define NOTIFICATION_H
#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>
#define TRAY_ICON_ID 1
#define WM_TRAYICON (WM_USER + 1)

int APIENTRY WinMain(HINSTANCE hINSTANCE,HINSTANCE hPrevInstance,LPSTR lpcmdline,int nShowcmd);
LRESULT CALLBACK Wndproc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam);


#endif