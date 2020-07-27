# RakMagic
Client for SA-MP

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

## Reporting bugs
Bugs are reported to the issue tracker. Or you can send email to me <p3g4sus@foxmail.com>

(I'm not a good UI designer. Also, this project is combined with C++ and C)

## 软件显示的中文是乱码!!! 怎么办?
这里特别为中国用户说明，国内的服务器的中文内容是GB2312编码的，而RakMagic的界面上显示的是UTF-8的中文，
编码不同，自然就乱码了。

如果你需要转码，去xml设置里面，把`interface`里的`gb2312`改成1。之后RakMagic就会帮你转码 (但是日志里的编码仍然是GB2312)

本软件似乎不能在wine环境下显示中文

还有一点，软件目前**没有中文版本**，我没有找到多语言解决方案
