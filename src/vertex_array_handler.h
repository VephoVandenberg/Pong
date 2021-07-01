#ifndef VERTEXA_H
#define VERTEXA_H

#include <GL/GLEW.h>

class VertexArray
{
public:
    VertexArray(void);

    void bind_buffer(void) const;
    void unbind_buffer(void) const;
private:
    unsigned int vertex_array_ID;
};

#endif
