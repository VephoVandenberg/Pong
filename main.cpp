#include "src/game.h"

#define M_PI 3.1415926535897932

const unsigned int screen_width = 800;
const unsigned int screen_height = 800;

static paddle_movement left_paddle_move  = {0.0f, 0.0f};
static paddle_movement right_paddle_move = {0.0f, 0.0f};
static game_entity 	   game_scores       = {0, 0};
static ball_movement   ball_move;

void key_callback(GLFWwindow *window,
		  int key,
		  int scancode,
		  int action,
		  int mods);

void draw_circle(float x, float y, float radius,
		 int number_of_segments,
		 VertexArray &vao,
		 VertexBuffer &vbo,
		 float red, float green, float blue);

int main(int argc, char **argv)
{
	game game_object;
	/*
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

    
    ball_move.movementX = 0.2f;
    ball_move.movementY = -0.1f;

    ball_move.velocityX = 0.02f;
    ball_move.velocityY = 0.02f;

// Create player bar
    VertexArray player_array_object;
    VertexBuffer player_buffer_object(player_paddle, sizeof(player_paddle));
    IndexBuffer player_index_object(player_indices, sizeof(player_indices));

    player_array_object.set_attribute_pointer(0, 3, 6, 0); // vertex coords
    player_array_object.set_attribute_pointer(1, 3, 6, 3); // color

// Create ball
    
    VertexArray ball_array_object;
    VertexBuffer ball_buffer_object;

    unsigned int number_of_segments = 40;

    draw_circle(0.0f, 0.0f, 0.04f, number_of_segments,
		ball_array_object, ball_buffer_object,
		1.0f, 1.0f, 1.0f);

// Creating shaders for paddles and ball as well
    Shader player_shader_handler("shaders/vertex_shader.vert", "shaders/fragment_shader.frag");
    Shader ball_shader_handler("shaders/vertex_shader_ball.vert", "shaders/fragment_shader_ball.frag");
	*/
    //unsigned int number_of_bounces = 0;
    while (!game_object.is_finished())
    {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		game_object.handle_left_paddle();
		game_object.handle_right_padle();
		game_object.move_ball();
		game_object.collide_ball();
		game_object.swap_buffers();
		glfwPollEvents();
		
		/*
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
		
	// Ball's collision
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
		ball_shader_handler.set_matrix("movement", movement);
		*/
		//glDrawArrays(GL_TRIANGLE_FAN, 0, number_of_segments + 2);
    }
    
    return 0;
}
/*
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


void draw_circle(float x, float y, float radius,
		 int number_of_segments,
		 VertexArray &vao,
		 VertexBuffer &vbo,
		 float red, float green, float blue)
{
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

    vao.bind_buffer();
    vbo.bind_buffer();

    vbo.write_data(vertices, 6*number_of_vertices*sizeof(float));
    
    vao.set_attribute_pointer(0, 3, 6, 0);
    vao.set_attribute_pointer(1, 3, 6, 3);

    delete [] vertices;
}
*/