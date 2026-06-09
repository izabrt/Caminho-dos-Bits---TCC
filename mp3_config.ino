#include "mp3_config.h"

HardwareSerial mp3Serial(2);
DFRobotDFPlayerMini player;

bool dfplayer_is_playing() {
    return player.readState() == 1;
}

void dfplayer_init() {
    mp3Serial.begin(9600, SERIAL_8N1, 35, 0);  // RX=35(←TX do DFPlayer), TX=0(←RX do DFPlayer)

    if (!player.begin(mp3Serial)) {
        while (true) delay(500);  // trava piscando — RGB vermelho já está ativo
    }

    player.volume(7);
    delay(1000);
}

void dfplayer_play(uint8_t track) {
    player.play(track);
}

void dfplayer_stop() {
    player.stop();
}

void dfplayer_setVolume(uint8_t vol) {
    player.volume(vol);
}

void dfplayer_play_advert(uint8_t track) {
    player.advertise(track);
}