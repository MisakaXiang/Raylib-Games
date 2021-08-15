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