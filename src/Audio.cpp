#ifndef ESPWIFI_AUDIO_H
#define ESPWIFI_AUDIO_H

#include "ESPWiFi.h"

// Audio
AudioFileSourceLittleFS *fileLFS;
AudioGeneratorMP3 *mp3;
AudioOutputI2S *dac;

unsigned long lastAudioCheck = 0;
bool receivingAudio = false;
bool responded = true;

// Pins
IOPin ptt = IOPin(12);
IOPin spk = IOPin(A0, INPUT);

void ESPWiFi::startAudio() {
  dac = new AudioOutputI2S();
  dac->SetPinout(BCLK, LRC, DOUT);
  mp3 = new AudioGeneratorMP3();
}

void ESPWiFi::playMP3(String filename) {
  fileLFS = new AudioFileSourceLittleFS(filename.c_str());

  if (pttEnabled) {
    ptt.on();
    delay(600);
  }

  String fileExtention = getFileExtension(filename);
  if (fileExtention == "mp3") {
    Serial.println("Playing MP3 file: " + String(filename));
    if (!mp3->begin(fileLFS, dac)) {
      Serial.println("Failed to begin MP3 playback");
      return;
    }
  } else {
    Serial.println("Unsupported file format: " + fileExtention);
    return;
  }
}

void ESPWiFi::handleAudio(std::function<void()> respond) {
  if (mp3->isRunning()) {
    if (!mp3->loop()) {
      mp3->stop();
      delete fileLFS;
    }
  } else if (!receivingAudio) {
    if (pttEnabled) {
      ptt.off();
    }
  }
  runAtInterval(100, lastAudioCheck, [&]() {
    int audioLevel = spk.readA();
    if (audioLevel > 1000) {
      Serial.println("RX Audio: " + String(audioLevel));
      if (!receivingAudio) {
        receivingAudio = true;
        responded = false;
        // ptt.on();  // Enable push-to-talk
      }
    } else {
      if (receivingAudio) {
        receivingAudio = false;
      }
    }

    if (!receivingAudio && !responded) {
      respond();
      responded = true;
    }
  });
}

#endif  // ESPWIFI_AUDIO_H