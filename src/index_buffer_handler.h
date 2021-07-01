#ifndef INDEX_H
#define INDEX_H

#include <GL/glew.h>

class IndexBuffer
{
public:
    IndexBuffer(const void *data, const unsigned int count);

    void bind_buffer(void) const;
    void unbind_buffer(void) const;
private:
    unsigned int index_buffer_ID;
    unsigned int count_indices;
};

#endif
