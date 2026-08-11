#pragma once

#include <Arduino.h>

enum GlobalFxType
{
    FX_CHORUS,
    FX_REVERB,
    FX_DELAY,
    FX_FLANGE,
    FX_BITCRUSHER,
    FX_GRANULAR,
    NUM_GLOBAL_FX
};

void initGlobalFx();

void nextGlobalFx();
GlobalFxType getCurrentGlobalFx();
const char* getGlobalFxName();
const char* getGlobalFxShortName();

void setGlobalFxMod(int value);
void setGlobalFxMix(int value);

String getGlobalFxModDisplay();
String getGlobalFxMixDisplay();
