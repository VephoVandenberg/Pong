#include "vertex_buffer_handler.h"

VertexBuffer::VertexBuffer(const void *data, const unsigned int size)
{    
    glGenBuffers(1, &vertex_buffer_ID);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::bind_buffer(void) const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::unbind_buffer(void) const
{
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ID);
}
