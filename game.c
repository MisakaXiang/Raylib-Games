#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include <stdio.h>
#include<stdlib.h>

#define G 400
#define PLAYER_JUMP_SPD 100.0f
#define PLAYER_HOR_SPD 200.0f

#define MAX_NOTE_NUM 999
typedef struct Player {
    Rectangle rect;
    Vector2 position;
    Vector2 speed;
    bool canJump;
} Player;

typedef struct Soul {
    Rectangle rect;
    Color color;
    bool flag;
    int miss;
    int collect;
    int pass;
} Soul;

typedef struct EnvItem {
    Rectangle rect;
    int blocking;
    Color color;
    int tex_type;
} EnvItem;

/*
static enum
{
    start = 0,
    theme = 1,
    menu = 2,
    rule = 3,
    journey = 4,
    failure = 5,
    play = 6
} interface;
*/




Vector2 build_size[6];
static const int screenWidth = 1000;
static const int screenHeight = 600;

static int on_build = 0;
static float score = 0.0f;
static float total_score = 0.0f;
static int perfect = 0;
static int great = 0;
static int miss = 0;

static void UpdateDrawFrame(void);  // Update and Draw (one frame)
static void InitGame(void);         // Initialize game
static void UnloadGame(void);       // Unload game


static EnvItem d_floor;
//{{ 0, screenHeight + 170, 20000, 400 }, 0, BROWN };
//d_floor.rect = (Rectangle){ 0, screenHeight + 170, 20000, 400};
//d_floor.blocking = 0;
//d_floor.color = BROWN;
//  = {{ 0, 0, 2000, 600 }, 0, BROWN };

static Player player = { 0 };
static EnvItem envItems[1000];
static Soul soul[MAX_NOTE_NUM];
static float notes[MAX_NOTE_NUM];

static Camera2D camera = { 0 };
static int cameraOption = 0;
static int cameraUpdatersLength = 0;

static void UpdateCameraCenter(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
static void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
static void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
static void UpdateCameraEvenOutOnLanding(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);
static void UpdateCameraPlayerBoundsPush(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height);

// Store pointers to the multiple update camera functions
static void (*cameraUpdaters[])(Camera2D*, Player*, EnvItem*, int, float, int, int) = {
        UpdateCameraCenter,
        UpdateCameraCenterInsideMap,
        UpdateCameraCenterSmoothFollow,
        UpdateCameraEvenOutOnLanding,
        UpdateCameraPlayerBoundsPush
};



static void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta);


static char *cameraDescriptions[] = {
        "Follow player center",
        "Follow player center, but clamp to map edges",
        "Follow player center; smoothed",
        "Follow player center horizontally; updateplayer center vertically after landing",
        "Player push camera on getting too close to screen edge"
};

static int envItemsLength = sizeof(envItems)/sizeof(envItems[0]);

static int note_num = 0;
static FILE *fpRead;
static FILE *fpWrite;

Texture2D start_tex = {0};
Texture2D theme_tex = {0};
Texture2D rule_tex = {0};
Texture2D journey_tex = {0};
Texture2D GameOver_tex = { 0 };
Texture2D play_tex = { 0 };
Texture2D score_tex = { 0 };

Texture2D player_tex = {0};
Texture2D soul_tex = {0};
Texture2D soul_boom_tex = {0};
Texture2D soul_miss_tex = {0};
Texture2D build_tex[5];
Sound soul_collect = {0};
Music back_sound = { 0 };
static bool GameOver = false;
static bool song_end = false;
static int interface = 0;
static int song_id = 1;


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 2d camera");
    InitAudioDevice();
    InitGame();

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {

        // Update and Draw
        //----------------------------------------------------------------------------------
        UpdateDrawFrame();
        //----------------------------------------------------------------------------------
        // Update
        //----------------------------------------------------------------------------------

    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadGame();         // Unload loaded data (textures, sounds, models...)
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void UpdatePlayer(Player *player, EnvItem *envItems, int envItemsLength, float delta)
{
    if (!GameOver)
    {

        player->position.x += player->speed.x;
        if (IsKeyDown(KEY_LEFT)) player->position.x -= PLAYER_HOR_SPD*delta;
        if (IsKeyDown(KEY_RIGHT)) player->position.x += PLAYER_HOR_SPD*delta;
        if (IsKeyDown(KEY_SPACE) && player->canJump)
        {
            player->speed.y = -PLAYER_JUMP_SPD;
            player->canJump = true;
        }


        int hitObstacle = 0;
        for (int i = 0; i < envItemsLength; i++)
        {
            EnvItem *ei = envItems + i;
            Vector2 *p = &(player->position);
            if (ei->blocking &&
                ei->rect.x <= p->x &&
                ei->rect.x + ei->rect.width >= p->x &&
                ei->rect.y >= p->y &&
                ei->rect.y < p->y + player->speed.y*delta)
            {
                hitObstacle = 1;
                player->speed.y = 0.0f;
                p->y = ei->rect.y;
            }
        }

        if (!hitObstacle)
        {
            player->position.y += player->speed.y*delta;
            player->speed.y += G*delta;
            player->canJump = true;
        }
        else player->canJump = true;
        player->rect.x = player->position.x - player->rect.width/2;
        player->rect.y = player->position.y - player->rect.height;


        for (int i = 0; i < note_num; i++){
            if ((player->position.x >= envItems[i].rect.x) && (player->position.x <= envItems[i].rect.x + envItems[i].rect.width)) {
                on_build = i;
            }
            if (CheckCollisionRecs(player->rect, envItems[i].rect) && (player->position.y > envItems[i].rect.y) && (on_build != i)){
                GameOver = true;
                interface = 4;
            }
        }

        for (int i = 0;i < note_num;i++)
        {
            if ((player->rect.x - soul[i].rect.x) > 10 && (soul[i].collect == 0) && soul[i].pass == 0)
            {
                miss += 1;
                soul[i].pass = 1;
                soul[i].miss = 1;
            }
        }

        if (IsKeyDown(KEY_A))
        {
            // int temp = 0;
            // find the closest soul
            for (int i = 0;i < note_num;i++)
            {
                //int min = 999;
                int dis = (player->rect.x - soul[i].rect.x) * (player->rect.x - soul[i].rect.x) + (player->rect.y - soul[i].rect.y) * (player->rect.y - soul[i].rect.y);
                if ((dis <= 400) && soul[i].pass == 0)
                {
                    PlaySound(soul_collect);
                    score = 400 - dis;
                    if (score > 200) {perfect += 1;soul[i].collect = 1; soul[i].miss = 0; soul[i].pass = 1;}
                    if (score > 0 && score < 200 ) {great += 1; soul[i].collect = 1;soul[i].miss = 0; soul[i].pass = 1;}
                    /*
                    if (dis < min)
                    {
                        min = dis;
                        temp = i;
                    }
                    */
                }
            }
            //score = 400 - (player->rect.x - soul[temp].rect.x) * (player->rect.x - soul[temp].rect.x);
            //if (score > 90) {perfect += 1;soul[temp].collect = 1; soul[temp].miss = 0; soul[temp].pass = 1;}
            //if (score > 0 && score < 90 ) {great += 1; soul[temp].collect = 1;soul[temp].miss = 0; soul[temp].pass = 1;}
        }




        if (player->position.y > envItems[1].rect.y + envItems[1].rect.height)
        {
            GameOver = true;
            interface = 4;
        }

        if (player->position.x > (notes[note_num - 1] + 100))
        {
            song_end = true;
            total_score = (perfect * 1.2 + great * 0.8)/ note_num;
            StopMusicStream(back_sound);
            interface = 6;
        }
    }

}

void UpdateCameraCenter(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    camera->target = player->position;
}

void UpdateCameraCenterInsideMap(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    camera->target = player->position;
    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    float minX = 1000, minY = 1000, maxX = -1000, maxY = -1000;

    for (int i = 0; i < envItemsLength; i++)
    {
        EnvItem *ei = envItems + i;
        minX = fminf(ei->rect.x, minX);
        maxX = fmaxf(ei->rect.x + ei->rect.width, maxX);
        minY = fminf(ei->rect.y, minY);
        maxY = fmaxf(ei->rect.y + ei->rect.height, maxY);
    }

    Vector2 max = GetWorldToScreen2D((Vector2){ maxX, maxY }, *camera);
    Vector2 min = GetWorldToScreen2D((Vector2){ minX, minY }, *camera);

    if (max.x < width) camera->offset.x = width - (max.x - width/2);
    if (max.y < height) camera->offset.y = height - (max.y - height/2);
    if (min.x > 0) camera->offset.x = width/2 - min.x;
    if (min.y > 0) camera->offset.y = height/2 - min.y;
}

void UpdateCameraCenterSmoothFollow(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    static float minSpeed = 30;
    static float minEffectLength = 10;
    static float fractionSpeed = 0.8f;

    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    Vector2 diff = Vector2Subtract(player->position, camera->target);
    float length = Vector2Length(diff);

    if (length > minEffectLength)
    {
        float speed = fmaxf(fractionSpeed*length, minSpeed);
        camera->target = Vector2Add(camera->target, Vector2Scale(diff, speed*delta/length));
    }
}

void UpdateCameraEvenOutOnLanding(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    static float evenOutSpeed = 700;
    static int eveningOut = false;
    static float evenOutTarget;

    camera->offset = (Vector2){ width/2.0f, height/2.0f };
    camera->target.x = player->position.x;

    if (eveningOut)
    {
        if (evenOutTarget > camera->target.y)
        {
            camera->target.y += evenOutSpeed*delta;

            if (camera->target.y > evenOutTarget)
            {
                camera->target.y = evenOutTarget;
                eveningOut = 0;
            }
        }
        else
        {
            camera->target.y -= evenOutSpeed*delta;

            if (camera->target.y < evenOutTarget)
            {
                camera->target.y = evenOutTarget;
                eveningOut = 0;
            }
        }
    }
    else
    {
        if (player->canJump && (player->speed.y == 0) && (player->position.y != camera->target.y))
        {
            eveningOut = 1;
            evenOutTarget = player->position.y;
        }
    }
}

void UpdateCameraPlayerBoundsPush(Camera2D *camera, Player *player, EnvItem *envItems, int envItemsLength, float delta, int width, int height)
{
    static Vector2 bbox = { 0.2f, 0.2f };

    Vector2 bboxWorldMin = GetScreenToWorld2D((Vector2){ (1 - bbox.x)*0.5f*width, (1 - bbox.y)*0.5f*height }, *camera);
    Vector2 bboxWorldMax = GetScreenToWorld2D((Vector2){ (1 + bbox.x)*0.5f*width, (1 + bbox.y)*0.5f*height }, *camera);
    camera->offset = (Vector2){ (1 - bbox.x)*0.5f * width, (1 - bbox.y)*0.5f*height };

    if (player->position.x < bboxWorldMin.x) camera->target.x = player->position.x;
    if (player->position.y < bboxWorldMin.y) camera->target.y = player->position.y;
    if (player->position.x > bboxWorldMax.x) camera->target.x = bboxWorldMin.x + (player->position.x - bboxWorldMax.x);
    if (player->position.y > bboxWorldMax.y) camera->target.y = bboxWorldMin.y + (player->position.y - bboxWorldMax.y);
}

// Initialize game variables
void InitGame(void)
{
    perfect = 0;
    miss = 0;
    great = 0;
    interface = 0;
    total_score = 0;
    GameOver = false;
    song_end = false;

    // read from note file and create notes
    double temp;
    if (song_id == 1){fpRead = fopen("song1.txt", "rb");}
    if (song_id == 2){fpRead = fopen("song2.txt", "rb");}
    if (song_id == 3){fpRead = fopen("song3.txt", "rb");}
    if (song_id == 4){fpRead = fopen("song4.txt", "rb");}

    note_num = 0;
    fpWrite = fopen("save_notes.txt", "w");
    while(fscanf(fpRead, "%lf", &temp) == 1)
    {
        notes[note_num] = (int)temp;
        note_num += 1;
    }
    fclose(fpRead);
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


    if (song_id == 1){back_sound = LoadMusicStream("resources/1.wav");}
    if (song_id == 2){back_sound = LoadMusicStream("resources/2.wav");}
    if (song_id == 3){back_sound = LoadMusicStream("resources/3.wav");}
    if (song_id == 4){back_sound = LoadMusicStream("resources/4.wav");}

    soul_collect = LoadSound("resources/soul_collect.wav");
    GameOver = false;


    d_floor.rect = (Rectangle){ 0, screenHeight+200, 20000, 400};
    d_floor.blocking = 0;
    d_floor.color = BROWN;

    srand((unsigned)(time(NULL)));

    build_size[0] = (Vector2){200, 400};
    build_size[1] = (Vector2){220, 600};
    build_size[2] = (Vector2){230, 500};
    build_size[3] = (Vector2){250, 600};
    build_size[4] = (Vector2){300, 500};

    envItems[0].rect.height = 200;
    envItems[0].rect.width = 400;
    envItems[0].rect.x = 0;
    envItems[0].rect.y = d_floor.rect.y - envItems[0].rect.height;
    //envItems[i].rect = (Rectangle){ i*300, d_floor.re  , 200, 400};
    envItems[0].blocking = 1;
    envItems[0].color = GRAY;


    for(int i = 1; i < note_num; i++) {
        //envItems[i].rect.height = 250;
        //envItems[i].rect.width = 200;
        //envItems[i].rect.x = i * 300;
        int num = rand() % 5;
        envItems[i].tex_type = num;
        envItems[i].rect.height = build_size[num].x;
        envItems[i].rect.width = build_size[num].y;
        envItems[i].rect.x = rand() % 100 + 100 + envItems[i - 1].rect.x + envItems[i - 1].rect.width;
        envItems[i].rect.y = d_floor.rect.y - envItems[i].rect.height;
        //envItems[i].rect = (Rectangle){ i*300, d_floor.re  , 200, 400};
        envItems[i].blocking = 1;
        envItems[i].color = GRAY;
    }

    player.rect = (Rectangle){ 0, d_floor.rect.y - 200 - 12.8, 12, 12.8};
    player.position = (Vector2){ player.rect.x, player.rect.y + player.rect.height };
    player.speed.y = 0;
    player.speed.x = 3;
    player.canJump = true;

    for (int i = 0; i < note_num; i++)
    {

        soul[i].rect.height = 10;
        soul[i].rect.width = 10;
        soul[i].rect.x = notes[i];
        soul[i].miss = 0;
        soul[i].collect = 0;
        soul[i].pass = 0;


        int s_on_build = -1;
        for (int j = 0; j < note_num; j++) {
            if ((soul[i].rect.x > envItems[j].rect.x) && (soul[i].rect.x < envItems[j].rect.x + envItems[j].rect.width)) {
                s_on_build = j;
                break;
            }
        }

        if (s_on_build >= 0) {
            soul[i].rect.y  =  envItems[s_on_build].rect.y - soul[i].rect.height - 5;
            soul[i].flag = true;
        }
        else {
            soul[i].rect.y  =  d_floor.rect.y - (rand() % 50 + 300);
            soul[i].flag = true;
        }

        soul[i].color = YELLOW;
    }


    camera.target = player.position;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    cameraOption = 0;
    cameraUpdatersLength = sizeof(cameraUpdaters)/sizeof(cameraUpdaters[0]);
}


// Update game (one frame)
void UpdateGame(void)
{
    if (interface == 0)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            interface = 1;
        }
    }

    if (interface == 1)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            interface = 2;
        }
    }

    if (interface == 2)
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            interface = 3;
        }
    }

    if (interface == 3)
    {
        // choose different song
        if (IsKeyPressed(KEY_Z))
        {
            song_id = 1;
            InitGame();
            interface = 5;
        }
        if (IsKeyPressed(KEY_X))
        {
            song_id = 2;
            InitGame();
            interface = 5;
        }
        if (IsKeyPressed(KEY_C))
        {
            song_id = 3;
            InitGame();
            interface =5;
        }
        if (IsKeyPressed(KEY_V))
        {
            song_id = 4;
            InitGame();
            interface = 5;
        }
    }

    if (interface == 4)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            interface = 3;
        }
    }

    if (interface == 5)
    {
        PlayMusicStream(back_sound);
        UpdateMusicStream(back_sound);
        float deltaTime = GetFrameTime();

        UpdatePlayer(&player, envItems, envItemsLength, deltaTime);

        if (player.position.x > notes[note_num - 1] + 500)
        {
            //StopMusicStream(back_sound);
        }

        if (IsKeyPressed(KEY_Q))
        {
            fprintf(fpWrite,"%lf\n", player.position.x);
        }

        if (IsKeyPressed(KEY_R))
        {
            player.position = (Vector2){ 400, 280 };
        }

        if (IsKeyPressed(KEY_C)) cameraOption = (cameraOption + 1)%cameraUpdatersLength;

        // Call update camera function by its pointer
        cameraUpdaters[cameraOption](&camera, &player, envItems, envItemsLength, deltaTime, screenWidth, screenHeight);
        //----------------------------------------------------------------------------------
    }

    if (interface == 6)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            interface = 3;
        }
    }

}

// Draw game (one frame)
void DrawGame(void)
{
    // Draw
    //----------------------------------------------------------------------------------
    if (interface == 4)
    {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        DrawTextureEx(GameOver_tex, (Vector2){0, 0}, 0, 1, WHITE);
        EndDrawing();
    }

    if (interface == 5)
    {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        DrawTextureEx(play_tex, (Vector2){0, 0}, 0, 1, WHITE);

        BeginMode2D(camera);

        for (int i = 0; i < envItemsLength; i++) DrawRectangleRec(envItems[i].rect, envItems[i].color);
        for (int i = 0; i < note_num; i++)
        {
            DrawRectangleRec(soul[i].rect, YELLOW);
            if (soul[i].miss) {DrawRectangleRec(soul[i].rect, ORANGE);}
            if (soul[i].collect) {DrawRectangleRec(soul[i].rect, RED);}
        }



        //Rectangle playerRect = { player.position.x - 20, player.position.y - 40, 40, 40 };
        //DrawRectangleRec(playerRect, RED);
        DrawTextureEx(player_tex, (Vector2){player.rect.x - player_tex.width * 0.06/2, player.rect.y - player_tex.height * 0.06/2 - 15}, 0, 0.06, WHITE);
        for (int i = 0; i < note_num; i++) {
            if (soul[i].collect) {
                DrawTextureEx(soul_boom_tex, (Vector2){soul[i].rect.x - soul_tex.width * 0.03/2 + 4, soul[i].rect.y - soul_tex.height * 0.03/2 + 4}, 0, 0.03, WHITE);
            } else if (soul[i].miss) {
                DrawTextureEx(soul_miss_tex, (Vector2){soul[i].rect.x - soul_tex.width * 0.02/2 + 4, soul[i].rect.y - soul_tex.height * 0.02/2 + 4}, 0, 0.02, WHITE);
            } else {
                DrawTextureEx(soul_tex, (Vector2){soul[i].rect.x - soul_tex.width * 0.02/2 + 4, soul[i].rect.y - soul_tex.height * 0.02/2 + 4}, 0, 0.02, WHITE);
            }

            int num = envItems[i].tex_type;
            DrawTextureEx(build_tex[num], (Vector2){envItems[i].rect.x, envItems[i].rect.y}, 0, 1, WHITE);
            //DrawTextureRec(build_tex[num], envItems[i].rect, (Vector2){envItems[i].rect.x - build_tex[num].width * 0.01/2, envItems[i].rect.y - build_tex[num].height * 0.01/2}, WHITE);
        }
        DrawRectangleRec(d_floor.rect, d_floor.color);

        EndMode2D();

        // DrawText(TextFormat("score: %05i", score), 307, 20, 40, GRAY);
        DrawText(TextFormat("perfect: %05i", perfect), 307, 20, 40, GRAY);
        DrawText(TextFormat("great: %05i", great), 307, 60, 40, GRAY);
        DrawText(TextFormat("miss: %05i", miss), 307, 100, 40, GRAY);


        DrawText("Controls:", 20, 20, 10, BLACK);
        DrawText("- Space to jump", 40, 40, 10, DARKGRAY);
        DrawText("- A to collect soul", 40, 60, 10, DARKGRAY);

        EndDrawing();

    }


    if (interface == 0)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextureEx(start_tex, (Vector2){0, 0}, 0, 1, WHITE);
        EndDrawing();
    }

    if (interface == 1)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextureEx(theme_tex, (Vector2){0, 0}, 0, 1, WHITE);
        EndDrawing();
    }

    if (interface == 2)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextureEx(rule_tex, (Vector2){0, 0}, 0, 1, WHITE);
        EndDrawing();
    }

    if (interface == 3)
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTextureEx(journey_tex, (Vector2){0, 0}, 0, 1, WHITE);
        EndDrawing();
    }
    if (interface == 6)
    {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        DrawTextureEx(score_tex, (Vector2){0, 0}, 0, 1, WHITE);
        DrawText(TextFormat("perfect: %05i", perfect), 57, 20, 40, WHITE);
        DrawText(TextFormat("great: %05i", great), 57, 60, 40, WHITE);
        DrawText(TextFormat("miss: %05i", miss), 57, 100, 40, WHITE);

        EndDrawing();
    }
    //----------------------------------------------------------------------------------
}

// Unload game variables
void UnloadGame(void)
{
    // TODO: Unload all dynamic loaded data (textures, sounds, models...)
}


// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}

