// graphics.h 绘制图像的头文件
#pragma once

#include "raylib/src/raylib.h"
#include "raylib/src/raymath.h"
#include "raylib/src/rlgl.h"

// DrawTexOnBackground 把背景渲染为指定颜色并在左上角绘制贴图
void DrawTexOnBackground(Color BackgroundColor, Texture2D tex);

// DrawScoreOnCanvas 在画布上绘制分数文本
void DrawScoreOnCanvas();