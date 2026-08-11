#include "AudioSystem.h"
#include "AudioObjects.h"
#include "FX.h"
#include "Voice.h"


AudioSynthWaveform* waveforms[8] =
{
    &waveform1,
    &waveform2,
    &waveform3,
    &waveform4,
    &waveform5,
    &waveform6,
    &waveform7,
    &waveform8
};

AudioFilterStateVariable* lpFilters[8] =
{
    &filter1,
    &filter4,
    &filter7,
    &filter10,
    &filter13,
    &filter16,
    &filter19,
    &filter22
};
AudioFilterStateVariable* bpFilters[8] =
{
    &filter2,
    &filter5,
    &filter8,
    &filter11,
    &filter14,
    &filter17,
    &filter20,
    &filter23
};
AudioFilterStateVariable* hpFilters[8] =
{
    &filter3,
    &filter6,
    &filter9,
    &filter12,
    &filter15,
    &filter18,
    &filter21,
    &filter24
};
AudioMixer4* filterMixers[8] =
{
    &mixer4,
    &mixer5,
    &mixer6,
    &mixer7,
    &mixer8,
    &mixer9,
    &mixer10,
    &mixer11
};

AudioEffectEnvelope* envelopes[8] =
{
    &envelope1,
    &envelope2,
    &envelope3,
    &envelope4,
    &envelope5,
    &envelope6,
    &envelope7,
    &envelope8
};

float currentFrequency[8] =
{
    220,220,220,220,
    220,220,220,220
};

int masterVolume = 1023;

static float noiseRate(const VoiceState &v)
{
    return map(v.sourceMod, 0, 1023, 500, 20000);
}

void initAudio()
{
    AudioMemory(180);


    mixer1.gain(0, 0.25);
    mixer1.gain(1, 0.25);
    mixer1.gain(2, 0.25);
    mixer1.gain(3, 0.25);

    mixer2.gain(0, 0.25);
    mixer2.gain(1, 0.25);
    mixer2.gain(2, 0.25);
    mixer2.gain(3, 0.25);

    mixer3.gain(0, 0.0);
    mixer3.gain(1, 0.5);
    mixer3.gain(2, 0.5);
    mixer3.gain(3, 0.0);

    initGlobalFx();


    audioShield.enable();

    audioShield.volume(0.5);

    setMasterVolume(masterVolume);

    // ---------------- Voices defaults ----------------

    for(int i=0;i<8;i++)
    {
        waveforms[i]->begin(
            0.5,
            currentFrequency[i],
            WAVEFORM_SAWTOOTH
        );

        waveforms[i]->amplitude(0.5);

        // Default filter = LP

        filterMixers[i]->gain(0,1);
        filterMixers[i]->gain(1,0);
        filterMixers[i]->gain(2,0);
        filterMixers[i]->gain(3,0);

        // Initialize all three filters

        lpFilters[i]->frequency(1000);
        hpFilters[i]->frequency(1000);
        bpFilters[i]->frequency(1000);

        lpFilters[i]->resonance(1.0);
        hpFilters[i]->resonance(1.0);
        bpFilters[i]->resonance(1.0);

        envelopes[i]->attack(5);
        envelopes[i]->decay(200);
        envelopes[i]->sustain(0.7);
        envelopes[i]->release(300);
    }

    for (int i = 0; i < 8; i++)
        updateVoice(i);
}


void playVoice(int voice, float frequency)
{
    currentFrequency[voice] = frequency;
    VoiceState &v = Voices.getVoice(voice);
    float detuneFactor =
        1.0f + map(v.detune, 0, 1023, -500, 500) / 10000.0f;

    if(v.waveform == NOISE)
        waveforms[voice]->frequency(noiseRate(v));
    else
        waveforms[voice]->frequency(
            currentFrequency[voice] * detuneFactor
        );

    envelopes[voice]->noteOn();
}

void stopVoice(int voice)
{
    envelopes[voice]->noteOff();
}


void updateVoice(int voice)
{
    VoiceState &v = Voices.getVoice(voice);

    float amp = v.volume / 1023.0f;

    float detuneFactor =
        1.0f + map(v.detune,0,1023,-500,500)/10000.0f;

    float freq = currentFrequency[voice] * detuneFactor;

    switch(v.waveform)
    {
    case SINE:
        waveforms[voice]->begin(amp, freq, WAVEFORM_SINE);
        break;

    case SAW:
        waveforms[voice]->begin(amp, freq, WAVEFORM_SAWTOOTH);
        break;

    case SQUARE:
        waveforms[voice]->begin(amp, freq, WAVEFORM_SQUARE);
        break;

    case TRIANGLE:
        waveforms[voice]->begin(amp, freq, WAVEFORM_TRIANGLE);
        break;

    case NOISE:
        waveforms[voice]->begin(amp, noiseRate(v), WAVEFORM_SAMPLE_HOLD);
        break;
    }

    
    // ---------------- Frequency ----------------

    float cutoff =
        map(
            v.cutoff,
            0,
            1023,
            40,
            12000
        );

    float resonance =
        map(
            v.resonance,
            0,
            1023,
            70,
            500
        ) / 100.0f;

    // Send cutoff/resonance to all filters

    lpFilters[voice]->frequency(cutoff);
    hpFilters[voice]->frequency(cutoff);
    bpFilters[voice]->frequency(cutoff);

    lpFilters[voice]->resonance(resonance);
    hpFilters[voice]->resonance(resonance);
    bpFilters[voice]->resonance(resonance);

    // ---------------- Filter Selection ----------------

    switch(v.filter)
    {
    case LOWPASS:

        filterMixers[voice]->gain(0,1);
        filterMixers[voice]->gain(1,0);
        filterMixers[voice]->gain(2,0);

        break;

    case HIGHPASS:

        filterMixers[voice]->gain(0,0);
        filterMixers[voice]->gain(1,0);
        filterMixers[voice]->gain(2,1);

        break;

    case BANDPASS:

        filterMixers[voice]->gain(0,0);
        filterMixers[voice]->gain(1,1);
        filterMixers[voice]->gain(2,0);

        break;
    }

    // ---------------- Envelope ----------------

    envelopes[voice]->attack(
        map(v.attack,0,1023,0,2000)
    );

    envelopes[voice]->decay(
        map(v.decay,0,1023,0,3000)
    );

    envelopes[voice]->sustain(
        v.sustain / 1023.0f
    );

    envelopes[voice]->release(
        map(v.release,0,1023,0,4000)
    );


}

float midiToFreq(uint8_t note)
{
    return 440.0f *
           powf(
               2.0f,
               (note - 69) / 12.0f
           );
}

void setMasterVolume(int value)
{
    masterVolume = constrain(value, 0, 1023);
    amp1.gain(masterVolume / 1023.0f);
}

String getMasterVolumeDisplay()
{
    return String(map(masterVolume, 0, 1023, 0, 100)) + "%";
}
