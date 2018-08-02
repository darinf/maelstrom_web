mergeInto(LibraryManager.library, {
  jslib_audio_play: function(samplesPtr, numSamples) {
    var mappedSamples = new Uint8Array(Module.HEAPU8.buffer, samplesPtr, numSamples);

    var copy = new Uint8Array(numSamples);
    copy.set(mappedSamples);

    worker_play_sound(copy);
  },
  jslib_audio_set_volume: function(volume) {
    worker_set_volume(volume);
  },
});
