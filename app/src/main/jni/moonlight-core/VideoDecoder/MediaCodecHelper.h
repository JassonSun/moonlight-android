//
// Created by Viktor Pih on 2020/8/10.
//

#ifndef MOONLIGHT_ANDROID_MEDIACODECHELPER_H
#define MOONLIGHT_ANDROID_MEDIACODECHELPER_H

#include <stdbool.h>
#include <stddef.h>
#include <media/NdkMediaExtractor.h>

#define Build_VERSION_CODES_KITKAT 19
#define Build_VERSION_CODES_LOLLIPOP 21
#define Build_VERSION_CODES_M 23
#define Build_VERSION_CODES_O 26
#define Build_VERSION_CODES_R 30

#define Build_VERSION_SDK_INT _Build_VERSION_SDK_INT()

bool MediaCodecHelper_decoderSupportsQcomVendorLowLatency(const char* decoderName);
bool MediaCodecHelper_decoderSupportsHisiVendorLowLatency(const char* decoderName);

bool MediaCodecHelper_decoderNeedsBaselineSpsHack(const char* decoderName);
bool MediaCodecHelper_decoderNeedsSpsBitstreamRestrictions(const char* decoderName);

bool MediaCodecHelper_needAlwaysDropFrames(const char* decoderName);

void MediaCodecHelper_setDecoderLowLatencyOptions(AMediaFormat* videoFormat, const char* decoderName, bool maxOperatingRate);

int _Build_VERSION_SDK_INT();



#endif //MOONLIGHT_ANDROID_MEDIACODECHELPER_H
