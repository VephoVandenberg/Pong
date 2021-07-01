#ifndef VERTEXB_H
#define VERTEXB_H

#include <GL/glew.h>

class VertexBuffer
{
public:
    VertexBuffer(const void *data, const unsigned int size);

    void bind_buffer(void) const;
    void unbind_buffer(void) const;
private:
    unsigned int vertex_buffer_ID;
};

#endif
