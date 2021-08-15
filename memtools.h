// tools.h 用于内存处理的头文件
#pragma once

#include <stdio.h>
#include <stdlib.h>

void CheckPointer(void *ptr)
{
    if (ptr == (void *)0)
    {
        fprintf(stderr, "Pointer is empty.");
        exit(1);
    }
}