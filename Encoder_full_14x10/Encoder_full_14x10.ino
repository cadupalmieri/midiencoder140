//Multiplexa por meio de uma matrix 10x14
//Envia por USB midi e midi hardware(desativado para testes)

// Compilar para Arduino Micro

//@cadupalmieri 
//V1.0 - 02/11/2021
//V1.1 - 09/11/2021


//#include <MIDI.h>
#include "MIDIUSB.h"
//#include <SoftwareSerial.h>


#define n_linha 10                     // Quantidade de n_linha.
#define n_coluna 14                     // Quantidade de n_coluna.
#define DEBOUNCE_LENGTH 5

#define canal 6

//SoftwareSerial softSerial(A5, A4);
//MIDI_CREATE_INSTANCE(SoftwareSerial, softSerial, MIDI);

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
 // MIDI.begin(canal);
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
    noteOff(canal - 1, n + 36, 0);
   // MIDI.sendNoteOff(n + 36, 0, canal);
  }

  encoder_init();

}
void loop() {
  encoder();
  //escala();
}

void escala() {
  for (int n = 0; n < 61; n++) {
   // MIDI.sendNoteOn(n + 36, 127, canal);
    delay(DEBOUNCE_LENGTH * 50);
   // MIDI.sendNoteOff(n + 36, 0, canal);
    delay(DEBOUNCE_LENGTH * 50);
  }
}

void encoder() {
  for (int n = 0; n < n_linha; n++) {
    demux(n);

    for (int i = 0; i < n_coluna; i++) {
      notas_temp = !digitalRead(pino_coluna[i]);
      if (notas[i + n * 14] != notas_temp) {
        delay(DEBOUNCE_LENGTH);
        if (notas_temp == !digitalRead(pino_coluna[i])) {
          notas[i + n * 14] = notas_temp;

          for (int i = 0; i < n_linha *  n_coluna; i++) {
            Serial.print(notas[i]);
            if (i % 14 == 13) {
              Serial.print(" ");
            }
            if (i == 61) {
              //Serial.print(" - ");
            }

            if (i == 122) {
              // Serial.print(" - ");
            }
          }
          Serial.println();


          if (notas[i + n * 14] == 1) {
           // MIDI.sendNoteOn(i + n * 14 + 36, 127, canal);
            noteOn(canal - 1, i + n * 14 + 36, 127);
          }
          else {
          //  MIDI.sendNoteOff(i + n * 14 + 36, 0, canal);
            noteOff(canal - 1, i + n * 14 + 36, 0);
          }
        }
      }
    }
  }
}

void encoder_init() {
  for (int n = 0; n < n_linha; n++) {
    demux(n_linha);
    for (int i = 0; i < n_coluna; i++) {
      notas_temp = !digitalRead(pino_coluna[i]);
      if (notas[i + n * 14] != notas_temp) {
        delay(DEBOUNCE_LENGTH);
        notas[i + n * 14] = notas_temp;

        if (notas[i + n * 14] == 1) {
         // MIDI.sendNoteOn(i + n * 14 + 36, 127, canal);
          noteOn(canal - 1, i + n * 14 + 36, 127);
        }
        else {
         // MIDI.sendNoteOff(i + n * 14 + 36, 0, canal);
          noteOff(canal - 1, i + n * 14 + 36, 0);
        }
      }
    }
  }
}
