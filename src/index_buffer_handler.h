#ifndef INDEX_H
#define INDEX_H

#include <GL/glew.h>

class IndexBuffer
{
public:
    IndexBuffer(const void *data, const unsigned int count);
    IndexBuffer(void);
    
    void bind_buffer(void) const;
    void unbind_buffer(void) const;
    void write_data(const void *data, const unsigned int count);
private:
    unsigned int index_buffer_ID;
private:
	IndexBuffer(const IndexBuffer& buffer_instance);
};

#endif
