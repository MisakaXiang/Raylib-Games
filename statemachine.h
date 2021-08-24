// statemachine.h 定义有限状态机的头文件
#pragma once

#include <stdint.h>

typedef void (*Action_Func)(void);

void action_draw_game_over(void);

void action_update_game_over(void);

void action_draw_play(void);

void action_update_play(void);

void action_draw_start(void);

void action_update_start(void);

void action_draw_theme(void);

void action_update_theme(void);

void action_draw_rule(void);

void action_update_rule(void);

void action_draw_select_music(void);

void action_update_select_music(void);

void action_draw_score(void);

void action_update_score(void);

// action_start 状态为start时的action
void action_start(void) 
{ 
    action_update_start();
    action_draw_start();
}

// action_theme 状态为theme时的action
void action_theme(void) 
{ 
    action_update_theme();
    action_draw_theme();
}

// action_rule 状态为rule时的action
void action_rule(void) 
{ 
    action_update_rule();
    action_draw_rule();  
}

// action_select_music 状态为select_music时的action
void action_select_music(void) 
{ 
    action_update_select_music();
    action_draw_select_music(); 
}

// action_play 状态为play时的action
void action_play(void) 
{ 
    action_update_play();
    action_draw_play(); 
}

// action_game_over 状态为game_over时的action
void action_game_over(void) 
{
    action_update_game_over();
    action_draw_game_over();
}

// action_score 状态为score时的action
void action_score(void) 
{ 
    action_update_score();
    action_draw_score(); 
}

// Event 不同事件
typedef enum
{
    event_start_game = 0,     // 事件：开始游戏
    event_press_key_enter,    // 事件：按下回车键
    event_press_key_space,    // 事件：按下空格键
    event_press_key_z,        // 事件：按下z键盘
    event_press_key_x,        // 事件：需要x键
    event_press_key_c,        // 事件：需要c键
    event_press_key_v,        // 事件：需要v键
    event_game_over           // 事件：游戏结束
} Event;

// State 不同状态
typedef enum
{
    state_START = 0,   // START 开始界面
    state_THEME = 1,   // THEME 介绍游戏主题界面
    state_RULE = 2,    // RULE 介绍游戏规则界面
    state_SELECT_MUSIC = 3,   // SELECT_MUSIC 选择音乐界面
    state_GAME_OVER = 4,    // GAME_OVER 一轮游戏结束界面
    state_PLAY = 5,   // PLAY 游戏主界面
    state_SCORE = 6   // SCORE 分数界面
} State;

// StateInfo 状态表结构体
typedef struct
{
    State curState;         // 当前所处状态
    Event event;            // 发生事件
    Action_Func action;     // 执行动作
    State nextState;        // 下一个状态
} StateInfo;

// StateMachine 状态机结构体
typedef struct
{
    State currentState;     // 当前状态
    StateInfo *stateTable;  // 状态表指针
    uint8_t stateTableSize; // 状态表大小(元素个数)
} StateMachine;

// state_table 状态表结构体数组
StateInfo state_table[] =
{
    {state_START, event_start_game, action_start, state_START},
    {state_START, event_press_key_space, action_theme, state_THEME},
    {state_THEME, event_press_key_enter, action_rule, state_RULE},
    {state_RULE, event_press_key_space, action_select_music, state_SELECT_MUSIC},
    {state_SELECT_MUSIC, event_press_key_z, action_play, state_PLAY},
    {state_SELECT_MUSIC, event_press_key_x, action_play, state_PLAY},
    {state_SELECT_MUSIC, event_press_key_c, action_play, state_PLAY},
    {state_SELECT_MUSIC, event_press_key_v, action_play, state_PLAY},
    {state_PLAY, event_game_over, action_select_music, state_SELECT_MUSIC},
    {state_GAME_OVER, event_press_key_enter, action_select_music, state_SELECT_MUSIC},
    {state_SCORE, event_press_key_enter, action_select_music, state_SELECT_MUSIC}
};

// state_machine_regist 注册状态机：将状态表的地址存入到状态机的状态表指针中
void state_machine_regist(StateMachine *pSM, StateInfo *pStateTable);

// find_state_table 查找状态表：根据状态机中的当前状态和发生的事件，从状态表中找到对应的结构体，返回指针
StateInfo *find_state_table(StateMachine *pSM, Event evt);

// runStateMachine 运行状态机：执行相应的动作，将结构体中存的下一个状态赋值给状态机的当前状态
void runStateMachine(StateMachine *pSM, Event evt);

// InitStateMachine 初始化有限状态机
void InitStateMachine(void);

// UpdateStateMachine 更新有限状态机
void UpdateStateMachine(void);

// SM_Type 有限状态机
StateMachine SM_Type;

// event 有限状态机的事件
Event event = event_press_key_enter;
