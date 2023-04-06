
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
#include"src/CSF.h"
#include"src/CSV.h"
#include"src/Setup.h"
#include"src/Save.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "src/Presave.h"
#include"src/Console.h"
#include"src/IMGUI_Themes.h"
Console console;

CSV vert;
CSF frag;
Functions func;
FlightController flightController;
Setup setup;
Save sve;
//Component scene;
// Create a new component object
//Component component;

// Add an object to the component
//component.AddObject("path/to/object.obj");


const float WorldRadius = 70;
const float objectWorldMult = 2;

bool run = false; 


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
std::array save = {1};
//finished









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
int saveFloatCurve = 100000;

float delta_time() {
	static double previous_time = glfwGetTime();

	double current_time = glfwGetTime();
	double elapsed_time = current_time - previous_time;
	previous_time = current_time;

	return static_cast<float>(elapsed_time);
}

void createStaticBox(btDynamicsWorld* dynamicsWorld, btVector3 position, btVector3 scale, btQuaternion rotation)
{
	btCollisionShape* boxShape = new btBoxShape(scale / objectWorldMult);
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
	
	Presave<float> myData;

	myData = Presave < float > ();
	myData.SafeOperation();

	bool vsync = myData.data[0];
	bool renderShadows = myData.data[1];
	int msaaSamples = myData.data[2];
	int bloom = myData.data[3];
	bool wireframe = myData.data[4];
	int width = myData.data[5];
	int height = myData.data[6];
	float gamma = myData.data[7];
	float exposure = myData.data[8];
	float normalSpeed = myData.data[9];
	float ctrlSpeed = myData.data[10];
	bool enableskybox = myData.data[11];
	float fogNear = myData.data[12];
	float viewFarPlane = myData.data[13];
	bool bakeShadows = myData.data[14];
	static int colorChoice = myData.data[15];

	float FXAA_SPAN_MAX = myData.data[16];
	float FXAA_REDUCE_MIN = myData.data[17];
	float FXAA_REDUCE_MUL = myData.data[18];

	float BloomSpreadBlur = myData.data[19];

	int shadowMapWidth = myData.data[20];
	int	shadowMapHeight = myData.data[21];
	int	shadowSampleRadius = myData.data[22];
	float DepthBias1 = myData.data[23];
	float DepthBias2 = myData.data[24];
	
	// Initialize GLFW
	setup.SETUP_GLFW();
	

	
	
	//GLFWwindow* window = glfwCreateWindow(width, height, "Caliber window", glfwGetPrimaryMonitor(), NULL);
	
	
	
	GLFWwindow* window = glfwCreateWindow(width, height, "Loading Caliber Engine...", NULL, NULL);

	
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

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	if (setup.IsError)
	{
		return -1;
	}


	
	//load Icon
	int wid, hei;
	int channels;
	//!rememer! make sure to install the icon
	unsigned char* pixels = stbi_load("Icon.png", &wid, &hei, &channels, 4);

	//change icon
	GLFWimage images[1];
	images[0].width = wid;
	images[0].height = hei;
	images[0].pixels = pixels;
	glfwSetWindowIcon(window, 1, images);
	

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, width, height);


	setup.SETUP_IMGUI(window);
	// Generates shaders
	Shader shaderProgram(vert.Default, frag.Default);
	Shader unlitProgram(vert.Default, frag.Unlit);
	Shader framebufferProgram(vert.Frame, frag.Frame);
	Shader shadowMapProgram(vert.ShadowMap, frag.NONE);
	Shader UniversalDepthProgram(vert.Default, frag.NONE);
	Shader blurProgram(vert.Frame, frag.Blur);
	Shader skyboxShader(vert.Skybox, frag.Skybox);

	Shader outlineShader(vert.Outline, frag.Ouline);


	

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 0.8f, 0.6f, 1.0f) * 3.0f;

	blurProgram.Activate();
	glUniform1f(glGetUniformLocation(blurProgram.ID, "spreadBlur"), BloomSpreadBlur);

	glm::vec3 lightPos = glm::vec3(0.5f, 1, 0.5f);
	unlitProgram.Activate();
	glUniform4f(glGetUniformLocation(unlitProgram.ID, "color"), 0, 1, 0, 1);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "near"), fogNear);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "far"), viewFarPlane);


	glUniform1f(glGetUniformLocation(shaderProgram.ID, "worldRadius"), WorldRadius);

	glUniform1f(glGetUniformLocation(shaderProgram.ID, "bias1"), DepthBias1);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "bias2"), DepthBias2);

	glUniform1i(glGetUniformLocation(shaderProgram.ID, "sampleRadius"), shadowSampleRadius);
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
	//AA
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "minEdgeContrast"), FXAA_REDUCE_MIN);
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "subPixelAliasing"), FXAA_REDUCE_MUL);
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "maximumEdgeDetection"), FXAA_SPAN_MAX);

	glUniform2f(glGetUniformLocation(framebufferProgram.ID, "resolution"), width, height);

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
	Camera camera(width, height, glm::vec3(0, 0, 0.0f));


	//camera stacking
	//Camera camera2(width, height, glm::vec3(22.0f, 15.0, 0.0f));
	

	
	
	
	
	// Load in models

	
	

	Component scene;
	Model sceneObjects[] = { Model("models/ExampleModel/scene.gltf"),
		};
	Model GizmosBoundry = ("models/Gizmos/BoundSphere/scene.gltf");

	//glm::vec3 hello = sve.loadVec3("work.txt", 0);

	std::string ModelLine;
	for (size_t i = 1; i < sizeof(sceneObjects) / sizeof(sceneObjects[0]) + 1; i++)
	{

	
	

		//save[i] = std::stof(ModelLine);
		sceneObjects[i].translation = sve.loadVec3("Metric/world.metric", i);
		sceneObjects[i].rotation = sve.loadVec4("Metric/orian.metric", i);
		sceneObjects[i].scale = sve.loadVec3("Metric/scale.metric", i);
		i++;
	}

		

	


	//sceneObjects[2].scale = glm::vec3(2.0f);
	
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
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaaSamples, GL_RGB16F, width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture, 0);

	// Create Render Buffer Object
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaSamples, GL_DEPTH24_STENCIL8, width, height);
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
	// Texture for Shadow Map FBO
	

	//shadowMapWidth = 15000, shadowMapHeight = 15000;

	
	

	
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

	
	//glm::mat4 orthgonalProjection = glm::ortho(-WorldRadius, WorldRadius, -WorldRadius, WorldRadius, WorldRadius, farPlane);
	//glm::mat4 orthgonalProjectionLow = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, 1.0f, farPlane);,ks89oxb
	//glm::mat4 perspectiveProjection = glm::perspective(glm::radians(30.0f), 1.0f, 0.1f, farPlane);

	//0.29, 1.00, 0.62,
	
	DefaultTheme();
	


	

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

	


	btCollisionShape* sphereShape = new btBoxShape(btVector3(0.3, 1.5, 0.3)); // replace btBoxShape with btSphereShape and the size parameter with 1
	btDefaultMotionState* sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))); // replace boxMotionState with sphereMotionState
	
	btVector3 sphereInertia(0, 0, 0);
	sphereShape->calculateLocalInertia(1, sphereInertia);
	btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(1, sphereMotionState, sphereShape, sphereInertia);
	btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
	dynamicsWorld->addRigidBody(sphereRigidBody); // replace boxRigidBody with sphereRigidBody
	sphereRigidBody->setSleepingThresholds(0,0);
	sphereRigidBody->setActivationState(DISABLE_DEACTIVATION);


	// Perform simulation
	const int substep = 10;
	Model PhysicsCube("models/cube/scene.gltf");

	PhysicsCube.phys.setOrigin(btVector3(0, 20, 0));
	PhysicsCube.scale = glm::vec3(4, 4, 4);
	PhysicsCube.BindPhysics(dynamicsWorld, objectWorldMult, true);
	PhysicsCube.PHYSICS_SETUP();
	


	
	//camera.getInputAtRun = true;
	// Main while loop
	
	bool bake = true;


	GLuint UniversalDepthframebuffer;
	glGenFramebuffers(1, &UniversalDepthframebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, UniversalDepthframebuffer);

	// Create a depth texture
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Attach the depth texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// Check if the framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		// handle error
	}

	const float fixed_timestep = 1.0f / 120.0;
	
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_HOME))
	{
		if (bakeShadows && bake)
		{
			renderShadows = true;
			bake = false;
		}
		else if ((bakeShadows && !bake)) {
			renderShadows = false;
		}

		sceneObjects[2].translation = glm::vec3(70);
		// Check if the user has entered a specific string
		
		if (strcmp(console.input_buf, "quit") == 0 && glfwGetKey(window, GLFW_KEY_ENTER))
		{
				// The user has written the "quit" string
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		else
		{
				// The user has written a different string
				// ...
		}

			


		
		//sceneObjects[1].UpdatePhysics();
		
		
		//float randomFloat = static_cast<float>(rand()) / RAND_MAX;
		//std::cout << randomFloat << std::endl;
		// Update the flight controller with user input

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			run = false;
		}
		if (wireframe == true && !run) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			
		}



		if (vsync == 0)
		{
			glfwSwapInterval(0);
		}
		if (vsync == 1)
		{
			glfwSwapInterval(1);
		}
		
		// Updates counter and times
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		if (!run) {
			camera.TrackBallMouse(window);
		}
		else {

			camera.Mouse(window);
		}

		if (timeDiff >= fixed_timestep) {
			// Creates new title
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "Caliber renderer window - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			// Resets times and counter
			prevTime = crntTime;
			counter = 0;
			
			if (run) {
				camera.Inputs(window, (ctrlSpeed)*fixed_timestep, (normalSpeed)*fixed_timestep);

			}
			else {
				camera.Trackaballmovement(window, (ctrlSpeed)*fixed_timestep, (normalSpeed)*fixed_timestep);
			}
			
			

			if (run) {

				btTransform currentTransform;
				sphereRigidBody->getMotionState()->getWorldTransform(currentTransform);
				btVector3 currentPosition = currentTransform.getOrigin();

				// Calculate the desired camera position, independent of the physics simulation
				glm::vec3 desiredCameraPosition = camera.Position;

				// Calculate the required velocity to move the sphere rigid body to the desired camera position
				btVector3 desiredPosition = btVector3(desiredCameraPosition.x, desiredCameraPosition.y, desiredCameraPosition.z);
				btVector3 velocity = (desiredPosition - currentPosition) / (fixed_timestep);

				// Apply the velocity to the sphere rigid body
				sphereRigidBody->setLinearVelocity(velocity);
				sphereRigidBody->setAngularVelocity(btVector3(0, 0, 0));
				sphereRigidBody->setAngularFactor(btVector3(0, 0, 0));

				sphereRigidBody->setGravity(btVector3(0, 0, 0));

				// Step the simulation
				dynamicsWorld->stepSimulation(fixed_timestep, substep);

				// Update the camera position based on the new position of the sphere
				btTransform updatedTransform;
				sphereRigidBody->getMotionState()->getWorldTransform(updatedTransform);
				camera.Position = glm::vec3(updatedTransform.getOrigin().getX(), updatedTransform.getOrigin().getY(), updatedTransform.getOrigin().getZ());

			}




			float deadZone = WorldRadius / 100000000000000000; // The size of the dead zone around the edge of the radius

			if (glm::length(camera.Position - glm::vec3(0)) > WorldRadius  - deadZone) {
				// Camera is outside of the boundary, set position back to the edge of the radius
				glm::vec3 direction = glm::normalize(camera.Position - glm::vec3(0));
				camera.Position = glm::vec3(0) + direction * (WorldRadius - deadZone);
			}


			btVector3 rayFromWorld(camera.Position.x, camera.Position.y, camera.Position.x); // starting point of the ray
			btVector3 rayToWorld(camera.Position.x, camera.Position.y - 1, camera.Position.x); // ending point of the ray

			btCollisionWorld::ClosestRayResultCallback rayCallback(rayFromWorld, rayToWorld);
			dynamicsWorld->rayTest(rayFromWorld, rayToWorld, rayCallback);

			if (rayCallback.hasHit()) {
				printf("Collision!!!!!");
			}



			GizmosBoundry.scale = glm::vec3(WorldRadius);
		}


		
		
		
		glm::mat4 orthgonalProjection = glm::ortho(-WorldRadius, WorldRadius, -WorldRadius, WorldRadius, 0.1f, viewFarPlane);
		//direc lights

		glm::mat4 lightView = glm::lookAt(20.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightProjection = orthgonalProjection * lightView;

		
		glEnable(GL_DEPTH_TEST);
		
		// Preparations for the Shadow Map
		if (renderShadows) {



			shadowMapProgram.Activate();
			glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

			//-------spot lights
			//glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
			//glm::mat4 lightProjection = perspectiveProjection * lightView;

			glViewport(0, 0, shadowMapWidth, shadowMapHeight);
			glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
		
			glClear(GL_DEPTH_BUFFER_BIT);

			
		
				
			scene.TRY_DRAWING(sizeof(sceneObjects) / sizeof(sceneObjects[0]), sceneObjects, shadowMapProgram, camera, objectWorldMult);
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
		if (renderShadows) {
			// Bind the Shadow Map
			glActiveTexture(GL_TEXTURE0 + 2);

			glBindTexture(GL_TEXTURE_2D, shadowMap);

			glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowMap"), 2);
		}
		// Enable depth testing since it's disabled when the framebuffer rectangle
		glEnable(GL_DEPTH_TEST);
		


		//glClearColor(pow(0.07f, gamma), pow(0.13f, gamma), pow(0.17f, gamma), 1.0f);
		glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
		
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix3D(60.0f, 0.1f, viewFarPlane);
		\
		//camera.updateMatrix2D(0.005f,0.4f, viewFarPlane);


		//camera stacking
		//calibericon.Draw(shaderProgram, camera2, glm::vec3(0, 0, 0.0f), euler_to_quat(0, 0, 0), glm::vec3(20, 20, 20));

		///Drawing///
		//for (int i = 0; i < objectsAmount; i++)
		//{
			//sceneObjects[i].Draw(shaderProgram, camera, glm::vec3(0, 0, 0.0f), glm::quat(0, 0, 0, 0), glm::vec3(20, 20, 20));
		//}

		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, UniversalDepthframebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Drawing depth
		scene.TRY_DRAWING(sizeof(sceneObjects) / sizeof(sceneObjects[0]), sceneObjects, UniversalDepthProgram, camera, objectWorldMult);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		//Normal scene drawing
		scene.TRY_DRAWING(sizeof(sceneObjects) / sizeof(sceneObjects[0]), sceneObjects, shaderProgram, camera, objectWorldMult);
		GizmosBoundry.Draw(shaderProgram, camera, 1);

		

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(5.0f);
		

		PhysicsCube.Draw(unlitProgram, camera, objectWorldMult);
		PhysicsCube.PhysicsUpdate(true);
		PhysicsCube.PHYSICS_SETUP();
		PhysicsCube.scale = glm::vec3(4, 4, 4);


		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(1.0f);
		//sceneObjects[1].translation = glm::vec3(0, 1, 0);
		
		
		
		
		//sceneObjects[1].translation = glm::vec3(cameratrans.getOrigin().getX(), cameratrans.getOrigin().getY(), cameratrans.getOrigin().getZ());
		//sceneObjects[1].rotation = glm::quat(cameratrans.getRotation().getW(), cameratrans.getRotation().getX(), cameratrans.getRotation().getY(), cameratrans.getRotation().getZ());
		

		//SKYBOX
		//------------
		if (enableskybox) {
			glDepthFunc(GL_LEQUAL);

			skyboxShader.Activate();
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
			// We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and columns
			// The last row and column affect the translation of the skybox (which we don't want to affect)
			view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
			projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.1f, viewFarPlane);
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
		}
		
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
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		console.Draw();
		if (run == false) {
			
			ImGui::Begin("Viewport");
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
				ImGui::BeginTabBar("Viewport");
				if (ImGui::BeginTabItem("Render")) {
					ImGui::BeginChild("ViewportRender");
					ImGui::Image((ImTextureID)postProcessingTexture, ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Shadow framebuffer")) {
					ImGui::BeginChild("ViewportRender");
					ImGui::Image((ImTextureID)shadowMap, ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Depth framebuffer")) {
					ImGui::BeginChild("ViewportRender");
					ImGui::Image((ImTextureID)depthTexture, ImGui::GetWindowSize(), ImVec2(0, 1), ImVec2(1, 0));
					ImGui::EndChild();
					ImGui::EndTabItem();
				}

			}
			ImGui::End();


			ImGui::Begin("Preferences");
			{

				ImGui::BeginTabBar("Preferences Bar");
				if (ImGui::BeginTabItem("Styles")) {
					

					if (ImGui::RadioButton("Default", &colorChoice, 0) || colorChoice == 0)
					{
						DefaultTheme();
					}
					if (ImGui::RadioButton("Light", &colorChoice, 1) || colorChoice == 1)
					{
						LightTheme();
					}
					if (ImGui::RadioButton("Future", &colorChoice, 2) || colorChoice == 2)
					{
						FutureTheme();
					}
					if (ImGui::RadioButton("Hacker", &colorChoice, 3) || colorChoice == 3)
					{
						HackerTheme();
					}
					if (ImGui::RadioButton("Cyan_Blue", &colorChoice, 4) || colorChoice == 4)
					{
						Cyan_Blue_Theme();
					}
					if (ImGui::RadioButton("EyeSore (For all of you weirdoz)", &colorChoice, 5) || colorChoice == 5)
					{
						EyESoRETheme();
					}

				}

			}
			ImGui::End();

			ImGui::Begin("background", 0, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

			


			

			if (ImGui::Begin("project settings")) {
				if (ImGui::BeginTabBar("project tabs"))
				{

					if (ImGui::BeginTabItem("Graphics"))
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
						ImGui::Text("This will change how your game looks, make sure you have the proper graphics card.");
						ImGui::PopStyleColor();

						if (ImGui::CollapsingHeader("Anti-Aliasing", ImGuiTreeNodeFlags_DefaultOpen)) {
							ImGui::Columns(2, nullptr, true);

							ImGui::Text("Multisample Anti-Aliasing (Needs restart to change)");
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
							ImGui::Text("MSAA Needs restart to apply it.");
							ImGui::PopStyleColor();
							ImGui::InputInt("MSSA Samples", &msaaSamples);

							ImGui::NextColumn();
							ImGui::Text("Fast Approximate Anti-Aliasing");
							ImGui::InputFloat("FXAA_SPAN_MAX", &FXAA_SPAN_MAX);
							ImGui::InputFloat("FXAA_REDUCE_MIN", &FXAA_REDUCE_MIN);
							ImGui::InputFloat("FXAA_REDUCE_MUL", &FXAA_REDUCE_MUL);
							glUniform1f(glGetUniformLocation(framebufferProgram.ID, "minEdgeContrast"), FXAA_REDUCE_MIN);
							glUniform1f(glGetUniformLocation(framebufferProgram.ID, "subPixelAliasing"), FXAA_REDUCE_MUL);
							glUniform1f(glGetUniformLocation(framebufferProgram.ID, "maximumEdgeDetection"), FXAA_SPAN_MAX);
							ImGui::Columns(1, nullptr, false);
						}
						
						ImGui::Separator();

						if (ImGui::CollapsingHeader("Post-Processing")) {
							ImGui::InputFloat("Gamma correction value", &gamma, 0.3f, 1, "%.3f", 0);
							ImGui::InputFloat("Exposure value", &exposure, 0.3f, 1, "%.3f", 0);
							glUniform1f(glGetUniformLocation(framebufferProgram.ID, "Gamma"), gamma);
							glUniform1f(glGetUniformLocation(framebufferProgram.ID, "Exposure"), exposure);
							if (ImGui::CollapsingHeader("Bloom settings")) {
								ImGui::InputInt("Bloom Amount", &bloom, 1, 100);
								ImGui::InputFloat("Bloom Spread", &BloomSpreadBlur);
								glUniform1f(glGetUniformLocation(blurProgram.ID, "spreadBlur"), BloomSpreadBlur);
							}


						}
						ImGui::Separator();


						if (ImGui::CollapsingHeader("Shadow's", ImGuiTreeNodeFlags_DefaultOpen)) {

							ImGui::InputInt("Shadow Map Width", &shadowMapWidth, 1, 100);
							ImGui::InputInt("Shadow Map Height", &shadowMapHeight, 1, 100);
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
							ImGui::Text("Baking the shadow's will not improve the quality it will only freeze the texture");
							ImGui::PopStyleColor();
							ImGui::Checkbox("Bake/Static Shadows", &bakeShadows);
							if (!bakeShadows) {
								ImGui::Checkbox("Enable shadows", &renderShadows);
							}
							ImGui::InputInt("Sample Radius", &shadowSampleRadius, 0, 100);

							ImGui::InputFloat("Depth Bias 1", &DepthBias1);
							ImGui::InputFloat("Depth Bias 2", &DepthBias2);

							
						}
						ImGui::Separator();

						if (ImGui::CollapsingHeader("Atmospheric")) {



				
							ImGui::SliderFloat("Fog Near Value", &fogNear, 0, 3, "%.3f", 0);
							ImGui::InputFloat("Far Plane View distance", &viewFarPlane, 1, 30);
							shaderProgram.Activate();
							glUniform1f(glGetUniformLocation(shaderProgram.ID, "near"), fogNear);
							glUniform1f(glGetUniformLocation(shaderProgram.ID, "far"), viewFarPlane);


							ImGui::Checkbox("Enable Skybox", &enableskybox);
						}
						ImGui::Separator();

						if (ImGui::CollapsingHeader("Screen", ImGuiTreeNodeFlags_DefaultOpen)) {
							ImGui::Checkbox("Enable Vsync", &vsync);
						}

						

						

						

						


						ImGui::EndTabItem();

					}

					if (ImGui::BeginTabItem("Viewport settings"))
					{
						ImGui::InputFloat("Shift Speed", &normalSpeed, 0.3f, 1, "%.3f", 0);
						ImGui::InputFloat("Normal Speed", &ctrlSpeed, 0.3f, 1, "%.3f", 0);

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Debug"))
					{
						ImGui::Checkbox("Enable Wireframe", &wireframe);
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
					
				}

				ImGui::End();

			}
		}
		
		
			
		
		
		
		
		

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());




		
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
		
	
		

	}

	//ISSUE! THE MODEL IS BEING SAVED TWICE
	// ___________________________________
	//sve.clearFile("Metric/world.metric");
	remove("Metric/world.metric");
	//sve.clearFile("Metric/orian.metric");
	remove("Metric/orian.metric");
	//sve.clearFile("Metric/scale.metric");
	remove("Metric/scale.metric");
	size_t numObjects = sizeof(sceneObjects) / sizeof(sceneObjects[0]);
	for (size_t i = 0; i < numObjects; i++)
	{

		sve.saveVec3(sceneObjects[i].scale, "Metric/scale.metric");
		sve.saveVec3(sceneObjects[i].translation, "Metric/world.metric");
		sve.saveVec4(sceneObjects[i].rotation, "Metric/orian.metric");
	}
	myData.data = { float(vsync), float(renderShadows), float(msaaSamples), float(bloom), float(wireframe),
					float(width), float(height), gamma, exposure, normalSpeed, ctrlSpeed, float(enableskybox),
					fogNear, viewFarPlane, float(bakeShadows), float(colorChoice), FXAA_SPAN_MAX, FXAA_REDUCE_MIN, FXAA_REDUCE_MUL, BloomSpreadBlur, 
	float(shadowMapWidth), float(shadowMapHeight), float(shadowSampleRadius), DepthBias1, DepthBias2};
	
	myData.saveData();
	
	
	
	// Clean up
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

	



	return 0;

	
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}











