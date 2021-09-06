#ifndef VERTEXA_H
#define VERTEXA_H

#include <GL/GLEW.h>

class VertexArray
{
public:
    VertexArray(void);

    void bind_buffer(void) const;
    void unbind_buffer(void) const;
    void set_attribute_pointer(unsigned int location,
			       unsigned int dimmension,
			       unsigned int attribute_count,
			       unsigned int location_in_bytes);
private:
    unsigned int vertex_array_ID;
private:
	VertexArray(const VertexArray& vertex_array_buffer);
};

#endif
