A simple ping-pong game implemented for self-education purposes.

Code organization
The main game logic init/update/render is implemented in Game.cpp.

Class game_object represents a single game entity, in our case a ball or the players.

Class Ball represents the ball object in the game with its functions to move, bounce off walls and players' paddles and reset on goal.

The drawing functions are implemented in sprite_renderer.cpp.

The logic behind the configuration of our shader program is in Shader.cpp as well as some utility methods for our shaders.

The window initialization and main loop is implemented in Main.cpp.


Building
The repository is a project for Visual Studio 2019 which contains all necessary dependencies. If you have problem loading the project in Visual Studio, you can try building with CMake.

Credits
http://learnopengl.com was amazingly helpful to learn basics of OpenGL. Some utility code was based on samples from there.
