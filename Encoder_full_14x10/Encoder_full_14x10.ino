//Multiplexa por meio de uma matrix 10x14
//Envia por USB midi e midi hardware(desativado para testes)

// Compilar para Arduino Micro

//@cadupalmieri
//V1.0 - 02/11/2021
//V1.1 - 09/11/2021


#include <MIDI.h>
#include "MIDIUSB.h"
#include <SoftwareSerial.h>


#define n_linha 10                     // Quantidade de n_linha.
#define n_coluna 14                     // Quantidade de n_coluna.
#define DEBOUNCE_LENGTH 5


SoftwareSerial softSerial(A5, A4);
MIDI_CREATE_INSTANCE(SoftwareSerial, softSerial, MIDI);

#define demux_A0 13
#define demux_A1 5
#define demux_A2 10
#define demux_EN 22
const uint8_t pino_coluna[n_coluna] = {19, 18, 21, 20, 8, 9, 2, 6, 11, 3, 16, 14, 7, 15}; // Pinos que estão ligados as n_coluna.

int notas[n_linha * n_coluna];
bool notas_temp;

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

void demux(int linha) {
  if (linha == 0) {
    digitalWrite(demux_EN, HIGH);
    digitalWrite(demux_A0, LOW);
    digitalWrite(demux_A1, LOW);
    digitalWrite(demux_A2, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(4,  HIGH);
  }

  else if (linha == 1) {
    digitalWrite(demux_EN, HIGH);
    digitalWrite(demux_A0, HIGH);
    digitalWrite(demux_A1, LOW);
    digitalWrite(demux_A2, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(4,  HIGH);
  }

  else if (linha == 2) {
    digitalWrite(demux_EN, HIGH);
    digitalWrite(demux_A0, LOW);
    digitalWrite(demux_A1, HIGH);
    digitalWrite(demux_A2, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(4,  HIGH);
  }

  else if (linha == 3) {
    digitalWrite(demux_EN, HIGH);
    digitalWrite(demux_A0, HIGH);
    digitalWrite(demux_A1, HIGH);
    digitalWrite(demux_A2, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(4,  HIGH);
  }

  else if (linha == 4) {
    digitalWrite(demux_EN, HIGH);
    digitalWrite(demux_A0, LOW);
    digitalWrite(demux_A1, LOW);
    digitalWrite(demux_A2, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(4,  HIGH);
  }

  else if (linha == 5) {
    digitalWrite(demux_EN, HIGH);
    digitalWrite(demux_A0, HIGH);
    digitalWrite(demux_A1, LOW);
    digitalWrite(demux_A2, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(4,  HIGH);
  }

  else if (linha == 6) {
    digitalWrite(demux_EN, HIGH);
    digitalWrite(demux_A0, LOW);
    digitalWrite(demux_A1, HIGH);
    digitalWrite(demux_A2, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(4,  HIGH);
  }

  else if (linha == 7) {
    digitalWrite(demux_EN, HIGH);
    digitalWrite(demux_A0, HIGH);
    digitalWrite(demux_A1, HIGH);
    digitalWrite(demux_A2, HIGH);
    digitalWrite(12, HIGH);
    digitalWrite(4,  HIGH);
  }

  else if (linha == 8) {
    digitalWrite(demux_EN, LOW);
    digitalWrite(demux_A0, LOW);
    digitalWrite(demux_A1, LOW);
    digitalWrite(demux_A2, LOW);
    digitalWrite(12, LOW);
    digitalWrite(4,  HIGH);
  }

  else if (linha == 9) {
    digitalWrite(demux_EN, LOW);
    digitalWrite(demux_A0, LOW);
    digitalWrite(demux_A1, LOW);
    digitalWrite(demux_A2, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(4,  LOW);
  }

  else {
    digitalWrite(demux_EN, LOW);
    digitalWrite(demux_A0, LOW);
    digitalWrite(demux_A1, LOW);
    digitalWrite(demux_A2, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(4,  HIGH);
  }



}
void setup() {
  delay(3000);
  MIDI.begin(0);

  for (int i = 0; i < n_coluna; i++) {
    pinMode(pino_coluna[i], INPUT_PULLUP);
  }

  pinMode(demux_A0, OUTPUT);
  pinMode(demux_A1, OUTPUT);
  pinMode(demux_A2, OUTPUT);
  pinMode(demux_EN, OUTPUT);

  pinMode(12, OUTPUT);
  pinMode(4, OUTPUT);

  digitalWrite(demux_EN, HIGH);
  digitalWrite(demux_A0, LOW);
  digitalWrite(demux_A1, LOW);
  digitalWrite(demux_A2, LOW);

  digitalWrite(12, HIGH);
  digitalWrite(4, HIGH);

  for (int n = 0; n < 61; n++) {
    noteOff(0, n + 36, 0);
    noteOff(1, n + 36, 0);
    noteOff(2, n + 36, 0);
    noteOff(3, n + 36, 0);
    MIDI.sendNoteOff(n + 36, 0, 0);
    MIDI.sendNoteOff(n + 36, 0, 1);
    MIDI.sendNoteOff(n + 36, 0, 2);
    MIDI.sendNoteOff(n + 36, 0, 3);
  }


}
void loop() {
  encoder();
}


void encoder() {
  for (int n = 0; n < n_linha; n++) {
    demux(n);

    for (int i = 0; i < n_coluna; i++) {
      notas_temp = !digitalRead(pino_coluna[i]);

      byte nota = i + n * 14;
      if (notas[nota] != notas_temp) {
        delay(DEBOUNCE_LENGTH);
        if (notas_temp == !digitalRead(pino_coluna[i])) {
          notas[nota] = notas_temp;



          if (notas[nota] == 1) {
            if (nota >= 0 and nota <= 12) {
              MIDI.sendNoteOn(nota + 24, 127, 0);
              noteOn(0, nota + 24 , 127);
            }
            else if (nota >= 14 and nota <= 57) {
              MIDI.sendNoteOn(nota  + 27, 127, 1);
              noteOn(1, nota + 27 , 127);
            }
            else  if (nota >= 70 and nota <= 113) {
              MIDI.sendNoteOn(nota - 29, 127, 2);
              noteOn(2, nota - 29 , 127);
            }
            else if (nota == 13 or (nota >= 58 and nota <= 69) or nota >= 114) {
              MIDI.sendNoteOn(nota - 58, 127, 3);
              noteOn(3, nota - 58 , 127);
            }
            else {
            }
          }

          else {

            if (nota >= 0 and nota <= 12) {
              MIDI.sendNoteOff(nota + 24, 0, 0);
              noteOff(0, nota + 24 , 0);
            }
            else if (nota >= 14 and nota <= 57) {
              MIDI.sendNoteOff(nota  + 27, 0, 1);
              noteOff(1, nota + 27 , 0);
            }
            else  if (nota >= 70 and nota <= 113) {
              MIDI.sendNoteOff(nota - 29, 0, 2);
              noteOff(2, nota - 29 , 0);
            }
            else if (nota == 13 or (nota >= 58 and nota <= 69) or nota >= 114) {
              MIDI.sendNoteOff(nota - 58, 0,3);
              noteOff(3, nota - 58 , 0);
            }
            else {
            }
          }
        }
      }
    }
  }
}
