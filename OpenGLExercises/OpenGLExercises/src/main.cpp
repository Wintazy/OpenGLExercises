#include <iostream>
#include <map>

#include <GLFW\glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Model\Model.h"
#include "Camera.h"

//Vertex shaders
const GLchar* NORMAL_VERTEX_SHADER_PATH = "../Data/Shaders/Normal.vs";
const GLchar* TRANSFORM_VERTEX_SHADER_PATH = "../Data/Shaders/Transform.vs";
const GLchar* TRANSFORM_2D_3D_VERTEX_SHADER_PATH = "../Data/Shaders/2dTo3d.vs";
const GLchar* GLYPH_VERTEX_SHADER_PATH = "../Data/Shaders/Glyph.vs";
//Fragment shaders
const GLchar* FADED_SHADER_PATH = "../Data/Shaders/SlowlyFaded.fs";
const GLchar* TEXTURE_FRAGMENT_SHADER_PATH = "../Data/Shaders/ApplyTexture.fs";
const GLchar* LIGHTING_SHADER_PATH = "../Data/Shaders/Lighting.fs";
const GLchar* LIGHTING_SOURCE_SHADER_PATH = "../Data/Shaders/LightingSource.fs";
const GLchar* NORMAL_MODEL_SHADER_PATH = "../Data/Shaders/NormalModel.fs";
const GLchar* GLYPH_FRAG_SHADER_PATH = "../Data/Shaders/Glyph.fs";
const GLchar* DEPTH_VISUAL_SHADER_PATH = "../Data/Shaders/DepthTest.fs";
const GLchar* SINGLE_COLOR_SHADER_PATH = "../Data/Shaders/SingleColor.fs";
//Textures
const GLchar* CONTAINER_TEXTURE_PATH = "../Data/Textures/container.jpg";
const GLchar* FACE_TEXTURE_PATH = "../Data/Textures/awesomeface.png";
const GLchar* NICE_TEXTURE_PATH = "../Data/Textures/Texture.png";
const GLchar* GRASS_TEXTURE_PATH = "../Data/Textures/grass.png";
const GLchar* GLASS_WINDOW_TEXTURE_PATH = "../Data/Textures/blending_transparent_window.png";
//Lighting maps
const GLchar* CRATE_DIFFUSE_PATH = "../Data/LightingMaps/diffuse_wooden_crate.png";
const GLchar* CRATE_SPECULAR_PATH = "../Data/LightingMaps/specular_wooden_crate.png";
//Models
const char* NANO_SUIT_PATH = "../Data/Models/nanosuit/nanosuit.obj";
const char* AERITH_MODEL_PATH = "../Data/Models/crisis_core/Aerith.DAE";
//Fonts
const char* FONT_PATH = "../Data/Fonts/arial.ttf";

struct Character {
	GLuint		m_textureId;
	glm::ivec2	m_size;
	glm::ivec2	m_bearing;
	GLuint		m_advance;
};
std::map<GLchar, Character> Characters;

bool isAlwaysPassDepthTest = false;
bool isDepthVisualizedEnable = false;
bool isStencilTestEnable = false;
bool isCullFrontFace = false;
std::map<int, int> KeyState;

float lastFrameTime = 0.0f;
float deltaTime = 0.0f;
int frameCount = 0;
int FPS;
float secCount = 0;

float lastMousePosX = SCREEN_WIDTH / 2;
float lastMousePosY = SCREEN_HEIGHT / 2;

void onFrameBufferSizeChanged(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void onMousePosChanged(GLFWwindow *window, double posX, double posY);
void onMouseScrolled(GLFWwindow *window, double offsetX, double offsetY);
unsigned int LoadTexture(const char * path, GLuint wrappMethod = GL_REPEAT);
void RenderText(ShadersLoader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

Camera* camera = new Camera();
GLuint glyphVAO, glyphVBO;

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

	ShadersLoader depthVisualShader = ShadersLoader();
	depthVisualShader.LoadShaders(NORMAL_VERTEX_SHADER_PATH, DEPTH_VISUAL_SHADER_PATH);

	ShadersLoader singleColorShader = ShadersLoader();
	singleColorShader.LoadShaders(NORMAL_VERTEX_SHADER_PATH, SINGLE_COLOR_SHADER_PATH);

	/*----Load models data----*/
	//Model customModel = Model(AERITH_MODEL_PATH);
	/*-----------------*/
	/*Prepare vertex data to render*/
	float cubeVertices[] = {
		//Positions			//Tex coords
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, //1
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //0
		0.5f, 0.5f, 0.0f,	1.0f, 1.0f, //2

		0.5f, 0.5f, 0.0f,	1.0f, 1.0f, //2
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //0
		-0.5f, 0.5f, 0.0f,	0.0f, 1.0f, //3

		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, //1			 
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //0
		0.5f, -0.5f, 1.0f,	1.0f, 1.0f, //5
														
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //0
		-0.5f, -0.5f, 1.0f,	0.0f, 1.0f, //4
		0.5f, -0.5f, 1.0f,	1.0f, 1.0f, //5
														 
		0.5f, 0.5f, 0.0f,	1.0f, 1.0f, //2
		-0.5f, 0.5f, 0.0f,	0.0f, 1.0f, //3
		-0.5f, 0.5f, 1.0f,	0.0f, 0.0f, //7
													
		0.5f, 0.5f, 0.0f,	1.0f, 1.0f, //2
		-0.5f, 0.5f, 1.0f,	0.0f, 0.0f, //7
		0.5f, 0.5f, 1.0f,	1.0f, 0.0f, //6
														
		-0.5f, -0.5f, 1.0f,	0.0f, 1.0f, //4
		0.5f, -0.5f, 1.0f,	1.0f, 1.0f, //5
		-0.5f, 0.5f, 1.0f,	0.0f, 0.0f, //7
														
		0.5f, -0.5f, 1.0f,	1.0f, 1.0f, //5
		0.5f, 0.5f, 1.0f,	1.0f, 0.0f, //6
		-0.5f, 0.5f, 1.0f,	0.0f, 0.0f, //7
													
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //0
		-0.5f, 0.5f, 1.0f,	1.0f, 1.0f, //11
		-0.5f, 0.5f, 0.0f,	0.0f, 1.0f, //3
														
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //0
		-0.5f, -0.5f, 1.0f,	1.0f, 0.0f, //8
		-0.5f, 0.5f, 1.0f,	1.0f, 1.0f, //11
		
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, //1
		0.5f, 0.5f, 0.0f,	1.0f, 1.0f, //2
		0.5f, 0.5f, 1.0f,	0.0f, 1.0f, //10

		0.5f, -0.5f, 1.0f,	0.0f, 0.0f, //9
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, //1
		0.5f, 0.5f, 1.0f,	0.0f, 1.0f //10
	};

	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	float grassVertices[] = {
		//Pos				//Tex
		0.0f, 0.5f, 0.0f,	0.0f, 0.0f,
		0.0f, -0.5f, 0.0f,	0.0f, 1.0f,
		1.0f, -0.5f, 0.0f,	1.0f, 1.0f,

		0.0f, 0.5f, 0.0f,	0.0f, 0.0f,
		1.0f, -0.5f, 0.0f,	1.0f, 1.0f,
		1.0f, 0.5f, 0.0f,	1.0f, 0.0f
	};

	float lineVertices[] = {
		5.0f, 0.0f, 0.0f,
		-5.0f, 0.0f, 0.0f,
		0.0f, 5.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		0.0f, 0.0f, 5.0f,
		0.0f, 0.0f, -5.0f,
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
	//Grasses
	std::vector<glm::vec3> grasses;
	grasses.push_back(glm::vec3(-1.5f, 0.0f, 0.1f));
	grasses.push_back(glm::vec3(1.5f, 0.0f, 1.51f));
	grasses.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	grasses.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	grasses.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	unsigned int grassVAO, grassVBO;
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);
	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), &grassVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
	std::cout << "Init grass VAO. glGetError: " << glGetError() << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Axes
	unsigned int axesVAO, axesVBO;
	glGenVertexArrays(1, &axesVAO);
	glGenBuffers(1, &axesVBO);
	glBindVertexArray(axesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), &lineVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/*Prepare vertex data to render END*/

	/*---Load texture---*/
	unsigned int cubeTexture = LoadTexture(CRATE_DIFFUSE_PATH);
	std::cout << "Load cubeTexture. glGetError: " << glGetError() << std::endl;
	unsigned int floorTexture = LoadTexture(NICE_TEXTURE_PATH);	
	std::cout << "Load floorTexture. glGetError: " << glGetError() << std::endl;
	unsigned int grassTexture = LoadTexture(GLASS_WINDOW_TEXTURE_PATH, GL_CLAMP_TO_EDGE);
	std::cout << "Load grassTexture. glGetError: " << glGetError() << std::endl;
	/*---Load texture end---*/

	/*--Bind texture to shader--*/
	/*--Bind texture to shader END--*/

	/*------ Further openGL config -------*/
	//Optional: Enable blending to correct alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Setup input mode
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window, lastMousePosX, lastMousePosY);
	//Handle mouse input
	glfwSetCursorPosCallback(window, onMousePosChanged);
	glfwSetScrollCallback(window, onMouseScrolled);
	/*------ Further openGL config END-------*/

	/*--Freetype setup--*/
	FT_Library freeTypeLib;
	if (FT_Init_FreeType(&freeTypeLib))
	{
		std::cout << "ERROR::FREETYPE: Failed to init" << std::endl;
	}
	FT_Face fontFace;
	if (FT_New_Face(freeTypeLib, FONT_PATH, 0, &fontFace))
	{
		std::cout << "ERROR::FREETYPE: Failed to loat font " << FONT_PATH << std::endl;
	}

	FT_Set_Pixel_Sizes(fontFace, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR--FREETYPE Failed to load glyph " << c << std::endl;
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D,
					0,
					GL_RED,
					fontFace->glyph->bitmap.width,
					fontFace->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					fontFace->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
			glm::ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
			fontFace->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}

	FT_Done_Face(fontFace);
	FT_Done_FreeType(freeTypeLib);

	glGenVertexArrays(1, &glyphVAO);
	glGenBuffers(1, &glyphVBO);
	glBindVertexArray(glyphVAO);
	glBindBuffer(GL_ARRAY_BUFFER, glyphVBO);
	//6 vertices of 4 floats
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	ShadersLoader glyphShader;
	glyphShader.LoadShaders(GLYPH_VERTEX_SHADER_PATH, GLYPH_FRAG_SHADER_PATH);
	glm::mat4 textProjectionMat = glm::ortho(0.0f, static_cast<GLfloat>(SCREEN_WIDTH), 0.0f, static_cast<GLfloat>(SCREEN_HEIGHT));
	glyphShader.EnableShaderProgram();
	glyphShader.SetMat4f("projectionMat", glm::value_ptr(textProjectionMat));
	/*--Freetype setup END--*/

	/*--Custom setup--*/
	camera->SetPosition(glm::vec3(-3.0f, 3.0f, 5.0f));
	camera->UpdateAngles(40.0f, -30.0f);
	std::string insText = "Hot key. M: depth test mode, V: visualize, T: stencil test, C: cull_face mode";
	KeyState.insert(std::pair<int, int>(GLFW_KEY_M, GLFW_RELEASE));
	KeyState.insert(std::pair<int, int>(GLFW_KEY_V, GLFW_RELEASE));
	KeyState.insert(std::pair<int, int>(GLFW_KEY_T, GLFW_RELEASE));
	KeyState.insert(std::pair<int, int>(GLFW_KEY_C, GLFW_RELEASE));
	/*--Custom setup END--*/

	
	//Render loop - keep running till window should stop
	while (!glfwWindowShouldClose(window))
	{
		//Handle all user input
		processInput(window);

		//Enable depth buffer
		glEnable(GL_DEPTH_TEST);
		//
		glEnable(GL_CULL_FACE);

		//Enable stencil test
		if(isStencilTestEnable)
		{
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}
		/*---Rendering part---*/
		//Set color to clear screen with
		glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
		//Clear color buffer, depth buffer, stencil buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		float currentFrameTime = glfwGetTime();

		//Update delta time for input handle
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		if (currentFrameTime - secCount >= 1)
		{
			secCount = currentFrameTime;
			FPS = frameCount;
			frameCount = 0;
		}
		else
		{
			++frameCount;
		}

		//Update cull mode
		if (isCullFrontFace)
		{
			glCullFace(GL_FRONT);
		}
		else
		{
			glCullFace(GL_BACK);
		}
		//Update depth test mode
		ShadersLoader currentShader;
		if (isAlwaysPassDepthTest)
			glDepthFunc(GL_ALWAYS);
		else
			glDepthFunc(GL_LESS);

		if(isDepthVisualizedEnable)
			currentShader = depthVisualShader;
		else
			currentShader = shadersLoader;

		currentShader.EnableShaderProgram();
		currentShader.SetInt("customTexture1", 0);
		currentShader.SetMat4f("viewMat", glm::value_ptr(camera->GetViewMat()));
		currentShader.SetMat4f("projectionMat", glm::value_ptr(camera->GetProjectionMat()));
		//
		if(isStencilTestEnable)
			glStencilMask(0x00);
		//Axes
		glBindVertexArray(axesVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		currentShader.SetMat4f("modelMat", glm::value_ptr(glm::mat4()));
		glDrawArrays(GL_LINES, 0, 6);
		glBindVertexArray(0);
		//floor
		glm::mat4 model;
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		currentShader.SetMat4f("modelMat", glm::value_ptr(glm::mat4()));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		//Cubes
		if (isStencilTestEnable)
		{
			//Enable stencil mask updating
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
		}
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		currentShader.SetMat4f("modelMat", glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//2nd cube
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		currentShader.SetMat4f("modelMat", glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		//
		if (isStencilTestEnable)
			glStencilMask(0x00);
		//Render text---------------
		std::string cameraPos = "CameraPos: " + std::to_string(camera->GetViewPos().x) + " " + std::to_string(camera->GetViewPos().y) + " " + std::to_string(camera->GetViewPos().z);
		RenderText(glyphShader, cameraPos, 5.0f, 5.0f, TEXT_DEFAULT_SIZE, glm::vec3(0.5, 0.8f, 0.2f));

		RenderText(glyphShader, insText, 5.0f, TEXT_TOP_SCREEN_OFFSET, TEXT_DEFAULT_SIZE, glm::vec3(0.5, 0.8f, 0.2f));

		RenderText(glyphShader, "FPS: " + std::to_string(FPS), 5.0f, 5.0f + TEXT_PADDING, TEXT_DEFAULT_SIZE, glm::vec3(0.5, 0.8f, 0.2f));
		//
		if (isStencilTestEnable)
		{
			//Disable stencil mask updating
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			//Draw 2 cubes again after scaled
			singleColorShader.EnableShaderProgram();
			glBindTexture(GL_TEXTURE_2D, cubeTexture);
			singleColorShader.SetMat4f("viewMat", glm::value_ptr(camera->GetViewMat()));
			singleColorShader.SetMat4f("projectionMat", glm::value_ptr(camera->GetProjectionMat()));
			glBindVertexArray(cubeVAO);
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
			model = glm::scale(model, glm::vec3(1.1f));
			singleColorShader.SetMat4f("modelMat", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			//2nd cube
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.1f));
			singleColorShader.SetMat4f("modelMat", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			//
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			glEnable(GL_DEPTH_TEST);
		}

		//Grasses
		glDisable(GL_CULL_FACE);
		currentShader.EnableShaderProgram();
		glBindVertexArray(grassVAO);
		glBindTexture(GL_TEXTURE_2D, grassTexture);
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < grasses.size(); i++)
		{
			float distance = glm::length(camera->GetViewPos() - grasses[i]);
			sorted[distance] = grasses[i];
		}
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4();
			model = glm::translate(model, it->second);
			currentShader.SetMat4f("modelMat", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
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
	if (KeyState[GLFW_KEY_M] != glfwGetKey(window, GLFW_KEY_M))
	{
		KeyState[GLFW_KEY_M] = glfwGetKey(window, GLFW_KEY_M);
		if(KeyState[GLFW_KEY_M] == GLFW_RELEASE)
			isAlwaysPassDepthTest = !isAlwaysPassDepthTest;
	}
	if (KeyState[GLFW_KEY_V] != glfwGetKey(window, GLFW_KEY_V))
	{
		KeyState[GLFW_KEY_V] = glfwGetKey(window, GLFW_KEY_V);
		if (KeyState[GLFW_KEY_V] == GLFW_RELEASE)
			isDepthVisualizedEnable = !isDepthVisualizedEnable;
	}
	if (KeyState[GLFW_KEY_T] != glfwGetKey(window, GLFW_KEY_T))
	{
		KeyState[GLFW_KEY_T] = glfwGetKey(window, GLFW_KEY_T);
		if (KeyState[GLFW_KEY_T] == GLFW_RELEASE)
			isStencilTestEnable = !isStencilTestEnable;
	}
	if (KeyState[GLFW_KEY_C] != glfwGetKey(window, GLFW_KEY_C))
	{
		KeyState[GLFW_KEY_C] = glfwGetKey(window, GLFW_KEY_C);
		if (KeyState[GLFW_KEY_C] == GLFW_RELEASE)
			isCullFrontFace = !isCullFrontFace;
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

unsigned int LoadTexture(char const *path, GLuint wrappMethod)
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappMethod);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappMethod);
		
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

void RenderText(ShadersLoader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	shader.EnableShaderProgram();

	shader.SetVec3f("textColor", glm::value_ptr(color));
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(glyphVAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xPos = x + ch.m_bearing.x * scale;
		GLfloat yPos = y - (ch.m_size.y - ch.m_bearing.y) * scale;

		GLfloat width = ch.m_size.x * scale;
		GLfloat height = ch.m_size.y * scale;

		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xPos,			yPos + height,  0.0, 0.0 },
			{ xPos,			yPos,			0.0, 1.0 },
			{ xPos + width, yPos,			1.0, 1.0 },

			{ xPos,			yPos + height,  0.0, 0.0 },
			{ xPos + width, yPos,			1.0, 1.0 },
			{ xPos + width, yPos + height,  1.0, 0.0 }
		};

		glBindTexture(GL_TEXTURE_2D, ch.m_textureId);
		glBindBuffer(GL_ARRAY_BUFFER, glyphVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		x += (ch.m_advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}