#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<stddef.h>
#define MAX_PATH 70
#define max_buffer_size 16384 //why this i don't know random size


void file_actions(DWORD Action){
    const char action[]={
        "added",
        "removed",
        "modified",
        "renamed from something",
        "renamed to something"
    };
    char Buffer[24];
    if(Action>= RTL_NUMBER_OF(action)){
        sprintf(Buffer,"%lU",Action);
        return Buffer;
    }
    return action[Action];

}

void watch_directory(LPCWSTR directory){
    HANDLE HDIR;
    FILE_NOTIFY_INFORMATION *file_notification = NULL;
    DWORD bytes_returned;
    DWORD buffer_len;
    BOOL RET;
    BYTE *file_notify_buffer;
    file_notify_buffer =malloc(max_buffer_size);
    if(!file_notify_buffer){
        fprintf(stderr,"unable to allocate buffer size");
        return;
    }

    char filename[MAX_PATH];
    HDIR= CreateFile(directory,
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL);
    if(HDIR ==INVALID_HANDLE_VALUE){
        fprintf(stderr,"unable to create handle %lu ",GetLastError());
        CloseHandle(HDIR);
        return;
    }
    while(1){
        RET =ReadDirectoryChangesW(HDIR,
            file_notify_buffer,
            sizeof(file_notify_buffer),
            TRUE,
            FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_SECURITY,
            &bytes_returned,
            NULL,
            NULL);
            
        if(!RET){
            fprintf(stderr,"read directory changes failed %lu",GetLastError());
            CloseHandle(HDIR);
            break;
        }
        file_notification = file_notify_buffer;
    }


}


int main(int argc, char *argv[]){
    if(argc <1){
        fprintf(stderr,"please pass the dirrectoy path you want to use");
        exit(EXIT_FAILURE);
    }
    char *directory = argv[1];
    watch_directory(directory);
    return 0;
}


//add parsing of the FILE_NOTIFY_INFORMATION structure and print the file name and action
//THIngs to keep track off 
//handle for crea