#include <Arduino.h>

#include "Sequencer.h"
#include "Keyboard.h"
#include "Voice.h"
#include "AudioSystem.h"
#include "Display.h"


uint32_t nextStepTime = 0;
int tempoBPM = 120;
uint32_t stepInterval = 125;
uint32_t stepIntervalMicros = 125000;


namespace
{
    VoiceSequence sequences[NUM_SEQUENCER_VOICES];

    SequencerMode mode = STOPPED;

    int editStep = 0;
    int playStep = 0;
    int lastPlayedStep = 0;
    int sequenceLength = 16;

    bool waitingForRelease = false;

    bool pendingRecord = false;
    int pendingRecordStep = 0;
    uint8_t pendingMidiNote = 60;
    uint16_t pendingGateTime = 125;

    // Recording timing
    uint32_t notePressTime = 0;
    bool livePlayHeld = false;
    int livePlayVoice = 0;

    // Playback timing, tracked independently for each synth lane.
    bool playbackHeld[NUM_SEQUENCER_VOICES] = {};
    uint32_t playbackStartTime[NUM_SEQUENCER_VOICES] = {};
    SequenceStep* playbackStep[NUM_SEQUENCER_VOICES] = {};
}

void advanceStep();
static void updatePlaybackGates(uint32_t now);
static void commitPendingRecord();
static void playCurrentStep();
static void stopPlaybackVoice(int voice);
static void stopAllPlaybackVoices();
static bool selectedVoiceResting();

void Sequencer::begin()
{
    mode = STOPPED;

    editStep = 0;
    playStep = 0;
    lastPlayedStep = 0;
    sequenceLength = 16;

    for(int v=0; v<NUM_SEQUENCER_VOICES; v++)
    {
        for(int s=0; s<MAX_STEPS; s++)
        {
            sequences[v].steps[s].active = false;
            sequences[v].steps[s].midiNote = 60;
            sequences[v].steps[s].gateTime = stepInterval;
        }
    }
}

void advanceStep()
{
    playStep++;

    if(playStep >= sequenceLength)
        playStep = 0;
}

void Sequencer::update()
{
    switch(mode)
    {
        case STOPPED:

            if(Keyboard::notePressed())
            {
                playVoice(
                    Voices.getCurrentVoice(),
                    midiToFreq(
                        Keyboard::getPressedMidiNote()
                    )
                );
            }

            if(Keyboard::noteReleased())
            {
                stopVoice(
                    Voices.getCurrentVoice()
                );
            }
            break;

        case STEP_EDIT:
        {
            int voice = Voices.getCurrentVoice();

            if(Keyboard::notePressed() && !waitingForRelease)
            {
                SequenceStep &step =
                    sequences[voice].steps[editStep];

                step.active = true;
                step.midiNote = Keyboard::getPressedMidiNote();

                notePressTime = millis();

                playVoice(
                    voice,
                    midiToFreq(step.midiNote)
                );

                waitingForRelease = true;
            }

            if(waitingForRelease && Keyboard::noteReleased())
            {
                SequenceStep &step =
                    sequences[voice].steps[editStep];

                stopVoice(voice);

                step.gateTime = min(
                    (uint16_t)(millis() - notePressTime),
                    (uint16_t)stepInterval
                );

                waitingForRelease = false;

                editStep++;

                if(editStep >= sequenceLength)
                {
                    editStep = 0;
                    mode = STOPPED;

                    Serial.println("Sequence complete.");
                    OLED.showPopup("Sequence", "Complete!");
                }
            }

            break;
        }

        case PLAYING:
        {
            uint32_t now = millis();
            uint32_t nowMicros = micros();

            updatePlaybackGates(now);

            if(Keyboard::notePressed() && selectedVoiceResting())
            {
                playVoice(
                    Voices.getCurrentVoice(),
                    midiToFreq(
                        Keyboard::getPressedMidiNote()
                    )
                );

                livePlayHeld = true;
                livePlayVoice = Voices.getCurrentVoice();
            }

            if(livePlayHeld && Keyboard::noteReleased())
            {
                stopVoice(livePlayVoice);
                livePlayHeld = false;
            }

            // Advance sequencer
            if((int32_t)(nowMicros - nextStepTime) >= 0)
            {
                playCurrentStep();
                advanceStep();
                nextStepTime += stepIntervalMicros;

                if((int32_t)(nowMicros - nextStepTime) >=
                    (int32_t)stepIntervalMicros)
                {
                    nextStepTime = nowMicros + stepIntervalMicros;
                }
            }

            break;
        }

        case LIVE_RECORD:
        {
            uint32_t now = millis();
            uint32_t nowMicros = micros();

            updatePlaybackGates(now);

            if((int32_t)(nowMicros - nextStepTime) >= 0)
            {
                commitPendingRecord();
                playCurrentStep();
                advanceStep();
                nextStepTime += stepIntervalMicros;

                if((int32_t)(nowMicros - nextStepTime) >=
                    (int32_t)stepIntervalMicros)
                {
                    nextStepTime = nowMicros + stepIntervalMicros;
                }
            }

            if(Keyboard::notePressed())
            {
                pendingMidiNote =
                    Keyboard::getPressedMidiNote();

                playVoice(
                    Voices.getCurrentVoice(),
                    midiToFreq(pendingMidiNote)
                );

                pendingRecord = true;

                int32_t remainingMicros =
                    (int32_t)(nextStepTime - micros());

                if(remainingMicros < 0)
                    remainingMicros = 0;

                uint32_t elapsedMicros =
                    stepIntervalMicros -
                    (uint32_t)remainingMicros;

                if(elapsedMicros < stepIntervalMicros/2)
                    pendingRecordStep = lastPlayedStep;
                else
                {
                    pendingRecordStep = playStep;

                    if(pendingRecordStep >= sequenceLength)
                        pendingRecordStep = 0;
                }

                notePressTime = millis();
            }

            if(Keyboard::noteReleased())
            {
                uint16_t heldTime = millis() - notePressTime;
                pendingGateTime = min(heldTime, (uint16_t)stepInterval);
                
                stopVoice(
                    Voices.getCurrentVoice()
                );
            }

            break;
        }
    }
}


void Sequencer::play()
{
    nextStepTime = micros() + stepIntervalMicros;
    mode = PLAYING;
}

void Sequencer::stop()
{
    stopAllPlaybackVoices();
    mode = STOPPED;
}

void Sequencer::togglePlay()
{
    if(mode == PLAYING)
    {
        stopAllPlaybackVoices();

        mode = STOPPED;

        playStep = 0;
        lastPlayedStep = 0;

        return;
    }

    if(mode == LIVE_RECORD)
    {
        commitPendingRecord();
        mode = PLAYING;
        return;
    }

    if(mode == STOPPED)
    {
        editStep = 0;
        playStep = 0;
        lastPlayedStep = 0;

        nextStepTime = micros() + stepIntervalMicros;

        mode = PLAYING;

        pendingRecord = false;
        return;
    }
}

void Sequencer::toggleRecord()
{
    if(mode == STEP_EDIT)
    {
        int voice = Voices.getCurrentVoice();

        sequences[voice].steps[editStep].active = false;

        editStep++;

        if(editStep >= sequenceLength)
        {
            editStep = 0;
            mode = STOPPED;
            Serial.println("Sequence complete.");
            OLED.showPopup("Sequence", "Complete!");
        }

        return;
    }

    if(mode == PLAYING)
    {
        mode = LIVE_RECORD;
    } 

    else if(mode == LIVE_RECORD) {
        commitPendingRecord();
        mode = PLAYING;
    }
}

void Sequencer::toggleStepMode()
{
    if(mode == LIVE_RECORD)
    {
        int voice = Voices.getCurrentVoice();

        sequences[voice]
            .steps[lastPlayedStep]
            .active = false;

        return;
    }

    if(mode == STEP_EDIT)
    {
        mode = STOPPED;
    }
    else
    {
        mode = STEP_EDIT;
        editStep = 0;
    }
}

int Sequencer::currentStep()
{
    if(mode == STEP_EDIT)
        return editStep;

    return playStep;
}


static void playCurrentStep()
{
    lastPlayedStep = playStep;

    for(int voice = 0; voice < NUM_SEQUENCER_VOICES; voice++)
    {
        SequenceStep &step =
            sequences[voice].steps[playStep];

        if(!step.active)
        {
            stopPlaybackVoice(voice);
            continue;
        }

        updateVoice(voice);

        playVoice(
            voice,
            midiToFreq(step.midiNote)
        );

        playbackHeld[voice] = true;
        playbackStartTime[voice] = millis();
        playbackStep[voice] = &step;
    }
}


static void updatePlaybackGates(uint32_t now)
{
    for(int voice = 0; voice < NUM_SEQUENCER_VOICES; voice++)
    {
        if(playbackHeld[voice] &&
            playbackStep[voice] &&
            now - playbackStartTime[voice] >= playbackStep[voice]->gateTime)
        {
            stopPlaybackVoice(voice);
        }
    }
}

static void commitPendingRecord()
{
    if(!pendingRecord)
        return;

    int voice = Voices.getCurrentVoice();

    SequenceStep &step =
        sequences[voice].steps[pendingRecordStep];

    step.active = true;
    step.midiNote = pendingMidiNote;
    step.gateTime = pendingGateTime;

    pendingRecord = false;
}

static void stopPlaybackVoice(int voice)
{
    if(playbackHeld[voice])
        stopVoice(voice);

    playbackHeld[voice] = false;
    playbackStep[voice] = nullptr;
}

static void stopAllPlaybackVoices()
{
    for(int voice = 0; voice < NUM_SEQUENCER_VOICES; voice++)
        stopPlaybackVoice(voice);

    if(livePlayHeld)
    {
        stopVoice(livePlayVoice);
        livePlayHeld = false;
    }
}

bool Sequencer::isPlaying()
{
    return mode == PLAYING || mode == LIVE_RECORD;
}

static bool selectedVoiceResting()
{
    int voice = Voices.getCurrentVoice();

    return !sequences[voice].steps[lastPlayedStep].active;
}

void Sequencer::setTempo(int bpm)
{
    tempoBPM = constrain(bpm,40,240);

    stepIntervalMicros = 60000000UL / (tempoBPM * 4UL);
    stepInterval = stepIntervalMicros / 1000UL;

    pendingGateTime = stepInterval;

    for(int v=0; v<NUM_SEQUENCER_VOICES; v++)
    {
        for(int s=0; s<MAX_STEPS; s++)
        {
            if(sequences[v].steps[s].gateTime > stepInterval)
                sequences[v].steps[s].gateTime = stepInterval;
        }
    }
}
int Sequencer::getTempo()
{
    return tempoBPM;
}

void Sequencer::setLength(int length)
{
    length = constrain(length, 1, MAX_STEPS);

    sequenceLength = length;

    if(playStep >= sequenceLength)
        playStep = 0;

    if(editStep >= sequenceLength)
        editStep = 0;
}
int Sequencer::getLength()
{
    return sequenceLength;
}

SequencerMode Sequencer::getMode()
{
    return mode;
}
