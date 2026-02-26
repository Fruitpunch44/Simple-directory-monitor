#include "test.h"
#include "notification.h"

LRESULT CALLBACK Wndproc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam){
    switch(message){
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
        return DefWindowProc(hwnd,message,wparam,lparam);
    }
    return 0;
}

int APIENTRY WinMain(HINSTANCE hINSTANCE,HINSTANCE hPrevInstance,LPSTR lpcmdline,int nShowcmd){
    if(!SetConsoleCtrlHandler(console_handler,TRUE)){
        fprintf(stderr,"unable to set control handler %lu",GetLastError());
        exit(EXIT_FAILURE);
    }
LPCSTR directory= lpcmdline;
if(!directory){
    MessageBoxA(NULL,"PLEASE PASS THE DIRECTORY PATH AS A COMMAND LINE ARGUMENT","ERROR",MB_OK|MB_ICONERROR);
    return 0;
}

char classname[]= "windows test";
WNDCLASS wc ={0};
wc.lpfnWndProc = Wndproc;
wc.hInstance = hINSTANCE;
wc.lpszClassName = classname;
if(!RegisterClass(&wc)){
    MessageBox(NULL,"failed to register class","error",MB_OK|MB_ICONERROR);
    return 0;
}
HWND hwnd ;
hwnd =CreateWindowEx(
    0,
    wc.lpszClassName,
    "test",
    0,
    0,0,0,0,
    NULL,
    NULL,
    hINSTANCE,
    NULL);

    HANDLE hThread = CreateThread(NULL,0,
        watch_directory_thread,
        (LPVOID)directory,
        0,NULL);

    if(!hThread){
        MessageBox(NULL,"failed to create thread","error",MB_OK|MB_ICONERROR);
        return 0;
    }

    NOTIFYICONDATA nid ={0};
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon=LoadIcon(NULL,IDI_INFORMATION);
    nid.dwInfoFlags = NIIF_INFO;
    lstrcpy(nid.szTip,"Directory Monitor");
    lstrcpy(nid.szInfoTitle,"Directory Monitor");
    lstrcpy(nid.szInfo,"Monitoring directory for changes...");
    Shell_NotifyIcon(NIM_MODIFY,&nid);
    Shell_NotifyIcon(NIM_ADD,&nid);


    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Shell_NotifyIcon(NIM_DELETE,&nid);
    return 0;
}