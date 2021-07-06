#include "vertex_array_handler.h"

VertexArray::VertexArray(void)
{
    glGenVertexArrays(1, &vertex_array_ID);
    glBindVertexArray(vertex_array_ID);
}

void VertexArray::bind_buffer(void) const
{
    glBindVertexArray(vertex_array_ID);
}

void VertexArray::unbind_buffer(void) const
{
    glBindVertexArray(0);
}

void VertexArray::set_attribute_pointer(unsigned int location,
					unsigned int dimmension,
					unsigned int attribute_count,
					unsigned int location_in_bytes)
{
    glVertexAttribPointer(location, dimmension, GL_FLOAT, GL_FALSE, attribute_count*sizeof(float), (void*)(location_in_bytes*sizeof(float)));
    glEnableVertexAttribArray(location);
}

