#include "updateplayer.h"

int ComputeDistance(Player *pPlayer, Soul *pSouls) 
{
    return  (pPlayer->rect.x - pSouls->rect.x) * (pPlayer->rect.x - pSouls->rect.x) 
    + (pPlayer->rect.y - pSouls->rect.y) * (pPlayer->rect.y - pSouls->rect.y);
}

void CheckIfSoulsEnd(Player *pPlayer, Soul **pSouls)
{
    CheckPointer(pPlayer);
    CheckPointer(pSouls);
    if (pPlayer->position.x > ((*pSouls)[note_num - 1].rect.x + 100))
    {
        song_end = true;
        total_score = (total_perfect * 1.2 + total_great * 0.8) / note_num;
        StopMusicStream(back_sound);
        INTERFACE =SCORE;
    }
}

void CheckIfGameOver(Player *pPlayer, EnvItem **pEnvBlocks)
{
    CheckPointer(pPlayer);
    CheckPointer(pEnvBlocks);
    int on_build = 0;
    for (int i = 0; i < note_num; i++)
    {
        // 找到player所处的envblock的索引
        if ((pPlayer->position.x >= (*pEnvBlocks)[i].rect.x) && 
            (pPlayer->position.x <= (*pEnvBlocks)[i].rect.x + (*pEnvBlocks)[i].rect.width))
        {
            on_build = i;
        }
        // player跳到了envblock的下面则gameover
        if (CheckCollisionRecs(pPlayer->rect, (*pEnvBlocks)[i].rect) && (pPlayer->position.y > (*pEnvBlocks)[i].rect.y) && (on_build != i))
        {
            GameOver = true;
            INTERFACE = GAME_OVER;
        }
    }
}

void CheckIfSoulsPass(Player *pPlayer, Soul **pSouls)
{
    CheckPointer(pPlayer);
    CheckPointer(pSouls);
    for (int i = pass_soul_index; i < note_num; i++)
    {
        if ((pPlayer->rect.x - (*pSouls)[i].rect.x) > 10 && ((*pSouls)[i].collect == 0) && (*pSouls)[i].pass == 0)
        {
            total_miss += 1;
            (*pSouls)[i].pass = 1;
            (*pSouls)[i].miss = 1;
            pass_soul_index = i;
        }
    }
}

void CheckIfJumpOnBlocks(Player *pPlayer, EnvItem **pEnvBlocks, int *JumpOnBlocks, float delta)
{
    CheckPointer(pPlayer);
    CheckPointer(pEnvBlocks);
    CheckPointer(JumpOnBlocks);
    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = *pEnvBlocks + i;
        Vector2 *p = &(pPlayer->position);
        // 如果 player->x在envblock之中, 且player->y在delta时间内会大于envblock->y, 则可以跳上
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

void ComputeScore(Player *pPlayer, Soul **pSouls)
{
    CheckPointer(pPlayer);   
    CheckPointer(pSouls); 

    // 通过距离函数判断分数, 并更新total_perfect和total_great
    // pass_soul_index + 1 则为距离 player 最近的 soul 的索引
    int dis = ComputeDistance(pPlayer, *pSouls + pass_soul_index + 1);
    if ((dis <= 400) && (*pSouls)[pass_soul_index + 1].pass == 0)
    {
        PlaySound(soul_collect);
        int score = 400 - dis;
        if (score > 200)
        {
            total_perfect += 1;
            (*pSouls)[pass_soul_index + 1].collect = 1;
            (*pSouls)[pass_soul_index + 1].miss = 0;
            (*pSouls)[pass_soul_index + 1].pass = 1;
        }
        if (score > 0 && score < 200)
        {
            total_great += 1;
            (*pSouls)[pass_soul_index + 1].collect = 1;
            (*pSouls)[pass_soul_index + 1].miss = 0;
            (*pSouls)[pass_soul_index + 1].pass = 1;
        }
    }
}

void UpdatePlayerRect(Player *pPlayer)
{
    CheckPointer(pPlayer);   
    pPlayer->rect.x = pPlayer->position.x - pPlayer->rect.width / 2;
    pPlayer->rect.y = pPlayer->position.y - pPlayer->rect.height;
}

void UpdatePlayerPosition(Player *pPlayer, int *JumpOnBlocks, float delta)
{
    CheckPointer(pPlayer);   
    CheckPointer(JumpOnBlocks); 
    pPlayer->position.x += pPlayer->speed.x;
    // 还没有跳上envblock, 根据G和speed.y更新player的位置
    if (!JumpOnBlocks)
    {
        pPlayer->position.y += pPlayer->speed.y * delta;
        pPlayer->speed.y += G * delta;
    }
}

void UpdatePlayer(Player *player, EnvItem *envBlocks, int envItemsLength, float delta)
{
    if (GameOver) return;
    if (!GameOver)
    {
        // 按下空格键, 改变 player 的y轴速度
        if (IsKeyDown(KEY_SPACE))
        {
            player->speed.y = -PLAYER_JUMP_SPD;
        }
        
        int JumpOnBlocks = 0;
        // 判断 player 是否跳上了envblock, 若能跳上，则更新player的y轴速度以及y轴位置
        CheckIfJumpOnBlocks(player, envBlocks, &JumpOnBlocks, delta);
        
        // 更新 player 的位置
        UpdatePlayerPosition(player, &JumpOnBlocks, delta);
        UpdatePlayerRect(player);

        // 按下A键, 判定分数
        if (IsKeyDown(KEY_A))
        {
            ComputeScore(player, souls);
        }

        CheckIfSoulsPass(player, souls);
        CheckIfGameOver(player, envBlocks);
        CheckIfSoulsEnd(player, souls);

    }
}
