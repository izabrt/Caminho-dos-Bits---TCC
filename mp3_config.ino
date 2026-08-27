#include "mp3_config.h"

HardwareSerial mp3Serial(2);
DFRobotDFPlayerMini player;

// bool dfplayer_is_playing() {
//     return player.readState() == 1;
// }

bool dfplayer_ready = false;

void dfplayer_init() {
    mp3Serial.begin(9600, SERIAL_8N1, 35, 0);  // RX=35(←TX do DFPlayer), TX=0(←RX do DFPlayer)

    dfplayer_ready = player.begin(mp3Serial, /*isACK=*/true, /*doReset=*/true);

    if (!dfplayer_ready) {
        Serial.println("DFPlayer não respondeu — seguindo sem áudio");
        return; // não mexe mais no objeto player, evita crash
    }

    player.volume(15);
    delay(200); // 1000ms geralmente não é necessário depois do begin(), mas mantenha se preferir
}

void dfplayer_play(uint8_t track) {
    if (!dfplayer_ready) return;
    player.play(track);
}

void dfplayer_stop() {
    if (!dfplayer_ready) return;
    player.stop();
}

void dfplayer_setVolume(uint8_t vol) {
    if (!dfplayer_ready) return;
    player.volume(vol);
}

void dfplayer_play_advert(uint8_t track) {
    if (!dfplayer_ready) return;
    player.advertise(track);
}

bool dfplayer_is_playing() {
    if (!dfplayer_ready) return false;
    return player.readState() == 1;
}