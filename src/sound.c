#include "sound.h"

#define PI 3.14159265358979323846
#define AMPLITUDE 127
#define FREQUENCY 44100

void audio_callback(void *user_data, Uint8 *stream, int len) {
    SDL_memset(stream, 0, len);
    (void) user_data; // tell the compile that user_data is not used

    for (int i = 0; i < len; i++) {
        stream[i] = (AMPLITUDE * sin(i * PI * 2 * 604.1 / FREQUENCY));
    }
}

void sound_play() {
    SDL_PauseAudio(0);
}

void sound_stop() {
    SDL_PauseAudio(1);
}

int sound_init() {
    SDL_AudioSpec want;
    want.freq = FREQUENCY;
    want.format = AUDIO_S8;
    want.channels = 1;
    want.samples = 128; // buffer-size
    want.callback = audio_callback; // callback to refill buffer

    SDL_AudioSpec have;
    if(SDL_OpenAudio(&want, &have)) {
        SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Failed to open audio: %s", SDL_GetError());
        return 1;
    }

    return 0;
}

void handle_sound() {
    if (m.should_beep) {
        sound_play();
    } else {
        sound_stop();
    }
}