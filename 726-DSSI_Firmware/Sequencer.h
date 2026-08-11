#pragma once

#include <Arduino.h>
#include "Voice.h"

const int MAX_STEPS = 64;
const int NUM_SEQUENCER_VOICES = 8;

enum SequencerMode
{
    STOPPED,
    PLAYING,
    LIVE_RECORD,
    STEP_EDIT
};

struct SequenceStep
{
    bool active = false;
    uint8_t midiNote = 60;

    uint16_t gateTime = 125;   // milliseconds
};

struct VoiceSequence
{
    SequenceStep steps[MAX_STEPS];
};

namespace Sequencer
{
    void begin();
    void update();

    void play();
    void stop();

    void togglePlay();
    void toggleRecord();
    void toggleStepMode();
    int currentStep();

    bool isPlaying();

    void setTempo(int bpm);
    int getTempo();

    void setLength(int length);
    int getLength();

    SequencerMode getMode();
}
