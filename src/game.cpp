#include "game.h"

#define M_PI 3.1415926535897932

paddle_movement game::left_paddle_move  = {0.0f, 0.0f};
paddle_movement game::right_paddle_move = {0.0f, 0.0f};

game::game(GLFWwindow *window_arg)
{
	ball_move.movementX = 0.2f;
    ball_move.movementY = -0.1f;

    ball_move.velocityX = 0.02f;
    ball_move.velocityY = 0.02f;

	if (window_arg)
	{
		window = window_arg;
	}
	glfwSetKeyCallback(window, key_callback);
}

game::~game(void)
{

}

void game::create_player()
{
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
 	
 	player_array_object.bind_buffer();
    player_buffer_object.write_data(player_paddle, sizeof(player_paddle));
    player_index_object.write_data(player_indices, sizeof(player_indices));
    player_array_object.set_attribute_pointer(0, 3, 6, 0); // vertex coords
    player_array_object.set_attribute_pointer(1, 3, 6, 3); // color

}

void game::create_ball(float x, float y, float radius,
		 			   int number_of_segs,
		 			   float red, float green, float blue)
{
	number_of_segments = number_of_segs;
	int number_of_vertices = 3 + number_of_segments - 1;
    float twice_pi = 2.0f * M_PI;

    float *vertices = new float[6*number_of_vertices];

    vertices[0] = x;
    vertices[1] = y;
    vertices[2] = 0.0f;

    vertices[3] = red;
    vertices[4] = blue;
    vertices[5] = green;

    for (int i = 1; i < number_of_vertices; i++)
    {
		vertices[6*i] = x + radius*cos(i*twice_pi/number_of_segments);
		vertices[6*i + 1] = y + radius*sin(i*twice_pi/number_of_segments);
		vertices[6*i + 2] = 0.0f;

		vertices[6*i + 3] = red;
		vertices[6*i + 4] = blue;
		vertices[6*i + 5] = green;
    }

    ball_array_object.bind_buffer();
    ball_buffer_object.bind_buffer();

    ball_buffer_object.write_data(vertices, 6*number_of_vertices*sizeof(float));
    
    ball_array_object.set_attribute_pointer(0, 3, 6, 0);
    ball_array_object.set_attribute_pointer(1, 3, 6, 3);

    delete [] vertices;
}


void game::create_shaders(const char *paddle_vertex_shader,
						  const char *paddle_fragment_shader,
					      const char *ball_vertex_shader,
					      const char *ball_fragment_shader)
{
	player_shader_handler = new Shader(paddle_vertex_shader, paddle_fragment_shader);
	ball_shader_handler	  = new Shader(ball_vertex_shader, ball_fragment_shader);
}

void game::handle_left_paddle()
{
	player_shader_handler->use();
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
	       player_shader_handler->set_matrix("transform", trans_left);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void game::handle_right_padle(void)
{
	player_shader_handler->use();
	player_array_object.bind_buffer();
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
	player_shader_handler->set_matrix("transform", trans_right);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void game::move_ball(void)
{
	ball_shader_handler->use();
	ball_array_object.bind_buffer();

	if (ball_move.movementX + ball_move.velocityX >= 1.0f)
	{
	    ball_move.movementX = 0.0f;
	    ball_move.signX = false;
	    game_scores.left_score++;
	    std::cout << "Remaining score " << game_scores.left_score << ":" << game_scores.right_score << std::endl;
	    ball_move.velocityX = 0.02f;
	}
	else if (ball_move.movementX - ball_move.velocityX <= -1.0f)
	{
	    ball_move.movementX = 0.0f;
	    ball_move.signX = true;
	    game_scores.right_score++;
	    std::cout << "Remaining score " << game_scores.left_score << ":" << game_scores.right_score << std::endl;
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
}

void game::collide_ball(void)
{
	glm::mat4 movement;

	if (ball_move.movementX - 0.04f <= -0.90f &&
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
	else if (ball_move.movementX + 0.04f >= 0.90f &&
		 ball_move.movementX + 0.04f < 0.95f &&
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
	ball_shader_handler->set_matrix("movement", movement);
	
	glDrawArrays(GL_TRIANGLE_FAN, 0, number_of_segments + 2);
}

GLFWwindow *game::get_window(void)
{
	return window;
}

void game::key_callback(GLFWwindow *window,
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
