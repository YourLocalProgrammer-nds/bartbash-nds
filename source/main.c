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
int room = 8;
bool dialog = true;
int spikex = 160;
bool down = false;
int thwompy = 5;
int killtick = 0;
int nsp = 0;
int cary = 90;
int pacframe = 0;
int frametick = 0;
bool nextroom = true;
bool rightbart = false;
bool bpressed = false;
s16 pacpos[3];
char barttext[20][100] = {
    "click all the barts!!!!!",
    "spike the barts!!!!!",
    "bart splat!!!!!!!!!",
    "bart goes boom!!!!!!!!!!",
    "bart turns into frogger!!!!!!!",
    "eat the barts!!!!!!",
    "where's the real bart????",
    "you ARE bart!!!!!!!",
    "attack!!!!!!!!",
};
s16 barts[3][10];
s16 bullet[120][2];
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
    return sqrt(((start[0] - end[0]) * (start[0] - end[0])) + ((start[1] - end[1]) * (start[1] - end[1])));
}
int loadroom(int roomnum) {
    nextroom = true;
    killedbarts = 0;
    down = false;
    killtick = 0;
    switch (roomnum) {
        case 0:
            for (int i = 0; i < 30; i++) {
                bartdata[i][1] = rand() % 256;
                bartdata[i][2] = rand() % 192;
                bartdata[i][3] = 1 + (rand() % 359);
                bartdata[i][4] = 1 + (rand() % 5);
                NF_CreateSprite(1, i, 0, 0, bartdata[i][1], bartdata[i][2]);
            }
            break;
        case 1:
            mmUnload(MOD_LVL1MUSIC);
            mmLoad(MOD_LVL2MUSIC);
            mmStart(MOD_LVL2MUSIC, MM_PLAY_LOOP);
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
            break;
        case 2:
            mmUnload(MOD_LVL2MUSIC);
            mmLoad(MOD_LVL3MUSIC);
            mmStart(MOD_LVL3MUSIC, MM_PLAY_LOOP);
            NF_CreateSprite(1, 0, 0, 0, 112, 5);
            NF_SpriteFrame(1, 0, 2);
            NF_CreateSprite(1, 1, 0, 0, 112, 120);
            NF_CreateSprite(1, 2, 0, 0, 112, 40);
            NF_SpriteFrame(1, 2, 3);
            thwompy = 5;
            break;
        case 3:
            mmUnload(MOD_LVL3MUSIC);
            mmLoad(MOD_LVL4MUSIC);
            mmStart(MOD_LVL4MUSIC, MM_PLAY_LOOP);
            NF_CreateSprite(1, 0, 0, 0, 90, 35);
            NF_CreateSprite(1, 1, 0, 0, 110, 140);
            NF_SpriteFrame(1, 1, 4);
            nsp = 0;
            break;
        case 4:
            mmUnload(MOD_LVL4MUSIC);
            mmLoad(MOD_LVL5MUSIC);
            mmStart(MOD_LVL5MUSIC, MM_PLAY_LOOP);
            NF_CreateSprite(1, 0, 0, 0, 10, 45);
            NF_CreateSprite(1, 1, 1, 1, 100, 90);
            NF_CreateSprite(1, 2, 1, 1, 160, 90);
            barts[1][1] = 10;
            break;
        case 5:
            mmUnload(MOD_LVL5MUSIC);
            mmLoad(MOD_LVL6MUSIC);
            mmStart(MOD_LVL6MUSIC, MM_PLAY_LOOP);
            NF_CreateSprite(1, 0, 2, 2, 10, 100);
            NF_CreateSprite(1, 1, 0, 0, 112, 30);
            NF_CreateSprite(1, 2, 0, 0, 60, 120);
            NF_CreateSprite(1, 3, 0, 0, 170, 150);
            NF_CreateSprite(1, 4, 0, 0, 200, 45);
            pacpos[1] = 10;
            pacpos[2] = 100;
            bartdata[0][1] = 112;
            bartdata[0][2] = 30;
            bartdata[1][1] = 60;
            bartdata[1][2] = 120;
            bartdata[2][1] = 170;
            bartdata[2][2] = 150;
            bartdata[3][1] = 200;
            bartdata[3][2] = 45;
            break;
        case 6:
            mmUnload(MOD_LVL6MUSIC);
            mmLoad(MOD_LVL7MUSIC);
            mmStart(MOD_LVL7MUSIC, MM_PLAY_LOOP);
            NF_CreateSprite(1, 0, 0, 0, 25, 150);
            NF_CreateSprite(1, 1, 0, 0, 0, 50);
            NF_CreateSprite(1, 2, 0, 0, 32, 105);
            NF_CreateSprite(1, 3, 0, 0, 45, 50);
            NF_CreateSprite(1, 4, 0, 0, 60, 10);
            NF_CreateSprite(1, 5, 0, 0, 120, 0);
            NF_CreateSprite(1, 6, 0, 0, 165, 20);
            NF_CreateSprite(1, 7, 0, 0, 155, 60);
            NF_CreateSprite(1, 8, 0, 0, 90, 100);
            NF_CreateSprite(1, 9, 0, 0, 205, 100);
            NF_CreateSprite(1, 10, 0, 0, 190, 145);
            bartdata[0][1] = 25;
            bartdata[0][2] = 150;
            bartdata[1][1] = 0;
            bartdata[1][2] = 50;
            bartdata[2][1] = 32;
            bartdata[2][2] = 105;
            bartdata[3][1] = 45;
            bartdata[3][2] = 50;
            bartdata[4][1] = 60;
            bartdata[4][2] = 10;
            bartdata[5][1] = 120;
            bartdata[5][2] = 0;
            bartdata[6][1] = 165;
            bartdata[6][2] = 20;
            bartdata[7][1] = 155;
            bartdata[7][2] = 60;
            bartdata[8][1] = 90;
            bartdata[8][2] = 100;
            bartdata[9][1] = 205;
            bartdata[9][2] = 100;
            bartdata[10][1] = 190;
            bartdata[10][2] = 145;
            break;
        case 7:
            mmUnload(MOD_LVL7MUSIC);
            mmLoad(MOD_LVL8MUSIC);
            mmStart(MOD_LVL8MUSIC, MM_PLAY_LOOP);
            NF_CreateSprite(1, 0, 0, 0, 35, 100);
            pacpos[1] = 35;
            pacpos[2] = 100;
            NF_CreateSprite(1, 1, 0, 0, 90, 120);
            NF_CreateSprite(1, 2, 0, 0, 145, 110);
            NF_CreateSprite(1, 3, 0, 0, 145, 35);
            NF_CreateSprite(1, 4, 0, 0, 205, 80);
            NF_CreateSprite(1, 5, 0, 0, 185, 20);
            NF_SpriteFrame(1, 1, 1);
            NF_SpriteFrame(1, 2, 1);
            NF_SpriteFrame(1, 3, 1);
            NF_SpriteFrame(1, 4, 1);
            NF_SpriteFrame(1, 5, 2);
            bartdata[0][1] = 90;
            bartdata[0][2] = 120;
            bartdata[1][1] = 145;
            bartdata[1][2] = 110;
            bartdata[2][1] = 145;
            bartdata[2][2] = 35;
            bartdata[3][1] = 205;
            bartdata[3][2] = 80;
            bartdata[4][1] = 185;
            bartdata[4][2] = 20;
            thwompy = 20;
            break;
        case 8:
            mmUnload(MOD_LVL8MUSIC);
            mmLoad(MOD_LVL9MUSIC);
            mmStart(MOD_LVL9MUSIC, MM_PLAY_LOOP);
            NF_CreateSprite(1, 0, 0, 0, 100, 100);
            break;
    }
}
bool distcheck(int sx,int sy,int ex,int ey,int distance) {
    s16 st[3];
    s16 eat[2];
    st[0] = sx;
    st[1] = sy;
    eat[0] = ex;
    eat[1] = ey;
    return distanceto(st, eat)<distance;
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
    NF_LoadSpriteGfx("sprite/sprites", 0, 32, 32);
    NF_LoadSpritePal("sprite/sprites", 0);
    NF_LoadSpriteGfx("sprite/bigsprites", 1, 32, 64);
    NF_LoadSpritePal("sprite/bigsprites", 1);
    NF_LoadSpriteGfx("sprite/pacman", 2, 32, 32);
    NF_LoadSpritePal("sprite/pacman", 2);
    NF_LoadCollisionBg("maps/gmhit", 0, 256, 256);
    NF_LoadTiledBg("bg/bg1", "level", 256, 256);
    NF_VramSpriteGfx(1, 0, 0, false);
    NF_VramSpritePal(1, 0, 0);
    NF_VramSpriteGfx(1, 1, 1, false);
    NF_VramSpritePal(1, 1, 1);
    NF_VramSpriteGfx(1, 2, 2, false);
    NF_VramSpritePal(1, 2, 2);
    mmInitDefault("nitro:/soundbank.bin");
    mmSelectMode(MM_MODE_C);
    mmLoadEffect(SFX_OW);
    mmLoadEffect(SFX_BOINK2);
    mmLoadEffect(SFX_GUNSHOT2);
    mmLoadEffect(SFX_SIRENLOOP);
    mmLoadEffect(SFX_SUCCESS);
    mmLoad(MOD_LVL1MUSIC);
    mmStart(MOD_LVL1MUSIC, MM_PLAY_LOOP);
    while (1)
    {
        int held = keysHeld();
        int pressed = keysDown();
        NF_ClearTextLayer(1, 2);
        scanKeys();
        touchPosition touchpos;
        touchRead(&touchpos);
        if (touchpos.px == 0 & touchpos.py == 0) {
            isheld = false;
            bpressed = false;
        }
        else {
            isheld = true;
        }
        if (dialog == true) {
            if (nextroom == false) {
                if (room == 6) {
                    if (rightbart == true) {
                        NF_WriteText(1, 2, 1, 2, "sweet nice job!!!!");
                    }
                    else {
                        NF_WriteText(1, 2, 1, 2, "wrong bart idiot!!!!!!!!!");
                    }
                }
            }
            else {
                NF_WriteText(1, 2, 1, 2, barttext[room]);
            }
            if (bpressed == false && isheld == true) {
                if (NF_GetPoint(0, (touchpos.px), (touchpos.py)) == 4) {
                    bpressed = true;
                    dialog = false;
                    if (nextroom == true) {
                        loadroom(room);
                    }
                    else {
                        if (room == 6) {
                            for (int i = 0; i < 11; i++) {
                                if (bartdata[i][1] != -64) {
                                    NF_ShowSprite(1, i, true);
                                }
                            }
                        }
                    }
                    if (nextroom == true) {
                        char bgn[10];
                        sprintf(bgn, "bg/bg%d", room + 1);
                        NF_UnloadTiledBg("level");
                        NF_LoadTiledBg(bgn, "level", 256, 256);
                        NF_DeleteTiledBg(1, 3);
                        NF_CreateTiledBg(1, 3, "level");
                    }
                    if (rightbart == true) {
                        bpressed = true;
                        dialog = true;
                        nextroom = true;
                        room++;
                        for (int i = 0; i < 11; i++) {
                            if (bartdata[i][1] != -64) {
                                NF_DeleteSprite(1, i);
                            }
                        }
                    }
                }
            }
        }
        else {
            switch (room) {
            case 0:
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
                            if (distcheck(touchpos.px, touchpos.py, bartdata[b][1], bartdata[b][2], 20)) {
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
                break;
            case 1:
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
                    NF_MoveSprite(1, i + 1, barts[i][1], barts[i][2]);
                    for (int b = 0; b < 3; b++) {
                        if (barts[b][1] != -64) {
                            if (distcheck(spikex,136,barts[b][1],barts[b][2],16)) {
                                mmEffect(SFX_OW);
                                NF_DeleteSprite(1, 1 + b);
                                barts[b][1] = -64;
                                barts[b][2] = '\0';
                                barts[b][3] = '\0';
                                killedbarts++;
                            }
                        }
                    }
                }
                if (killedbarts == 3) {
                    dialog = true;
                    NF_DeleteSprite(1, 0);
                    room++;
                    NF_DeleteTiledBg(1, 3);
                    NF_CreateTiledBg(1, 3, "gmbox");
                }
                break;
            case 2:
                if (pressed & KEY_DOWN) {
                    down = true;
                    NF_ShowSprite(1, 2, false);
                }
                if (down == true) {
                    thwompy = thwompy + 4;
                }
                NF_MoveSprite(1, 0, 112, thwompy);
                if (killtick > 0) {
                    killtick--;
                    mmEffect(SFX_BOINK2);
                    if (killtick == 0) {
                        dialog = true;
                        NF_DeleteSprite(1, 0);
                        room++;
                        NF_DeleteTiledBg(1, 3);
                        NF_CreateTiledBg(1, 3, "gmbox");
                    }
                }
                if (thwompy == 93) {
                    killtick = 15;
                    NF_DeleteSprite(1, 1);
                }
                break;
            case 3:
                if (pressed & KEY_A) {
                    NF_CreateSprite(1, nsp+2, 0, 0, 110, 140);
                    NF_SpriteFrame(1, nsp + 2, 5);
                    bullet[nsp][1] = 140;
                    nsp++;
                    mmEffect(SFX_GUNSHOT2);
                }
                if (down == true) {
                    dialog = true;
                    NF_DeleteSprite(1, 0);
                    NF_DeleteSprite(1, 1);
                    room++;
                    NF_DeleteTiledBg(1, 3);
                    NF_CreateTiledBg(1, 3, "gmbox");
                }
                for (int i = 0; i < nsp; i++) {
                    if (bullet[i][1] != -64) {
                        bullet[i][1] = bullet[i][1] - 2;
                        NF_MoveSprite(1, i + 2, 110, bullet[i][1]);
                        if (bullet[i][1] <= 40) {
                            down = true;
                        }
                        if (down == true) {
                            bullet[i][1] = -64;
                            NF_DeleteSprite(1, i + 2);
                            mmEffect(SFX_OW);
                        }
                    }
                }
                break;
            case 4:
                if (held & KEY_UP) {
                    cary = cary - 3;
                }
                if (held & KEY_DOWN) {
                    cary = cary + 3;
                }
                barts[1][1] = barts[1][1] + 4;
                if (barts[1][1] > 256) {
                    barts[1][1] = 0;
                }
                NF_MoveSprite(1, 0, barts[1][1], 45);
                NF_MoveSprite(1, 1, 100, cary);
                NF_MoveSprite(1, 2, 160, cary);
                if (distcheck(100, cary, barts[1][1], 45, 16)) {
                    barts[1][1] = -64;
                    mmEffect(SFX_OW);
                    NF_DeleteSprite(1, 0);
                    NF_DeleteSprite(1, 1);
                    NF_DeleteSprite(1, 2);
                    dialog = true;
                    room++;
                    NF_DeleteTiledBg(1, 3);
                    NF_CreateTiledBg(1, 3, "gmbox");
                }
                if (distcheck(160, cary, barts[1][1], 45, 16)) {
                    barts[1][1] = -64;
                    mmEffect(SFX_OW);
                    NF_DeleteSprite(1, 0);
                    NF_DeleteSprite(1, 1);
                    NF_DeleteSprite(1, 2);
                    dialog = true;
                    room++;
                    NF_DeleteTiledBg(1, 3);
                    NF_CreateTiledBg(1, 3, "gmbox");
                }
                break;
            case 5:
                frametick++;
                if (frametick == 3) {
                    frametick = 0;
                    pacframe++;
                    if (pacframe == 8) {
                        pacframe = 0;
                    }
                    NF_SpriteFrame(1, 0, pacframe);
                }
                if (held & KEY_UP) {
                    pacpos[2]--;
                }
                if (held & KEY_DOWN) {
                    pacpos[2]++;
                }
                if (held & KEY_LEFT) {
                    pacpos[1]--;
                }
                if (held & KEY_RIGHT) {
                    pacpos[1]++;
                }
                NF_MoveSprite(1, 0, pacpos[1], pacpos[2]);
                for (int i = 0; i < 4; i++) {
                    if (bartdata[i][1] != -64) {
                        if (distcheck(pacpos[1], pacpos[2], bartdata[i][1], bartdata[i][2], 16)) {
                            mmEffect(SFX_OW);
                            NF_DeleteSprite(1, i + 1);
                            bartdata[i][1] = -64;
                            killedbarts++;
                        }
                    }
                }
                if (killedbarts == 4) {
                    NF_DeleteSprite(1, 0);
                    dialog = true;
                    room++;
                    NF_DeleteTiledBg(1, 3);
                    NF_CreateTiledBg(1, 3, "gmbox");
                }
                break;
            case 6:
                for (int i = 0; i < 10; i++) {
                    if (distcheck(touchpos.px, touchpos.py, bartdata[i][1], bartdata[i][2], 20)) {
                        nextroom = false;
                        if (i == 0) {
                            mmEffect(SFX_SUCCESS);
                            rightbart = true;
                        }
                        else {
                            mmEffect(SFX_SIRENLOOP);
                            rightbart = false;
                        }
                        NF_DeleteSprite(1, i);
                        bartdata[i][1] = -64;
                        killedbarts++;
                        dialog = true;
                        NF_DeleteTiledBg(1, 3);
                        NF_CreateTiledBg(1, 3, "gmbox");
                        for (i = 0; i < 11; i++) {
                            if (bartdata[i][1] != -64) {
                                NF_ShowSprite(1, i, false);
                            }
                        }
                    }
                }
            case 7:
                if (held & KEY_UP) {
                    pacpos[2]--;
                }
                if (held & KEY_DOWN) {
                    pacpos[2]++;
                    down = true;
                }
                if (held & KEY_LEFT) {
                    pacpos[1]--;
                }
                if (held & KEY_RIGHT) {
                    pacpos[1]++;
                }
                if (down == true) {
                    thwompy = thwompy + 4;
                    bartdata[4][2] = thwompy;
                }
                NF_MoveSprite(1, 0, pacpos[1], pacpos[2]);
                NF_MoveSprite(1, 5, 185, thwompy);
                for (int i = 0; i < 6; i++) {
                    int dist = 0;
                    if (i == 4) {
                        dist = 32;
                    }
                    else {
                        dist = 16;
                    }
                    if (distcheck(pacpos[1], pacpos[2], bartdata[i][1], bartdata[i][2], dist)) {
                        mmEffect(SFX_OW);
                        for (int i = 0; i < 6; i++) {
                            NF_DeleteSprite(1, i);
                        }
                        dialog = true;
                        room++;
                        NF_DeleteTiledBg(1, 3);
                        NF_CreateTiledBg(1, 3, "gmbox");
                    }
                }
                break;
            }
        }
        NF_SpriteOamSet(0);
        NF_SpriteOamSet(1);

        swiWaitForVBlank();

        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
        NF_UpdateTextLayers();
    }

    return 0;
}
