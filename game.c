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



void UpdateDrawFrame(void); // Update and Draw (one frame)
void InitGame(void);        // Initialize game
void UnloadGame(void);      // Unload game

void UpdatePlayer(Player *player, EnvItem *envBlocks, int envItemsLength, float delta);

int envItemsLength = sizeof(envBlocks) / sizeof(envBlocks[0]);

FILE *fpWrite;

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

// TODO：重构作业
void UpdatePlayer(Player *player, EnvItem *envBlocks, int envItemsLength, float delta)
{
    if (!GameOver)
    {
        player->position.x += player->speed.x;
        if (IsKeyDown(KEY_LEFT))
            player->position.x -= PLAYER_HOR_SPD * delta;
        if (IsKeyDown(KEY_RIGHT))
            player->position.x += PLAYER_HOR_SPD * delta;
        if (IsKeyDown(KEY_SPACE) && player->canJump)
        {
            player->speed.y = -PLAYER_JUMP_SPD;
            player->canJump = true;
        }

        int hitObstacle = 0;
        for (int i = 0; i < envItemsLength; i++)
        {
            EnvItem *ei = envBlocks + i;
            Vector2 *p = &(player->position);
            if (ei->blocking &&
                ei->rect.x <= p->x &&
                ei->rect.x + ei->rect.width >= p->x &&
                ei->rect.y >= p->y &&
                ei->rect.y < p->y + player->speed.y * delta)
            {
                hitObstacle = 1;
                player->speed.y = 0.0f;
                p->y = ei->rect.y;
            }
        }

        if (!hitObstacle)
        {
            player->position.y += player->speed.y * delta;
            player->speed.y += G * delta;
            player->canJump = true;
        }
        else
            player->canJump = true;
        player->rect.x = player->position.x - player->rect.width / 2;
        player->rect.y = player->position.y - player->rect.height;

        for (int i = 0; i < note_num; i++)
        {
            if ((player->position.x >= envBlocks[i].rect.x) && (player->position.x <= envBlocks[i].rect.x + envBlocks[i].rect.width))
            {
                on_build = i;
            }
            if (CheckCollisionRecs(player->rect, envBlocks[i].rect) && (player->position.y > envBlocks[i].rect.y) && (on_build != i))
            {
                GameOver = true;
                interface = 4;
            }
        }

        for (int i = 0; i < note_num; i++)
        {
            if ((player->rect.x - souls[i].rect.x) > 10 && (souls[i].collect == 0) && souls[i].pass == 0)
            {
                total_miss += 1;
                souls[i].pass = 1;
                souls[i].miss = 1;
            }
        }

        if (IsKeyDown(KEY_A))
        {
            // int temp = 0;
            // find the closest soul
            for (int i = 0; i < note_num; i++)
            {
                //int min = 999;
                int dis = (player->rect.x - souls[i].rect.x) * (player->rect.x - souls[i].rect.x) + (player->rect.y - souls[i].rect.y) * (player->rect.y - souls[i].rect.y);
                if ((dis <= 400) && souls[i].pass == 0)
                {
                    PlaySound(soul_collect);
                    int score = 400 - dis;
                    if (score > 200)
                    {
                        total_perfect += 1;
                        souls[i].collect = 1;
                        souls[i].miss = 0;
                        souls[i].pass = 1;
                    }
                    if (score > 0 && score < 200)
                    {
                        total_great += 1;
                        souls[i].collect = 1;
                        souls[i].miss = 0;
                        souls[i].pass = 1;
                    }
                    /*
                    if (dis < min)
                    {
                        min = dis;
                        temp = i;
                    }
                    */
                }
            }
            //score = 400 - (player->rect.x - soul[temp].rect.x) * (player->rect.x - soul[temp].rect.x);
            //if (score > 90) {perfect += 1;soul[temp].collect = 1; soul[temp].miss = 0; soul[temp].pass = 1;}
            //if (score > 0 && score < 90 ) {great += 1; soul[temp].collect = 1;soul[temp].miss = 0; soul[temp].pass = 1;}
        }

        if (player->position.y > envBlocks[1].rect.y + envBlocks[1].rect.height)
        {
            GameOver = true;
            interface = 4;
        }

        if (player->position.x > (souls[note_num - 1].rect.x + 100))
        {
            song_end = true;
            total_score = (total_perfect * 1.2 + total_great * 0.8) / note_num;
            StopMusicStream(back_sound);
            interface = 6;
        }
    }
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
    fpWrite = fopen("save_notes.txt", "w");
    CheckPointer(fpWrite);
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

        if (player.position.x > souls[note_num - 1].rect.x + 500)
        {
            //StopMusicStream(back_sound);
        }

        if (IsKeyPressed(KEY_Q))
        {
            fprintf(fpWrite, "%lf\n", player.position.x);
        }

        if (IsKeyPressed(KEY_R))
        {
            player.position = (Vector2){400, 280};
        }

        if (IsKeyPressed(KEY_C))
            cameraOption = (cameraOption + 1) % cameraUpdatersLength;

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
    // TODO：重构作业
    if (interface == 5)
    {
        BeginDrawing();
        DrawTexOnBackground(LIGHTGRAY, play_tex);

        BeginMode2D(camera);

        for (int i = 0; i < envItemsLength; i++)
            DrawRectangleRec(envBlocks[i].rect, envBlocks[i].color);
        for (int i = 0; i < note_num; i++)
        {
            DrawRectangleRec(souls[i].rect, YELLOW);
            if (souls[i].miss)
            {
                DrawRectangleRec(souls[i].rect, ORANGE);
            }
            if (souls[i].collect)
            {
                DrawRectangleRec(souls[i].rect, RED);
            }
        }

        //Rectangle playerRect = { player.position.x - 20, player.position.y - 40, 40, 40 };
        //DrawRectangleRec(playerRect, RED);
        DrawTextureEx(player_tex, (Vector2){player.rect.x - player_tex.width * 0.06 / 2, player.rect.y - player_tex.height * 0.06 / 2 - 15}, 0, 0.06, WHITE);
        for (int i = 0; i < note_num; i++)
        {
            if (souls[i].collect)
            {
                DrawTextureEx(soul_boom_tex, (Vector2){souls[i].rect.x - soul_tex.width * 0.03 / 2 + 4, souls[i].rect.y - soul_tex.height * 0.03 / 2 + 4}, 0, 0.03, WHITE);
            }
            else if (souls[i].miss)
            {
                DrawTextureEx(soul_miss_tex, (Vector2){souls[i].rect.x - soul_tex.width * 0.02 / 2 + 4, souls[i].rect.y - soul_tex.height * 0.02 / 2 + 4}, 0, 0.02, WHITE);
            }
            else
            {
                DrawTextureEx(soul_tex, (Vector2){souls[i].rect.x - soul_tex.width * 0.02 / 2 + 4, souls[i].rect.y - soul_tex.height * 0.02 / 2 + 4}, 0, 0.02, WHITE);
            }

            int num = envBlocks[i].tex_type;
            DrawTextureEx(build_tex[num], (Vector2){envBlocks[i].rect.x, envBlocks[i].rect.y}, 0, 1, WHITE);
            //DrawTextureRec(build_tex[num], envBlocks[i].rect, (Vector2){envBlocks[i].rect.x - build_tex[num].width * 0.01/2, envBlocks[i].rect.y - build_tex[num].height * 0.01/2}, WHITE);
        }
        DrawRectangleRec(d_floor.rect, d_floor.color);

        EndMode2D();

        DrawScoreOnCanvas();

        DrawText("Controls:", 20, 20, 10, BLACK);
        DrawText("- Space to jump", 40, 40, 10, DARKGRAY);
        DrawText("- A to collect soul", 40, 60, 10, DARKGRAY);

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
