#include "Controls.h"
#include "Pins.h"
#include "Voice.h"
#include "AudioSystem.h"
#include "FX.h"
#include "Keyboard.h"
#include "Sequencer.h"
#include "Display.h"

#include <Wire.h>

#include <Adafruit_MCP23X17.h>
Adafruit_MCP23X17 mcp;


static bool lastOctUp = false;
static bool lastOctDown = false;
static uint32_t lastOctaveTime = 0;
const uint32_t OCTAVE_DEBOUNCE_MS = 150;
const int KNOB_CHANGE_THRESHOLD = 3;
const int KNOB_SMOOTHING_SHIFT = 2;
const int MUX_READ_SAMPLES = 8;
const int STEPPED_CONTROL_HYSTERESIS = 8;

// ---------------- MUX ----------------

int lastKnobValues[16];
int smoothedKnobValues[16];

static int stableMappedValue(int rawValue,
                             int currentValue,
                             int outMin,
                             int outMax)
{
    int candidate = map(rawValue, 0, 1023, outMin, outMax);

    if(currentValue < 0 || candidate == currentValue)
        return candidate;

    long center =
        (long)(currentValue - outMin) * 1023L /
        (long)(outMax - outMin);

    if(abs(rawValue - center) >= STEPPED_CONTROL_HYSTERESIS)
        return candidate;

    return currentValue;
}

static String percentValue(int value)
{
    return String(map(value, 0, 1023, 0, 100)) + "%";
}

static String cutoffValue(int value)
{
    return String(map(value, 0, 1023, 40, 12000)) + " Hz";
}

static String resonanceValue(int value)
{
    int qTimes100 = map(value, 0, 1023, 70, 500);

    return String(qTimes100 / 100) + "." +
           String((qTimes100 / 10) % 10) +
           String(qTimes100 % 10) + " Q";
}

static String attackValue(int value)
{
    return String(map(value, 0, 1023, 0, 2000)) + " ms";
}

static String decayValue(int value)
{
    return String(map(value, 0, 1023, 0, 3000)) + " ms";
}

static String releaseValue(int value)
{
    return String(map(value, 0, 1023, 0, 4000)) + " ms";
}

static String detuneValue(int value)
{
    return String(map(value, 0, 1023, -500, 500) / 10.0f, 1) + "%";
}

void selectMuxChannel(uint8_t channel)
{
    digitalWrite(MUX_S0, channel & 0x01);
    digitalWrite(MUX_S1, (channel >> 1) & 0x01);
    digitalWrite(MUX_S2, (channel >> 2) & 0x01);
    digitalWrite(MUX_S3, (channel >> 3) & 0x01);

    delayMicroseconds(5);
}

int readMux(uint8_t channel)
{
    selectMuxChannel(channel);

    analogRead(MUX_SIG);

    int total = 0;

    for(int i = 0; i < MUX_READ_SAMPLES; i++)
        total += analogRead(MUX_SIG);

    return total / MUX_READ_SAMPLES;
}

void Controls::begin()
{
    // ---------- I2C ----------
    Wire2.begin();

    Serial.println("Initializing MCP...");
    bool found = false;
    for (int i = 0; i < 10; i++)
    {
        if (mcp.begin_I2C(MCP_ADDR, &Wire2))
        {
            found = true;
            break;
        }

        delay(10);
    }
    if (!found)
    {
        Serial.println("FAILED");
        while (1);
    }
    Serial.println("SUCCESS");


    // ---------- Multiplexer ----------

    pinMode(MUX_S0, OUTPUT);
    pinMode(MUX_S1, OUTPUT);
    pinMode(MUX_S2, OUTPUT);
    pinMode(MUX_S3, OUTPUT);

    pinMode(MUX_SIG, INPUT);

    // ---------- MCP Buttons ----------

    mcp.pinMode(BTN_FILTERMODE, INPUT_PULLUP);
    mcp.pinMode(BTN_SOURCE, INPUT_PULLUP);
    mcp.pinMode(BTN_NEXTVOICE, INPUT_PULLUP);
    mcp.pinMode(BTN_PREVVOICE, INPUT_PULLUP);
    mcp.pinMode(BTN_OCTAVE_UP, INPUT_PULLUP);
    mcp.pinMode(BTN_OCTAVE_DOWN, INPUT_PULLUP);
    mcp.pinMode(BTN_RECORD, INPUT_PULLUP);
    mcp.pinMode(BTN_PLAYSTOP, INPUT_PULLUP);
    mcp.pinMode(BTN_STEP, INPUT_PULLUP);
    mcp.pinMode(BTN_TOGGLEFX, INPUT_PULLUP);

    Serial.print("GPIOA = ");
    Serial.println(mcp.readGPIOA(), BIN);

    Serial.print("GPIOB = ");
    Serial.println(mcp.readGPIOB(), BIN);

    // ---------- Initialize knob history ----------

    for (int i = 0; i < 16; i++)
    {
        lastKnobValues[i] = -1;
        smoothedKnobValues[i] = -1;
    }

    Serial.println("Controls initialized.");
}


void Controls::update()
{
    const uint8_t channels[] =
    {
        DETUNE_CH,
        VOICEVOL_CH,
        SOURCEMOD_CH,
        RESONANCE_CH,
        CUTOFF_CH,
        ATTACK_CH,
        DECAY_CH,
        SUSTAIN_CH,
        RELEASE_CH,
        TEMPO_CH,
        NUMSTEPS_CH,
        FXMOD_CH,
        MIX_CH,
        MASTER_CH
    };

    for(size_t i = 0; i < sizeof(channels)/sizeof(channels[0]); i++)
    {
        int rawValue = readMux(channels[i]);
        uint8_t channel = channels[i];

        if(smoothedKnobValues[channel] < 0)
            smoothedKnobValues[channel] = rawValue;
        else
            smoothedKnobValues[channel] =
                ((smoothedKnobValues[channel] *
                  ((1 << KNOB_SMOOTHING_SHIFT) - 1)) +
                 rawValue) >>
                KNOB_SMOOTHING_SHIFT;

        int value = smoothedKnobValues[channel];

        if(abs(value-lastKnobValues[channel]) >= KNOB_CHANGE_THRESHOLD)
        {
            lastKnobValues[channel]=value;

            switch(channels[i])
            {
                case DETUNE_CH:
                    Voices.setDetune(value);
                    updateVoice(Voices.getCurrentVoice());

                    OLED.showPopup(
                        "Detune",
                        detuneValue(value)
                    );
                    break;

                case VOICEVOL_CH:
                    Voices.setVolume(value);
                    updateVoice(Voices.getCurrentVoice());

                    OLED.showPopup(
                        "Volume",
                        percentValue(value)
                    );
                    break;

                case SOURCEMOD_CH:
                    Voices.setSourceMod(value);
                    updateVoice(Voices.getCurrentVoice());

                    OLED.showPopup(
                        "Mod",
                        percentValue(value)
                    );
                    break;

                case CUTOFF_CH:
                    Voices.setCutoff(value);
                    updateVoice(Voices.getCurrentVoice());

                    OLED.showPopup(
                        "Cutoff",
                        cutoffValue(value)
                    );
                    break;

                case RESONANCE_CH:
                    Voices.setResonance(value);
                    updateVoice(Voices.getCurrentVoice());

                    OLED.showPopup(
                        "Resonance",
                        resonanceValue(value)
                    );
                    break;

                case ATTACK_CH:
                    Voices.setAttack(value);
                    updateVoice(Voices.getCurrentVoice());

                    OLED.showPopup(
                        "Attack",
                        attackValue(value)
                    );
                    break;

                case DECAY_CH:
                    Voices.setDecay(value);
                    updateVoice(Voices.getCurrentVoice());

                    OLED.showPopup(
                        "Decay",
                        decayValue(value)
                    );
                    break;

                case SUSTAIN_CH:
                    Voices.setSustain(value);
                    updateVoice(Voices.getCurrentVoice());

                    OLED.showPopup(
                        "Sustain",
                        percentValue(value)
                    );
                    break;

                case RELEASE_CH:
                    Voices.setRelease(value);
                    updateVoice(Voices.getCurrentVoice());

                    OLED.showPopup(
                        "Release",
                        releaseValue(value)
                    );
                    break;

                case TEMPO_CH:
                {
                    static int lastTempo = -1;

                    int bpm = stableMappedValue(value, lastTempo, 40, 240);

                    if(bpm != lastTempo)
                    {
                        lastTempo = bpm;
                        Sequencer::setTempo(bpm);
                        OLED.showPopup(
                            "Tempo",
                            String(bpm) + " BPM"
                        );
                    }

                    break;
                }

                case NUMSTEPS_CH:
                {
                    static int lastSteps = -1;

                    int steps = stableMappedValue(value, lastSteps, 1, 64);

                    if(steps != lastSteps)
                    {
                        lastSteps = steps;
                        Sequencer::setLength(steps);
                        OLED.showPopup(
                            "Steps",
                            String(steps)
                        );
                    }

                    break;
                }

                case FXMOD_CH:
                    setGlobalFxMod(value);
                    OLED.showPopup(
                        String(getGlobalFxShortName()) + " Mod",
                        getGlobalFxModDisplay()
                    );
                    break;

                case MIX_CH:
                    setGlobalFxMix(value);
                    OLED.showPopup(
                        String(getGlobalFxShortName()) + " Mix",
                        getGlobalFxMixDisplay()
                    );
                    break;

                case MASTER_CH:
                    setMasterVolume(value);
                    OLED.showPopup(
                        "Master",
                        getMasterVolumeDisplay()
                    );
                    break;
            }
        }
    }

    //---------------- Buttons ----------------

    static bool lastNext = HIGH;
    static bool lastPrev = HIGH;
    static bool lastSource = HIGH;
    static bool lastFilter = HIGH;
    static bool lastPlayStop = HIGH;
    static bool lastRecord = HIGH;
    static bool lastStep = HIGH;
    static bool lastFx = HIGH;

    bool next = mcp.digitalRead(BTN_NEXTVOICE);
    bool prev = mcp.digitalRead(BTN_PREVVOICE);
    bool source = mcp.digitalRead(BTN_SOURCE);
    bool filter = mcp.digitalRead(BTN_FILTERMODE);
    bool playStop = mcp.digitalRead(BTN_PLAYSTOP);
    bool record   = mcp.digitalRead(BTN_RECORD);
    bool step     = mcp.digitalRead(BTN_STEP);
    bool fx       = mcp.digitalRead(BTN_TOGGLEFX);

    if (next != lastNext)
    {
        if (!next)
        {
            Voices.nextVoice();
            updateVoice(Voices.getCurrentVoice());

            OLED.showPopup(
                "Voice",
                String(Voices.getCurrentVoice() + 1)
            );

            Serial.print("Voice ");
            Serial.println(Voices.getCurrentVoice()+1);
        }
        lastNext = next;
    }

    if (prev != lastPrev)
    {
        if (!prev)
        {
            Voices.previousVoice();
            updateVoice(Voices.getCurrentVoice());

            OLED.showPopup(
                "Voice",
                String(Voices.getCurrentVoice() + 1)
            );

            Serial.print("Voice ");
            Serial.println(Voices.getCurrentVoice()+1);
        }
        lastPrev = prev;
    }

    if (source != lastSource)
    {
        if(!source)
        {
            Voices.nextWaveform();
            updateVoice(Voices.getCurrentVoice());

            switch(Voices.current().waveform)
            {
                case SINE:
                    OLED.showPopup("Wave","Sine");
                    break;

                case SAW:
                    OLED.showPopup("Wave","Saw");
                    break;

                case SQUARE:
                    OLED.showPopup("Wave","Square");
                    break;

                case TRIANGLE:
                    OLED.showPopup("Wave","Triangle");
                    break;

                case NOISE:
                    OLED.showPopup("Wave","Noise");
                    break;
            }
            
        }
        lastSource = source;
    }

    if (filter != lastFilter)
    {
        if(!filter)
        {
            Voices.nextFilter();
            updateVoice(Voices.getCurrentVoice());

            switch(Voices.current().filter)
            {
                case LOWPASS:
                    OLED.showPopup("Filter","LP");
                    break;

                case HIGHPASS:
                    OLED.showPopup("Filter","HP");
                    break;

                case BANDPASS:
                    OLED.showPopup("Filter","BP");
                    break;
            }
        }
        lastFilter = filter;
    }


    bool up = !mcp.digitalRead(BTN_OCTAVE_UP);
    bool down = !mcp.digitalRead(BTN_OCTAVE_DOWN);

    uint32_t now = millis();
    if(up && !lastOctUp && now - lastOctaveTime > OCTAVE_DEBOUNCE_MS)
    {
        Keyboard::octaveUp();
        OLED.showPopup(
            "Octave",
            "+" + String(Keyboard::getOctave())
        );
        lastOctaveTime = now;
    }

    if(down && !lastOctDown && now - lastOctaveTime > OCTAVE_DEBOUNCE_MS)
    {
        Keyboard::octaveDown();
        OLED.showPopup(
            "Octave",
            "+" + String(Keyboard::getOctave())
        );
        lastOctaveTime = now;
    }

    lastOctUp = up;
    lastOctDown = down;



    if(playStop != lastPlayStop)
    {
        if(!playStop)
        {
            Sequencer::togglePlay();
        }

        lastPlayStop = playStop;
    }

    if(record != lastRecord)
    {
        if(!record)
        {
            Sequencer::toggleRecord();
        }

        lastRecord = record;
    }

    if(step != lastStep)
    {
        if(!step)
        {
            Sequencer::toggleStepMode();
        }

        lastStep = step;
    }

    if(fx != lastFx)
    {
        if(!fx)
        {
            nextGlobalFx();
            OLED.showPopup("FX", getGlobalFxName());
        }

        lastFx = fx;
    }
}
