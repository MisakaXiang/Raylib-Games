// updateplayer.h 用于处理player每一帧逻辑的头文件
#pragma once

#include "game.h"
#include "memtools.h"
#include "statemachine.h"

// 用来保存已经播放过的 soul 索引
int pass_soul_index = 0;

// CheckIfSoulsEnd 判断 player 是否跑到最后一个音符, 若是, 停止播放音乐, 回到结束界面
void CheckIfSoulsEnd(Player *pPlayer, Soul **pSouls);

// CheckIfGameOver 判断 player 与 envblocks 是否相撞, 若是, 则Gameover
void CheckIfGameOver(Player *pPlayer, EnvItem **pEnvBlocks);

// CheckIfSoulsPass 判断 音符 是否已经播放 (pass)
void CheckIfSoulsPass(Player *pPlayer, Soul **pSouls);

// CheckCanJumpOnBlocks 判断 player 是否会跳上envblock, 若能跳上，则更新player的y轴速度以及y轴位置
void CheckCanJumpOnBlocks(Player *pPlayer, EnvItem **pEnvBlocks, int *JumpOnBlocks, float delta);

// ComputeScore 判定分数
void ComputeScore(Player *pPlayer, Soul **pSouls);

// UpdatePlayerRect 根据 player 的位置更新play.rect显示的位置
void UpdatePlayerRect(Player *pPlayer);

// UpdatePlayerPosition 更新 player 的位置
void UpdatePlayerPosition(Player *pPlayer, int *JumpOnBlocks, float delta);

// ComputeDistance 计算 player 和 soul 的距离函数
int ComputeDistance(Player *pPlayer, Soul *pSouls);

// UpdatePlayer 更新 player 的各种操作
void UpdatePlayer(Player *player, EnvItem *envBlocks, int envItemsLength, float delta);