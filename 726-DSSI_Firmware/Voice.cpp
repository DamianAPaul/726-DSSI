#include "Voice.h"
#include "AudioSystem.h"

VoiceManager Voices;

void VoiceManager::begin()
{
    currentVoice = 0;

    for(int i=0;i<8;i++)
    {
        voices[i].waveform = SAW;

        voices[i].filter = LOWPASS;

        voices[i].detune = 0;

        voices[i].volume = 1023;

        voices[i].sourceMod = 0;

        voices[i].cutoff = 700;

        voices[i].resonance = 150;

        voices[i].attack = 5;

        voices[i].decay = 200;

        voices[i].sustain = 700;

        voices[i].release = 200;
    }
}

bool VoiceManager::needsDisplayUpdate() const
{
    return displayDirty;
}

void VoiceManager::clearDisplayDirty()
{
    displayDirty = false;
}


void VoiceManager::nextVoice()
{
    currentVoice++;

    if(currentVoice > 7)
        currentVoice = 0;

    displayDirty = true;
}

void VoiceManager::previousVoice()
{
    currentVoice--;

    if(currentVoice < 0)
        currentVoice = 7;
    
    displayDirty = true;
}

int VoiceManager::getCurrentVoice() const
{
    return currentVoice;
}

VoiceState& VoiceManager::current()
{
    return voices[currentVoice];
}
VoiceState& VoiceManager::getVoice(int index)
{
    return voices[index];
}



void VoiceManager::setDetune(int value)
{
    voices[currentVoice].detune = value;
    updateVoice(currentVoice);
    displayDirty = true;
}

void VoiceManager::setVolume(int value)
{
    voices[currentVoice].volume = value;
    updateVoice(currentVoice);
    displayDirty = true;
}

void VoiceManager::setSourceMod(int value)
{
    voices[currentVoice].sourceMod = value;
    updateVoice(currentVoice);
    displayDirty = true;
}

void VoiceManager::setCutoff(int value)
{
    voices[currentVoice].cutoff = value;
    updateVoice(currentVoice);
    displayDirty = true;
}

void VoiceManager::setResonance(int value)
{
    voices[currentVoice].resonance = value;
    updateVoice(currentVoice);
    displayDirty = true;
}

void VoiceManager::setAttack(int value)
{
    voices[currentVoice].attack = value;
    updateVoice(currentVoice);
    displayDirty = true;
}

void VoiceManager::setDecay(int value)
{
    voices[currentVoice].decay = value;
    updateVoice(currentVoice);
    displayDirty = true;
}

void VoiceManager::setSustain(int value)
{
    voices[currentVoice].sustain = value;
    updateVoice(currentVoice);
    displayDirty = true;
}

void VoiceManager::setRelease(int value)
{
    voices[currentVoice].release = value;
    updateVoice(currentVoice);
    displayDirty = true;
}



void VoiceManager::nextWaveform()
{
    auto &v = voices[currentVoice];

    v.waveform = WaveformType((v.waveform + 1) % 5);
    updateVoice(currentVoice);

    displayDirty = true;
}

void VoiceManager::nextFilter()
{
    auto &v = voices[currentVoice];

    v.filter = FilterType((v.filter + 1) % 3);
    updateVoice(currentVoice);

    displayDirty = true;
}


