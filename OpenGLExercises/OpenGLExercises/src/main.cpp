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
const GLchar* SCREEN_VERTEX_SHADER_PATH = "../Data/Shaders/TextureBuffer.vs";
const GLchar* SKYBOX_VERTEX_SHADER_PATH = "../Data/Shaders/Skybox.vs";
const GLchar* REFLECT_VERTEX_SHADER_PATH = "../Data/Shaders/Reflect.vs";
const GLchar* MODEL_VERTEX_SHADER_PATH = "../Data/Shaders/Model.vs";
const GLchar* NV_VISUALIZE_VERTEX_SHADER_PATH = "../Data/Shaders/NormVecVisualize.vs";
const GLchar* MULTI_INSTANCE_VERTEX_SHADER_PATH = "../Data/Shaders/MultiInstance.vs";
//Geometry shader
const GLchar* EXPLODE_GEO_SHADER_PATH = "../Data/Shaders/Explode.gs";
const GLchar* NV_VISUALIZE_GEO_SHADER_PATH = "../Data/Shaders/NormVecVisualize.gs";
//Fragment shaders
const GLchar* FADED_SHADER_PATH = "../Data/Shaders/SlowlyFaded.fs";
const GLchar* TEXTURE_FRAGMENT_SHADER_PATH = "../Data/Shaders/ApplyTexture.fs";
const GLchar* LIGHTING_SHADER_PATH = "../Data/Shaders/Lighting.fs";
const GLchar* LIGHTING_SOURCE_SHADER_PATH = "../Data/Shaders/LightingSource.fs";
const GLchar* NORMAL_MODEL_SHADER_PATH = "../Data/Shaders/NormalModel.fs";
const GLchar* GLYPH_FRAG_SHADER_PATH = "../Data/Shaders/Glyph.fs";
const GLchar* DEPTH_VISUAL_SHADER_PATH = "../Data/Shaders/DepthTest.fs";
const GLchar* SINGLE_COLOR_SHADER_PATH = "../Data/Shaders/SingleColor.fs";
const GLchar* SCREEN_FRAG_SHADER_PATH = "../Data/Shaders/ScreenBuffer.fs";
const GLchar* EDGE_DETECT_SHADER_PATH = "../Data/Shaders/EdgeDetect.fs";
const GLchar* SKYBOX_FRAG_SHADER_PATH = "../Data/Shaders/Skybox.fs";
const GLchar* REFLECT_FRAG_SHADER_PATH = "../Data/Shaders/Reflect.fs";
const GLchar* REFRACT_FRAG_SHADER_PATH = "../Data/Shaders/Refract.fs";
const GLchar* NV_VISUALIZE_FRAG_SHADER_PATH = "../Data/Shaders/NormVecVisualize.fs";
//Textures
const GLchar* CONTAINER_TEXTURE_PATH = "../Data/Textures/2DTextures/container.jpg";
const GLchar* FACE_TEXTURE_PATH = "../Data/Textures/2DTextures/awesomeface.png";
const GLchar* NICE_TEXTURE_PATH = "../Data/Textures/2DTextures/Texture.png";
const GLchar* GRASS_TEXTURE_PATH = "../Data/Textures/2DTextures/grass.png";
const GLchar* GLASS_WINDOW_TEXTURE_PATH = "../Data/Textures/2DTextures/blending_transparent_window.png";
//Lighting maps
const GLchar* CRATE_DIFFUSE_PATH = "../Data/LightingMaps/diffuse_wooden_crate.png";
const GLchar* CRATE_SPECULAR_PATH = "../Data/LightingMaps/specular_wooden_crate.png";
//Models
const char* AERITH_MODEL_PATH = "../Data/Models/crisis_core/Aerith.DAE";
const char* PLANET_MODEL_PATH = "../Data/Models/planet/planet.obj";
const char* ROCK_MODEL_PATH = "../Data/Models/rock/rock.obj";
//Fonts
const char* FONT_PATH = "../Data/Fonts/arial.ttf";
//Skybox cubemap
const std::vector<std::string> SKYBOX_CUBE_FACES_PATH
{
	"../Data/Textures/CubeTextures/mp_downunder/down-under_ft.tga",
	"../Data/Textures/CubeTextures/mp_downunder/down-under_bk.tga",
	"../Data/Textures/CubeTextures/mp_downunder/down-under_up.tga",
	"../Data/Textures/CubeTextures/mp_downunder/down-under_dn.tga",
	"../Data/Textures/CubeTextures/mp_downunder/down-under_rt.tga",
	"../Data/Textures/CubeTextures/mp_downunder/down-under_lf.tga"
};

struct Character {
	GLuint		m_textureId;
	glm::ivec2	m_size;
	glm::ivec2	m_bearing;
	GLuint		m_advance;
};
std::map<GLchar, Character> Characters;

bool isAlwaysPassDepthTest = false;
bool isDepthVisualizedEnabled = false;
bool isStencilTestEnabled = false;
bool isCullFrontFace = false;
bool isEdgeDetectEnabled = false;
bool isNormVecVisualizeEnabled = false;
bool isWireframeEnabled = false;
bool isMSAAEnabled = false;
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
unsigned int LoadCubeTexture(std::vector<std::string> faces);
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


	/*------ Further openGL config -------*/
	//Register callback function handle window's buffer size changes
	glfwSetFramebufferSizeCallback(window, onFrameBufferSizeChanged);
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

	/*---Init shaders---*/
	ShadersLoader normalTextureShader = ShadersLoader();
	normalTextureShader.LoadShaders(NORMAL_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
	normalTextureShader.LoadShaders(TEXTURE_FRAGMENT_SHADER_PATH, GL_FRAGMENT_SHADER);

	ShadersLoader depthVisualShader = ShadersLoader();
	depthVisualShader.LoadShaders(NORMAL_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
	depthVisualShader.LoadShaders(DEPTH_VISUAL_SHADER_PATH, GL_FRAGMENT_SHADER);

	ShadersLoader singleColorShader = ShadersLoader();
	singleColorShader.LoadShaders(NORMAL_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
	singleColorShader.LoadShaders(SINGLE_COLOR_SHADER_PATH, GL_FRAGMENT_SHADER);

	ShadersLoader reflectShader = ShadersLoader();
	reflectShader.LoadShaders(REFLECT_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
	reflectShader.LoadShaders(REFLECT_FRAG_SHADER_PATH, GL_FRAGMENT_SHADER);

	ShadersLoader refractShader = ShadersLoader();
	refractShader.LoadShaders(REFLECT_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
	refractShader.LoadShaders(REFRACT_FRAG_SHADER_PATH, GL_FRAGMENT_SHADER);

	ShadersLoader modelShader = ShadersLoader();
	modelShader.LoadShaders(MODEL_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
	//modelShader.LoadShaders(EXPLODE_GEO_SHADER_PATH, GL_GEOMETRY_SHADER);
	modelShader.LoadShaders(NORMAL_MODEL_SHADER_PATH, GL_FRAGMENT_SHADER);

	ShadersLoader normVecVisualizeShader = ShadersLoader();
	normVecVisualizeShader.LoadShaders(NV_VISUALIZE_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
	normVecVisualizeShader.LoadShaders(NV_VISUALIZE_GEO_SHADER_PATH, GL_GEOMETRY_SHADER);
	normVecVisualizeShader.LoadShaders(NV_VISUALIZE_FRAG_SHADER_PATH, GL_FRAGMENT_SHADER);

	ShadersLoader multiInstanceShader = ShadersLoader();
	multiInstanceShader.LoadShaders(MULTI_INSTANCE_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
	multiInstanceShader.LoadShaders(NORMAL_MODEL_SHADER_PATH, GL_FRAGMENT_SHADER);
	/*---Init shaders END---*/

	/*----Load models data----*/
	Model customModel = Model(AERITH_MODEL_PATH);
	modelShader.EnableShaderProgram();
	modelShader.SetVec3f("light.position", 1.0f, 1.0f, 1.0f);
	modelShader.SetVec3f("light.ambient", 0.5f, 0.5f, 0.5f);
	modelShader.SetVec3f("light.diffuse", 0.8f, 0.8f, 0.8f);
	modelShader.SetVec3f("light.specular", 1.0f, 1.0f, 1.0f);
	modelShader.SetFloat("light.constant", 1.0f);
	modelShader.SetFloat("light.linear", 0.09);
	modelShader.SetFloat("light.quadratic", 0.002);

	Model planetModel = Model(PLANET_MODEL_PATH);

	Model rockModel = Model(ROCK_MODEL_PATH);
	/*-----------------*/
	/*Prepare vertex data to render*/
	float cubeVertices[] = {
		//Positions			//Norm vec
		0.5f, -0.5f, 0.0f,	0.0f, 0.0f, -1.0f, //1
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, -1.0f, //0
		0.5f, 0.5f, 0.0f,	0.0f, 0.0f, -1.0f, //2

		0.5f, 0.5f, 0.0f,	0.0f, 0.0f, -1.0f, //2
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, -1.0f, //0
		-0.5f, 0.5f, 0.0f,	0.0f, 0.0f, -1.0f, //3

		-0.5f, -0.5f, 0.0f,	0.0f, -1.0f, 0.0f, //0
		0.5f, -0.5f, 0.0f,	0.0f, -1.0f, 0.0f, //1
		0.5f, -0.5f, 1.0f,	0.0f, -1.0f, 0.0f, //5
												
		-0.5f, -0.5f, 1.0f,	0.0f, -1.0f, 0.0f, //4		
		-0.5f, -0.5f, 0.0f,	0.0f, -1.0f, 0.0f, //0
		0.5f, -0.5f, 1.0f,	0.0f, -1.0f, 0.0f, //5
														 
		0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f, //2
		-0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f, //3
		-0.5f, 0.5f, 1.0f,	0.0f, 1.0f, 0.0f, //7
													
		0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f, //2
		-0.5f, 0.5f, 1.0f,	0.0f, 1.0f, 0.0f, //7
		0.5f, 0.5f, 1.0f,	0.0f, 1.0f, 0.0f, //6
														
		-0.5f, -0.5f, 1.0f,	0.0f, 0.0f, 1.0f, //4
		0.5f, -0.5f, 1.0f,	0.0f, 0.0f, 1.0f, //5
		-0.5f, 0.5f, 1.0f,	0.0f, 0.0f, 1.0f, //7
														
		0.5f, -0.5f, 1.0f,	0.0f, 0.0f, 1.0f, //5
		0.5f, 0.5f, 1.0f,	0.0f, 0.0f, 1.0f, //6
		-0.5f, 0.5f, 1.0f,	0.0f, 0.0f, 1.0f, //7
													
		-0.5f, -0.5f, 0.0f,	-1.0f, 0.0f, 0.0f, //0
		-0.5f, 0.5f, 1.0f,	-1.0f, 0.0f, 0.0f, //11
		-0.5f, 0.5f, 0.0f,	-1.0f, 0.0f, 0.0f, //3
														
		-0.5f, -0.5f, 0.0f,	-1.0f, 0.0f, 0.0f, //0
		-0.5f, -0.5f, 1.0f,	-1.0f, 0.0f, 0.0f, //8
		-0.5f, 0.5f, 1.0f,	-1.0f, 0.0f, 0.0f, //11
		
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f, //1
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f, //2
		0.5f, 0.5f, 1.0f,	1.0f, 0.0f, 0.0f, //10

		0.5f, -0.5f, 1.0f,	1.0f, 0.0f, 0.0f, //9
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f, //1
		0.5f, 0.5f, 1.0f,	1.0f, 0.0f, 0.0f //10
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

	float quadVertices[] = {
		//Position		//TexCoords
		-1.0f, -1.0f,	0.0f, 0.0f,
		1.0f, -1.0f,	1.0f, 0.0f,
		-1.0f, 1.0f,	0.0f, 1.0f,

		1.0f, -1.0f,	1.0f, 0.0f,
		1.0f, 1.0f,		1.0f, 1.0f,
		-1.0f, 1.0f,	0.0f, 1.0f
	};

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	std::cout << "Init cubes VAO. glGetError: " << glGetError() << std::endl;
	glBindVertexArray(0);
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
	//Planet setup
	glm::mat4 planetModelMat = glm::mat4();
	planetModelMat = glm::translate(planetModelMat, glm::vec3(0.0f, 2.0f, -1.0f));
	planetModelMat = glm::scale(planetModelMat, glm::vec3(0.2f, 0.2f, 0.2f));
	//Generate rocks VAO
	multiInstanceShader.EnableShaderProgram();
	multiInstanceShader.SetVec3f("light.position", 1.0f, 1.0f, 1.0f);
	multiInstanceShader.SetVec3f("light.ambient", 0.5f, 0.5f, 0.5f);
	multiInstanceShader.SetVec3f("light.diffuse", 0.8f, 0.8f, 0.8f);
	multiInstanceShader.SetVec3f("light.specular", 1.0f, 1.0f, 1.0f);
	multiInstanceShader.SetFloat("light.constant", 1.0f);
	multiInstanceShader.SetFloat("light.linear", 0.09);
	multiInstanceShader.SetFloat("light.quadratic", 0.002);
	//Gegnerate random position for the rocks
	unsigned int amount = 200;
	glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // initialize random seed	
	float radius = 3.0f;
	float offset = 0.3f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y + 2, z - 1));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = (rand() % 5) / 100.0f + 0.02;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}
	//Generate vertex Buffer Object for the rocks
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < rockModel.GetMeshes().size(); i++)
	{
		unsigned int VAO = rockModel.GetMeshes()[i].GetVAO();
		glBindVertexArray(VAO);
		// vertex Attributes
		GLsizei vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * vec4Size));
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		std::cout << "Init rock VAO: " + std::to_string(i) + " glGetError: " << glGetError() << std::endl;
		glBindVertexArray(0);
	}
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
	glyphShader.LoadShaders(GLYPH_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
	glyphShader.LoadShaders(GLYPH_FRAG_SHADER_PATH, GL_FRAGMENT_SHADER);
	glm::mat4 textProjectionMat = glm::ortho(0.0f, static_cast<GLfloat>(SCREEN_WIDTH), 0.0f, static_cast<GLfloat>(SCREEN_HEIGHT));
	glyphShader.EnableShaderProgram();
	glyphShader.SetMat4f("projectionMat", glm::value_ptr(textProjectionMat));
	/*--Freetype setup END--*/

	/*--Custom setup--*/
	camera->SetPosition(glm::vec3(-3.0f, 3.0f, 5.0f));
	camera->UpdateAngles(40.0f, -30.0f);
	std::string insText = "Hot key. M: depth test mode, V: visualize, T: stencil test, C: cull_face mode, E: Edge detecting mode";
	std::string insText_2 = "N: Norm vec visualize. P: Wireframe mode";
	KeyState.insert(std::pair<int, int>(GLFW_KEY_M, GLFW_RELEASE));
	KeyState.insert(std::pair<int, int>(GLFW_KEY_V, GLFW_RELEASE));
	KeyState.insert(std::pair<int, int>(GLFW_KEY_T, GLFW_RELEASE));
	KeyState.insert(std::pair<int, int>(GLFW_KEY_C, GLFW_RELEASE));
	KeyState.insert(std::pair<int, int>(GLFW_KEY_E, GLFW_RELEASE));
	KeyState.insert(std::pair<int, int>(GLFW_KEY_N, GLFW_RELEASE));
	KeyState.insert(std::pair<int, int>(GLFW_KEY_P, GLFW_RELEASE));
	KeyState.insert(std::pair<int, int>(GLFW_KEY_X, GLFW_RELEASE));
	/*--Custom setup END--*/

	/*--Texture buffer setup--*/
	unsigned int frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);	

	unsigned int texColorBuffer;
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texColorBuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, GL_TRUE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texColorBuffer, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER: Framebuffer is not complete " << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);

	ShadersLoader screenShader = ShadersLoader();
	screenShader.LoadShaders(SCREEN_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
	screenShader.LoadShaders(SCREEN_FRAG_SHADER_PATH, GL_FRAGMENT_SHADER);
	screenShader.EnableShaderProgram();
	screenShader.SetInt("screenTexture", 0);
	ShadersLoader edgeDetectShader = ShadersLoader();
	edgeDetectShader.LoadShaders(SCREEN_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
	edgeDetectShader.LoadShaders(EDGE_DETECT_SHADER_PATH, GL_FRAGMENT_SHADER);
	edgeDetectShader.EnableShaderProgram();
	edgeDetectShader.SetInt("screenTexture", 0);
	/*--Texture buffer setup END--*/

	/*--Skybox setup--*/
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	ShadersLoader skyboxShader;
	skyboxShader.LoadShaders(SKYBOX_VERTEX_SHADER_PATH, GL_VERTEX_SHADER);
	skyboxShader.LoadShaders(SKYBOX_FRAG_SHADER_PATH, GL_FRAGMENT_SHADER);

	unsigned int skyboxTexture = LoadCubeTexture(SKYBOX_CUBE_FACES_PATH);
	/*--Skybox setup END--*/
	
	//Render loop - keep running till window should stop
	while (!glfwWindowShouldClose(window))
	{
		//Handle all user input
		processInput(window);
		//Bind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		//Enable depth buffer
		glEnable(GL_DEPTH_TEST);
		//
		glEnable(GL_CULL_FACE);
		//Clear color buffer, depth buffer
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Enable stencil test
		if(isStencilTestEnabled)
		{
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}
		//Set draw style to wireframe polygons
		if(isWireframeEnabled)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//Multi sampling toogle
		if(isMSAAEnabled)
			glEnable(GL_MULTISAMPLE);
		else
			glDisable(GL_MULTISAMPLE);
		/*---Rendering part---*/

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
			glDepthFunc(GL_LEQUAL);

		if(isDepthVisualizedEnabled)
			currentShader = depthVisualShader;
		else
			currentShader = normalTextureShader;

		currentShader.EnableShaderProgram();
		currentShader.SetInt("customTexture1", 0);
		currentShader.SetMat4f("viewMat", glm::value_ptr(camera->GetViewMat()));
		currentShader.SetMat4f("projectionMat", glm::value_ptr(camera->GetProjectionMat()));
		//
		if(isStencilTestEnabled)
			glStencilMask(0x00);
		//Axes
		glm::mat4 model;
		glBindVertexArray(axesVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		currentShader.SetMat4f("modelMat", glm::value_ptr(glm::mat4()));
		glDrawArrays(GL_LINES, 0, 6);
		glBindVertexArray(0);
		//floor
		/*
		glDisable(GL_CULL_FACE);
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		currentShader.SetMat4f("modelMat", glm::value_ptr(glm::mat4()));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glEnable(GL_CULL_FACE);*/

		//Model
		modelShader.EnableShaderProgram();
		modelShader.SetMat4f("viewMat", glm::value_ptr(camera->GetViewMat()));
		modelShader.SetMat4f("projectionMat", glm::value_ptr(camera->GetProjectionMat()));
		modelShader.SetVec3f("viewPos", glm::value_ptr(camera->GetViewPos()));
		modelShader.SetFloat("time", currentFrameTime);
		glm::mat4 customModelTransMat;
		customModelTransMat = glm::translate(customModelTransMat, glm::vec3(0.0f, 0.0f, -1.0f));
		customModelTransMat = glm::scale(customModelTransMat, glm::vec3(0.2f, 0.2f, 0.2f));
		modelShader.SetMat4f("modelMat", glm::value_ptr(customModelTransMat));

		customModel.Render(modelShader);
		//Planet model
		model = glm::rotate(planetModelMat, currentFrameTime, glm::vec3(0.0f, 1.0f, 0.0f));
		modelShader.SetMat4f("modelMat", model);
		planetModel.Render(modelShader);

		// draw meteorites
		multiInstanceShader.EnableShaderProgram();
		model = camera->GetViewMat();
		model = glm::rotate(model, currentFrameTime, glm::vec3(0.0f, 1.0f, 0.0f));
		multiInstanceShader.SetMat4f("viewMat", model);
		multiInstanceShader.SetMat4f("projectionMat", glm::value_ptr(camera->GetProjectionMat()));
		multiInstanceShader.SetVec3f("viewPos", glm::value_ptr(camera->GetViewPos()));
		multiInstanceShader.SetInt("material_texture_diffuse1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rockModel.GetLoadedTexture()[0].m_id);
		for (unsigned int i = 0; i < rockModel.GetMeshes().size(); i++)
		{
			glBindVertexArray(rockModel.GetMeshes()[i].GetVAO());
			glDrawElementsInstanced(
				GL_TRIANGLES, rockModel.GetMeshes()[i].GetIndices().size(), GL_UNSIGNED_INT, 0, amount
			);
			glBindVertexArray(0);
		}
		//Cubes
		if (isStencilTestEnabled)
		{
			//Enable stencil mask updating
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
		}
		reflectShader.EnableShaderProgram();
		reflectShader.SetMat4f("viewMat", glm::value_ptr(camera->GetViewMat()));
		reflectShader.SetMat4f("projectionMat", glm::value_ptr(camera->GetProjectionMat()));
		reflectShader.SetVec3f("cameraPos", camera->GetViewPos());
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, cubeTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		reflectShader.SetMat4f("modelMat", glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//2nd cube
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		refractShader.EnableShaderProgram();
		refractShader.SetMat4f("viewMat", glm::value_ptr(camera->GetViewMat()));
		refractShader.SetMat4f("projectionMat", glm::value_ptr(camera->GetProjectionMat()));
		refractShader.SetVec3f("cameraPos", camera->GetViewPos());
		refractShader.SetMat4f("modelMat", glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//Normal vector visualize
		if(isNormVecVisualizeEnabled)
		{
			normVecVisualizeShader.EnableShaderProgram();
			normVecVisualizeShader.SetMat4f("viewMat", glm::value_ptr(camera->GetViewMat()));
			normVecVisualizeShader.SetMat4f("projectionMat", glm::value_ptr(camera->GetProjectionMat()));
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
			normVecVisualizeShader.SetMat4f("modelMat", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
			normVecVisualizeShader.SetMat4f("modelMat", glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);

			model = glm::rotate(planetModelMat, currentFrameTime, glm::vec3(0.0f, 1.0f, 0.0f));
			modelShader.SetMat4f("modelMat", model);
			normVecVisualizeShader.SetMat4f("modelMat", model);
			planetModel.Render(normVecVisualizeShader);
		}
		glBindVertexArray(0);
		//
		if (isStencilTestEnabled)
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
		//Skybox
		glDisable(GL_CULL_FACE);
		skyboxShader.EnableShaderProgram();
		glm::mat4 skyboxView = glm::mat4(glm::mat3(camera->GetViewMat()));
		skyboxShader.SetMat4f("viewMat", glm::value_ptr(skyboxView));
		skyboxShader.SetMat4f("projectionMat", glm::value_ptr(camera->GetProjectionMat()));
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glEnable(GL_CULL_FACE);
		glBindVertexArray(0);

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
		glEnable(GL_CULL_FACE);
		glBindVertexArray(0);
		//Pass default framebuffer
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBuffer);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		if (isEdgeDetectEnabled)
			edgeDetectShader.EnableShaderProgram();
		else
			screenShader.EnableShaderProgram();
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glBindVertexArray(0);
		//Render text---------------
		std::string cameraPos = "CameraPos: " + std::to_string(camera->GetViewPos().x) + " " + std::to_string(camera->GetViewPos().y) + " " + std::to_string(camera->GetViewPos().z);
		RenderText(glyphShader, cameraPos, 5.0f, 5.0f, TEXT_DEFAULT_SIZE, glm::vec3(0.5, 0.8f, 0.2f));

		RenderText(glyphShader, insText, 5.0f, TEXT_TOP_SCREEN_OFFSET, TEXT_DEFAULT_SIZE, glm::vec3(0.5, 0.8f, 0.2f));
		RenderText(glyphShader, insText_2, 5.0f, TEXT_TOP_SCREEN_OFFSET - TEXT_PADDING, TEXT_DEFAULT_SIZE, glm::vec3(0.5, 0.8f, 0.2f));

		RenderText(glyphShader, "FPS: " + std::to_string(FPS), 5.0f, 5.0f + TEXT_PADDING, TEXT_DEFAULT_SIZE, glm::vec3(0.5, 0.8f, 0.2f));
		glEnable(GL_CULL_FACE);
		//
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
	glDeleteVertexArrays(1, &grassVAO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);	
	glDeleteBuffers(1, &grassVBO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &skyboxVBO);
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
			isDepthVisualizedEnabled = !isDepthVisualizedEnabled;
	}
	if (KeyState[GLFW_KEY_T] != glfwGetKey(window, GLFW_KEY_T))
	{
		KeyState[GLFW_KEY_T] = glfwGetKey(window, GLFW_KEY_T);
		if (KeyState[GLFW_KEY_T] == GLFW_RELEASE)
			isStencilTestEnabled = !isStencilTestEnabled;
	}
	if (KeyState[GLFW_KEY_C] != glfwGetKey(window, GLFW_KEY_C))
	{
		KeyState[GLFW_KEY_C] = glfwGetKey(window, GLFW_KEY_C);
		if (KeyState[GLFW_KEY_C] == GLFW_RELEASE)
			isCullFrontFace = !isCullFrontFace;
	}
	if (KeyState[GLFW_KEY_E] != glfwGetKey(window, GLFW_KEY_E))
	{
		KeyState[GLFW_KEY_E] = glfwGetKey(window, GLFW_KEY_E);
		if (KeyState[GLFW_KEY_E] == GLFW_RELEASE)
			isEdgeDetectEnabled = !isEdgeDetectEnabled;
	}
	if (KeyState[GLFW_KEY_N] != glfwGetKey(window, GLFW_KEY_N))
	{
		KeyState[GLFW_KEY_N] = glfwGetKey(window, GLFW_KEY_N);
		if (KeyState[GLFW_KEY_N] == GLFW_RELEASE)
			isNormVecVisualizeEnabled = !isNormVecVisualizeEnabled;
	}
	if (KeyState[GLFW_KEY_P] != glfwGetKey(window, GLFW_KEY_P))
	{
		KeyState[GLFW_KEY_P] = glfwGetKey(window, GLFW_KEY_P);
		if (KeyState[GLFW_KEY_P] == GLFW_RELEASE)
			isWireframeEnabled = !isWireframeEnabled;
	}
	if (KeyState[GLFW_KEY_X] != glfwGetKey(window, GLFW_KEY_X))
	{
		KeyState[GLFW_KEY_X] = glfwGetKey(window, GLFW_KEY_X);
		if (KeyState[GLFW_KEY_X] == GLFW_RELEASE)
			isMSAAEnabled = !isMSAAEnabled;
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

	int width, height;
	int nrComponents = -1;
	unsigned char *data;
	if (path == nullptr)
	{
		data = nullptr;
		std::cout << "Generating empty texture" << std::endl;
	}
	else
	{
		data = stbi_load(path, &width, &height, &nrComponents, 0);
	}

	GLenum format;
	if (nrComponents == 1)
		format = GL_RED;
	else if (nrComponents == 3)
		format = GL_RGB;
	else if (nrComponents == 4)
		format = GL_RGBA;
	else if (nrComponents == -1)
		format = GL_RGB;

	glBindTexture(GL_TEXTURE_2D, textureID);
	if (data == nullptr)
	{
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, SCREEN_WIDTH, SCREEN_HEIGHT, GL_FALSE);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrappMethod);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrappMethod);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_image_free(data);

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


unsigned int LoadCubeTexture(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load cubemap texture at " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}