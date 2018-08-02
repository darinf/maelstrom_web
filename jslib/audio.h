#ifndef JSLIB_AUDIO_H_
#define JSLIB_AUDIO_H_

extern "C" {

extern void jslib_audio_play(void* samples, int num_samples);
extern void jslib_audio_set_volume(float volume);

}  // extern "C"

#endif  // JSLIB_AUDIO_H_
