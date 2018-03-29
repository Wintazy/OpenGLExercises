#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <iostream>

#include "Shader\ShadersLoader.h"
#include "Utilities\stbImageLoader\stb_image.h"

const GLchar* VERTEX_SHADER_PATH = "../Data/Shaders/Normal.vs";
const GLchar* FRAGMENT_SHADER_PATH = "../Data/Shaders/SlowlyFaded.fs";
const GLchar* CONTAINER_TEXTURE_PATH = "../Data/Textures/container.jpg";
const GLchar* FACE_TEXTURE_PATH = "../Data/Textures/awesomeface.png";

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Register callback function handle window's buffer size changes
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	ShadersLoader shadersLoader = ShadersLoader();
	shadersLoader.LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	//prepare vertex data to render
	float vertices[] = {
		//Positions       Colors             Texture coords
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
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
	//Here is position attribute: the vertex attribute at location 0, have 3 values, type is FLOAT, shouldn't normalize,
	//size of the stride to next vertex atrribute set is 6*sizeof(float),
	//and offset of where the data begins in the buffer is 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//Remember to enable these attribute at that location before render
	glEnableVertexAttribArray(0);

	//Same with color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Buffer could be Unbound after the attributes were registered
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//So is the vertex array
	glBindVertexArray(0);

	/*---Load texture---*/
	unsigned int containerTextureId;
	glGenTextures(1, &containerTextureId);
	glBindTexture(GL_TEXTURE_2D, containerTextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Load texture from file
	int imgWidth, imgHeight, colorChannelsNum;

	//Prevent flip upside-down image, as different in coordinates system
	stbi_set_flip_vertically_on_load(true);

	unsigned char* textureData = stbi_load(CONTAINER_TEXTURE_PATH, &imgWidth, &imgHeight, &colorChannelsNum, 0);
	if (textureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::wcout << "Failed to load texture: " << CONTAINER_TEXTURE_PATH << std::endl;
	}
	//Load Face texture
	unsigned int faceTextureId;
	glGenTextures(1, &faceTextureId);
	glBindTexture(GL_TEXTURE_2D, faceTextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	textureData = stbi_load(FACE_TEXTURE_PATH, &imgWidth, &imgHeight, &colorChannelsNum, 0);
	if (textureData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::wcout << "Failed to load texture: " << FACE_TEXTURE_PATH << std::endl;
	}
	//Clean up
	stbi_image_free(textureData);
	/*---Load texture end---*/

	//Activate shader before setting uniform
	shadersLoader.EnableShaderProgram();

	shadersLoader.SetInt("customTexture1", 0);
	shadersLoader.SetInt("customTexture2", 1);

	//Optional: Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

		//Update uniform color
		float time = glfwGetTime();
		float alpha = sin(time) / 2.0f + 0.5f;
		shadersLoader.SetFloat("customAlpha", alpha);

		//Draw triangle part
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, containerTextureId);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, faceTextureId);

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