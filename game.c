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



void UpdateDrawFrame(void); // Update and Draw (one frame)
void InitGame(void);        // Initialize game
void UnloadGame(void);      // Unload game


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
    UnloadGame();  // Unload loaded data (textures, sounds, models...)
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
    interface = 0;
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
}

// Update game (one frame)
void UpdateGame(void)
{
    if (interface == 0)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            interface = 1;
        }
    }

    if (interface == 1)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            interface = 2;
        }
    }

    if (interface == 2)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            interface = 3;
        }
    }

    if (interface == 3)
    {
        // choose different song
        if (IsKeyPressed(KEY_Z))
        {
            song_id = 1;
            InitGame();
            interface = 5;
        }
        if (IsKeyPressed(KEY_X))
        {
            song_id = 2;
            InitGame();
            interface = 5;
        }
        if (IsKeyPressed(KEY_C))
        {
            song_id = 3;
            InitGame();
            interface = 5;
        }
        if (IsKeyPressed(KEY_V))
        {
            song_id = 4;
            InitGame();
            interface = 5;
        }
    }

    if (interface == 4)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            interface = 3;
        }
    }

    if (interface == 5)
    {
        PlayMusicStream(back_sound);
        UpdateMusicStream(back_sound);

        float deltaTime = GetFrameTime();
        UpdatePlayer(&player, envBlocks, envItemsLength, deltaTime);

        if (IsKeyPressed(KEY_Q))
        {
            WriteSoulsToFile(song_id, player.position.x);
        }

        // Call update camera function by its pointer
        cameraUpdaters[cameraOption](&camera, &player, envBlocks, envItemsLength, deltaTime, screenWidth, screenHeight);
        //----------------------------------------------------------------------------------
    }

    if (interface == 6)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            interface = 3;
        }
    }
}

// Draw game (one frame)
void DrawGame(void)
{
    // Draw
    //----------------------------------------------------------------------------------
    if (interface == 4)
    {
        BeginDrawing();
        DrawTexOnBackground(LIGHTGRAY, GameOver_tex);
        EndDrawing();
    }
    
    if (interface == 5)
    {
        BeginDrawing();
        DrawTexOnBackground(LIGHTGRAY, play_tex);

        BeginMode2D(camera);
                
            DrawSoulsOnCanvas();
            DrawEnvBlocksOnCanvas();
            DrawPlayerOnCanvas();
            DrawFloorOnCanvas();

        EndMode2D();

        DrawScoreOnCanvas();
        DrawControlsOnCanvas();

        EndDrawing();
    }

    if (interface == 0)
    {
        BeginDrawing();
        DrawTexOnBackground(RAYWHITE, start_tex);
        EndDrawing();
    }

    if (interface == 1)
    {
        BeginDrawing();
        DrawTexOnBackground(RAYWHITE, theme_tex);
        EndDrawing();
    }

    if (interface == 2)
    {
        BeginDrawing();
        DrawTexOnBackground(RAYWHITE, rule_tex);
        EndDrawing();
    }

    if (interface == 3)
    {
        BeginDrawing();
        DrawTexOnBackground(RAYWHITE, journey_tex);
        EndDrawing();
    }

    if (interface == 6)
    {
        BeginDrawing();
        DrawTexOnBackground(LIGHTGRAY, score_tex);
        DrawScoreOnCanvas();
        EndDrawing();
    }
    //----------------------------------------------------------------------------------
}

// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}
