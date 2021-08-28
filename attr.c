#include <stdio.h>

#include "attr.h"
#include "game.h"
#include "errcode.h"

int GameAttrInit(EnvItem *pDFloor, Vector2 *pVector2[5], EnvItem **pEnvBlocks)
{
    if (!pDFloor || !pVector2 || !*pVector2 || !pEnvBlocks || !*pEnvBlocks)
    {
        return NULL_POINTER_ERROR;
    }
    (*pDFloor).rect = (Rectangle){0, screenHeight + 200, 20000, 400};
    (*pDFloor).color = BROWN;

    (*pVector2)[0] = (Vector2){200, 400};
    (*pVector2)[1] = (Vector2){220, 600};
    (*pVector2)[2] = (Vector2){230, 500};
    (*pVector2)[3] = (Vector2){250, 600};
    (*pVector2)[4] = (Vector2){300, 500};

    (*pEnvBlocks)[0].rect.height = 200;
    (*pEnvBlocks)[0].rect.width = 400;
    (*pEnvBlocks)[0].rect.x = 0;
    (*pEnvBlocks)[0].rect.y = (*pDFloor).rect.y - (*pEnvBlocks)[0].rect.height;
    (*pEnvBlocks)[0].color = GRAY;

    for (int i = 1; i < note_num; i++)
    {
        srand((unsigned)(time(NULL)));
        int num = rand() % 5;
        (*pEnvBlocks)[i].tex_type = num;
        (*pEnvBlocks)[i].rect.height = (*pVector2)[num].x;
        (*pEnvBlocks)[i].rect.width = (*pVector2)[num].y;
        (*pEnvBlocks)[i].rect.x = rand() % 100 + 100 + (*pEnvBlocks)[i - 1].rect.x + (*pEnvBlocks)[i - 1].rect.width;
        (*pEnvBlocks)[i].rect.y = d_floor.rect.y - (*pEnvBlocks)[i].rect.height;
        (*pEnvBlocks)[i].color = GRAY;
    }
    return SUCCESS;
}

int LoadGameResources(int song_id)
{
    // read from note file and create notes
    char songFileName[50];
    if (song_id < 1 || song_id > 4)
    {
        return SONG_ID_INVAILD;
    }

    start_tex = LoadTexture("resources/start.png");
    theme_tex = LoadTexture("resources/theme.png");
    rule_tex = LoadTexture("resources/rule.png");
    journey_tex = LoadTexture("resources/journey.png");
    GameOver_tex = LoadTexture("resources/GameOver.png");
    play_tex = LoadTexture("resources/play.png");
    score_tex = LoadTexture("resources/score.png");

    player_tex = LoadTexture("resources/player.png");
    soul_tex = LoadTexture("resources/soul.png");
    soul_boom_tex = LoadTexture("resources/soul_boom.png");
    soul_miss_tex = LoadTexture("resources/soul_miss.png");

    build_tex[0] = LoadTexture("resources/200400.png");
    build_tex[1] = LoadTexture("resources/220600.png");
    build_tex[2] = LoadTexture("resources/230500.png");
    build_tex[3] = LoadTexture("resources/250600.png");
    build_tex[4] = LoadTexture("resources/300500.png");

    sprintf(songFileName, "resources/%d.wav", song_id);
    back_sound = LoadMusicStream(songFileName);
    soul_collect = LoadSound("resources/soul_collect.wav");
    return SUCCESS;
}

int GamePlayerInit(Player *pPlayer)
{
    if (!pPlayer)
    {
        return NULL_POINTER_ERROR;
    }

    (*pPlayer).rect = (Rectangle){
        0,
        d_floor.rect.y - 200 - 12.8,
        12,
        12.8};
    (*pPlayer).position = (Vector2){
        (*pPlayer).rect.x,
        (*pPlayer).rect.y + (*pPlayer).rect.height};
    (*pPlayer).speed.y = 0;
    (*pPlayer).speed.x = 3;
    return SUCCESS;
}

int GameSoulsInit(Soul **pSouls)
{
    if (!pSouls || !*pSouls)
    {
        return NULL_POINTER_ERROR;
    }
    for (int i = 0; i < note_num; i++)
    {
        (*pSouls)[i].rect.height = 10;
        (*pSouls)[i].rect.width = 10;
        (*pSouls)[i].miss = 0;
        (*pSouls)[i].collect = 0;
        (*pSouls)[i].pass = 0;
        int s_on_build = -1;
        for (int j = 0; j < note_num; j++)
        {
            if (((*pSouls)[i].rect.x > envBlocks[j].rect.x) && ((*pSouls)[i].rect.x < envBlocks[j].rect.x + envBlocks[j].rect.width))
            {
                s_on_build = j;
                break;
            }
        }
        if (s_on_build >= 0)
        {
            (*pSouls)[i].rect.y =
                envBlocks[s_on_build].rect.y - (*pSouls)[i].rect.height - 5;
        }
        else
        {
            (*pSouls)[i].rect.y = d_floor.rect.y - (rand() % 50 + 300);
        }
        (*pSouls)[i].flag = true;
        (*pSouls)[i].color = YELLOW;
    }
    return SUCCESS;
}