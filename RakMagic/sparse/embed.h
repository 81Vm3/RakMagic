#ifndef EMBED_H
#define EMBED_H

class EmbeddedResource {
public:
	std::size_t size_bytes = 0;
	void* ptr = nullptr;

	EmbeddedResource(int resource_id);
	~EmbeddedResource();

private:
	HRSRC hResource = nullptr;
	HGLOBAL hMemory = nullptr;
};

#endif