#define GLEW_STATIC

/*
  FINISH setting vertex attributes
*/

#include "src/shader_handler.h"
#include "src/vertex_buffer_handler.h"
#include "src/index_buffer_handler.h"
#include "src/vertex_array_handler.h"
#include "src/stb_image_loader.h"

#include <GLFW/glfw3.h>

const unsigned int screen_width = 800;
const unsigned int screen_height = 500;

// variables to move bars
static float movement_left = 0.0f;
static float move_dir_left = 0.0f;

static float movement_right = 0.0f;
static float move_dir_right = 0.0f;

// variables for movement of ball
static float movementX = 0.2f;
static float movementY = -0.1f;

static bool signX;
static bool signY;

static bool left_center;
static bool right_center;

// score variables
static unsigned int left_score = 0;
static unsigned int right_score = 0;

// velocity changes
static float velocityX = 0.02f;
static float velocityY = 0.02f;

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

    GLFWwindow *window = glfwCreateWindow(screen_width,
					  screen_height,
					  "Pong",
					  0,
					  0);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    if (glewInit() != GLEW_OK)
    {
	std::cout << "Could not initialize GLEW" << std::endl;
    }
    
    float player_paddle[24] =
    {
	-0.05f, -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom left
	 0.0f,  -0.2f, 0.0f, 1.0f, 1.0f, 1.0f, // bottom right
	-0.05f,  0.2f, 0.0f, 1.0f, 1.0f, 1.0f, // top left
	 0.0f,   0.2f, 0.0f, 1.0f, 1.0f, 1.0f  // top right
    };

    unsigned int player_indices[6] =
    {
	0, 1, 2, // first triangle
	1, 2, 3  // second triangle 
    };
    
    float ball[32] = 
    {
	-0.04f, -0.04f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom left
	 0.04f, -0.04f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
	-0.04f,  0.04f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
  	 0.04f,  0.04f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f  // top right
    };

    unsigned int ball_indices[6] = 
    {
	0, 1, 2, // first triangle
	1, 2, 3  // second triangle 
    };

    unsigned int ball_texture;

// Create player bar
    VertexArray player_array_object;
    VertexBuffer player_buffer_object(player_paddle, sizeof(player_paddle));
    IndexBuffer player_index_object(player_indices, sizeof(player_indices));

    player_array_object.set_attribute_pointer(0, 3, 6, 0); // vertex coords
    player_array_object.set_attribute_pointer(1, 3, 6, 3); // color

// Create ball
    VertexArray ball_array_object;
    VertexBuffer ball_buffer_object(ball, sizeof(ball));
    IndexBuffer ball_index_object(ball_indices, sizeof(ball_indices));

    ball_array_object.set_attribute_pointer(0, 3, 8, 0); // vertex coords
    ball_array_object.set_attribute_pointer(1, 3, 8, 3); // color
    ball_array_object.set_attribute_pointer(2, 2, 8, 6); // texture coords

// Initializing ball's texture
    int width, height, nr_channels;
    unsigned char *data = stbi_load("textures/ball.png", &width, &height, &nr_channels, 0);

    glGenTextures(1, &ball_texture);
    glBindTexture(GL_TEXTURE_2D, ball_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
	std::cout << "Failed to load"  << std::endl;
    }
    stbi_image_free(data);

    Shader player_shader_handler("shaders/vertex_shader.shr", "shaders/fragment_shader.shr");
    Shader ball_shader_handler("shaders/vertex_shader_ball.shr", "shaders/fragment_shader_ball.shr");

    unsigned int number_of_bounces = 0;
    while (!glfwWindowShouldClose(window))
    {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

// Left player paddle  movement
	player_shader_handler.use();
	player_array_object.bind_buffer();
	glm::mat4 trans_left;
	if (movement_left + move_dir_left + 0.2f >= 1.0f)
	{
	    movement_left = 1.0f - 0.20f;
	}
	else if (movement_left + move_dir_left - 0.2f <= -1.0f)
	{
	    movement_left = -1.0f + 0.20f;
	}
	else
	{
	    movement_left += move_dir_left;
	}
	trans_left = glm::translate(trans_left, glm::vec3(-0.9f, movement_left, 0.0f));
        player_shader_handler.set_matrix("transform", trans_left);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// Right player paddle  movement
	glm::mat4 trans_right;
	if (movement_right + move_dir_right + 0.20f >= 1.0f)
	{
	    movement_right = 1.0f - 0.20f;
	}
	else if (movement_right + move_dir_right - 0.20f  <= -1.0f)
	{
	    movement_right = -1.0f + 0.20f;
	}
	else
	{
	    movement_right += move_dir_right;
	}
	trans_right = glm::translate(trans_right, glm::vec3(0.95f, movement_right, 0.0f));
	player_shader_handler.set_matrix("transform", trans_right);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// Ball's movement
	ball_shader_handler.use();
	ball_array_object.bind_buffer();
	glm::mat4 movement;

	if (movementX + velocityX >= 1.0f)
	{
	    movementX = 0.0f;
	    signX = false;
	    left_score++;
	    std::cout << "Remaining score " << left_score << ":" << right_score << std::endl;
	    velocityX = 0.02f;
	}
	else if (movementX - velocityX <= -1.0f)
	{
	    movementX = 0.0f;
	    signX = true;
	    right_score++;
	    std::cout << "Remaining score " << left_score << ":" << right_score << std::endl;
	    velocityX = 0.02f;
	}
	else
	{
	    if (signX)
	    {
		movementX += velocityX;
	    }
	    else
	    {
		movementX -= velocityX;
	    }
	}
	
	if (movementY + velocityY >= 1.0f)
	{
	    movementY -= velocityY;
	    signY = false;
	    if (velocityY == 0.02f)
	    {
		velocityY += 0.04f;
	    }
	    else
	    {
		velocityY -= 0.04f;
	    }
	}
	else if (movementY - velocityY <= -1.0f)
	{
	    movementY += velocityY;
	    signY = true;
	    if (velocityY == 0.02f)
	    {
		velocityY += 0.04f;
	    }
	    else
	    {
		velocityY -= 0.04f;
	    }
	}
	else
	{
	    if (!left_center || !right_center)
	    {
		if (signY)
		{
		    movementY += velocityY;
		}
		else
		{
		    movementY -= velocityY;
		}
	    }
	}
	
// Ball's collision
	if (movementX - 0.04f <= -0.9f &&
	    movementX - 0.04f > -0.95f &&
	    movementY <= movement_left + 0.2f &&
	    movementY >= movement_left - 0.2f)
	{
	    if (movementY <= movement_left + 0.2f && movementY > movement_left + 0.05f)
	    {
		signY = true;
		left_center = false;
	    }
	    else if (movementY >= movement_left - 0.2f && movementY < movement_left - 0.05f)
	    {
		signY = false;
		left_center = false;
	    }
	    else
	    {
		left_center = true;
	    }
	    signX = !signX;
	    number_of_bounces++;
	}
	else if (movementX + 0.04f >= 0.9f &&
		 movementX + 0.04f > 0.95f &&
		 movementY <= movement_right + 0.2f &&
		 movementY >= movement_right - 0.2f)
	{
	    if (movementY <= movement_right + 0.2f && movementY > movement_right + 0.05f)
	    {
		signY = true;
		right_center = false;
	    }
	    else if (movementY >= movement_right - 0.2f && movementY < movement_right - 0.05f)
	    {
		signY = false;
		right_center = false;
	    }
	    else
	    {
		right_center = true;			       
	    }
	    signX = !signX;
	    number_of_bounces++;
	}

	if (movementX == 0)
	{
	    movementY = 0;
	}
	if (number_of_bounces == 5)
	{
	    number_of_bounces = 0;
	    velocityX += 0.001f;
	}

	movement = glm::translate(movement, glm::vec3(movementX, movementY, 0.0f));
	ball_shader_handler.set_matrix("movement", movement);
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
		move_dir_left += 0.1f;
	    }
	    else if (action == GLFW_RELEASE)
	    {
		move_dir_left -= 0.1f;
	    }
	}break;

	case GLFW_KEY_S:
	{
	    if (action == GLFW_PRESS)
	    {
		move_dir_left -= 0.1f;       
	    }
	    else if (action == GLFW_RELEASE)
	    {
		move_dir_left += 0.1f;
	    }
	}break;

	case GLFW_KEY_UP:
	{
	    if (action == GLFW_PRESS)
	    {
		move_dir_right += 0.1f;
	    }
	    else if (action == GLFW_RELEASE)
	    {
		move_dir_right -= 0.1f;
	    }
	}break;

	case GLFW_KEY_DOWN:
	{
	    if (action == GLFW_PRESS)
	    {
		move_dir_right -= 0.1f;
	    }
	    else if (action == GLFW_RELEASE)
	    {
		move_dir_right += 0.1f;
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


