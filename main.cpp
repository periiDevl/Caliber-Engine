
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include"src/Model.h"
#include<iostream>
#include<fstream>
#include<string>
#include<Windows.h>
#include"src/FlightController.h"
#include"src/Functions.h"
#include"src/Component.h"
#include <bullet/btBulletDynamicsCommon.h>
Functions func;
FlightController flightController;
//Component scene;
// Create a new component object
//Component component;

// Add an object to the component
//component.AddObject("path/to/object.obj");

const int objectsAmount = 2;
bool run = false;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
std::array save = {1};
//finished
int vsync;
bool renderShadows;
int samples;
int bloom;
int highQualtiyShdows;
int HighLightView;
int wireframe;
int width;
int height;
float gamma;
float exposure;
float normalSpeed;
float ctrlSpeed;
bool FullCockpit = true;
bool enableskybox = true;
float FogNear = 0.0f;
int viewFarPlane;







//collision
//__________
bool colidedX = false;
float camPosX;

float camPosZ;
bool colidedZ = false;
//__________


float rectangleVertices[] =
{
	//  Coords   // texCoords
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};
int saveFloatCurve = 100;

float delta_time() {
	static double previous_time = glfwGetTime();

	double current_time = glfwGetTime();
	double elapsed_time = current_time - previous_time;
	previous_time = current_time;

	return static_cast<float>(elapsed_time);
}

void createStaticBox(btDynamicsWorld* dynamicsWorld, btVector3 position, btVector3 scale, btQuaternion rotation)
{
	btCollisionShape* boxShape = new btBoxShape(scale);
	btDefaultMotionState* boxMotionState = new btDefaultMotionState(btTransform(rotation, position));
	btScalar mass = 0;
	btVector3 boxInertia(0, 0, 0);
	boxShape->calculateLocalInertia(mass, boxInertia);
	btRigidBody* boxRigidBody = new btRigidBody(mass, boxMotionState, boxShape, boxInertia);
	boxRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
	dynamicsWorld->addRigidBody(boxRigidBody);
}


int main()
{

	
	//scene.TRY_OBJ_RECOVERING_TEST(objectAmt, sceneObjects);
	//PlaySound(TEXT("balls.wav"), NULL, SND_ASYNC);

	


	std::string line;
	std::ifstream saveFile("projectname.caliber");
	int i = 0;
	while (!saveFile.eof())
	{
		std::getline(saveFile, line);
		if (saveFile.good())
		{
			save[i] = std::stof(line);
			i++;
		}

	}
	samples = save[0];
	vsync = save[1];
	gamma = save[2];
	exposure = save[3];
	bloom = save[4];
	renderShadows = save[5];
	highQualtiyShdows = save[6];
	HighLightView = save[7];
	enableskybox = save[8];

	height = save[9];
	width = save[10];
	wireframe = save[11];
	ctrlSpeed = save[12];
	normalSpeed = save[13];
	FullCockpit = save[14];
	FogNear = save[15];
	viewFarPlane = save[16];


	exposure = exposure / saveFloatCurve;
	gamma = gamma / saveFloatCurve;
	FogNear = FogNear / saveFloatCurve;

	ctrlSpeed = ctrlSpeed / saveFloatCurve;
	normalSpeed = normalSpeed / saveFloatCurve;

	int mockwidth = width;
	int mockheight = height;
	
	
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Only use this if you don't have a framebuffer
	//glfwWindowHint(GLFW_SAMPLES, samples);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	
	
	
	//GLFWwindow* window = glfwCreateWindow(width, height, "Caliber window", glfwGetPrimaryMonitor(), NULL);
	
	
	
	GLFWwindow* window = glfwCreateWindow(width, height, "Caliber window", NULL, NULL);
	
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1; 
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	/*
	//load Icon
	int wid, hei;
	int channels;
	//!rememer! make sure to install the icon
	unsigned char* pixels = stbi_load("C:/Users/�����/source/repos/caliber_engine/engine_assets/caliberLogo.jpeg", &wid, &hei, &channels, 4);

	//change icon
	GLFWimage images[1];
	images[0].width = wid;
	images[0].height = hei;
	images[0].pixels = pixels;
	glfwSetWindowIcon(window, 1, images);
	*/

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);


	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	// Generates shaders
	Shader shaderProgram("shaders/default.vert", "shaders/default.frag");
	Shader framebufferProgram("shaders/framebuffer.vert", "shaders/framebuffer.frag");
	Shader shadowMapProgram("shaders/shadowMap.vert", "shaders/shadowMap.frag");
	Shader blurProgram("shaders/framebuffer.vert", "shaders/blur.frag");
	Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");

	Shader outlineShader("shaders/outlining.vert", "shaders/outlining.frag");


	

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1, 1, 1, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 1, 0.5f);
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "near"), FogNear);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "far"), viewFarPlane);
	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);
	/*
	glm::vec4 lColor = glm::vec4(10, 10, 10, 1);
	glm::vec3 lPos = glm::vec3(1.9f, 1, 0.5f);


	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lColor"), lColor.x, lColor.y, lColor.z, lColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lPos"), lPos.x, lPos.y, lPos.z);
	*/

	framebufferProgram.Activate();
	glUniform1i(glGetUniformLocation(framebufferProgram.ID, "screenTexture"), 0);
	glUniform1i(glGetUniformLocation(framebufferProgram.ID, "bloomTexture"), 1);
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "gamma"), gamma);
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "exposure"), exposure);

	blurProgram.Activate();
	glUniform1i(glGetUniformLocation(blurProgram.ID, "screenTexture"), 0);


	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	//outlining
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


	// Enables Multisampling
	glEnable(GL_MULTISAMPLE);

	// Enables Cull Facing
	glEnable(GL_CULL_FACE);
	// Keeps front faces
	glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	glFrontFace(GL_CCW);

	// Creates camera object
	Camera camera(width, height, glm::vec3(22.0f, 15.0, 0.0f));

	//camera stacking
	//Camera camera2(width, height, glm::vec3(22.0f, 15.0, 0.0f));
	

	
	
	
	
	// Load in models

	
	

	Component scene;
	const int ObjectsAmt = 3;
	Model sceneObjects[ObjectsAmt] = { Model("models/rocket/scene.gltf"), Model("models/cube/scene.gltf"), Model("models/cliberDeafult/scene.gltf") };
	scene.TRY_OBJ_RECOVERING_TEST(ObjectsAmt, sceneObjects);

	scene.SuffleObjectsID(ObjectsAmt, sceneObjects);


	sceneObjects[2].scale = glm::vec3(2.0f);
	
	//Model rocket("models/rocket/scene.gltf");

	

	// Prepare framebuffer rectangle VBO and VAO
	unsigned int rectVAO, rectVBO;
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));



	// Variables to create periodic event for FPS displaying
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// Keeps track of the Blur_amount of frames in timeDiff
	unsigned int counter = 0;

	// Use this to disable VSync (not advized)
	//glfwSwapInterval(0);


	// Create Frame Buffer Object
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Create Framebuffer Texture
	unsigned int framebufferTexture;
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB16F, width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture, 0);

	// Create Render Buffer Object
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	// Error checking framebuffer
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	// Create Ping Pong Framebuffers for repetitive blurring
	unsigned int pingpongFBO[2];
	unsigned int pingpongBuffer[2];
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongBuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0);

		fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Ping-Pong Framebuffer error: " << fboStatus << std::endl;
	}

	// Create Frame Buffer Object
	unsigned int postProcessingFBO;
	glGenFramebuffers(1, &postProcessingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);

	// Create Framebuffer Texture
	unsigned int postProcessingTexture;
	glGenTextures(1, &postProcessingTexture);
	glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);

	// Create Second Framebuffer Texture
	unsigned int bloomTexture;
	glGenTextures(1, &bloomTexture);
	glBindTexture(GL_TEXTURE_2D, bloomTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bloomTexture, 0);

	// Tell OpenGL we need to draw to both attachments
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	// Error checking framebuffer
	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Post-Processing Framebuffer error: " << fboStatus << std::endl;


	// Framebuffer for Shadow Map
	unsigned int shadowMapFBO;
	glGenFramebuffers(1, &shadowMapFBO);
	unsigned int shadowMapWidth, shadowMapHeight;
	// Texture for Shadow Map FBO
	if (highQualtiyShdows == 1) {
		shadowMapWidth = 3000, shadowMapHeight = 3000;
	}
	else
	{
		shadowMapWidth = 550, shadowMapHeight = 550;

	}
	
	unsigned int shadowMap;
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// Prevents darkness outside the frustrum
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	// Needed since we don't touch the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Matrices needed for the light's perspective
	float farPlane = 200.0f;
	glm::mat4 orthgonalProjection;
	glm::mat4 orthgonalProjectionLow = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, 1.0f, farPlane);
	glm::mat4 orthgonalProjectionHigh = glm::ortho(-70.0f, 70.0f, -70.0f, 70.0f, 1.0f, farPlane);
	//you can change how far shadows go!!! from 10 to 70 and more
	glm::mat4 perspectiveProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);
	if (HighLightView == 0) {
		orthgonalProjection = orthgonalProjectionLow;

	}
	else if (HighLightView == 1) {
		orthgonalProjection = orthgonalProjectionHigh;
	}
	//direc lights
	glm::mat4 lightView = glm::lookAt(140.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightProjection = orthgonalProjection * lightView;

	//-------spot lights
	//glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	//glm::mat4 lightProjection = perspectiveProjection * lightView;

	shadowMapProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
	//0.29, 1.00, 0.62,
	
	ImVec4 backroundColor = ImVec4(0.22, 0.23, 0.25, 1);
	ImVec4 TitleColor = ImVec4(0.43, 0.54, 0.05, 0.95);
	ImVec4 BorderColor = ImVec4(0, 0, 0, 0.2);
	ImVec4 wigitsInsideHover = ImVec4(0.2, 0.2, 0.2, 1);
	ImVec4 wigitsInside = ImVec4(0.3, 0.30, 0.30, 1);
	ImVec4 wigitsInsideActive = ImVec4(0.1, 0.1, 0.10, 1);
	ImVec4 windowWhite = ImVec4(1, 1, 1, 1);

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0;
	style.Colors[ImGuiCol_WindowBg] = backroundColor;
	style.Colors[ImGuiCol_Border] = windowWhite;
	style.Colors[ImGuiCol_CheckMark] = windowWhite;
	style.Colors[ImGuiCol_Text] = windowWhite;
	
	
	
	style.Colors[ImGuiCol_Border] = BorderColor;

	style.Colors[ImGuiCol_FrameBg] = wigitsInside;
	style.Colors[ImGuiCol_FrameBgHovered] = wigitsInsideHover;
	style.Colors[ImGuiCol_FrameBgActive] = wigitsInsideActive;
	style.Colors[ImGuiCol_TitleBg] = TitleColor;
	style.Colors[ImGuiCol_TitleBgActive] = TitleColor;
	style.Colors[ImGuiCol_TabActive] = wigitsInsideActive;
	style.Colors[ImGuiCol_TabHovered] = wigitsInsideHover;
	style.Colors[ImGuiCol_Tab] = wigitsInside;
	style.WindowRounding = 0;
	

	bool v = true;
	if (vsync == 1)
	{
		v = true;
	}
	else {
		v = false;
	}
	
	
	bool wireBool = false;
	if (wireframe == 0) {
		wireBool = false;
	}
	else
	{
		wireBool = true;
	}

	bool lightVLow = false;
	if (HighLightView == 0) {
		lightVLow = false;
	}

	else
	{
		lightVLow = true;
	}

	bool hqs = false;
	if (highQualtiyShdows == 0) {
		hqs = false;
	}
	else
	{
		hqs = true;
	}

	// Create VAO, VBO, and EBO for the skybox
	unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), &skyboxIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	// All the faces of the cubemap (make sure they are in this exact order)
	std::string facesCubemap[6] =
	{
		"skybox/deafult/right.jpg",
		"skybox/deafult/left.jpg",
		"skybox/deafult/top.jpg",
		"skybox/deafult/bottom.jpg",
		"skybox/deafult/front.jpg",
		"skybox/deafult/back.jpg"
	};

	// Creates the cubemap texture object
	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// These are very important to prevent seams
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// This might help with seams on some systems
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Cycles through all the textures and attaches them to the cubemap object
	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D
			(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB,
				width,
				height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				data
			);
			stbi_image_free(data);
		}
		else
		{
		
			std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
			stbi_image_free(data);
		}
	}





	
	// Initialize the Bullet physics engine
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));  // Apply gravity


	

	// Create a falling box
	btCollisionShape* boxShape = new btBoxShape(btVector3(1, 1, 1));
	btDefaultMotionState* boxMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 15, 0)));
	btScalar mass = 1;
	btVector3 boxInertia(0, 0, 0);
	boxShape->calculateLocalInertia(mass, boxInertia);
	btRigidBody::btRigidBodyConstructionInfo boxRigidBodyCI(mass, boxMotionState, boxShape, boxInertia);
	btRigidBody* boxRigidBody = new btRigidBody(boxRigidBodyCI);
	dynamicsWorld->addRigidBody(boxRigidBody);

	
	createStaticBox(dynamicsWorld, btVector3(0, 0, 0), btVector3(1, 1, 1), btQuaternion(0, 0, 0, 1));



	




	// Perform simulation
	const int substep = 10;

	
	
	
	
	const float fixed_timestep = 1.0f / 100.0;
	
	// Main while loop
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_HOME))
	{
		
		

		


		btTransform trans;
		boxRigidBody->getMotionState()->getWorldTransform(trans);
		printf("Box position: %f, %f, %f\n", trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
		btQuaternion rotation = trans.getRotation();
		
		
		
		
		//float randomFloat = static_cast<float>(rand()) / RAND_MAX;
		//std::cout << randomFloat << std::endl;
		// Update the flight controller with user input
		flightController.update(0.1f, window);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			run = false;
		}
		if (wireBool == true && !run) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (run == true || FullCockpit) {

			
			
			
			if (lightVLow == false)
			{
				HighLightView = 1;
			}
			else {
				HighLightView = 0;
			}
			
		} 
		
		if (v == true)
		{
			vsync = 1;
		}
		else {
			vsync = 0;
		}


		if (vsync == 0)
		{
			glfwSwapInterval(0);
		}
		if (vsync == 1)
		{
			glfwSwapInterval(1);
		}
//		camera.Orientation = glm::vec3(-90, -60, 0);
//		camera.Position = glm::vec3(22.0f, 15.0, 0.0f);

		// Updates counter and times
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (timeDiff >= fixed_timestep) {
			// Creates new title
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "Caliber renderer window - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());


			// Resets times and counter
			prevTime = crntTime;
			counter = 0;

			camera.Inputs(window, ctrlSpeed, normalSpeed);
			if (run) {
				dynamicsWorld->stepSimulation(fixed_timestep, substep);
			}
			


		}
		
		




		
		glEnable(GL_DEPTH_TEST);
		
		// Preparations for the Shadow Map
		if (renderShadows == 1 || run || FullCockpit) {
			glViewport(0, 0, shadowMapWidth, shadowMapHeight);
			glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
		}
		glClear(GL_DEPTH_BUFFER_BIT);

		// Draw scene for shadow map
		
		if (renderShadows == 1) {
				
			scene.TRY_DRAWING(ObjectsAmt, sceneObjects, shadowMapProgram, camera);
		}
			
		
	
		
		


		// Switch back to the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Switch back to the default viewport
		glViewport(0, 0, width, height);
		// Bind the custom framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		// Specify the color of the background
		




		

		//camera stack
		//camera2.updateMatrix(60.0f, 0.1f, farPlane);
		


		// Send the light matrix to the shader
		
		shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

		// Bind the Shadow Map
		glActiveTexture(GL_TEXTURE0 + 2);

		glBindTexture(GL_TEXTURE_2D, shadowMap);

		glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowMap"), 2);
		
		// Enable depth testing since it's disabled when the framebuffer rectangle
		glEnable(GL_DEPTH_TEST);
		


		//glClearColor(pow(0.07f, gamma), pow(0.13f, gamma), pow(0.17f, gamma), 1.0f);
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		

		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(60.0f, 0.1f, viewFarPlane);

		//camera stacking
		//calibericon.Draw(shaderProgram, camera2, glm::vec3(0, 0, 0.0f), euler_to_quat(0, 0, 0), glm::vec3(20, 20, 20));

		///Drawing///
		//for (int i = 0; i < objectsAmount; i++)
		//{
			//sceneObjects[i].Draw(shaderProgram, camera, glm::vec3(0, 0, 0.0f), glm::quat(0, 0, 0, 0), glm::vec3(20, 20, 20));
		//}
		scene.TRY_DRAWING(ObjectsAmt, sceneObjects, shaderProgram, camera);

		btVector3 origin = trans.getOrigin();
		sceneObjects[0].translation = glm::vec3(origin.getX(), origin.getY(), origin.getZ());
		sceneObjects[0].rotation = glm::quat(rotation.getW(), rotation.getX(), rotation.getY(), rotation.getZ());



		
		
		

		//SKYBOX
		//------------
		glDepthFunc(GL_LEQUAL);

		skyboxShader.Activate();
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
		// The last row and column affect the translation of the skybox (which we don't want to affect)
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.1f, 500.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Draws the cubemap as the last object so we can save a bit of performance by discarding all fragments
		// where an object is present (a depth of 1.0f will always fail against any object's depth value)
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Switch back to the normal depth function
		glDepthFunc(GL_LESS);
		//------------

		
		//OUTLINE
		//-----------

		// Make it so the stencil test always passes
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		// Enable modifying of the stencil buffer
		glStencilMask(0xFF);
		// Draw the normal model
		//House.Draw(shaderProgram, camera, glm::vec3(0), euler_to_quat(0, 0, 0), glm::vec3(10));
		
		//sphere.Draw(shaderProgram, camera, glm::vec3(0), euler_to_quat(0, 0, 0), glm::vec3(10));
		// Make it so only the pixels without the value 1 pass the test
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		// Disable modifying of the stencil buffer
		glStencilMask(0x00);
		// Disable the depth buffer
		glDisable(GL_DEPTH_TEST);
		// Second method from the tutorial
		outlineShader.Activate();
		glUniform1f(glGetUniformLocation(outlineShader.ID, "outlining"), 0.05f);
		//House.Draw(outlineShader, camera, glm::vec3(0), euler_to_quat(0, 0, 0), glm::vec3(10));
		//sphere.Draw(outlineShader, camera, glm::vec3(0), euler_to_quat(0, 0, 0), glm::vec3(10));

		// Enable modifying of the stencil buffer
		glStencilMask(0xFF);
		// Clear stencil buffer
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		// Enable the depth buffer
		glEnable(GL_DEPTH_TEST);

		//-----------
		

		//rocket.Draw(shaderProgram, camera);
		//rocket.translation = flightController.getPosition();
		//rocket.rotation = flightController.getOrientation();
		//rocket.scale = glm::vec3(1, 1, 1);
		
		// Make it so the multisampling FBO is read while the post-processing FBO is drawn
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFBO);
		// Conclude the multisampling and copy it to the post-processing FBO
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		// Bounce the image data around to blur multiple times
		bool horizontal = true, first_iteration = true;
		int Blur_amount = bloom;
		blurProgram.Activate();
		for (unsigned int i = 0; i < Blur_amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			glUniform1i(glGetUniformLocation(blurProgram.ID, "horizontal"), horizontal);

			// In the first bounc we want to get the data from the bloomTexture
			if (first_iteration)
			{
				glBindTexture(GL_TEXTURE_2D, bloomTexture);
				first_iteration = false;
			}
			// Move the data between the pingPong textures
			else
			{
				glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
			}

			// Render the image
			glBindVertexArray(rectVAO);
			glDisable(GL_DEPTH_TEST);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// Switch between vertical and horizontal blurring
			horizontal = !horizontal;
		}

		// Bind the default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Draw the framebuffer rectangle
		framebufferProgram.Activate();
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
		glActiveTexture(GL_TEXTURE1);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (run || FullCockpit) {
			glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
		}
		else
		{

			glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
		}
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		
		if (run == false) {
			
			ImGui::Begin("Cockpit");
			{
				
				if (ImGui::Button("play"))
				{
					if (run == false) {
						run = true;
					}
					else if (run == true)
					{
						run = false;
					}
				}
				ImGui::BeginChild("VieportRender");
				ImVec2 wsize = ImGui::GetWindowSize();

				ImGui::Image((ImTextureID)postProcessingTexture, wsize, ImVec2(0, 1), ImVec2(1, 0));
				ImGui::EndChild();
			}
			ImGui::End();
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.6, 0.6, 0.6, 1);
			//ImGui::Begin("backround", 0, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			style.Colors[ImGuiCol_WindowBg] = backroundColor;
			

			ImGui::End();

			

			if (ImGui::Begin("project settings")) {
				if (ImGui::BeginTabBar("project tabs"))
				{

					if (ImGui::BeginTabItem("General"))
					{
						ImGui::InputInt("Window width (restart to apply changes)", &mockwidth, 20, 1, 0);
						ImGui::InputInt("Window height (restart to apply changes)", &mockheight, 20, 1, 0);

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Graphics"))
					{
						style.Colors[ImGuiCol_Text] = ImVec4(1, 0, 0, 1);
						ImGui::Text("This will change how your game looks, make sure you have the proper graphics card.");
						style.Colors[ImGuiCol_Text] = windowWhite;

						ImGui::DragInt("MSSA samples (Needs restart to change)", &samples, 0.03f, 1, 40);
						ImGui::Checkbox("Enable vsync", &v);
						ImGui::InputFloat("Gamma correction value", &gamma, 0.3f, 1, "%.3f", 0);

						ImGui::InputFloat("Exposure value", &exposure, 0.3f, 1, "%.3f", 0);
						glUniform1f(glGetUniformLocation(framebufferProgram.ID, "gamma"), gamma);
						//gamma = realGamma;
						glUniform1f(glGetUniformLocation(framebufferProgram.ID, "exposure"), exposure);

						//ImGui::DragInt("bloom amount", &bloom, 0.012f, 0, 40);
						ImGui::Checkbox("Enable shadows", &renderShadows);
						ImGui::Checkbox("Enable high qualtiy shadows (Needs restart to change)", &hqs);

						style.Colors[ImGuiCol_Text] = ImVec4(1, 0, 0, 1);
						ImGui::Text("This will make your shadows go longer with a cost of shadow resolution");
						style.Colors[ImGuiCol_Text] = windowWhite;
						ImGui::Checkbox("Enable high resulotion light view point (Needs restart to change)", &lightVLow);
						
						ImGui::Checkbox("Enable skybox", &enableskybox);


						ImGui::EndTabItem();

						if (ImGui::BeginTabBar("fog"))
						{
							ImGui::InputInt("Far plane view distance", &viewFarPlane, 1, 30);
							ImGui::InputFloat("Fog near value", &FogNear, 0.3f, 1, "%.3f", 0);
							glUniform1f(glGetUniformLocation(shaderProgram.ID, "near"), FogNear);
							glUniform1f(glGetUniformLocation(shaderProgram.ID, "far"), viewFarPlane);

						}
						ImGui::EndTabBar();
					}

					if (ImGui::BeginTabItem("Cockpit settings"))
					{
						ImGui::InputFloat("Shift speed speed", &normalSpeed, 0.3f, 1, "%.3f", 0);
						ImGui::InputFloat("Normal speed", &ctrlSpeed, 0.3f, 1, "%.3f", 0);

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Debug"))
					{
						ImGui::Checkbox("Enable full cockpit", &FullCockpit);
						ImGui::Checkbox("Enable wireframe", &wireBool);
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
					
				}

				ImGui::End();

			}
		}
		
		
			
		if (camera.cinamaticview)
		{
			ImGui::Begin("cinamatic mode tool");
			style.Colors[ImGuiCol_Text] = ImVec4(1, 0, 0, 1);
			ImGui::Text("press : ENTER + N : to leave cinamatic mode (you will be in run mode)");
			style.Colors[ImGuiCol_Text] = windowWhite;
			ImGui::End();
		}
		
		
		
		

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());





		
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
		
	
		

	}
	scene.TRY_OBJ_SORTER_TEST(objectsAmount, sceneObjects);
	// Wait for 10 seconds after the window is closed
	//double desiredTime = glfwGetTime() + 1.0;  // Wait for 1 seconds
	//while (glfwGetTime() < desiredTime) {

	//}

	
	



	if (v == true)
	{
		vsync = 1;
	}
	else {
		vsync = 0;
	}

	if (wireBool == true)
	{
		wireframe = 1;
	}
	else {
		wireframe = 0;
	}
	if (hqs == true)
	{
		highQualtiyShdows = 1;
	}
	else {
		highQualtiyShdows = 0;
	}
	
	
	// Clean up
	dynamicsWorld->removeRigidBody(boxRigidBody);
	delete boxRigidBody->getMotionState();
	delete boxRigidBody;
	delete boxShape;
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
	// Delete all the objects we've created
	shaderProgram.Delete();
	framebufferProgram.Delete();
	shadowMapProgram.Delete();
	blurProgram.Delete();
	skyboxShader.Delete();
	outlineShader.Delete();

	glDeleteFramebuffers(1, &FBO);
	glDeleteFramebuffers(1, &postProcessingFBO);

	// Deletes all ImGUI instances
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


	
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	

	
	std::ofstream SaveFileWr("projectname.caliber");
	

	SaveFileWr << samples << "\n";
	SaveFileWr << vsync << "\n";
	SaveFileWr << gamma * saveFloatCurve << "\n";
	SaveFileWr << exposure * saveFloatCurve << "\n";
	SaveFileWr << bloom << "\n";
	SaveFileWr << renderShadows << "\n";
	SaveFileWr << highQualtiyShdows << "\n";
	SaveFileWr << lightVLow << "\n";
	SaveFileWr << enableskybox << "\n";
	SaveFileWr << mockheight << "\n";
	SaveFileWr << mockwidth << "\n";
	SaveFileWr << wireframe << "\n";
	SaveFileWr << ctrlSpeed * saveFloatCurve << "\n";
	SaveFileWr << normalSpeed * saveFloatCurve << "\n";
	SaveFileWr << FullCockpit << "\n";
	SaveFileWr << FogNear * saveFloatCurve << "\n";
	SaveFileWr << viewFarPlane << "\n";



	return 0;

	
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}











