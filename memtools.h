// tools.h 用于内存处理的头文件
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>

#include "errcode.h"

int CheckPointer(void *ptr)
{
    if (ptr == (void *)0)
    {
        fprintf(stderr, "Pointer is empty.");
        return NULL_POINTER_ERROR;
    }
    return SUCCESS;
}

void CheckPointerWithExit(void *ptr)
{
    if (ptr == (void *)0)
    {
        fprintf(stderr, "Pointer is empty.");
        exit(NULL_POINTER_ERROR);
    }
}

void PrintBacktrace()
{
    void *buffer[100];
    char **strings;
    const int SIZE = 5;
    int nptrs = backtrace(buffer, SIZE);
    strings = backtrace_symbols(buffer, nptrs);

    if (strings == NULL) {
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