# RakMagic
Fake client for SA-MP

## Introduction
RakMagic is a tool for SA-MP which can connect a fake client to a SA-MP server. 
Its has graphical UI, ability to read and write NPC record, simulation of player (Taking damage, doing animations)

## Compiling requirements:
Visual Studio 2017

[GLFW library](https://www.glfw.org/)

Add GLFW library:
 - Create two new directories: `lib`, `lib/GLFW`
 - Put GLFW library you compiled and its headers to the directory together
 - **DO NOT** put the headers and the library seperately

(RakNet and ImGui is included by default)

## Frequent Asked Questions

## Q. ***How to spawn the client?***
When you are ready to spawn, click **Request spawn** first, then click **Spawn** to spawn the client.
You must click **Request spawn** first or you will get yourself banned on the server.

## Q. ***How to get into a vehicle?***
Type command `!veh` to set driving vehicle ID. To cancel driving, use -1 as argument.
If the vehicle ID is invalid, the program will find another vehicle that has same ID.

## Q. ***What is playback?***
A playback is a NPC record which has packets written in.

You can only play playback & record playback when the client is spawned. 

There's a [NPC recording tutorial](https://forum.sa-mp.com/showthread.php?t=95034) on SA-MP forum.

## Q. ***Is there a 0.3.DL version?***
It's not recommended to join a 0.3.DL server with RakMagic. However there is a trick: By changing netgame version.

**4057** is for 0.3.7

**4062** is for 0.3.DL


## Q. ***The program is unable to render Chinese characters (中文乱码)***
国内的服务器的中文内容是GB2312编码的，而RakMagic的界面上显示的是UTF-8的中文

如果你需要转码，去xml设置里面，把interface里的gb2312改成1。之后RakMagic就会帮你转码 (但是日志里的编码仍然是GB2312)

本软件似乎不能在wine环境下显示中文

## Reporting bugs
Bugs are reported to the issue tracker. Or you can mail me via <p3g4sus@foxmail.com>

(I'm not a good UI designer. Also, this project is combined with C++ and C)
