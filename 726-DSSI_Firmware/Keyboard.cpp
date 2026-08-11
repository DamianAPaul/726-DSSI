#include "Keyboard.h"
#include "Pins.h"
#include "AudioSystem.h"
#include "Voice.h"


int octaveOffset = 0;


const uint8_t rowPins[3] =
{
    KEY_ROW0,
    KEY_ROW1,
    KEY_ROW2
};

const uint8_t colPins[4] =
{
    KEY_COL0,
    KEY_COL1,
    KEY_COL2,
    KEY_COL3
};

// MIDI notes, instead of frequencies
const uint8_t keyNotes[3][4] =
{
    {60,61,62,63},
    {64,65,66,67},
    {68,69,70,71}
};


bool keyState[3][4] = { false };

int activeRow = -1;
int activeCol = -1;


bool notePressedFlag = false;
bool noteReleasedFlag = false;
uint8_t lastMidiNote = 60;



void Keyboard::begin()
{
    for(int r=0;r<3;r++)
    {
        pinMode(rowPins[r],OUTPUT);

        digitalWrite(rowPins[r],HIGH);
    }

    for(int c=0;c<4;c++)
    {
        pinMode(
            colPins[c],
            INPUT_PULLUP
        );
    }
}

void Keyboard::update()
{
    for(int r=0;r<3;r++)
    {
        for(int i=0;i<3;i++)
            digitalWrite(rowPins[i],HIGH);

        digitalWrite(rowPins[r],LOW);

        delayMicroseconds(5);

        for(int c=0;c<4;c++)
        {
            bool pressed =
                digitalRead(colPins[c]) == LOW;

            if(
                pressed &&
                !keyState[r][c]
            )
            {
                keyState[r][c] = true;

                uint8_t note =
                    keyNotes[r][c]
                    +
                    octaveOffset*12;

                activeRow = r;
                activeCol = c;

                lastMidiNote = note;
                notePressedFlag = true;
            }

            if(
                !pressed &&
                keyState[r][c]
            )
            {
                keyState[r][c] = false;

                if(activeRow == r &&
                    activeCol == c)
                    {
                        activeRow = -1;
                        activeCol = -1;

                        noteReleasedFlag = true;
                    }
            }
        }
    }
}


void Keyboard::octaveUp()
{
    if(octaveOffset < 3)
        octaveOffset++;
    Serial.print("Octave = ");
    Serial.println(octaveOffset);
}

void Keyboard::octaveDown()
{
    if(octaveOffset > -3)
        octaveOffset--;
    Serial.print("Octave = ");
    Serial.println(octaveOffset);
}


bool Keyboard::notePressed()
{
    bool result = notePressedFlag;
    notePressedFlag = false;
    return result;
}

bool Keyboard::noteReleased()
{
    bool result = noteReleasedFlag;
    noteReleasedFlag = false;
    return result;
}

uint8_t Keyboard::getPressedMidiNote()
{
    return lastMidiNote;
}

int Keyboard::getOctave()
{
    return octaveOffset;
}