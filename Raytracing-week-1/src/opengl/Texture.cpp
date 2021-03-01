#include "Texture.h"

#include <cassert>
#include <algorithm>

Texture::Texture(uint32_t width, uint32_t height)
    : m_ID(0), m_Width(width), m_Height(height), m_BPP(4)
{
    m_InternalFormat = GL_RGBA8;
    m_DataFormat = GL_RGBA;

    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    glTexStorage2D(GL_TEXTURE_2D, 1, m_InternalFormat, m_Width, m_Height);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   
}

Texture::Texture(std::string path):m_ID(0), m_Width(0), m_Height(0), m_BPP(0){
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* m_Data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 0);

    GLenum internalFormat = 0, dataFormat = 0;
    switch (m_BPP) {
    case 1:	dataFormat = GL_RED; internalFormat = GL_RED;    break;
    case 2: dataFormat = GL_RG; internalFormat = GL_RG8;     break;
    case 3: dataFormat = GL_RGB; internalFormat = GL_RGB8;    break;
    case 4: dataFormat = GL_RGBA; internalFormat = GL_RGBA8;   break;
    }
    m_InternalFormat = internalFormat;
    m_DataFormat = dataFormat;
    
    if (m_Data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, GL_UNSIGNED_BYTE, m_Data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(m_Data);
}



Texture::~Texture()
{
    glDeleteTextures(1, &m_ID);
}

void Texture::Bind(uint32_t slot){
	glActiveTexture(GL_TEXTURE0+slot);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind(){
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetData(void* data, uint32_t size) {
    assert(size == m_Width * m_Height * m_BPP && "Wrong texture size!");

    SetData(data, size, 0, 0, m_Width, m_Height);
}

void Texture::SetData(void* data, uint32_t size, uint32_t xOffset, uint32_t yOffset, uint32_t width, uint32_t height) {
    assert(size <= m_Width * m_Height * m_BPP && "Wrong texture size!");

    glBindTexture(GL_TEXTURE_2D, m_ID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, width, height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}

bool Texture::operator==(const Texture& other){
    return m_ID == other.m_ID;
}


