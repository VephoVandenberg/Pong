#ifndef VERTEX_H
#define VERTEX_H

#include <GL/glew.h>

class VertexBuffer
{
public:
    VertexBuffer(const void *data, const unsigned int size);

    inline void bind_buffer(void) const;
    inline void unbind_buffer(void) const;
private:
    unsigned int vertex_buffer_ID;
};

#endif
