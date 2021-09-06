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
	game(GLFWwindow *window_arg);
	~game(void);
	void create_player(void);
	void create_ball(float x, float y, float radius,
			 			   int number_of_segments,
			 			   float red, float green, float blue);
	void create_shaders(const char *paddle_vertex_shader,
							  const char *paddle_fragment_shader,
						      const char *ball_vertex_shader,
						      const char *ball_fragment_shader);
	void handle_left_paddle(void);
	void handle_right_padle(void);
	void move_ball(void);
	void collide_ball(void);
	GLFWwindow *get_window(void);
private:
	VertexArray  player_array_object;
	VertexBuffer player_buffer_object;
	IndexBuffer  player_index_object;
	Shader 		 *player_shader_handler = 0;

	VertexArray  ball_array_object;
	VertexBuffer ball_buffer_object;
	Shader    *ball_shader_handler = 0;
	int number_of_segments = 0;

	GLFWwindow *window = 0;

	game_entity 	game_scores = {0, 0};
	ball_movement   ball_move;
	
	static paddle_movement left_paddle_move;
	static paddle_movement right_paddle_move;
	
	unsigned int number_of_bounces = 0;
private:
	static void key_callback(GLFWwindow *window,
				  			   	   int key,
			  					   int scancode,
			  					   int action,
			  					   int mods);

	game(const game& game_instance);
	
};

#endif GAME_H