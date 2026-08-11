#include "FX.h"
#include "AudioObjects.h"

const int CHORUS_DELAY_LENGTH = AUDIO_BLOCK_SAMPLES * 16;
short chorusDelayLine[CHORUS_DELAY_LENGTH];

const int FLANGE_DELAY_LENGTH = AUDIO_BLOCK_SAMPLES * 6;
short flangeDelayLine[FLANGE_DELAY_LENGTH];

const int GRANULAR_MEMORY_SIZE = 12800;
int16_t granularMemory[GRANULAR_MEMORY_SIZE];

int currentGlobalFx = FX_CHORUS;
int globalFxMod[NUM_GLOBAL_FX] =
{
    256, 700, 350, 512, 256, 512
};
int globalFxMix[NUM_GLOBAL_FX] =
{
    0, 0, 0, 0, 0, 0
};

static float normalized(int value)
{
    return constrain(value, 0, 1023) / 1023.0f;
}

static void updateGlobalFx(GlobalFxType fx)
{
    int mod = globalFxMod[fx];

    switch(fx)
    {
        case FX_CHORUS:
            chorus1.voices(map(mod, 0, 1023, 2, 8));
            break;

        case FX_REVERB:
            reverb1.roomsize(normalized(mod));
            reverb1.damping(0.35f + normalized(mod) * 0.45f);
            break;

        case FX_DELAY:
            delay1.delay(0, map(mod, 0, 1023, 40, 850));
            break;

        case FX_FLANGE:
        {
            int offset = FLANGE_DELAY_LENGTH / 4;
            int depth = map(mod, 0, 1023, 8, FLANGE_DELAY_LENGTH / 3);
            float rate = 0.05f + normalized(mod) * 4.95f;

            flange1.voices(offset, depth, rate);
            break;
        }

        case FX_BITCRUSHER:
            bitcrusher1.bits(map(mod, 0, 1023, 16, 4));
            bitcrusher1.sampleRate(map(mod, 0, 1023, 44100, 2500));
            break;

        case FX_GRANULAR:
            granular1.beginPitchShift(map(mod, 0, 1023, 20, 180));
            granular1.setSpeed(0.5f + normalized(mod) * 1.5f);
            break;

        case NUM_GLOBAL_FX:
            break;
    }
}

static void updateGlobalFxMixers()
{
    mixer12.gain(0, 1.0f);
    mixer12.gain(1, normalized(globalFxMix[FX_CHORUS]) * 0.7f);
    mixer12.gain(2, normalized(globalFxMix[FX_REVERB]) * 0.7f);
    mixer12.gain(3, normalized(globalFxMix[FX_DELAY]) * 0.7f);

    mixer13.gain(0, normalized(globalFxMix[FX_BITCRUSHER]) * 0.7f);
    mixer13.gain(1, normalized(globalFxMix[FX_GRANULAR]) * 0.7f);
    mixer13.gain(2, normalized(globalFxMix[FX_FLANGE]) * 0.7f);
    mixer13.gain(3, 0.0f);

    mixer14.gain(0, 1.0f);
    mixer14.gain(1, 1.0f);
    mixer14.gain(2, 0.0f);
    mixer14.gain(3, 0.0f);
}

void initGlobalFx()
{
    mixer12.gain(0, 1.0);
    mixer12.gain(1, 0.0);
    mixer12.gain(2, 0.0);
    mixer12.gain(3, 0.0);

    mixer13.gain(0, 0.0);
    mixer13.gain(1, 0.0);
    mixer13.gain(2, 0.0);
    mixer13.gain(3, 0.0);

    mixer14.gain(0, 1.0);
    mixer14.gain(1, 1.0);
    mixer14.gain(2, 0.0);
    mixer14.gain(3, 0.0);

    chorus1.begin(chorusDelayLine, CHORUS_DELAY_LENGTH, 2);
    flange1.begin(
        flangeDelayLine,
        FLANGE_DELAY_LENGTH,
        FLANGE_DELAY_LENGTH / 4,
        FLANGE_DELAY_LENGTH / 4,
        0.5f
    );
    reverb1.roomsize(0.6);
    reverb1.damping(0.5);
    delay1.delay(0, 320);
    bitcrusher1.bits(16);
    bitcrusher1.sampleRate(44100);
    granular1.begin(granularMemory, GRANULAR_MEMORY_SIZE);
    granular1.beginPitchShift(80);
    granular1.setSpeed(1.0);

    for(int fx = 0; fx < NUM_GLOBAL_FX; fx++)
        updateGlobalFx((GlobalFxType)fx);

    updateGlobalFxMixers();
}

void nextGlobalFx()
{
    currentGlobalFx++;

    if(currentGlobalFx >= NUM_GLOBAL_FX)
        currentGlobalFx = 0;
}

GlobalFxType getCurrentGlobalFx()
{
    return (GlobalFxType)currentGlobalFx;
}

const char* getGlobalFxName()
{
    switch(getCurrentGlobalFx())
    {
        case FX_CHORUS:     return "Chorus";
        case FX_REVERB:     return "Reverb";
        case FX_DELAY:      return "Delay";
        case FX_FLANGE:     return "Flange";
        case FX_BITCRUSHER: return "Bitcrush";
        case FX_GRANULAR:   return "Granular";
        case NUM_GLOBAL_FX:  return "FX";
    }

    return "FX";
}

const char* getGlobalFxShortName()
{
    switch(getCurrentGlobalFx())
    {
        case FX_CHORUS:     return "Chorus";
        case FX_REVERB:     return "Reverb";
        case FX_DELAY:      return "Delay";
        case FX_FLANGE:     return "Flange";
        case FX_BITCRUSHER: return "Bitcr.";
        case FX_GRANULAR:   return "Gran.";
        case NUM_GLOBAL_FX:  return "FX";
    }

    return "FX";
}

void setGlobalFxMod(int value)
{
    globalFxMod[currentGlobalFx] = constrain(value, 0, 1023);
    updateGlobalFx(getCurrentGlobalFx());
}

void setGlobalFxMix(int value)
{
    globalFxMix[currentGlobalFx] = constrain(value, 0, 1023);
    updateGlobalFxMixers();
}

String getGlobalFxModDisplay()
{
    int mod = globalFxMod[currentGlobalFx];

    switch(getCurrentGlobalFx())
    {
        case FX_CHORUS:
            return String(map(mod, 0, 1023, 2, 8)) + " voices";

        case FX_REVERB:
            return String(map(mod, 0, 1023, 0, 100)) + "% room";

        case FX_DELAY:
            return String(map(mod, 0, 1023, 40, 850)) + " ms";

        case FX_FLANGE:
            return String(0.05f + normalized(mod) * 4.95f, 2) + " Hz";

        case FX_BITCRUSHER:
            return String(map(mod, 0, 1023, 16, 4)) + " bits";

        case FX_GRANULAR:
            return String(map(mod, 0, 1023, 50, 200) / 100.0f, 2) + "x";

        case NUM_GLOBAL_FX:
            break;
    }

    return "";
}

String getGlobalFxMixDisplay()
{
    return String(map(globalFxMix[currentGlobalFx], 0, 1023, 0, 100)) + "%";
}
