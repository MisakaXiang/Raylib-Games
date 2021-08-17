#include "updateplayer.h"


void UpdatePlayer(Player *player, EnvItem *envBlocks, int envItemsLength, float delta)
{
    if (!GameOver)
    {

        if (IsKeyDown(KEY_SPACE))
        {
            player->speed.y = -PLAYER_JUMP_SPD;
        }
        
        int JumpOnBlocks = 0;
        CheckIfJumpOnBlocks(player, envBlocks, &JumpOnBlocks, delta);
        
        UpdatePlayerPosition(player, &JumpOnBlocks, delta);
        UpdatePlayerRect(player);

        if (IsKeyDown(KEY_A))
        {
            ComputeScore(player, souls);
        }

        CheckIfSoulsPass(player, souls);
        CheckIfGameOver(player, envBlocks);
        CheckIfSoulsEnd(player, souls);

        /* ???
        if (player->position.y > envBlocks[1].rect.y + envBlocks[1].rect.height)
        {
            GameOver = true;
            interface = 4;
        }
        */
    }
}
