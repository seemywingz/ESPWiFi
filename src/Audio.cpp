#ifndef ESPWIFI_AUDIO_H
#define ESPWIFI_AUDIO_H

#include "ESPWiFi.h"

// Audio
AudioFileSourceLittleFS *fileLFS;
AudioGeneratorMP3 *mp3;
static AudioOutputI2S *dac;

unsigned long lastAudioCheck = 0;
bool receivingAudio = false;
bool responded = true;

// Pins
IOPin ptt = IOPin(12);
IOPin spk = IOPin(A0, INPUT);

void ESPWiFi::startAudio() {
  dac = new AudioOutputI2S();
  dac->SetPinout(BCLK, LRC, DOUT);
  // mp3 = new AudioGeneratorMP3();
}

void ESPWiFi::playMP3(String filename) {
  // Reinitialize the MP3 decoder before every use
  if (mp3 != nullptr) {
    delete mp3;
    mp3 = nullptr;
  }
  mp3 = new AudioGeneratorMP3();
  if (!mp3) {
    Serial.println("Failed to create MP3 decoder");
    return;
  }

  if (fileLFS != nullptr) {
    delete fileLFS;
    fileLFS = nullptr;
  }
  fileLFS = new AudioFileSourceLittleFS(filename.c_str());
  if (!fileLFS || !fileLFS->isOpen()) {
    Serial.println("Failed to open file: " + filename);
    return;
  }

  if (pttEnabled) {
    ptt.on();
    delay(600);  // Consider making this non-blocking
  }

  String fileExtension = getFileExtension(filename);
  if (fileExtension == "mp3") {
    Serial.println("Playing MP3 file: " + filename);
    if (!mp3->begin(fileLFS, dac)) {
      Serial.println("Failed to begin MP3 playback");
      return;
    }
  } else {
    Serial.println("Unsupported file format: " + fileExtension);
    return;
  }
}

void ESPWiFi::handleAudio() {
  if (mp3->isRunning()) {
    if (!mp3->loop()) {
      mp3->stop();
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
      audioResponse();
      responded = true;
    }
  });
}

#endif  // ESPWIFI_AUDIO_H