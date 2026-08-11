#pragma once

#include <Arduino.h>

// ---------- I2C ----------
constexpr uint8_t I2C_SCL = 24;
constexpr uint8_t I2C_SDA = 25;

// ---------- Perform ----------
// Rows (outputs)
constexpr uint8_t KEY_ROW0 = 28;
constexpr uint8_t KEY_ROW1 = 33;
constexpr uint8_t KEY_ROW2 = 34;

// Columns (inputs)
constexpr uint8_t KEY_COL0 = 36;
constexpr uint8_t KEY_COL1 = 39;
constexpr uint8_t KEY_COL2 = 40;
constexpr uint8_t KEY_COL3 = 41;



// ---------- Multiplexer ----------

constexpr uint8_t MUX_S0 = 29;
constexpr uint8_t MUX_S1 = 30;
constexpr uint8_t MUX_S2 = 31;
constexpr uint8_t MUX_S3 = 32;

constexpr uint8_t MUX_SIG = A14;

// Multiplexer channels

constexpr uint8_t DETUNE_CH       = 15;
constexpr uint8_t VOICEVOL_CH     = 14;
constexpr uint8_t SOURCEMOD_CH    = 13;
constexpr uint8_t RESONANCE_CH    = 12;
constexpr uint8_t CUTOFF_CH       = 11;
constexpr uint8_t ATTACK_CH       = 10;
constexpr uint8_t DECAY_CH        = 9;
constexpr uint8_t SUSTAIN_CH      = 8;
constexpr uint8_t RELEASE_CH      = 7;
constexpr uint8_t NUMSTEPS_CH     = 6;
constexpr uint8_t TEMPO_CH        = 5;
constexpr uint8_t SWING_CH        = 4;
constexpr uint8_t FXMOD_CH        = 3;
constexpr uint8_t MIX_CH          = 2;
constexpr uint8_t MASTER_CH       = 1;

// ---------- MCP ----------

constexpr uint8_t MCP_ADDR = 0x20;

// Port A
constexpr uint8_t BTN_RECORD       = 0; // PA0
constexpr uint8_t BTN_PLAYSTOP     = 1; // PA1
constexpr uint8_t BTN_STEP         = 2; // PA2

// Port B
constexpr uint8_t BTN_TOGGLEFX    = 9;  // PB1
constexpr uint8_t BTN_OCTAVE_DOWN  = 10; // PB2;
constexpr uint8_t BTN_OCTAVE_UP    = 11; // PB3;
constexpr uint8_t BTN_FILTERMODE   = 12; // PB4
constexpr uint8_t BTN_SOURCE       = 13; // PB5
constexpr uint8_t BTN_NEXTVOICE    = 14; // PB6
constexpr uint8_t BTN_PREVVOICE    = 15; // PB7
