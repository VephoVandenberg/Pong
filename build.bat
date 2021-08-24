cl -DGLEW_STATIC main.cpp src/*.cpp ^
   -I dependencies/GLFW/include -I dependencies/GLEW/include -I dependencies/glm ^
   -link dependencies/GLFW/lib/glfw3dll.lib dependencies/GLEW/lib/glew32s.lib ^
   opengl32.lib user32.lib gdi32.lib
