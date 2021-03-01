#pragma once

#include <iostream>

#include "Utility.h"


class Texture {
private:
	unsigned int m_ID;
	int m_Width, m_Height, m_BPP;
	GLenum m_InternalFormat, m_DataFormat;
public:
	Texture(std::string path);
	Texture(uint32_t width, uint32_t height);
	~Texture();

	void Bind(uint32_t slot = 0);
	void Unbind();

	void SetData(void* data, uint32_t size);
	void SetData(void* data, uint32_t size, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height);

	inline int GetWidth()const{ return m_Width; };
	inline int GetHeight()const { return m_Height; };

	bool operator==(const Texture& other);
};