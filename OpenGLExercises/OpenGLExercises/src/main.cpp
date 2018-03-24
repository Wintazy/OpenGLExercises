#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

void frame_buffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	//Init GLFW
	glfwInit();
	//Configure OpenGL options
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //This make glfwCrateWindow fail on my PC at home... TODO: Investigate this
	//-------------

	//Create window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "HelloOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//-------------

	//Init GLAD before calling any OpenGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//-------------

	//Create a viewport to render
	glViewport(0, 0, 800, 600);
	//Register callback function handle window's buffer size changes
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	//Render loop - keep running till window should stop
	while (!glfwWindowShouldClose(window))
	{
		//Handle all user input
		processInput(window);

		/*---Rendering part---*/
		//Set color to clear screen with
		glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
		//Clear color buffer
		glClear(GL_COLOR_BUFFER_BIT);
		/*---Rendering end---*/

		//Swap color buffer used as output to the screen
		glfwSwapBuffers(window);
		//Handle any events are triggered
		glfwPollEvents();
	}
	//--------------

	//Clean up before exit
	glfwTerminate();

	//Application end
	return 0;
}