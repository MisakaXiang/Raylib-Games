// tools.h 用于内存处理的头文件
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
#include <execinfo.h>
#endif

#include "errcode.h"

// CheckPointer 检查指针，并返回对应的错误码
int CheckPointer(void *ptr)
{
    if (ptr == (void *)0)
    {
        fprintf(stderr, "Pointer is empty.");
        return NULL_POINTER_ERROR;
    }
    return SUCCESS;
}

// CheckPointerWithExit 检查指针，指针为空时直接退出程序
void CheckPointerWithExit(void *ptr)
{
    if (ptr == (void *)0)
    {
        fprintf(stderr, "Pointer is empty.");
        exit(NULL_POINTER_ERROR);
    }
}

// CheckPtrWithMsg 检查指针是否为空，为空时打印debug信息。
int CheckPtrWithMsg(void *ptr,
                         const char *file,
                         int line,
                         const char *msg)
{
    if (CheckPointer(ptr) != NULL_POINTER_ERROR)
    {
        return;
    }
    if (msg[strlen(msg) - 1] == "\n")
    {
        fprintf(stderr, "%s:%d %s", file, line, msg);
    }
    fprintf(stderr, "%s:%d %s\n", file, line, msg);
    return NULL_POINTER_ERROR;
}

// CheckPtrWithMsgExit 检查指针是否为空，为空时打印debug信息并退出。
void CheckPtrWithMsgExit(void *ptr,
                     const char *file,
                     int line,
                     const char *msg)
{
    CheckPtrWithMsg(ptr, file, line, msg);
    exit(NULL_POINTER_ERROR);
}

// PrintBacktrace 打印当前调用栈
#ifdef __linux__
void PrintBacktrace()
{
    void *buffer[100];
    char **strings;
    const int SIZE = 5;
    int nptrs = backtrace(buffer, SIZE);
    strings = backtrace_symbols(buffer, nptrs);

    if (strings == NULL)
    {
        perror("backtrace_symbols");
        exit(NULL_POINTER_ERROR);
    }

    fprintf(stderr, "Current Backtrace:\n");
    for (int j = 0; j < nptrs; j++)
    {
        fprintf(stderr, "%s\n", strings[j]);
    }
    free(strings);
}
#else
void PrintBacktrace()
{
    // do nothing
    fprintf(stderr,"Your system doesn't implemented backtrace() or we don't implement it.\n")
}
#endif // __linux__