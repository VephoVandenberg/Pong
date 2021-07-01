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
