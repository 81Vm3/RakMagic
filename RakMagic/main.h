#ifndef MAIN_H
#define MAIN_H

#define RAKMAGIC_NAME "RakMagic"
#define RAKMAGIC_VERSION "1.14"
#define RAKMAGIC_AUTHOR "Blume (aka 81Vm3)"

#define RAKMAGIC_ABOUT_MESSAGE RAKMAGIC_NAME RAKMAGIC_VERSION \
"\n\nCopyright (C) 2020 Blume <p3g4sus@foxmail.com> and contributors\n\
Special thanks to: RakSAMPClient, FCNPC"

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <ctime>
#include <Windows.h>
#include <GLFW/glfw3.h>

#include "tinyxml2.h"

#include "samp_netencr.h"
#include "samp_auth.h"
#include "SAMPRPC.h"
#include "SAMP_VER.h"
#include "samp_structures.h"
#include "CAnimationInfo.h"

#include "PacketEnumerations.h"
#include "RakNetworkFactory.h"
#include "RakClientInterface.h"
#include "NetworkTypes.h"
#include "BitStream.h"
#include "StringCompressor.h"

#include "imgui.h"
#include "extra_imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "IconsMaterialDesign.h"

//For some unintelligible problems, I'm unable to load image from memory
//#include "imageload.h"
//#include "embed.h"

#include "mathFunc.h"
#include "misc.h"
#include "encode.h"
#include "log.h"
#include "config.h"
#include "player.h"
#include "common.h"
#include "client.h"
#include "client_packet.h"
#include "client_ai.h"

#include "interface.h"
#include "console.h"
#include "dialog.h"
#include "scoreboard.h"
#include "playbackGUI.h"

#include "commands.h"
#include "playback.h"
#include "recorder.h"

extern struct stVehiclePool gVehiclePool[MAX_VEHICLES];
extern struct stPickupPool gPickupPool[MAX_PICKUPS];

#endif