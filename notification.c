#include <windows.h>
#include <shellapi.h>
#include <commctrl.h>

#define TRAY_ICON_ID 1
#define WM_TRAYICON (WM_USER + 1)

//create a window
LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam){
    if(nCode == HC_ACTION){
        KBDLLHOOKSTRUCT *kbp = (KBDLLHOOKSTRUCT*) lParam;
        char buff[40];
        switch(wParam){
            case WM_KEYDOWN:   
                if(kbp->vkCode == 'c'){
                    PostQuitMessage(0);
                    exit(0);
        }
        default:
        return CallNextHookEx(NULL,nCode,wParam,lParam);
    }

    }
    return CallNextHookEx(NULL,nCode,wParam,lParam);
}

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

    NOTIFYICONDATA nid ={0};
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon=LoadIcon(NULL,IDI_INFORMATION);
    nid.dwInfoFlags = NIIF_INFO;
    lstrcpy(nid.szTip,"THIS DOES NOT DO ANYTHING");
    lstrcpy(nid.szInfoTitle,"test notification");
    lstrcpy(nid.szInfo,"this is a test i do not how to advance");
    Shell_NotifyIcon(NIM_MODIFY,&nid);
    Shell_NotifyIcon(NIM_ADD,&nid);

    HINSTANCE instance = GetModuleHandle(NULL);
    HHOOK keys= SetWindowsHookEx(WH_KEYBOARD_LL,KeyboardProc,instance,0);

    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(keys);
    Shell_NotifyIcon(NIM_DELETE,&nid);
    return 0;

}