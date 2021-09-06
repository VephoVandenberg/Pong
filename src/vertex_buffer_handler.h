#ifndef VERTEXB_H
#define VERTEXB_H

#include <GL/glew.h>

class VertexBuffer
{
public:
    VertexBuffer(const void *data, const unsigned int size);
    VertexBuffer(void);
    
    void bind_buffer(void) const;
    void unbind_buffer(void) const;
    void write_data(const void *data, const unsigned int size);
private:
    unsigned int vertex_buffer_ID;
private:
	VertexBuffer(const VertexBuffer& vertex_buffer_handler);
};

#endif
