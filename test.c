#include "test.h"

LPCWSTR global_dir = NULL; // global variable to store the directory path

BOOL WINAPI console_handler(DWORD signal){
    if(signal == CTRL_C_EVENT){
        const char msg[] = "CTRL-C CAUGHT, exiting...\n";
        write(STDOUT_FILENO, msg, sizeof(msg) - 1);
        Beep(750,300);
        _exit(signal);
    }
    return TRUE;
}

char *return_current_time(){
    time_t now;
    time(&now);
    return ctime(&now);
}

const char *file_actions(DWORD Action){
    switch(Action){
        case FILE_ACTION_ADDED: return "ADDED";
        case FILE_ACTION_REMOVED:  return "REMOVED";
        case FILE_ACTION_MODIFIED: return "MODIFIED";
        case FILE_ACTION_RENAMED_NEW_NAME: return "RENAMED NEW NAME ";
        case FILE_ACTION_RENAMED_OLD_NAME: return "RENAMED OLD NAME";
        default: return "UNRECOGNIZED COMMMAND";
    }
}

DWORD WINAPI watch_directory_thread(LPVOID lpParam){
    HWND hwndmain=(HWND)lpParam;
    LPCWSTR directory = global_dir; // use the global variable to get the directory path
    HANDLE HDIR;
    HANDLE HEVENT;

    FILE_NOTIFY_INFORMATION *file_notification = NULL;
    DWORD bytes_returned;
    DWORD buffer_len;
    BOOL RET;
    PVOID file_notify_buffer;


    char filename[MAX_PATH_SIZE];

    HDIR= CreateFile(directory,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        NULL);
        
    if(HDIR ==INVALID_HANDLE_VALUE){
        fprintf(stderr,"unable to create handle %lu ",GetLastError());
        CloseHandle(HDIR);
        return EXIT_FAILURE;
    }
    HEVENT = CreateEvent(NULL,TRUE,FALSE,NULL);
    if(HEVENT == NULL){
        fprintf(stderr,"unable to create event %lu",GetLastError());
        CloseHandle(HDIR);
        return EXIT_FAILURE;
    }

    file_notify_buffer =malloc(max_buffer_size);
    if(!file_notify_buffer){
        fprintf(stderr,"unable to allocate buffer size");
        return EXIT_FAILURE;
    }

    fprintf(stdout,"watching directory %s for changes...\n",directory);

    while(1){
        ResetEvent(HEVENT);
        OVERLAPPED over ={0};
        over.hEvent = HEVENT;

        RET = ReadDirectoryChangesW(HDIR,
            file_notify_buffer,
            max_buffer_size,
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_SECURITY,
            NULL,
            &over,
            NULL);

        if(!RET){
            fprintf(stderr,"read directory changes failed %lu",GetLastError());
            CloseHandle(HDIR);
            break;
        }

        //get overlapped result
        RET = GetOverlappedResult(HDIR,
            &over,
            &bytes_returned,
            TRUE);
            
        if(!RET){
            fprintf(stderr,"get overlapped result failed %lu",GetLastError());
            CloseHandle(HDIR);
            break;
        }
        file_notification = (FILE_NOTIFY_INFORMATION *)file_notify_buffer;

        while(1){
            int filename_len= WideCharToMultiByte(CP_UTF8,0,file_notification->FileName,
            file_notification->FileNameLength/sizeof(WCHAR),
            filename,MAX_PATH_SIZE,NULL,NULL);
            filename[filename_len] = '\0';
            char buffer[300];
            sprintf(buffer,"File: %s was %s at %s\n", filename, file_actions(file_notification->Action), return_current_time());
            fprintf(stdout,"%s",buffer);
            WCHAR *file_copy= _wcsdup(file_notification->FileName);
            PostMessageW(hwndmain, WM_FILE_CHANGED, (WPARAM)file_copy, (LPARAM)file_notification->Action);
            //send message to main thread to show notificationp 
            if(file_notification->NextEntryOffset == 0){
                break;
            }
            file_notification = (FILE_NOTIFY_INFORMATION*)((char*)file_notification + file_notification->NextEntryOffset);
        }
    }
    free(file_notify_buffer);
    CloseHandle(HDIR);
    CloseHandle(HEVENT);
    return 0;
}
