#pragma once

#include <stdint.h>

void setDetune(int value);
void setVolume(int value);
void setSourceMod(int value);

void setCutoff(int value);
void setResonance(int value);

void setAttack(int value);
void setDecay(int value);
void setSustain(int value);
void setRelease(int value);

void nextWaveform();
void nextFilter();

enum WaveformType
{
    SINE,
    SAW,
    SQUARE,
    TRIANGLE,
    NOISE
};

enum FilterType
{
    LOWPASS,
    HIGHPASS,
    BANDPASS
};

struct VoiceState
{
    WaveformType waveform;
    FilterType filter;

    int detune;
    int volume;

    int sourceMod;

    int cutoff;
    int resonance;

    int attack;
    int decay;
    int sustain;
    int release;
};

class VoiceManager
{
public:

    void begin();

    void nextVoice();
    void previousVoice();

    int getCurrentVoice() const;

    VoiceState& current();
    VoiceState& getVoice(int index);

    void setDetune(int value);
    void setVolume(int value);
    void setSourceMod(int value);

    void setCutoff(int value);
    void setResonance(int value);

    void setAttack(int value);
    void setDecay(int value);
    void setSustain(int value);
    void setRelease(int value);

    void nextWaveform();
    void nextFilter();

    bool needsDisplayUpdate() const;
    void clearDisplayDirty();

private:

    VoiceState voices[8];

    int currentVoice = 0;

    bool displayDirty = true;
};

extern VoiceManager Voices;