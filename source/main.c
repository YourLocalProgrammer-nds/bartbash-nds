#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <complex.h>
#include <nds.h>
#include <filesystem.h>
#include <nds/ndstypes.h>
#include <nf_lib.h>
#include <maxmod9.h>
#include <time.h>
#include <stdlib.h> 
#include <nds/arm9/dldi.h>
#include <fat.h>
#include <gl2d.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include "soundbank.h"
// variable setup
bool isheld = false;
s16 bartdata[127][4];
const float pi = M_PI;
int killedbarts = 0;
int room = 0;
bool dialog = true;
int spikex = 160;
char barttext[20][100] = {
    "click all the barts!!!!!",
    "spike the barts!!!!!",
};
s16 barts[3][10];
//couple functions
int32_t degreesToNflibAngle(float degrees) {
    return (int32_t)(degrees * 512 / 360) % 512;
}
float nfLibAngleToDegrees(int32_t nfAngle) {
    return fmodf(nfAngle * (360 / 512.0f), 360);
}
float nfLibAngleToRadians(int32_t nfAngle) {
    return nfAngle * (2 * pi / 512.0f);
}
int32_t radiansToNflibAngle(float rad) {
    return (int32_t)(rad * (512.0f / (2.0f * pi))) % 512;
}
int distanceto(s16 start[3], s16 end[3]) {
    return sqrt(((start[1] - end[1]) * (start[1] - end[1])) + ((start[2] - end[2]) * (start[2] - end[2])));
}
int loadroom(int roomnum) {
    if (roomnum == 0) {
        for (int i = 0; i < 30; i++) {
            bartdata[i][1] = rand() % 256;
            bartdata[i][2] = rand() % 192;
            bartdata[i][3] = 1 + (rand() % 359);
            bartdata[i][4] = 1 + (rand() % 5);
            NF_CreateSprite(1, i, 0, 0, bartdata[i][1], bartdata[i][2]);
        }
    }
    else {
        if (roomnum == 1) {
            NF_CreateSprite(1, 0, 0, 0, spikex, 136);
            NF_SpriteFrame(1, 0, 1);
            NF_CreateSprite(1, 1, 0, 0, 180, 100);
            NF_CreateSprite(1, 2, 0, 0, 220, 80);
            NF_CreateSprite(1, 3, 0, 0, 240, 100);
            barts[0][1] = 180;
            barts[0][2] = 100;
            barts[1][1] = 220;
            barts[1][2] = 80;
            barts[2][1] = 240;
            barts[2][2] = 100;
            for (int i = 0; i < 3; i++) {
                barts[i][3] = 0;
            }
        }
    }
}
int main(int argc, char** argv)
{
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);
    consoleDemoInit();
    printf("\n NitroFS init. Please wait.\n\n");
    printf(" Iniciando NitroFS,\n por favor, espere.\n\n");
    swiWaitForVBlank();
    swiWaitForVBlank();

    ssize_t len;

    nitroFSInit(NULL);
    NF_SetRootFolder("NITROFS");
    NF_Set2D(0, 0);
    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(0);
    NF_InitTiledBgSys(1);

    NF_InitSpriteBuffers();
    NF_InitSpriteSys(0);
    NF_InitSpriteSys(1);

    NF_InitCmapBuffers();

    NF_LoadTiledBg("bg/gmdialog", "gmbox", 256, 256);

    NF_CreateTiledBg(1, 3, "gmbox");

    NF_LoadTextFont("fnt/default", "normal", 256, 256, 0);
    NF_CreateTextLayer(1, 2, 0, "normal");

    srand(time(NULL));
    //loadgame(argv);
    NF_LoadSpriteGfx("sprite/sprites", 0, 16, 32);
    NF_LoadSpritePal("sprite/sprites", 0);
    NF_LoadSpriteGfx("sprite/explode", 1, 32, 64);
    NF_LoadSpritePal("sprite/explode", 1);
    NF_LoadCollisionBg("maps/gmhit", 0, 256, 256);
    NF_LoadTiledBg("bg/bg1", "level", 256, 256);
    NF_VramSpriteGfx(1, 0, 0, false);
    NF_VramSpritePal(1, 0, 0);
    mmInitDefault("nitro:/soundbank.bin");
    mmSelectMode(MM_MODE_C);
    mmLoadEffect(SFX_OW);
    while (1)
    {
        int held = keysHeld();
        int pressed = keysDown();
        NF_ClearTextLayer(1, 2);
        //consoleClear();
        scanKeys();
        touchPosition touchpos;
        touchRead(&touchpos);
        if (touchpos.px == 0 & touchpos.py == 0) {
            isheld = false;
        }
        else {
            isheld = true;
        }
        if (dialog == true) {
            NF_WriteText(1, 2, 1, 2, barttext[room]);
            if (NF_GetPoint(0, (touchpos.px), (touchpos.py)) == 4) {
                dialog = false;
                loadroom(room);
                char bgn[10];
                sprintf(bgn,"bg/bg%d", room+1);
                NF_UnloadTiledBg("level");
                NF_LoadTiledBg(bgn, "level", 256, 256);
                NF_DeleteTiledBg(1, 3);
                NF_CreateTiledBg(1, 3, "level");
            }
        }
        else {
            if (room == 0) {
                if (killedbarts == 30) {
                    dialog = true;
                    room++;
                    NF_DeleteTiledBg(1, 3);
                    NF_CreateTiledBg(1, 3, "gmbox");
                }
                for (int i = 0; i < 30; i++) {
                    if (bartdata[i][1] != -64) {
                        s16 backup[3];
                        backup[1] = bartdata[i][1];
                        backup[2] = bartdata[i][2];
                        bartdata[i][1] = bartdata[i][1] + ceilf((int)(sinf(nfLibAngleToRadians(bartdata[i][3])) * bartdata[i][4]));
                        bartdata[i][2] = bartdata[i][2] + ceilf((cosf(nfLibAngleToRadians(bartdata[i][3])) * bartdata[i][4]));
                        if (bartdata[i][1] > 256) {
                            bartdata[i][1] = 5;
                        }
                        if (bartdata[i][1] < 0) {
                            bartdata[i][1] = 251;
                        }
                        if (bartdata[i][2] > 192) {
                            bartdata[i][2] = 5;
                        }
                        if (bartdata[i][2] < 0) {
                            bartdata[i][2] = 187;
                        }
                        if (backup[1] == bartdata[i][1]) {
                            bartdata[i][1] = bartdata[i][1] + ceilf((int)(sinf(nfLibAngleToRadians(bartdata[i][3])) * bartdata[i][4]));
                        }
                        if (backup[2] == bartdata[i][2]) {
                            bartdata[i][2] = bartdata[i][2] + ceilf((cosf(nfLibAngleToRadians(bartdata[i][3])) * bartdata[i][4]));
                        }
                        NF_MoveSprite(1, i, bartdata[i][1], bartdata[i][2]);
                    }
                }
                if (isheld == true) {
                    for (int b = 0; b < 30; b++) {
                        if (bartdata[b][1] != -64) {
                            s16 st[3];
                            st[1] = touchpos.px;
                            st[2] = touchpos.py;
                            s16 eat[3];
                            eat[1] = bartdata[b][1];
                            eat[2] = bartdata[b][2];
                            if (distanceto(st, eat) < 20) {
                                mmEffect(SFX_OW);
                                NF_DeleteSprite(1, b);
                                bartdata[b][1] = -64;
                                bartdata[b][2] = '\0';
                                bartdata[b][3] = '\0';
                                bartdata[b][4] = '\0';
                                killedbarts++;
                            }
                        }
                    }
                }
            }
            if (room == 1) {
                if (held & KEY_LEFT) {
                    spikex--;
                }
                NF_MoveSprite(1, 0, spikex, 136);
                for (int i = 0; i < 3; i++) {
                    barts[i][1] = barts[i][1] - 1;
                    barts[i][3]--;
                    if (barts[i][2] > 135) {
                        barts[i][3] = 8;
                    }
                    barts[i][2] = barts[i][2] - barts[i][3];
                    NF_MoveSprite(1,i+1,barts[i][1],barts[i][2]);
                    for (int b = 0; b < 3; b++) {
                        if (barts[b][1] != -64) {
                            s16 st[3];
                            st[1] = spikex;
                            st[2] = 136;
                            s16 eat[3];
                            eat[1] = barts[b][1];
                            eat[2] = barts[b][2];
                            if (distanceto(st, eat) < 16) {
                                mmEffect(SFX_OW);
                                NF_DeleteSprite(1, 1+b);
                                barts[b][1] = -64;
                                barts[b][2] = '\0';
                                barts[b][3] = '\0';
                                killedbarts++;
                            }
                        }
                    }
                }
            }
        
        }
        // Update OAM array
        NF_SpriteOamSet(0);
        NF_SpriteOamSet(1);

        // Wait for the screen refresh
        swiWaitForVBlank();

        // Update OAM
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
        NF_UpdateTextLayers();
    }

    return 0;
}
