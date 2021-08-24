#include "index_buffer_handler.h"

IndexBuffer::IndexBuffer(const void *data, const unsigned int count)
    : count_indices(count)
{    
    glGenBuffers(1, &index_buffer_ID);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_indices, data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(void)
{
	glGenBuffers(1, &index_buffer_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_ID);
}

void IndexBuffer::bind_buffer(void) const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::unbind_buffer(void) const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_ID);
}

void IndexBuffer::write_data(cosnt void *data, const unsigned int count)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_indices, data, GL_STATIC_DRAW);
}