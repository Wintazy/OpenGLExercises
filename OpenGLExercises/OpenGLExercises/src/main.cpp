#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <iostream>

#include "Shader\ShadersLoader.h"
#include "Utilities\stbImageLoader\stb_image.h"
#include "Camera.h"

const GLchar* VERTEX_SHADER_PATH = "../Data/Shaders/Normal.vs";
const GLchar* TRANSFORM_VERTEX_SHADER_PATH = "../Data/Shaders/Transform.vs";
const GLchar* TRANSFORM_2D_3D_VERTEX_SHADER_PATH = "../Data/Shaders/2dTo3d.vs";
const GLchar* FADED_SHADER_PATH = "../Data/Shaders/SlowlyFaded.fs";
const GLchar* FRAGMENT_SHADER_PATH = "../Data/Shaders/ApplyTexture.fs";
const GLchar* CONTAINER_TEXTURE_PATH = "../Data/Textures/container.jpg";
const GLchar* FACE_TEXTURE_PATH = "../Data/Textures/awesomeface.png";
const GLchar* LIGHTING_SHADER_PATH = "../Data/Shaders/Lighting.fs";
const GLchar* LIGHTING_SOURCE_SHADER_PATH = "../Data/Shaders/LightingSource.fs";

float lastTime = 0.0f;
float deltaTime = 0.0f;

float lastMousePosX = SCREEN_WIDTH / 2;
float lastMousePosY = SCREEN_HEIGHT / 2;

void onFrameBufferSizeChanged(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void onMousePosChanged(GLFWwindow *window, double posX, double posY);
void onMouseScrolled(GLFWwindow *window, double offsetX, double offsetY);

Camera* camera = new Camera();

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
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "HelloOpenGL", NULL, NULL);
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
	glfwSetFramebufferSizeCallback(window, onFrameBufferSizeChanged);

	ShadersLoader shadersLoader = ShadersLoader();
	shadersLoader.LoadShaders(TRANSFORM_2D_3D_VERTEX_SHADER_PATH, LIGHTING_SHADER_PATH);

	//prepare vertex data to render
	float vertices[] = {
		//Positions        //Norm vec
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, //0
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, //1
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, //2
		
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, //0
		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, //2
		-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, //3
														 
		-0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, //0
		0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, //1
		0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, //5
														
		-0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, //0
		-0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, //4
		0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, //5
														 
		0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //2
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //3
		-0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, //7
													
		0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, //2
		0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, //6
		-0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, //7
														
		-0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, //4
		0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, //5
		-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, //7
														
		0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, //5
		0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, //6
		-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, //7
													
		-0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, //0
		-0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, //3
		-0.5f, 0.5f, 1.0f, -1.0f, 0.0f, 0.0f, //11
														
		- 0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, //0
		-0.5f, -0.5f, 1.0f, -1.0f, 0.0f, 0.0f, //8
		-0.5f, 0.5f, 1.0f, -1.0f, 0.0f, 0.0f, //11
		
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //1
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //2
		0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, //10

		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //1
		0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, //9
		0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f //10		
	};
	/*
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 1, 5,
		0, 4, 5,
		2, 3, 7,
		2, 6, 7,
		4, 5, 7,
		5, 6, 7,

		0, 3, 11,
		0, 8, 11,
		1, 2, 10,
		1, 9, 10
	};*/

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	//Bind vertext buffer and set buffer data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	/*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	//Tell OpenGL how the hell it should read the vertex attributes data
	//Here is position attribute: the vertex attribute at location 0, have 3 values, type is FLOAT, shouldn't normalize,
	//size of the stride to next vertex atrribute set is 6*sizeof(float),
	//and offset of where the data begins in the buffer is 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//Remember to enable these attribute at that location before render
	glEnableVertexAttribArray(0);
	//Normal vec attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	/*
	//Same with color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);*/



	/*--Setup light source--*/
	ShadersLoader lightingShaderLoader = ShadersLoader();
	lightingShaderLoader.LoadShaders(TRANSFORM_2D_3D_VERTEX_SHADER_PATH, LIGHTING_SOURCE_SHADER_PATH);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	lightingShaderLoader.EnableShaderProgram();
	glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	/*----------------------*/

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
		std::cout << "Failed to load texture: " << CONTAINER_TEXTURE_PATH << std::endl;
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
		std::cout << "Failed to load texture: " << FACE_TEXTURE_PATH << std::endl;
	}
	//Clean up
	stbi_image_free(textureData);
	/*---Load texture end---*/

	//Activate shader before setting uniform
	shadersLoader.EnableShaderProgram();
	shadersLoader.SetVec3f("objectColor", glm::value_ptr(objectColor));
	shadersLoader.SetVec3f("lightColor", glm::value_ptr(lightColor));

	shadersLoader.SetInt("customTexture1", 0);
	shadersLoader.SetInt("customTexture2", 1);

	//Clones positions
	glm::vec3 clonePositions[] = {
		glm::vec3(0.0f,  0.0f, 0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	/*------ Further openGL config -------*/
	//Optional: Enable blending to correct alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Enable depth buffer
	glEnable(GL_DEPTH_TEST);
	//Setup input mode
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, lastMousePosX, lastMousePosY);
	//Handle mouse input
	glfwSetCursorPosCallback(window, onMousePosChanged);
	glfwSetScrollCallback(window, onMouseScrolled);
	/*------------------------- -------*/

	//Render loop - keep running till window should stop
	while (!glfwWindowShouldClose(window))
	{
		//Handle all user input
		processInput(window);

		/*---Rendering part---*/
		//Set color to clear screen with
		glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
		//Clear color buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Update uniform color
		shadersLoader.EnableShaderProgram();
		float currentTime = glfwGetTime();
		float alpha = sin(currentTime) / 2.0f + 0.5f;
		shadersLoader.SetFloat("customAlpha", alpha);

		//Update delta time for input handle
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;		


		glm::vec3 lightPos(sin(currentTime) * 3.0f, 1.0f, cos(currentTime) * 3.0f);
		//Draw triangle part
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, containerTextureId);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, faceTextureId);		

		//Draw objects
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//Apply transform matrix
		//shadersLoader.SetMat4f("transformMat", glm::value_ptr(transMat));
		shadersLoader.SetMat4f("viewMat", glm::value_ptr(camera->GetViewMat()));
		shadersLoader.SetMat4f("projectionMat", glm::value_ptr(camera->GetProjectionMat()));
		shadersLoader.SetVec3f("lightPos", glm::value_ptr(lightPos));
		shadersLoader.SetVec3f("viewPos", glm::value_ptr(camera->GetViewPos()));
		int clonesNumber = sizeof(clonePositions) / sizeof(*clonePositions);
		for (unsigned int i = 0; i < clonesNumber; i++)
		{
			glm::mat4 modelMat;
			modelMat = glm::translate(modelMat, clonePositions[i]);
			//modelMat = glm::rotate(modelMat, (i + 1) * currentTime * glm::radians(30.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			shadersLoader.SetMat4f("modelMat", glm::value_ptr(modelMat));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//Draw light source
		
		lightingShaderLoader.EnableShaderProgram();

		glm::mat4 projection = glm::perspective(glm::radians(LARGEST_FOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 modelMat = glm::mat4();
		glm::mat4 model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		lightingShaderLoader.SetMat4f("modelMat", glm::value_ptr(model));
		modelMat = glm::translate(modelMat, lightPos);
		modelMat = glm::scale(modelMat, glm::vec3(0.2f));
		lightingShaderLoader.SetMat4f("modelMat", glm::value_ptr(modelMat));
		lightingShaderLoader.SetMat4f("viewMat", glm::value_ptr(camera->GetViewMat()));
		lightingShaderLoader.SetMat4f("projectionMat", glm::value_ptr(projection));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		/*---Rendering end---*/

		//Swap color buffer used as output to the screen
		glfwSwapBuffers(window);
		//Handle any events are triggered
		glfwPollEvents();
	}
	//--------------

	//Clean up before exit
    glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
	glfwTerminate();

	//Application end
	return 0;
}

void onFrameBufferSizeChanged(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	float cameraSpeed = CAMERA_SPEED * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->MoveAlongDirection(cameraSpeed, camera->GetFrontDir());
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->MoveAlongDirection(-cameraSpeed, camera->GetFrontDir());
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->MoveAlongDirection(-cameraSpeed, camera->GetFrontCrossDir());
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->MoveAlongDirection(cameraSpeed, camera->GetFrontCrossDir());
	}
}

void onMousePosChanged(GLFWwindow *window, double posX, double posY)
{
	float offsetX = (posX - lastMousePosX) * MOUSE_SENSITIVITY;
	float offsetY = (lastMousePosY - posY) * MOUSE_SENSITIVITY;
	lastMousePosX = posX;
	lastMousePosY = posY;

	camera->UpdateAngles(offsetX, offsetY);
}

void onMouseScrolled(GLFWwindow *window, double offetX, double offsetY)
{
	camera->UpdateFOV(offsetY);
}