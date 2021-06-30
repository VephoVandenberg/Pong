#define GLEW_STATIC

#include "src/shader_handler.h"
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
    
    float player_bar[24] =
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

    unsigned int VAOs[2];
    unsigned int player_index_object, player_buffer_object;
    unsigned int ball_index_object, ball_buffer_object;
    unsigned int ball_texture;
    
    glGenVertexArrays(2, VAOs);
// Create player bar
    glBindVertexArray(VAOs[0]);
    glGenBuffers(1, &player_buffer_object);
    glGenBuffers(1, &player_index_object);
    
    glBindBuffer(GL_ARRAY_BUFFER, player_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(player_bar), player_bar, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, player_index_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(player_indices), player_indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

// Create ball
    glBindVertexArray(VAOs[1]);
    glGenBuffers(1, &ball_buffer_object);
    glGenBuffers(1, &ball_index_object);

    glBindBuffer(GL_ARRAY_BUFFER, ball_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ball), ball, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ball_index_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ball_indices), ball_indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    
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

// Left player bar movement
	player_shader_handler.use();
	glBindVertexArray(VAOs[0]);
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
// Right player bar movement
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
	glBindVertexArray(VAOs[1]);

	
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
		velocityY += 0.02f;
	    }
	    else
	    {
		velocityY -= 0.02f;
	    }
	}
	else if (movementY - velocityY <= -1.0f)
	{
	    movementY += velocityY;
	    signY = true;
	    if (velocityY == 0.02f)
	    {
		velocityY += 0.02f;
	    }
	    else
	    {
		velocityY -= 0.02f;
	    }
	}
	else
	{
	    if (!right_center || !left_center)
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
	    else
	    {
		std::cout << "Center" << std::endl;
	    }
	}
	
// Ball's collision
	if (movementX - 0.04f <= -0.9f &&
	    movementY <= movement_left + 0.2f &&
	    movementY >= movement_left - 0.2f)
	{
	    if (movementY <= movement_left + 0.2f && movementY > movement_left)
	    {
		signY = true;
		left_center = false;
	    }
	    else if (movementY == movement_left)
	    {
		left_center = true;
	    }
	    else
	    {
		signY = false;
		left_center = false;
	    }
	    signX = !signX;
	    number_of_bounces++;
	}
	else if (movementX + 0.04f >= 0.9f &&
		 movementY <= movement_right + 0.2f &&
		 movementY >= movement_right - 0.2f)
	{
	    if (movementY <= movement_right + 0.2f && movementY > movement_right)
	    {
		signY = true;
		right_center = false;
	    }
	    else if (movementY == movement_right)
	    {
		right_center = true;
	    }
	    else
	    {
		signY = false;
		right_center = false;
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


