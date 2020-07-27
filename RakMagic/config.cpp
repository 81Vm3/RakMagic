#include "main.h"

const char *gDefaultConfigPath = "settings.xml";

stConfig gConfig;

bool loadConfig(stConfig *config, const char *path) {
	using namespace tinyxml2;
	
	if (!config->bInitializedFirstEver)
		memset(config, 0x00, sizeof(stConfig));
	
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path);
	if (doc.ErrorID() != XML_SUCCESS)
		return false;

	XMLElement* elemRakMagic = doc.FirstChildElement("RakMagic");
	if (elemRakMagic) {
		char cache[128];

		XMLElement* elemServer = elemRakMagic->FirstChildElement("server");
		if (elemServer) {
			strcpy(config->szServerAddress, elemServer->Attribute("host"));
			strcpy(config->szServerPassword, elemServer->Attribute("password"));
			elemServer->QueryUnsignedAttribute("port", &config->iPort);
		}

		XMLElement* elemClient = elemRakMagic->FirstChildElement("client");
		if (elemClient) {
			elemClient->QueryUnsignedAttribute("update_rate", &config->iUpdateRate);
			elemClient->QueryIntAttribute("netgame", &config->iNetGameVersion);
			strcpy(config->szClientVersion, elemClient->Attribute("version"));

			elemClient->QueryIntAttribute("nickname_style", &config->iNicknameStyle);
			strcpy(config->szNickname, elemClient->Attribute("nickname"));

			elemClient->QueryUnsignedAttribute("reconnect_time", &config->uReconnectTime);
			elemClient->QueryIntAttribute("default_class", &config->iDefaultClass);

			if (!config->bInitializedFirstEver) {
				strcpy(cache, elemClient->Attribute("position"));
				sscanf(cache, "%f%f%f", &config->fPosition[0], &config->fPosition[1], &config->fPosition[2]);
			}

			elemClient->QueryBoolAttribute("keep_position", &config->bKeepPosition);
			elemClient->QueryBoolAttribute("damage", &config->bDamage);
			elemClient->QueryUnsignedAttribute("respawn_time", &config->uRespawnTick);
		}
		XMLElement* elemFollow = elemRakMagic->FirstChildElement("follow");
		if (elemFollow) {
			strcpy(cache, elemFollow->Attribute("follow_offset"));
			sscanf(cache, "%f%f%f", &config->fFollowOffset[0], &config->fFollowOffset[1], &config->fFollowOffset[2]);

			elemFollow->QueryFloatAttribute("enter_distance", &config->fEnterDistance);

			elemFollow->QueryBoolAttribute("copy_health", &config->bCopyHealth);
			elemFollow->QueryBoolAttribute("copy_weapon", &config->bCopyWeapon);
		}
		XMLElement* elemAnimation = elemRakMagic->FirstChildElement("animation");
		if (elemAnimation) {
			elemAnimation->QueryIntAttribute("index", (int *)&config->sAnimIndex);
			elemAnimation->QueryBoolAttribute("loop", &config->bAnimLoop);
			elemAnimation->QueryBoolAttribute("lockx", &config->bAnimLockX);
			elemAnimation->QueryBoolAttribute("locky", &config->bAnimLockY);
			elemAnimation->QueryIntAttribute("time", &config->iAnimTime);
		}
		XMLElement* elemPlayback = elemRakMagic->FirstChildElement("playback");
		if (elemPlayback) {
			elemPlayback->QueryBoolAttribute("autoplay", &config->bPlaybackAutoPlay);
			elemPlayback->QueryBoolAttribute("death_pause", &config->bPlaybackDeathPause);
		}
		XMLElement *elemInterface = elemRakMagic->FirstChildElement("interface");
		if (elemInterface) {
			strcpy(config->szFont, elemInterface->Attribute("font"));

			int fontSize;
			elemInterface->QueryIntAttribute("font_size", &fontSize);
			config->fFontSize = (float)fontSize;

			elemInterface->QueryBoolAttribute("gb2312", &config->bEncodeGB2312);

			strcpy(cache, elemInterface->Attribute("clear_color"));
			sscanf(cache, "%f%f%f", &config->InterfaceClearColor.x, &config->InterfaceClearColor.y, &config->InterfaceClearColor.z);
			config->InterfaceClearColor.w = 1.0f;
			//RGB to HSV
			config->InterfaceClearColor = ImColor((int)config->InterfaceClearColor.x, (int)config->InterfaceClearColor.y, (int)config->InterfaceClearColor.z);
		}
	}

	doc.Clear();

	return (config->bInitializedFirstEver = true);
}