//
// Created by Viktor Pih on 2020/8/10.
//

#include "MediaCodecHelper.h"
#include <sys/system_properties.h>
#include <android/log.h>
#include <ctype.h>

#define LOG_TAG    "MediaCodecHelper"
#ifdef LC_DEBUG
#define LOGD(...)  {__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__); /*printCache();*/}
#else
#define LOGD(...)
#endif

const char* kirinDecoderPrefixes[] = {"omx.hisi"};
const char* exynosDecoderPrefixes[] = {"omx.exynos"};
const char* qualcommDecoderPrefixes[] = {"omx.qcom", "c2.qti"};
const char* baselineProfileHackPrefixes[] = {"omx.intel"};
const char* spsFixupBitstreamFixupDecoderPrefixes[] = {"omx.nvidia", "omx.qcom", "omx.brcm"};

char * __cdecl stristr (
        const char * str1,
        const char * str2
)
{
    char *cp = (char *) str1;
    char *s1, *s2;

    if ( !*str2 )
        return((char *)str1);

    while (*cp)
    {
        s1 = cp;
        s2 = (char *) str2;

        while ( *s1 && *s2 && !(tolower(*s1)-tolower(*s2)) )
            s1++, s2++;

        if (!*s2)
            return(cp);

        cp++;
    }

    return(0);
}

bool isDecoderInList(const char* list[], size_t size, const char* decoderName) {

    for (int i = 0; i < size; i++) {
        if (stristr(decoderName, list[i]) == decoderName) {
            return true;
        }
    }
    return false;
}

int _Build_VERSION_SDK_INT() {
    static char sdk_ver_str[PROP_VALUE_MAX+1];
    static int sdk_ver = 0;
    if (sdk_ver == 0) {
        if (__system_property_get("ro.build.version.sdk", sdk_ver_str)) {
            sdk_ver = atoi(sdk_ver_str);
        } else {
            // Not running on Android or SDK version is not available
            // ...
            sdk_ver = -1;
            LOGD("sdk_version fail!");
        }
    }
    return sdk_ver;
}

#define IS_DECODER_IN_LIST(a, b) isDecoderInList(a, sizeof(a)/sizeof(*a), b)

bool MediaCodecHelper_decoderSupportsQcomVendorLowLatency(const char* decoderName) {
    // MediaCodec vendor extension support was introduced in Android 8.0:
    // https://cs.android.com/android/_/android/platform/frameworks/av/+/01c10f8cdcd58d1e7025f426a72e6e75ba5d7fc2
    return Build_VERSION_SDK_INT >= Build_VERSION_CODES_O &&
            IS_DECODER_IN_LIST(qualcommDecoderPrefixes, decoderName);
}

bool MediaCodecHelper_decoderSupportsHisiVendorLowLatency(const char* decoderName) {
    return //Build_VERSION_SDK_INT >= Build_VERSION_CODES_O &&
            IS_DECODER_IN_LIST(kirinDecoderPrefixes, decoderName);
}

bool MediaCodecHelper_decoderNeedsBaselineSpsHack(const char* decoderName) {
    return IS_DECODER_IN_LIST(baselineProfileHackPrefixes, decoderName);
}

bool MediaCodecHelper_decoderNeedsSpsBitstreamRestrictions(const char* decoderName) {
    return IS_DECODER_IN_LIST(spsFixupBitstreamFixupDecoderPrefixes, decoderName);
}

bool MediaCodecHelper_decoderSupportsAndroidRLowLatency(/*MediaCodecInfo decoderInfo, String mimeType*/) {

//    if (Build_VERSION_SDK_INT >= Build_VERSION_CODES_R) {
//        if (decoderInfo.getCapabilitiesForType(mimeType).isFeatureSupported(CodecCapabilities.FEATURE_LowLatency)) {
//            LimeLog.info("Low latency decoding mode supported (FEATURE_LowLatency)");
//            return true;
//        }
//    }
//
//    return false;
    return true;
}

bool MediaCodecHelper_needAlwaysDropFrames(const char* decoderName) {
    return IS_DECODER_IN_LIST(kirinDecoderPrefixes, decoderName);
}

void MediaCodecHelper_setDecoderLowLatencyOptions(AMediaFormat* videoFormat, const char* decoderName, bool maxOperatingRate) {

    // android 30+ 及其以上才支持低延迟模式，可以设置这个值
    if (Build_VERSION_SDK_INT >= Build_VERSION_CODES_R && MediaCodecHelper_decoderSupportsAndroidRLowLatency()) {
        AMediaFormat_setInt32(videoFormat, /*AMEDIAFORMAT_KEY_LATENCY*/"latency", 0);
    }else if (Build_VERSION_SDK_INT >= Build_VERSION_CODES_M) {
        // MediaCodec supports vendor-defined format keys using the "vendor.<extension name>.<parameter name>" syntax.
        // These allow access to functionality that is not exposed through documented MediaFormat.KEY_* values.
        // https://cs.android.com/android/platform/superproject/+/master:hardware/qcom/sdm845/media/mm-video-v4l2/vidc/common/inc/vidc_vendor_extensions.h;l=67
        //
        // MediaCodec vendor extension support was introduced in Android 8.0:
        // https://cs.android.com/android/_/android/platform/frameworks/av/+/01c10f8cdcd58d1e7025f426a72e6e75ba5d7fc2
        if (Build_VERSION_SDK_INT >= Build_VERSION_CODES_O) {
            // Try vendor-specific low latency options
            if (IS_DECODER_IN_LIST(qualcommDecoderPrefixes, decoderName)) {
                // Examples of Qualcomm's vendor extensions for Snapdragon 845:
                // https://cs.android.com/android/platform/superproject/+/master:hardware/qcom/sdm845/media/mm-video-v4l2/vidc/vdec/src/omx_vdec_extensions.hpp
                // https://cs.android.com/android/_/android/platform/hardware/qcom/sm8150/media/+/0621ceb1c1b19564999db8293574a0e12952ff6c
                AMediaFormat_setInt32(videoFormat, "vendor.qti-ext-dec-low-latency.enable", 1);
            }
            else if (IS_DECODER_IN_LIST(kirinDecoderPrefixes, decoderName)) {
                // Kirin low latency options
                // https://developer.huawei.com/consumer/cn/forum/topic/0202325564295980115
                AMediaFormat_setInt32(videoFormat, "vendor.hisi-ext-low-latency-video-dec.video-scene-for-low-latency-req", 1);
                AMediaFormat_setInt32(videoFormat, "vendor.hisi-ext-low-latency-video-dec.video-scene-for-low-latency-rdy", -1);
//                alwaysDropFrames = true;
            }
            else if (IS_DECODER_IN_LIST(exynosDecoderPrefixes, decoderName)) {
                // Exynos low latency option for H.264 decoder
                AMediaFormat_setInt32(videoFormat, "vendor.rtc-ext-dec-low-latency.enable", 1);
            }
        }

        if (maxOperatingRate) {
            //videoFormat.setInteger(MediaFormat.KEY_OPERATING_RATE, Short.MAX_VALUE);
            AMediaFormat_setInt32(videoFormat, "operating-rate", 32767); // Short.MAX_VALUE
        }
    }
}