#include "statemachine.h"
#include "game.h"
#include "updateplayer.h"
#include "attr.h"
#include "camera.h"
#include "graphics.h"
#include "file.h"

void action_draw_game_over(void)
{
    BeginDrawing();
    DrawTexOnBackground(LIGHTGRAY, GameOver_tex);
    EndDrawing();
}

void action_update_game_over(void)
{
    return;
}

void action_draw_play(void)
{
    BeginDrawing();
    DrawTexOnBackground(LIGHTGRAY, play_tex);

    BeginMode2D(camera);

    DrawSoulsOnCanvas();
    DrawEnvBlocksOnCanvas();
    DrawPlayerOnCanvas();
    DrawFloorOnCanvas();

    EndMode2D();

    DrawScoreOnCanvas();
    DrawControlsOnCanvas();

    EndDrawing();
}

void action_update_play(void)
{
    InitGame();
    PlayMusicStream(back_sound);
    UpdateMusicStream(back_sound);

    float deltaTime = GetFrameTime();
    UpdatePlayer(&player, envBlocks, envItemsLength, deltaTime);

    if (IsKeyPressed(KEY_Q))
    {
        WriteSoulsToFile(song_id, player.position.x);
    }

    // Call update camera function by its pointer
    cameraUpdaters[cameraOption](&camera, &player, envBlocks, envItemsLength, deltaTime, screenWidth, screenHeight);
    //----------------------------------------------------------------------------------
}

void action_draw_start(void)
{
    BeginDrawing();
    DrawTexOnBackground(RAYWHITE, start_tex);
    EndDrawing();
}

void action_update_start(void)
{
    return;
}

void action_draw_theme(void)
{
    BeginDrawing();
    DrawTexOnBackground(RAYWHITE, theme_tex);
    EndDrawing();
}

void action_update_theme(void)
{
    return;
}

void action_draw_rule(void)
{
    BeginDrawing();
    DrawTexOnBackground(RAYWHITE, rule_tex);
    EndDrawing();
}

void action_update_rule(void)
{
    return;
}

void action_draw_select_music(void)
{
    BeginDrawing();
    DrawTexOnBackground(RAYWHITE, journey_tex);
    EndDrawing();
}

void action_update_select_music(void)
{
    return;
}

void action_draw_score(void)
{
    BeginDrawing();
    DrawTexOnBackground(LIGHTGRAY, score_tex);
    DrawScoreOnCanvas();
    EndDrawing();
}

void action_update_score(void)
{
    return;
}

void state_machine_regist(StateMachine *pSM, StateInfo *pStateTable)
{
    CheckPtrWithMsgExit(pSM, __FILE__, __LINE__, "pSM null pointer.");
    CheckPtrWithMsgExit(pStateTable, __FILE__, __LINE__, "pStateTable null pointer.");
    pSM->stateTable = pStateTable;
}

StateInfo *find_state_table(StateMachine *pSM, Event evt)
{
    if (CheckPtrWithMsg(pSM, __FILE__, __LINE__, "pSM null pointer.") == NULL_POINTER_ERROR)
    {
        return NULL;
    }
    for (uint8_t i = 0; i < pSM->stateTableSize; i++)
    {
        if ((pSM->stateTable[i].curState == pSM->currentState) && (pSM->stateTable[i].event == evt))
        {
            return &pSM->stateTable[i];
        }
    }
}

void runStateMachine(StateMachine *pSM, Event evt)
{
    StateInfo *pStateInfo;

    CheckPtrWithMsgExit(pSM, __FILE__, __LINE__, "pSM null pointer.");
    CheckPtrWithMsgExit(pStateInfo = find_state_table(pSM, evt),
                        __FILE__, __LINE__, "pStateInfo null pointer.");

    Action_Func act = pStateInfo->action;
    CheckPtrWithMsgExit(act, __FILE__, __LINE__, "act null pointer.");
    act();

    pSM->currentState = pStateInfo->nextState;
}

void InitStateMachine(void)
{
    event = event_start_game;
    state_machine_regist(&SM_Type, state_table);
    SM_Type.currentState = state_START;
    SM_Type.stateTableSize = sizeof(state_table) / sizeof(state_table[0]);
}

typedef struct updateTbl
{
    int key;
    Event event;
} updateTbl;

// utable FSM中按下的键与发生的事件之间的映射表
const updateTbl utable[] =
    {{KEY_ENTER, event_press_key_enter},
     {KEY_SPACE, event_press_key_space},
     {KEY_Z, event_press_key_z},
     {KEY_X, event_press_key_x},
     {KEY_C, event_press_key_c},
     {KEY_V, event_press_key_v}};

void UpdateStateMachine(void)
{
    int key_pressed = GetKeyPressed();
    int i = 0;
    for (; i < sizeof(utable) / sizeof(utable[0]); i++)
    {
        if (utable[i].key == key_pressed)
        {
            event = utable[i].event;
            break;
        }
    }
    runStateMachine(&SM_Type, event);
}
