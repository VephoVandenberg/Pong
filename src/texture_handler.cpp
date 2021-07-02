#include "texture_handler.h"

TextureHandler::TextureHandler(const void *data,
			       const unsigned int width,
			       const unsigned int height)
{
    glGenTextures(1, &texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
	std::cout << "Failed to load"  << std::endl;
    }
}

void TextureHandler::bind_texture(void) const
{
    glBindTexture(GL_TEXTURE_2D, texture_ID);
}

void TextureHandler::unbind_texture(void) const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
