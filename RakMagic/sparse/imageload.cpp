#include "main.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

CImageLoad::CImageLoad() {

}

CImageLoad::CImageLoad(const char *file) {
    Load(file);
}


CImageLoad::~CImageLoad() {
    Unload();
}

bool CImageLoad::Load(const char *file) {

    if (IsValid())
        Unload();

    unsigned char* image_data = stbi_load(file, &iResolution[0], &iResolution[1], NULL, STBI_rgb_alpha);
	if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    glGenTextures(1, &uTexture);
    glBindTexture(GL_TEXTURE_2D, uTexture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iResolution[0], iResolution[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    //vColor = ImVec4(1, 1, 1, 1);

    return true;
}

bool CImageLoad::LoadEmbedded(int res_id) {

	if (IsValid())
		Unload();

	EmbeddedResource resource(res_id);
	if (!resource.ptr)
		return false;

	unsigned char* image_data = stbi_load_from_memory((unsigned char*)resource.ptr, (int)resource.size_bytes, &iResolution[0], &iResolution[1], NULL, STBI_rgb_alpha);

	if (image_data == NULL)
		return false;

	glGenTextures(1, &uTexture);
	glBindTexture(GL_TEXTURE_2D, uTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iResolution[0], iResolution[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	stbi_image_free(image_data);
	return true;
}

void CImageLoad::Unload() {
    if (glIsTexture(uTexture) == GL_TRUE)
        glDeleteTextures(1, &uTexture);
}

bool CImageLoad::IsValid() {
    if (glIsTexture(uTexture) == GL_TRUE)
        return true;
    return false;
}

ImVec2 CImageLoad::GetResolution() {
    return ImVec2((float)iResolution[0], (float)iResolution[1]);
}

ImTextureID CImageLoad::GetRenderTextureID() {
    return (void*)(intptr_t)uTexture;
}

namespace ResImage {
	CImageLoad AppIcon;

	bool InitializeResources() {
		return (AppIcon.LoadEmbedded(IDB_CREPPER));
	}
}