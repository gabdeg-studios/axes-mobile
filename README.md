# Axes

A game written in C for Android. Uses SDL2 and the Android NDK.

## Build

To build with Android debug keys:

1. Obtain SDL2 and SDL2_ttf source
2. Move SDL2 and SDL2_ttf folders to `jni/SDL2` and `jni/SD2_ttf`
3. `ndk-build`
4. `ant debug`
