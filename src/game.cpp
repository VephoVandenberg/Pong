#include "game.h"

game::game(void)
{
	init_window();
	create_player();
	create_ball(0.0f, 0.0f, 0.04f, number_of_segments,
				1.0f, 1.0f, 1.0f);
	create_shaders("shaders/vertex_shader.vert", 
 					"shaders/fragment_shader.frag",
    				"shaders/vertex_shader_ball.vert",
    				"shaders/fragment_shader_ball.frag");
}

game::~game(void)
{

}

void game::init_window()
{
	if (!glfwInit())
    {
    	std::cout << "Could not initialize GLFW" << std::endl;
    }
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    window = glfwCreateWindow(screen_width,
					  screen_height,
					  "Pong",
					  0,
					  0);
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    if (glewInit() != GLEW_OK)
    {
    	std::cout << "Could not initialize GLEW" << std::endl;
    }
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
		 			   int number_of_segments,
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
	ball_shader_handler = new Shader(ball_vertex_shader, ball_fragment_shader);
}

void game::handle_left_paddle()
{

}

void game::handle_right_padle()
{

}

void game::move_ball()
{

}

void game::collide()
{

}

GLFWwindow *game::get_window()
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
