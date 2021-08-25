#ifndef GAME_H
#define GAME_H

#include "shader_handler.h"
#include "vertex_buffer_handler.h"
#include "index_buffer_handler.h"
#include "vertex_array_handler.h"
#include "game_entities.h"

#include <GLFW/glfw3.h>

class game
{
public:
	game::game(void);
	game::~game(void);
	void game::init_window(void);
	void game::create_player(void);
	void game::create_ball(float x, float y, float radius,
			 			   int number_of_segments,
			 			   float red, float green, float blue);
	void game::create_shaders(const char *paddle_vertex_shader,
							  const char *paddle_fragment_shader,
						      const char *ball_vertex_shader,
						      const char *ball_fragment_shader);
	void game::handle_left_paddle(void);
	void game::handle_right_padle(void);
	void game::move_ball(void);
	void game::collide(void);
	GLFWwindow *game::get_window(void);
private:
	VertexArray  player_array_object;
	VertexBuffer player_buffer_object;
	IndexBuffer  player_index_object;

	VertexArray  ball_array_object;
	VertexBuffer ball_buffer_object;

	Shader 		 *player_shader_handler = 0;
	Shader 		 *ball_shader_handler 	= 0;

	static paddle_movement left_paddle_move;
	static paddle_movement right_paddle_move;
	
	GLFWwindow *window 			 = 0;

	const int screen_height 	 = 800;
	const int screen_width		 = 800;
	const int number_of_segments = 40;
private:
	static void game::key_callback(GLFWwindow *window,
				  			   	   int key,
			  					   int scancode,
			  					   int action,
			  					   int mods);
};

#endif GAME_H