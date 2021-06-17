# 说明

本项目着重针对官方代码进行大量优化，目前主要改进如下：

* 增加解码器API选项: Java（官方）、NDK（优化）
* 增加自定义分辨率
* 增加缓冲区设置
* 增加三指触摸控制性能信息显示和键盘弹出
* 提高渲染效率和实时性，画面更稳定
* 帧率统计信息更准确

由于使用了MediaCodec NDK对解码模块进行重写，API要求 >= 21（android系统版本5.0）


## 如何更改分辨率

在Nvidia控制面板中，增加自定义分辨率，并设置电脑分辨率。

为了使串流与电脑显示器不干扰，请在某宝购买”HDMI欺骗器“，最好定制HDR版本以享受手机端的HDR功能。

![16556A5CDE990FAE78F443B1EB9941F7](/screenshot/16556A5CDE990FAE78F443B1EB9941F7.jpg)

## 如何搭建5G 4K 100Mbps流畅串流环境

1、尽可能清空5G通道，能走有线的设备走有线

2、低速率设备放到2.4G，专门为游戏手机提供一个独立的5G通道

3、使用一个高端路由器，例如华硕RT-AX92U

## 一些建议

##### 1、尽可能对齐PC端的游戏帧率和设备的显示帧率

要获得最大流畅度，首先在设置里把设备刷新率开到最高，然后在moonlight里对齐与PC的刷新率（以实际游戏内设置的刷新率为准）

##### 2、使用Steam上的《手柄伴侣》作为快捷键

配置好windows快捷键，方便切换任务和操作窗口

##### 3、手柄

使用雷蛇骑仕可将手机变为掌机，更舒适，不累。

（注意：手柄通过蓝牙连接手机会带来一定延迟）

# Moonlight Android

[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/232a8tadrrn8jv0k/branch/master?svg=true)](https://ci.appveyor.com/project/cgutman/moonlight-android/branch/master)
[![Translation Status](https://hosted.weblate.org/widgets/moonlight/-/moonlight-android/svg-badge.svg)](https://hosted.weblate.org/projects/moonlight/moonlight-android/)

[Moonlight for Android](https://moonlight-stream.org) is an open source implementation of NVIDIA's GameStream, as used by the NVIDIA Shield.

Moonlight for Android will allow you to stream your full collection of games from your Windows PC to your Android device,
whether in your own home or over the internet.

Moonlight also has a [PC client](https://github.com/moonlight-stream/moonlight-qt) and [iOS/tvOS client](https://github.com/moonlight-stream/moonlight-ios).

You can follow development on our [Discord server](https://moonlight-stream.org/discord) and help translate Moonlight into your language on [Weblate](https://hosted.weblate.org/projects/moonlight/moonlight-android/).

## Downloads
* [Google Play Store](https://play.google.com/store/apps/details?id=com.limelight)
* [Amazon App Store](https://www.amazon.com/gp/product/B00JK4MFN2)
* [F-Droid](https://f-droid.org/packages/com.limelight)
* [APK](https://github.com/moonlight-stream/moonlight-android/releases)

## Building
* Install Android Studio and the Android NDK
* Run ‘git submodule update --init --recursive’ from within moonlight-android/
* In moonlight-android/, create a file called ‘local.properties’. Add an ‘ndk.dir=’ property to the local.properties file and set it equal to your NDK directory.
* Build the APK using Android Studio or gradle

## Authors

* [Cameron Gutman](https://github.com/cgutman)  
* [Diego Waxemberg](https://github.com/dwaxemberg)  
* [Aaron Neyer](https://github.com/Aaronneyer)  
* [Andrew Hennessy](https://github.com/yetanothername)

Moonlight is the work of students at [Case Western](http://case.edu) and was
started as a project at [MHacks](http://mhacks.org).
