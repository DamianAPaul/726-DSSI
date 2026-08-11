#pragma once

#include <stdint.h>
#include <Arduino.h>

void initAudio();

void updateVoice(int voice);

void playVoice(int voice, float frequency);

void stopVoice(int voice);

float midiToFreq(uint8_t note);

void setMasterVolume(int value);
String getMasterVolumeDisplay();
