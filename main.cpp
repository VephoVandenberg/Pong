#include "src/game.h"

const unsigned int screen_width = 800;
const unsigned int screen_height = 800;

int main(int argc, char **argv)
{	
	if (!glfwInit())
    {
    	std::cout << "Could not initialize GLFW" << std::endl;
    }
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    
    GLFWwindow *window = glfwCreateWindow(screen_width,
							  screen_height,
				    		  "Pong",
			    			  0,
							  0);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    if (glewInit() != GLEW_OK)
    {
    	std::cout << "Could not initialize GLEW" << std::endl;
    }	
	game game_object(window);
    
    game_object.create_player();
    std::cout << "OK";
    game_object.create_ball(0.0f, 0.0f, 0.04f, 40,
							1.0f, 1.0f, 1.0f);
    game_object.create_shaders("shaders/vertex_shader.vert", "shaders/fragment_shader.frag",
    							"shaders/vertex_shader_ball.vert", "shaders/fragment_shader_ball.frag");

    while (!glfwWindowShouldClose(game_object.get_window()))
    {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		game_object.handle_left_paddle();
		game_object.handle_right_padle();
		game_object.move_ball();
		game_object.collide_ball();
		glfwSwapBuffers(game_object.get_window());
		glfwPollEvents();
    }
    
    return 0;
}
