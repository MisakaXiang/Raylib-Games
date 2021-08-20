// attr.h 存储实体相关的结构
#pragma once

#include "raylib/src/raylib.h"
#include "raylib/src/raymath.h"
#include "raylib/src/rlgl.h"

// Player 玩家结构体
typedef struct Player
{
    Rectangle rect;
    Vector2 position;
    Vector2 speed;
} Player;

// Soul 音符结构体
typedef struct Soul
{
    Rectangle rect;
    Color color;
    bool flag;
    int miss;
    int collect;
    int pass;
} Soul;

// EnvItem 场景中的物体结构体
typedef struct EnvItem
{
    Rectangle rect;
    Color color;
    int tex_type;
} EnvItem;

// GameAttrInit 对游戏初始贴图场景进行初始化
int GameAttrInit(EnvItem *pDFloor, Vector2 *pVector2[5], EnvItem **pEnvBlocks);

// LoadGameResources 加载游戏资源
int LoadGameResources(int song_id);

// GamePlayerInit 对游戏初始状态下的玩家进行初始化
int GamePlayerInit(Player *pPlayer);

// GameSoulsInit 对游戏初始状态下的音符数组进行初始化
int GameSoulsInit(Soul **pSouls);