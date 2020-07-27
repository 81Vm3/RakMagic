#ifndef INTERFACE_H
#define INTERFACE_H

namespace Interface {
	extern int ScreenWidth;
	extern int ScreenHeight;

	extern bool bExit;

	const float fBoxAlpha = 0.4f;

	enum {
		NEARBY_PLAYER,
		NEARBY_VEHICLE,
		NEARBY_PICKUP
	};

	bool Initialize();
	void Uninitialize();
	void OnWindowResized(GLFWwindow* window, int width, int height);
	bool ShouldClose();

	void SetSubWindowTransparent();
	void Render();
	void RenderMenuBar();

	bool IsKeyDown(int key);
	bool IsKeyPressOnce(int key);
}

#endif