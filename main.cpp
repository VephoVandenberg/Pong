#define GLEW_STATIC

#include "src/shader_handler.h"
#include "src/vertex_buffer_handler.h"
#include "src/index_buffer_handler.h"
#include "src/vertex_array_handler.h"
#include "src/game_entities.h"
#include "src/stb_image_loader.h"

#include <GLFW/glfw3.h>

const unsigned int screen_width = 800;
const unsigned int screen_height = 500;

static paddle_movement left_paddle_move = {0.0f, 0.0f};
static paddle_movement right_paddle_move = {0.0f, 0.0f};
static ball_movement ball_move;
static game_entity game = {0, 0};

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

    ball_move.movementX = 0.2f;
    ball_move.movementY = -0.1f;

    ball_move.velocityX = 0.02f;
    ball_move.velocityY = 0.02f;

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
	if (left_paddle_move.movement + left_paddle_move.movement_dir + 0.2f >= 1.0f)
	{
	    left_paddle_move.movement = 1.0f - 0.20f;
	}
	else if (left_paddle_move.movement + left_paddle_move.movement_dir - 0.2f <= -1.0f)
	{
	    left_paddle_move.movement = -1.0f + 0.20f;
	}
	else
	{
	    left_paddle_move.movement += left_paddle_move.movement_dir;
	}
	trans_left = glm::translate(trans_left, glm::vec3(-0.9f, left_paddle_move.movement, 0.0f));
        player_shader_handler.set_matrix("transform", trans_left);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// Right player paddle  movement
	glm::mat4 trans_right;
	if (right_paddle_move.movement + right_paddle_move.movement_dir + 0.20f >= 1.0f)
	{
	    right_paddle_move.movement = 1.0f - 0.20f;
	}
	else if (right_paddle_move.movement + right_paddle_move.movement_dir - 0.20f  <= -1.0f)
	{
	    right_paddle_move.movement = -1.0f + 0.20f;
	}
	else
	{
	    right_paddle_move.movement += right_paddle_move.movement_dir;
	}
	trans_right = glm::translate(trans_right, glm::vec3(0.95f, right_paddle_move.movement, 0.0f));
	player_shader_handler.set_matrix("transform", trans_right);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// Ball's movement
	ball_shader_handler.use();
	ball_array_object.bind_buffer();
	glm::mat4 movement;

	if (ball_move.movementX + ball_move.velocityX >= 1.0f)
	{
	    ball_move.movementX = 0.0f;
	    ball_move.signX = false;
	    game.left_score++;
	    std::cout << "Remaining score " << game.left_score << ":" << game.right_score << std::endl;
	    ball_move.velocityX = 0.02f;
	}
	else if (ball_move.movementX - ball_move.velocityX <= -1.0f)
	{
	    ball_move.movementX = 0.0f;
	    ball_move.signX = true;
	    game.right_score++;
	    std::cout << "Remaining score " << game.left_score << ":" << game.right_score << std::endl;
	    ball_move.velocityX = 0.02f;
	}
	else
	{
	    if (ball_move.signX)
	    {
		ball_move.movementX += ball_move.velocityX;
	    }
	    else
	    {
		ball_move.movementX -= ball_move.velocityX;
	    }
	}
	
	if (ball_move.movementY + ball_move.velocityY >= 1.0f)
	{
	    ball_move.movementY -= ball_move.velocityY;
	    ball_move.signY = false;
	    if (ball_move.velocityY == 0.02f)
	    {
		ball_move.velocityY += 0.04f;
	    }
	    else
	    {
		ball_move.velocityY -= 0.04f;
	    }
	}
	else if (ball_move.movementY - ball_move.velocityY <= -1.0f)
	{
	    ball_move.movementY += ball_move.velocityY;
	    ball_move.signY = true;
	    if (ball_move.velocityY == 0.02f)
	    {
		ball_move.velocityY += 0.04f;
	    }
	    else
	    {
		ball_move.velocityY -= 0.04f;
	    }
	}
	else
	{
	    if (!ball_move.left_center || !ball_move.right_center)
	    {
		if (ball_move.signY)
		{
		    ball_move.movementY += ball_move.velocityY;
		}
		else
		{
		    ball_move.movementY -= ball_move.velocityY;
		}
	    }
	}
	
// Ball's collision
	if (ball_move.movementX - 0.04f <= -0.9f &&
	    ball_move.movementX - 0.04f > -0.95f &&
	    ball_move.movementY <= left_paddle_move.movement + 0.2f &&
	    ball_move.movementY >= left_paddle_move.movement - 0.2f)
	{
	    if (ball_move.movementY <= left_paddle_move.movement + 0.2f &&
		ball_move.movementY > left_paddle_move.movement + 0.05f)
	    {
		ball_move.signY = true;
		ball_move.left_center = false;
	    }
	    else if (ball_move.movementY >= left_paddle_move.movement - 0.2f
		     && ball_move.movementY < left_paddle_move.movement - 0.05f)
	    {
		ball_move.signY = false;
		ball_move.left_center = false;
	    }
	    else
	    {
		ball_move.left_center = true;
	    }
	    ball_move.signX = !ball_move.signX;
	    number_of_bounces++;
	}
	else if (ball_move.movementX + 0.04f >= 0.9f &&
		 ball_move.movementX + 0.04f > 0.95f &&
		 ball_move.movementY <= right_paddle_move.movement + 0.2f &&
		 ball_move.movementY >= right_paddle_move.movement - 0.2f)
	{
	    if (ball_move.movementY <= right_paddle_move.movement + 0.2f &&
		ball_move.movementY > right_paddle_move.movement + 0.05f)
	    {
		ball_move.signY = true;
		ball_move.right_center = false;
	    }
	    else if (ball_move.movementY >= right_paddle_move.movement - 0.2f &&
		     ball_move.movementY < right_paddle_move.movement - 0.05f)
	    {
		ball_move.signY = false;
		ball_move.right_center = false;
	    }
	    else
	    {
		ball_move.right_center = true;			       
	    }
	    ball_move.signX = !ball_move.signX;
	    number_of_bounces++;
	}

	if (ball_move.movementX == 0)
	{
	    ball_move.movementY = 0;
	}
	if (number_of_bounces == 5)
	{
	    number_of_bounces = 0;
	    ball_move.velocityX += 0.001f;
	}

	movement = glm::translate(movement, glm::vec3(ball_move.movementX, ball_move.movementY, 0.0f));
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
		left_paddle_move.movement_dir += 0.1f;
	    }
	    else if (action == GLFW_RELEASE)
	    {
		left_paddle_move.movement_dir -= 0.1f;
	    }
	}break;

	case GLFW_KEY_S:
	{
	    if (action == GLFW_PRESS)
	    {
	        left_paddle_move.movement_dir -= 0.1f;       
	    }
	    else if (action == GLFW_RELEASE)
	    {
	        left_paddle_move.movement_dir += 0.1f;
	    }
	}break;

	case GLFW_KEY_UP:
	{
	    if (action == GLFW_PRESS)
	    {
		right_paddle_move.movement_dir += 0.1f;
	    }
	    else if (action == GLFW_RELEASE)
	    {
	        right_paddle_move.movement_dir -= 0.1f;
	    }
	}break;

	case GLFW_KEY_DOWN:
	{
	    if (action == GLFW_PRESS)
	    {
	        right_paddle_move.movement_dir -= 0.1f;
	    }
	    else if (action == GLFW_RELEASE)
	    {
	        right_paddle_move.movement_dir += 0.1f;
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


