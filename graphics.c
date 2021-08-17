#include "graphics.h"
#include "game.h"

void DrawTexOnBackground(Color BackgroundColor, Texture2D tex)
{
    ClearBackground(BackgroundColor);
    DrawTextureEx(tex, (Vector2){0, 0}, 0, 1, WHITE);
}

void DrawScoreOnCanvas()
{
    DrawText(TextFormat("perfect: %05i", total_perfect), 307, 20, 40, GRAY);
    DrawText(TextFormat("great: %05i", total_great), 307, 60, 40, GRAY);
    DrawText(TextFormat("miss: %05i", total_miss), 307, 100, 40, GRAY);
}

void DrawSoulsOnCanvas()
{
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

    }
}


void DrawEnvBlocksOnCanvas()
{
    for (int i = 0; i < envItemsLength; i++)
    {
        DrawRectangleRec(envBlocks[i].rect, envBlocks[i].color);
        int num = envBlocks[i].tex_type;
        DrawTextureEx(build_tex[num], (Vector2){envBlocks[i].rect.x, envBlocks[i].rect.y}, 0, 1, WHITE);
        //DrawTextureRec(build_tex[num], envBlocks[i].rect, (Vector2){envBlocks[i].rect.x - build_tex[num].width * 0.01/2, envBlocks[i].rect.y - build_tex[num].height * 0.01/2}, WHITE);
    }
}


void DrawControlsOnCanvas()
{
    DrawText("Controls:", 20, 20, 10, BLACK);
    DrawText("- Space to jump", 40, 40, 10, DARKGRAY);
    DrawText("- A to collect soul", 40, 60, 10, DARKGRAY);
}


void DrawPlayerOnCanvas()
{
    DrawTextureEx(player_tex, (Vector2){player.rect.x - player_tex.width * 0.06 / 2, player.rect.y - player_tex.height * 0.06 / 2 - 15}, 0, 0.06, WHITE);         
}

void DrawFloorOnCanvas()
{
    DrawRectangleRec(d_floor.rect, d_floor.color);
}