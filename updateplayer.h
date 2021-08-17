// updateplayer.h 用于处理player每一帧逻辑的头文件
#pragma once

#include "game.h"


// 判断 player 是否跑到最后一个音符, 若是, 停止播放音乐, 回到结束界面
void CheckIfSoulsEnd(Player *pPlayer, Soul **pSouls)
{
    if (pPlayer->position.x > ((*pSouls)[note_num - 1].rect.x + 100))
    {
        song_end = true;
        total_score = (total_perfect * 1.2 + total_great * 0.8) / note_num;
        StopMusicStream(back_sound);
        interface = 6;
    }
}

// 判断 player 与 envblocks 是否相撞, 若是, 则Gameover
void CheckIfGameOver(Player *pPlayer, EnvItem **pEnvBlocks)
{
    int on_build = 0;
    for (int i = 0; i < note_num; i++)
    {
        // 找到player所处的envblock的索引
        if ((pPlayer->position.x >= (*pEnvBlocks)[i].rect.x) && (pPlayer->position.x <= (*pEnvBlocks)[i].rect.x + (*pEnvBlocks)[i].rect.width))
        {
            on_build = i;
        }
        // player跳到了envblock的下面则gameover
        if (CheckCollisionRecs(pPlayer->rect, (*pEnvBlocks)[i].rect) && (pPlayer->position.y > (*pEnvBlocks)[i].rect.y) && (on_build != i))
        {
            GameOver = true;
            interface = 4;
        }
    }
}

// 判断 音符 是否已经播放 (pass)
void CheckIfSoulsPass(Player *pPlayer, Soul **pSouls)
{
    for (int i = 0; i < note_num; i++)
    {
        if ((pPlayer->rect.x - (*pSouls)[i].rect.x) > 10 && ((*pSouls)[i].collect == 0) && (*pSouls)[i].pass == 0)
        {
            total_miss += 1;
            (*pSouls)[i].pass = 1;
            (*pSouls)[i].miss = 1;
        }
    }
}

// 判断 player 是否会跳上envblock, 若能跳上，则更新player的y轴速度以及y轴位置
void CheckIfJumpOnBlocks(Player *pPlayer, EnvItem **pEnvBlocks, int *JumpOnBlocks, float delta)
{
    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = *pEnvBlocks + i;
        Vector2 *p = &(pPlayer->position);
        if (ei->rect.x <= p->x &&
            ei->rect.x + ei->rect.width >= p->x &&
            ei->rect.y >= p->y &&
            ei->rect.y < p->y + pPlayer->speed.y * delta)
        {
            JumpOnBlocks = 1;
            pPlayer->speed.y = 0.0f;
            p->y = ei->rect.y;
        }
    }
}

// 判定分数
void ComputeScore(Player *pPlayer, Soul **pSouls)
{
    // find the closest soul找到最近的音符
    for (int i = 0; i < note_num; i++)
    {
        // 通过距离函数判断分数, 并更新total_perfect和total_great
        int dis = (pPlayer->rect.x - (*pSouls)[i].rect.x) * (pPlayer->rect.x - (*pSouls)[i].rect.x) + (pPlayer->rect.y - (*pSouls)[i].rect.y) * (pPlayer->rect.y - (*pSouls)[i].rect.y);
        if ((dis <= 400) && (*pSouls)[i].pass == 0)
        {
            PlaySound(soul_collect);
            int score = 400 - dis;
            if (score > 200)
            {
                total_perfect += 1;
                (*pSouls)[i].collect = 1;
                (*pSouls)[i].miss = 0;
                (*pSouls)[i].pass = 1;
            }
            if (score > 0 && score < 200)
            {
                total_great += 1;
                (*pSouls)[i].collect = 1;
                (*pSouls)[i].miss = 0;
                (*pSouls)[i].pass = 1;
            }
        }
    }
}

// 根据 player 的位置更新play.rect显示的位置
void UpdatePlayerRect(Player *pPlayer)

{
    pPlayer->rect.x = pPlayer->position.x - pPlayer->rect.width / 2;
    pPlayer->rect.y = pPlayer->position.y - pPlayer->rect.height;
}

// 更新 player 的位置
void UpdatePlayerPosition(Player *pPlayer, int *JumpOnBlocks, float delta)
{
    pPlayer->position.x += pPlayer->speed.x;
    // 还没有跳上envblock, 根据G和speed.y更新player的位置
    if (!JumpOnBlocks)
    {
        pPlayer->position.y += pPlayer->speed.y * delta;
        pPlayer->speed.y += G * delta;
    }
}