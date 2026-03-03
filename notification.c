#include "notification.h"
#include "test.h"


LRESULT CALLBACK Wndproc(HWND hwnd,UINT message,WPARAM wparam,LPARAM lparam){
    switch(message){
        case WM_MBUTTONDBLCLK:
            PostQuitMessage(0);
            break;

        case WM_FILE_CHANGED:
            LPWSTR filename = (LPWSTR)wparam;
            DWORD action = (DWORD)lparam;

            WCHAR buffer[512];
            //pls note to self use ls when working with wide strings to prevent stress
            //sucks 
            swprintf(buffer, 512, L"File: %ls was %s at %s", filename, file_actions(action), return_current_time());
            NOTIFYICONDATAW nid ={0};
            nid.cbSize = sizeof(nid);
            nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO;
            nid.uCallbackMessage = WM_TRAYICON;
            nid.hIcon=LoadIcon(NULL,IDI_INFORMATION);
            nid.dwInfoFlags = NIIF_INFO;
            lstrcpyW(nid.szInfo,buffer);
            Shell_NotifyIconW(NIM_ADD,&nid);
            Shell_NotifyIconW(NIM_MODIFY,&nid);
            Shell_NotifyIconW(NIM_DELETE,&nid);
    
            MessageBoxW(hwnd, buffer, L"Directory Change", MB_OK | MB_ICONINFORMATION);

            

            free(filename); // free the duplicated string
            break;

        default:
        return DefWindowProc(hwnd,message,wparam,lparam);

    }
    return 0;
}

int APIENTRY wWinMain(HINSTANCE hINSTANCE,HINSTANCE hPrevInstance,LPWSTR lpcmdline,int nShowcmd){
    //parse args
    LPWSTR *args;
    int arg_count;
    args= CommandLineToArgvW(GetCommandLineW(),&arg_count);
    if(arg_count < 2){
        MessageBoxW(NULL,L"Usage: program.exe <directory_to_watch>",L"error",MB_OK|MB_ICONERROR);
        return 0;
    }
    //debugging purposes
    for(int i =0; i<arg_count;i++){
        MessageBoxW(NULL,args[i],L"arg",MB_OK);
    }
    global_dir = args[1]; // set the global variable to the directory path  

    LPCWSTR classname = L"windows test";
    WNDCLASSW wc ={0};
    wc.lpfnWndProc = Wndproc;
    wc.hInstance = hINSTANCE;
    wc.lpszClassName = classname;
    if(!RegisterClassW(&wc)){
        MessageBoxW(NULL,L"failed to register class",L"error",MB_OK|MB_ICONERROR);
        return 0;
    }
    HWND hwnd ;
    hwnd =CreateWindowExW(
    0,
    (LPCWSTR)wc.lpszClassName,
    L"test",
    0,
    0,0,0,0,
    NULL,
    NULL,
    hINSTANCE,
    NULL);


    NOTIFYICONDATAW nid ={0};
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_INFO;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon=LoadIcon(NULL,IDI_INFORMATION);
    nid.dwInfoFlags = NIIF_INFO;
    lstrcpyW(nid.szTip,L"Directory Monitor");
    lstrcpyW(nid.szInfoTitle,L"Directory Monitor");
    lstrcpyW(nid.szInfo,L"Monitoring directory for changes...");
    Shell_NotifyIconW(NIM_ADD,&nid);
    Shell_NotifyIconW(NIM_MODIFY,&nid);
    
    //when working with threads make sure to pass a valid handle to the function and not a local var
    //like how i was doing before, it was a mess
    HANDLE hThread = CreateThread(NULL,0,
        watch_directory_thread,
        (LPVOID)hwnd,
        0,NULL);

    if(!hThread){
        MessageBoxW(NULL,L"failed to create thread",L"error",MB_OK|MB_ICONERROR);
        return 0;
    }

    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);

        DispatchMessage(&msg);
    }
    Shell_NotifyIconW(NIM_DELETE,&nid);
    return 0;
}