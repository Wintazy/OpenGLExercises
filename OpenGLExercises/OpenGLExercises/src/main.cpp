#include <iostream>

#include <GLFW\glfw3.h>

#include "Model\Model.h"
#include "Camera.h"

const GLchar* NORMAL_VERTEX_SHADER_PATH = "../Data/Shaders/Normal.vs";
const GLchar* TRANSFORM_VERTEX_SHADER_PATH = "../Data/Shaders/Transform.vs";
const GLchar* TRANSFORM_2D_3D_VERTEX_SHADER_PATH = "../Data/Shaders/2dTo3d.vs";

const GLchar* FADED_SHADER_PATH = "../Data/Shaders/SlowlyFaded.fs";
const GLchar* TEXTURE_FRAGMENT_SHADER_PATH = "../Data/Shaders/ApplyTexture.fs";
const GLchar* LIGHTING_SHADER_PATH = "../Data/Shaders/Lighting.fs";
const GLchar* LIGHTING_SOURCE_SHADER_PATH = "../Data/Shaders/LightingSource.fs";
const GLchar* NORMAL_MODEL_SHADER_PATH = "../Data/Shaders/NormalModel.fs";

const GLchar* CONTAINER_TEXTURE_PATH = "../Data/Textures/container.jpg";
const GLchar* FACE_TEXTURE_PATH = "../Data/Textures/awesomeface.png";

const GLchar* CRATE_DIFFUSE_PATH = "../Data/LightingMaps/diffuse_wooden_crate.png";
const GLchar* CRATE_SPECULAR_PATH = "../Data/LightingMaps/specular_wooden_crate.png";

const char* NANO_SUIT_PATH = "../Data/Models/nanosuit/nanosuit.obj";
const char* AERITH_MODEL_PATH = "../Data/Models/crisis_core/Aerith.DAE";

float lastTime = 0.0f;
float deltaTime = 0.0f;

float lastMousePosX = SCREEN_WIDTH / 2;
float lastMousePosY = SCREEN_HEIGHT / 2;

void onFrameBufferSizeChanged(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void onMousePosChanged(GLFWwindow *window, double posX, double posY);
void onMouseScrolled(GLFWwindow *window, double offsetX, double offsetY);
unsigned int LoadTexture(const char * path);

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
	shadersLoader.LoadShaders(NORMAL_VERTEX_SHADER_PATH, TEXTURE_FRAGMENT_SHADER_PATH);

	/*----Load models data----*/
	//Model customModel = Model(AERITH_MODEL_PATH);
	/*-----------------*/
	//prepare vertex data to render
	float cubeVertices[] = {
		//Positions			//Tex coords
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //0
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, //1
		0.5f, 0.5f, 0.0f,	1.0f, 1.0f, //2
		
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //0
		0.5f, 0.5f, 0.0f,	1.0f, 1.0f, //2
		-0.5f, 0.5f, 0.0f,	0.0f, 1.0f, //3
														 
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //0
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, //1
		0.5f, -0.5f, 1.0f,	1.0f, 1.0f, //5
														
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //0
		-0.5f, -0.5f, 1.0f,	0.0f, 1.0f, //4
		0.5f, -0.5f, 1.0f,	1.0f, 1.0f, //5
														 
		0.5f, 0.5f, 0.0f,	1.0f, 1.0f, //2
		-0.5f, 0.5f, 0.0f,	0.0f, 1.0f, //3
		-0.5f, 0.5f, 1.0f,	0.0f, 0.0f, //7
													
		0.5f, 0.5f, 0.0f,	1.0f, 1.0f, //2
		0.5f, 0.5f, 1.0f,	1.0f, 0.0f, //6
		-0.5f, 0.5f, 1.0f,	0.0f, 0.0f, //7
														
		-0.5f, -0.5f, 1.0f,	0.0f, 1.0f, //4
		0.5f, -0.5f, 1.0f,	1.0f, 1.0f, //5
		-0.5f, 0.5f, 1.0f,	0.0f, 0.0f, //7
														
		0.5f, -0.5f, 1.0f,	1.0f, 1.0f, //5
		0.5f, 0.5f, 1.0f,	1.0f, 0.0f, //6
		-0.5f, 0.5f, 1.0f,	0.0f, 0.0f, //7
													
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //0
		-0.5f, 0.5f, 0.0f,	0.0f, 1.0f, //3
		-0.5f, 0.5f, 1.0f,	1.0f, 1.0f, //11
														
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //0
		-0.5f, -0.5f, 1.0f,	1.0f, 0.0f, //8
		-0.5f, 0.5f, 1.0f,	1.0f, 1.0f, //11
		
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, //1
		0.5f, 0.5f, 0.0f,	1.0f, 1.0f, //2
		0.5f, 0.5f, 1.0f,	0.0f, 1.0f, //10

		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, //1
		0.5f, -0.5f, 1.0f,	0.0f, 0.0f, //9
		0.5f, 0.5f, 1.0f,	0.0f, 1.0f //10
	};

	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};
	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	std::cout << "Init cubes VAO. glGetError: " << glGetError() << std::endl;
	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	std::cout << "Init plane VAO. glGetError: " << glGetError() << std::endl;
	

	//Buffer could be Unbound after the attributes were registered
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*---Load texture---*/
	unsigned int cubeTexture = LoadTexture(CONTAINER_TEXTURE_PATH);
	std::cout << "Load cubeTexture. glGetError: " << glGetError() << std::endl;
	unsigned int floorTexture = LoadTexture(FACE_TEXTURE_PATH);	
	std::cout << "Load floorTexture. glGetError: " << glGetError() << std::endl;
	/*---Load texture end---*/

	/*--Bind texture to shader--*/
	//Activate shader before setting uniform
	shadersLoader.EnableShaderProgram();

	shadersLoader.SetInt("customTexture1", 0);
	/*--Bind texture to shader END--*/

	/*------ Further openGL config -------*/
	//Optional: Enable blending to correct alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Enable depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//Setup input mode
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, lastMousePosX, lastMousePosY);
	//Handle mouse input
	glfwSetCursorPosCallback(window, onMousePosChanged);
	glfwSetScrollCallback(window, onMouseScrolled);
	/*------ Further openGL config END-------*/

	/*--Custom setup--*/
	camera->SetPosition(glm::vec3(-3.0f, 3.0f, 5.0f));
	camera->UpdateAngles(40.0f, -30.0f);
	/*--Custom tweak END--*/

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

		//Update delta time for input handle
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;	

		//Render loaded model
		shadersLoader.SetMat4f("viewMat", glm::value_ptr(camera->GetViewMat()));
		shadersLoader.SetMat4f("projectionMat", glm::value_ptr(camera->GetProjectionMat()));

		//Cubes
		glm::mat4 model;
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shadersLoader.SetMat4f("modelMat", glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//2nd cube
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shadersLoader.SetMat4f("modelMat", glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//floor
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		shadersLoader.SetMat4f("modelMat", glm::value_ptr(glm::mat4()));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		/*---Rendering end---*/
		

		//Swap color buffer used as output to the screen
		glfwSwapBuffers(window);
		//Handle any events are triggered
		glfwPollEvents();
	}
	//--------------

	//Clean up before exit
    glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);	
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

unsigned int LoadTexture(char const *path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}