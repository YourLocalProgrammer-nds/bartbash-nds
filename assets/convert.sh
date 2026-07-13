#!/bin/bash

BLOCKSDS="${BLOCKSDS:-/opt/blocksds/core/}"
GRIT=$BLOCKSDS/tools/grit/grit

$GRIT sprites.png -ftB -fh! -gTFF00FF -gt -gB8 -m!
$GRIT bigsprites.png -ftB -fh! -gTFF00FF -gt -gB8 -m!
$GRIT pacman.png -ftB -fh! -gTFF00FF -gt -gB8 -m!
$GRIT explode.png -ftB -fh! -gTFF00FF -gt -gB8 -m!

mv *.pal *.img ../nitrofiles/sprite

$GRIT bg1.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT bg2.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT bg3.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT bg4.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT bg5.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT bg6.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT bg7.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT bg8.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT bg9.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs
$GRIT gmdialog.png -ftB -fh! -gTFF00FF -gt -gB8 -mR8 -mLs

mv *.pal *.img *.map ../nitrofiles/bg

$GRIT gmhit.png -ftB -fh! -gt -gB8 -mRtp -mLf -p!

mv *.img *.map ../nitrofiles/maps

$GRIT default.png -ftB -fh! -gTFF00FF -gt -gB8 -m!

mv *.pal *.img ../nitrofiles/fnt