// file.h 定义游戏中的对文件的I/O封装操作
#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "attr.h"
#include "errcode.h"

int ReadSoulsFromFile(int song_id, Soul *souls,int *p_note_num)
{
    // read from note file and create notes
    char soulFileName[10];
    if (song_id < 1 || song_id > 4)
    {
        fprintf(stderr, "song_id invaild.\n");
        return SONG_ID_INVAILD;
    }
    sprintf(soulFileName, "song%d.txt", song_id);
    FILE *fpRead = fopen(soulFileName, "rb");
    if (!fpRead)
    {
        fprintf(stderr, "Song souls file read error.\n");
        return NULL_POINTER_ERROR;
    }
    double soul_temp;
    *p_note_num = 0; // 这首歌曲的音符数
    // 将文件中的音符全部读入到souls数组中
    while (fscanf(fpRead, "%lf", &soul_temp) == 1)
    {
        souls[*p_note_num].rect.x = (int)soul_temp;
        *p_note_num += 1;
    }
    fclose(fpRead);
    return SUCCESS;
}


int WriteSoulsToFile(int song_id, int player_position_x) 
{
    char SaveSoulFileName[10];
    if (song_id < 1 || song_id > 4)
    {
        fprintf(stderr, "song_id invaild.\n");
        return SONG_ID_INVAILD;
    }
    sprintf(SaveSoulFileName, "save_song%d.txt", song_id);
    FILE *fpWrite = fopen(SaveSoulFileName, "w");
    if (!fpWrite)
    {
        fprintf(stderr, "write souls to file error.\n");
        return NULL_POINTER_ERROR;
    }
    fprintf(fpWrite, "%lf\n", player_position_x);
    fclose(fpWrite);
    return SUCCESS;
}