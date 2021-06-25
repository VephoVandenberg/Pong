#define GLEW_STATIC

#include "shader_handler.h"

#include <GLFW/glfw3.h>

const int width = 800;
const int height = 500;

static float movement_left = 0.0f;
static float move_dir_left = 0.0f;

static float movement_right = 0.0f;
static float move_dir_right = 0.0f;

void key_callback(GLFWwindow *window,
		  int key,
		  int scancode,
		  int action,
		  int mods);
void framebuffer_size_callback(GLFWwindow *window,
			       int width, int height);

int main(int argc, char **argv)
{
    if (!glfwInit())
    {
	std::cout << "Could not initialize GLFW" << std::endl;
    }

    GLFWwindow *window = glfwCreateWindow(width,
					  height,
					  "Pong",
					  0,
					  0);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK)
    {
	std::cout << "Could not initialize GLEW" << std::endl;
    }
    
    float player1[24] =
    {
	-0.95f, -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, // left bottom
	-0.9f,  -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, // right bottom
	-0.95f,  0.2f, 0.0f, 1.0f, 1.0f, 1.0f, // top left
	-0.9f,   0.2f, 0.0f, 1.0f, 1.0f, 1.0f // top right
    };

    unsigned int indicies[6] =
    {
	0, 1, 2, // first triangle
	1, 2, 3  // second triangle 
    };

    unsigned int EBO, VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(player1), player1, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shader_handler("vertex_shader.shr", "fragment_shader.shr");
    shader_handler.use();
    
    while (!glfwWindowShouldClose(window))
    {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

// Left player bar movement
	glm::mat4 trans_left;
	if (movement_left + move_dir_left + 0.2f >= 1.0f)
	{
	    movement_left = 1.0f - 0.21f;
	}
	else if (movement_left - move_dir_left - 0.2f <= -1.0f)
	{
	    movement_left = -1.0f + 0.21f;
	}
	else
	{
	    movement_left += move_dir_left;
	}
	trans_left = glm::translate(trans_left, glm::vec3(0.0f, movement_left, 0.0f));
        shader_handler.set_matrix("transform", trans_left);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// Right player bar movement
	glm::mat4 trans_right;
	if (movement_right + move_dir_right + 0.2f >= 1.0f)
	{
	    movement_right = 1.0f - 0.21f;
	}
	else if (movement_right - move_dir_right - 0.2f <= -1.0f)
	{
	    movement_right = -1.0f + 0.21f;
	}
	else
	{
	    movement_right += move_dir_right;
	}
	trans_right = glm::translate(trans_right, glm::vec3(1.85f, movement_right, 0.0f));
	shader_handler.set_matrix("transform", trans_right);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	glfwSwapBuffers(window);
	glfwPollEvents();

    }
    
    return 0;
}

void key_callback(GLFWwindow *window,
		  int key,
		  int scancode,
		  int action,
		  int mods)
{
    switch(key)
    {
	case GLFW_KEY_W:
	{
	    if (action == GLFW_PRESS)
	    {
		move_dir_left += 0.001f;
	    }
	    else if (action == GLFW_RELEASE)
	    {
		move_dir_left -= 0.001f;
	    }
	}break;

	case GLFW_KEY_S:
	{
	    if (action == GLFW_PRESS)
	    {
		move_dir_left -= 0.001f;       
	    }
	    else if (action == GLFW_RELEASE)
	    {
		move_dir_left += 0.001f;
	    }
	}break;

	case GLFW_KEY_UP:
	{
	    if (action == GLFW_PRESS)
	    {
		move_dir_right += 0.001f;
	    }
	    else if (action == GLFW_RELEASE)
	    {
		move_dir_right -= 0.001f;
	    }
	}break;

	case GLFW_KEY_DOWN:
	{
	    if (action == GLFW_PRESS)
	    {
		move_dir_right -= 0.001f;
	    }
	    else if (action == GLFW_RELEASE)
	    {
		move_dir_right += 0.001f;
	    }
	}break;

	default:
	{
	}break;
    }
}

void framebuffer_size_callback(GLFWwindow *window,
			       int width, int height)
{
    glViewport(0, 0, width, height);
}


