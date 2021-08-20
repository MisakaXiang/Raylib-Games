// camera.h 定义对相机的操作
#pragma once
#include "attr.h"
#include "game.h"
#include "raylib/src/raylib.h"
#include "raylib/src/raymath.h"
#include "raylib/src/rlgl.h"

Camera2D camera = {0};
int cameraOption = 0;
int cameraUpdatersLength = 0;

static char *cameraDescriptions[] = {
    "Follow player center",
    "Follow player center, but clamp to map edges",
    "Follow player center; smoothed",
    "Follow player center horizontally; updateplayer center vertically after landing",
    "Player push camera on getting too close to screen edge"};


void UpdateCameraCenter(Camera2D *camera, Player *player, EnvItem *envBlocks, int envItemsLength, float delta, int width, int height);

void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envBlocks, int envItemsLength, float delta, int width, int height);

void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player, EnvItem *envBlocks, int envItemsLength, float delta, int width, int height);

void UpdateCameraEvenOutOnLanding(Camera2D *camera, Player *player, EnvItem *envBlocks, int envItemsLength, float delta, int width, int height);

void UpdateCameraPlayerBoundsPush(Camera2D *camera, Player *player, EnvItem *envBlocks, int envItemsLength, float delta, int width, int height);

// Store pointers to the multiple update camera functions
void (*cameraUpdaters[])(Camera2D *, Player *, EnvItem *, int, float, int, int) = {
    UpdateCameraCenter,
    UpdateCameraCenterInsideMap,
    UpdateCameraCenterSmoothFollow,
    UpdateCameraEvenOutOnLanding,
    UpdateCameraPlayerBoundsPush};

// GameCameraInit 对游戏初始状态的相机设置参数
int GameCameraInit(Camera2D *pCamera);