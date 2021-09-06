#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <iostream>

class TextureHandler
{
public:
    TextureHandler(const void *data,
		   const unsigned int width,
		   const unsigned int height);

    void bind_texture(void) const;
    void unbind_texture(void) const;
private:
    unsigned int texture_ID;
private:
	TextureHandler(const TextureHandler& texture_handler_instance);
};

#endif
