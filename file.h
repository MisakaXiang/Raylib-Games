// file.h 定义游戏中的对文件的I/O封装操作
#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "attr.h"
#include "errcode.h"


// ReadSoulsFromFile 从文件中读入音符数据并且创建音符
int ReadSoulsFromFile(int song_id, Soul *souls,int *p_note_num);

// WriteSoulsToFile 把 player 的位置数据写入文件
int WriteSoulsToFile(int song_id, int player_position_x);