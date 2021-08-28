#include "raylib/src/raylib.h"
#include "raylib/src/raymath.h"
#include "raylib/src/rlgl.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "attr.h"
#include "camera.h"
#include "file.h"
#include "memtools.h"
#include "game.h"
#include "graphics.h"
#include "updateplayer.h"
#include "statemachine.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");
    InitAudioDevice();
    InitGame();
    StartGame();

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {

        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
        // Update
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    ReleaseResource();  // Unload loaded data (textures, sounds, models...)
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


// InitGame 游戏启动前进行初始化
void InitGame(void)
{
    souls = (Soul *)malloc(sizeof(Soul) * MAX_NOTE_NUM);
    CheckPointer(souls);
    // 初始化游戏所需资源
    int res = LoadGameResources(song_id);
    if (!res)
    {
        exit(1);
    }
}

// StartGame 初始化新的游戏
void StartGame(void)
{
    // 对游戏固有属性进行初始化
    total_perfect = 0;
    total_miss = 0;
    total_great = 0;
    total_score = 0;

    GameOver = false;
    song_end = false;

    // 初始化游戏场景数据
    int res = GameAttrInit(&d_floor, build_size, &(envBlocks[0]));
    if (!res)
    {
        exit(1);
    }

    // 初始化游戏人物数据
    res = GamePlayerInit(&player);
    if (!res)
    {
        exit(1);
    }

    // 从文件中读取音符数据
    res = ReadSoulsFromFile(song_id, souls, &note_num);
    if (!res)
    {
        exit(1);
    }

    // 初始化游戏音符数据
    res = GameSoulsInit(&souls);
    if (!res)
    {
        exit(1);
    }

    // 初始化游戏相机数据
    res = GameCameraInit(&camera);
    if (!res)
    {
        exit(1);
    }
    InitStateMachine();
}

// Unload game variables
void ReleaseResource(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateStateMachine();
}
