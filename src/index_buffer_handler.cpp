#include "index_buffer_handler.h"

IndexBuffer::IndexBuffer(const void *data, const unsigned int count)
{    
    glGenBuffers(1, &index_buffer_ID);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, data, GL_STATIC_DRAW);
}

inline void IndexBuffer::bind_buffer(void) const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

inline void IndexBuffer::unbind_buffer(void) const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_ID);
}
