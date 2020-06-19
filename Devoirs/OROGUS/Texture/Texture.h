#ifndef _TEXTURE_TEXTURE_H_
#define _TEXTURE_TEXTURE_H_

#include <glew/glew.h>

#include "../Utilities/Color.h"
#include "../Utilities/Types.h"

#include <string>

class Texture2D
{
public:
    static Texture2D* CreateTexture(const std::string& fileName);
    //static Texture2D* CreateTexture(uint32 width, uint32 height, const Color * const data);

    Texture2D(const Texture2D& other) = delete;
    Texture2D(Texture2D&& other) = delete;
	~Texture2D();
	
    const std::string& getName() const;

	bool bind(uint32 bindingUnit) const;

private:
    Texture2D(const std::string& fileName);
    Texture2D(uint32 width, uint32 height, const Color * const data);

    bool load();

    std::string m_name;

    uint32 m_textureID;
    uint32 m_width;
    uint32 m_height;
    Color* m_textureData;
};

#endif
