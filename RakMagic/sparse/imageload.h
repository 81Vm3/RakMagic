#ifndef IMAGELOAD_H
#define IMAGELOAD_H

class CImageLoad {
public:
    CImageLoad();
    CImageLoad(const char *file);
    ~CImageLoad();

    bool Load(const char *file);
	bool LoadEmbedded(int res_id);
    void Unload();

    bool IsValid();
    ImVec2 GetResolution();
    ImTextureID GetRenderTextureID();

	//This is messed up
    //void DrawAsWallpaper(int screenWeight, int screenHeight, int style);

private:
    int iResolution[2];
    GLuint uTexture;
};

namespace ResImage {
	bool InitializeResources();
	extern CImageLoad AppIcon;
}

#endif
