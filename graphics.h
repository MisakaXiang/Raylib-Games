// graphics.h 绘制图像的头文件
#pragma once

#include "raylib/src/raylib.h"
#include "raylib/src/raymath.h"
#include "raylib/src/rlgl.h"

// DrawTexOnBackground 把背景渲染为指定颜色并在左上角绘制贴图
void DrawTexOnBackground(Color BackgroundColor, Texture2D tex);

// DrawScoreOnCanvas 在画布上绘制分数文本
void DrawScoreOnCanvas();

// DrawSoulsOnCanvas 在画布上绘制音符
void DrawSoulsOnCanvas();

// DrawControlsOnCanvas 在画布上绘制控制说明
void DrawControlsOnCanvas();

// DrawEnvBlocksOnCanvas 在画布上绘制EnvBlocks
void DrawEnvBlocksOnCanvas();

// DrawPlayerOnCanvas 在画布上绘制Player
void DrawPlayerOnCanvas();

// DrawFlorrOnCanvas 在画布上绘制Floor
void DrawFloorOnCanvas();