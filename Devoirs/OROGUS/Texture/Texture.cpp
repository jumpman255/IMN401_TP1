#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

const uint32 NO_TEXTURE_ID = -1;

Texture2D* Texture2D::CreateTexture(const std::string& fileName)
{
    return new Texture2D(fileName);
}

// ************************************************
// Create a new Texture2D from an image
// file content
//
// Inputs
//   - filename : Name of the file that
//                contains the texture data.
// ************************************************
Texture2D::Texture2D(const std::string& fileName)
	: m_textureID(NO_TEXTURE_ID)
	, m_textureData(nullptr)
	, m_width(0)
	, m_height(0)
    , m_name(fileName)
{
    int w;
    int h;
    int comp;
    unsigned char* image = stbi_load(fileName.c_str(), &w, &h, &comp, STBI_rgb_alpha);
        
    if (image == nullptr)
        throw(std::string("Failed to load texture"));

    m_width = w;
    m_height = h;

    m_textureData = new Color[m_width * m_height];
    for (uint32 i = 0; i < m_width * m_height; ++i)
    {
        m_textureData[i] = Color(image[i * 4], image[(i * 4) + 1], image[(i * 4) + 2], image[(i * 4) + 3] / 255.f);
    }

    load();

    stbi_image_free(image);

}

// ************************************************
// Create a new Texture2D from color data
//
// Inputs
//   - width : Width of the texture, in pixels
//   - height: Height of the texture, in pixels
//   - data  : Color data for each pixels, in row format
// ************************************************
Texture2D::Texture2D(uint32 width, uint32 height, const Color * const data)
	: m_textureID(NO_TEXTURE_ID)
	, m_textureData(nullptr)
	, m_width(width)
	, m_height(height)
    , m_name("Custom")
{
	m_textureData = new Color[width * height];
	for (uint32 i = 0; i < width * height; ++i)
	{
		m_textureData[i] = data[i];
	}

	load();
}


Texture2D::~Texture2D()
{
	if (m_textureData)
	{
		delete[] m_textureData;
		m_textureData = nullptr;
	}

	if (m_textureID != NO_TEXTURE_ID)
	{
		glDeleteTextures(1, &m_textureID);
		m_textureID = NO_TEXTURE_ID;
	}	
}

const std::string& Texture2D::getName() const
{
    return m_name;
}

bool Texture2D::bind(uint32 bindingUnit) const
{
	glActiveTexture(GL_TEXTURE0 + bindingUnit);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	return true;
}


bool Texture2D::load()
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_FLOAT, m_textureData);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	return true;
}