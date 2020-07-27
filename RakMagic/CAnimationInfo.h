/* =========================================

	- File: AnimationInfo.hpp
	- Author(s): ziggi

  =========================================*/

#ifndef ANIMATIONINFO_H
#define ANIMATIONINFO_H

#define MAX_ANIMATIONS 1813
#define MAX_ANIMATION_NAME 36

class CAnimationInfo {
public:
	static char *GetNameByIndex(short wIndex);
	static short GetIndexByName(char *szName);

private:
	static char m_cAnimationsName[MAX_ANIMATIONS][36];
};

#endif
