#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
int compileShader(const char* shaderSource, GLuint type);

const char *vertexShaderSource = "#version 330\n"
	"layout (location = 0)\n"
	"in vec3 aPos;\n"
	"out vec4 vertexColor;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"	vertexColor = vec4(0.4, 0.0, 0.0, 1.0);\n"
	"}\0";

const char *fragmentShaderSource = "#version 330\n"
	"uniform vec4 customColor;\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = customColor;\n"
	"}\0";

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

	//Set draw style to wireframe polygons
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Register callback function handle window's buffer size changes
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	//Compile shader
	int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
	int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
	//------

	//Link shader
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader( shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Failed to link shader program!\n" << infoLog << std::endl;
	}

	//Delete shader after linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	//-----

	//prepare vertex data to render
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//Bind vertext buffer and set buffer data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Tell OpenGL how the hell it should read the vertex attributes data
	//Here is: the vertext attribute at location 0, have 3 values, type is FLOAT, shouldn't normalize,
	//size of the stride to next vertex atrribute set is 3*sizeof(float),
	//and offset of where the data begins in the buffer is 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//Remember to enable these attribute at that location before render
	glEnableVertexAttribArray(0);

	//Buffer could be Unbound after the attributes were registered
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//So is the vertex array
	glBindVertexArray(0);

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

		//Activate shader
		glUseProgram(shaderProgram);
		//Update uniform color
		float time = glfwGetTime();
		float greenValue = sin(time) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "customColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		//Draw triangle part
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		/*---Rendering end---*/

		//Swap color buffer used as output to the screen
		glfwSwapBuffers(window);
		//Handle any events are triggered
		glfwPollEvents();
	}
	//--------------

	//Clean up before exit
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
	glfwTerminate();

	//Application end
	return 0;
}

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

int compileShader(const char* shaderSource, GLuint type)
{
	int shaderResult = glCreateShader(type);
	glShaderSource(shaderResult, 1, &shaderSource, NULL);
	glCompileShader(shaderResult);

	int success;
	char infoLog[512];
	glGetShaderiv(shaderResult, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderResult, 512, NULL, infoLog);
		std::cout << "Failed to compile shader!\n" << infoLog << std::endl;
		return -1;
	}

	return shaderResult;
}