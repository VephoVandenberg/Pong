<<<<<<< HEAD
cl main.cpp src/*cpp ^
=======
cl main.cpp src/*.cpp ^
>>>>>>> 0ef57c89242c316278463bf32479dc2a24f80a64
   -I dependencies/GLFW/include -I dependencies/GLEW/include -I dependencies/glm ^
   -link dependencies/GLFW/lib/glfw3dll.lib dependencies/GLEW/lib/glew32s.lib ^
   opengl32.lib user32.lib gdi32.lib
   
