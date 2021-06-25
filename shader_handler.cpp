#include "shader_handler.h"

Shader::Shader(const char *vertex_path, const char *fragment_path)
{
    std::string vertex_shader;
    std::string fragment_shader;

    std::ifstream vertex_file;
    std::ifstream fragment_file;

    vertex_file.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    fragment_file.exceptions(std::ifstream::failbit|std::ifstream::badbit);
    
    try
    {
	vertex_file.open(vertex_path);
	fragment_file.open(fragment_path);

	std::stringstream vertex_stream, fragment_stream;

	vertex_stream << vertex_file.rdbuf();
	fragment_stream << fragment_file.rdbuf();

	vertex_file.close();
	fragment_file.close();

	vertex_shader = vertex_stream.str();
	fragment_shader = fragment_stream.str();
    }
    catch(std::ifstream::failure e)
    {
	std::cout << "file or files weren't read succesfully." << std::endl;
    }

    const char *vertex_shader_src = vertex_shader.c_str();
    const char *fragment_shader_src = fragment_shader.c_str();

    char infolog[512];
    int success;

    unsigned int vertex_ID, fragment_ID;

    vertex_ID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_ID, 1, &vertex_shader_src, 0);
    glCompileShader(vertex_ID);
    glGetShaderiv(vertex_ID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
	glGetShaderInfoLog(vertex_ID, 512, 0, infolog);
	std::cout << "VERTEX:" << infolog;
    }

    fragment_ID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_ID, 1, &fragment_shader_src, 0);
    glCompileShader(fragment_ID);
    glGetShaderiv(fragment_ID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
	glGetShaderInfoLog(fragment_ID, 512, 0, infolog);
	std::cout << "FRAGMENT:" << infolog;
    }

    shader_ID = glCreateProgram();
    glAttachShader(shader_ID, vertex_ID);
    glAttachShader(shader_ID, fragment_ID);
    glLinkProgram(shader_ID);

    glDeleteShader(vertex_ID);
    glDeleteShader(fragment_ID);
}

void Shader::use(void)
{
    glUseProgram(shader_ID);
}

inline void Shader::set_bool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(shader_ID, name.c_str()), value);
}

inline void Shader::set_int(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(shader_ID, name.c_str()), (int)value);
}

inline void Shader::set_float(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(shader_ID, name.c_str()), value);
}

void Shader::set_matrix(const std::string& name, glm::mat4 matrix) const
{
    glUniformMatrix4fv(glGetUniformLocation(shader_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
