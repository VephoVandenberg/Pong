#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader
{
public:
    Shader(const char *vertex_path, const char *fragment_path);
    void use(void);

    inline void set_bool(const std::string &name, bool value) const;
    inline void set_int(const std::string &name, int value) const;
    inline void set_float(const std::string &name, float value) const;
    void set_matrix(const std::string& name, glm::mat4 matrix) const;
private:
    unsigned int shader_ID;
};

#endif
