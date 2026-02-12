#include<stdio.h>
#include<stdlib.h>
#include<windows.h>

#define MAX_PATH 70
#define max_buffer_size 16384

PCSTR Get_action_name(DWORD action){
    const PCSTR action_names[]={
        "0",
        "File ADDED",
        "File REMOVED",
        "File MODIFIED",
        "File RENAMED OLD NAME",
        "File RENAMED NEW NAME"
    };
    CHAR buffer[50];
    if(action >= RTL_NUMBER_OF(action_names)){
        sprintf(buffer,"Unknown action %d",action);
        return buffer;
    }
    return action_names[action];
}

int main(){
    WCHAR directroryname[MAX_PATH];
    HANDLE directory_handle;
    OVERLAPPED overlapped = {0};
    FILE_NOTIFY_INFORMATION *FileNotifyinfo;
    BYTE *FileNotifyInformationBuffer;
    DWORD BytesReturned;
    BOOL RET;


    if(!GetCurrentDirectory(RTL_NUMBER_OF(directroryname),directroryname)){
        fprintf(stderr," unable to get current director %lu",GetLastError());
        exit(EXIT_FAILURE);
    }
    directory_handle =CreateFile(directroryname,
                        FILE_LIST_DIRECTORY,
                        FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_FLAG_BACKUP_SEMANTICS,
                        NULL);
if(directory_handle == INVALID_HANDLE_VALUE){
    fprintf(stderr,"an error %lu occured",GetLastError());
    exit(EXIT_FAILURE);
}
overlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
if(overlapped.hEvent == NULL){
    fprintf(stderr,"Created event failed %lu",GetLastError());
    exit(EXIT_FAILURE);
}

FileNotifyInformationBuffer = malloc (1024);
if(FileNotifyInformationBuffer == NULL){
    fprintf(stderr,"Memory allocation failed");
    exit(EXIT_FAILURE);
}
FileNotifyInformationBuffer = malloc(max_buffer_size);
if(!FileNotifyInformationBuffer){
    fprintf(stderr,"Memory allocation failed");
    exit(EXIT_FAILURE);
    CloseHandle(overlapped.hEvent);
    CloseHandle(directory_handle);
}

while(1){
    RET = ReadDirectoryChangesW(directory_handle,FileNotifyInformationBuffer,max_buffer_size,TRUE,
                                FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME|
                                FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_SIZE|
                                FILE_NOTIFY_CHANGE_LAST_WRITE|FILE_NOTIFY_CHANGE_CREATION,
                                &BytesReturned,NULL,NULL);
    if(!RET){
        fprintf(stderr,"ReadDirectoryChangesW failed %lu",GetLastError());
        free(FileNotifyInformationBuffer);
        CloseHandle(overlapped.hEvent);
        CloseHandle(directory_handle);
        exit(EXIT_FAILURE);
    }
}
