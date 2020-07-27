#include "main.h"

EmbeddedResource::EmbeddedResource(int resource_id) {
	ptr = 0;
	hResource = FindResource(nullptr, MAKEINTRESOURCE(resource_id), RT_RCDATA);
	if (hResource == NULL)
		return;

	hMemory = LoadResource(nullptr, hResource);
	if (hMemory == NULL)
		return;

	size_bytes = SizeofResource(nullptr, hResource);
	ptr = LockResource(hMemory);
}

EmbeddedResource::~EmbeddedResource() {
	if (ptr) {
		UnlockResource(ptr);
		FreeResource(ptr);
	}
}