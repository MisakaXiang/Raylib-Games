#pragma once

#include "raylib/src/raylib.h"
#include "raylib/src/raymath.h"
#include "raylib/src/rlgl.h"
#include "attr.h"
#include "camera.h"
#include "statemachine.h"

// G 游戏中的重力加速度
const int G = 400;

// PLAYER_JUMP_SPD 玩家向上跳跃的初速度
const float PLAYER_JUMP_SPD = 100.0f;

// PLAYER_HOR_SPD 玩家水平奔跑的速度
const float PLAYER_HOR_SPD = 200.0f;

// MAX_NOTE_NUM 一场游戏中最多的音符数
const int MAX_NOTE_NUM = 999;

// build_size EnvBlocks的大小
Vector2 build_size[5];
// EnvBlock的数量
const int envItemsLength = 1000;
// screenWidth 游戏的显示宽度
const int screenWidth = 1000;
// screenHeight 游戏的显示高度
const int screenHeight = 600;
// total_score 玩家的总分数
float total_score = 0.0f;
// total_perfect 玩家总perfect数
int total_perfect = 0;
// total_great 玩家总great数
int total_great = 0;
// total_miss 玩家总miss数
int total_miss = 0;

// d_floor 场景地板
EnvItem d_floor;

// player 当前场景玩家实体
Player player = {0};
// envBlocks 玩家行走过程中所处的大方块
EnvItem envBlocks[1000];
// soul 场景歌曲的音符集合
Soul *souls;
// note_num 场景歌曲的音符总数
int note_num = 0;

Texture2D start_tex = {0};
Texture2D theme_tex = {0};
Texture2D rule_tex = {0};
Texture2D journey_tex = {0};
Texture2D GameOver_tex = {0};
Texture2D play_tex = {0};
Texture2D score_tex = {0};

Texture2D player_tex = {0};
Texture2D soul_tex = {0};
Texture2D soul_boom_tex = {0};
Texture2D soul_miss_tex = {0};
Texture2D build_tex[5];
Sound soul_collect = {0};
Music back_sound = {0};
bool GameOver = false;
bool song_end = false;
int song_id = 1;

// UpdateDrawFrame 每一帧更新和画图
void UpdateDrawFrame(void); 

// InitGame 初始化游戏
void InitGame(void);      

// UnloadGame 释放游戏中的资源
void UnloadGame(void);   

// StartGame 开始游戏
void StartGame(void);       


