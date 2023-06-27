
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
#include"src/CSF.h"
#include"src/CSV.h"
#include"src/Setup.h"
#include"src/Save.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "src/Presave.h"
#include"src/Console.h"
#include"src/IMGUI_Themes.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include <stb_image_write.h>
#include"src/UIIMGUI.h"
#include<thread>
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

static glm::vec3 Deg(const glm::vec3& radians)
{
	return glm::vec3(radians.x * 180.0f / 3.14159f,
		radians.y * 180.0f / 3.14159f,
		radians.z * 180.0f / 3.14159f);
}


const float WorldRadius = 700;
const float objectWorldMult = 2;

bool run = false; 



void saveScreenshot(GLFWwindow* window, const char* filename) {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	unsigned char* pixels = new unsigned char[width * height * 4];
	glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	for (int y = 0; y < height / 2; ++y) {
		for (int x = 0; x < width; ++x) {
			for (int c = 0; c < 4; ++c) {
				std::swap(pixels[(y * width + x) * 4 + c], pixels[((height - y - 1) * width + x) * 4 + c]);
			}
		}
	}

	std::thread saveThread([=]() {
		//stbi_write_png(filename, width, height, 4, pixels, 0);
		delete[] pixels;
		});

	saveThread.detach();
}


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
	1, 2, 6,
	6, 5, 1,
	0, 4, 7,
	7, 3, 0,
	4, 5, 6,
	6, 7, 4,
	0, 3, 2,
	2, 1, 0,
	0, 1, 5,
	5, 4, 0,
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

bool checkMouseOverObject(const glm::vec3& position, const glm::vec3& cameraPosition, const glm::vec3& cameraOrientation, float width, float height, GLFWwindow* window) {
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraPosition + cameraOrientation, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), width / height, 0.1f, 100.0f);
	glm::vec4 viewport = glm::vec4(0, 0, width, height);

	glm::vec3 screenPos = glm::project(position, viewMatrix, projectionMatrix, viewport);
	screenPos.y = height - screenPos.y; // Invert y-axis to match OpenGL convention

	glm::vec3 mousePos3D = glm::unProject(glm::vec3(mouseX, height - mouseY, screenPos.z), viewMatrix, projectionMatrix, viewport);

	float distance = glm::length(mousePos3D - position);

	float radius = 0.6f;
	if (distance <= radius) {
		return true;
	}
	return false;
}

bool isMouseDragging = false;
glm::vec2 initialMousePosition;


void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			initialMousePosition = glm::vec2(xpos, ypos);
			isMouseDragging = true;
		}
		else if (action == GLFW_RELEASE) {

			isMouseDragging = false;
			
		}
	}
}



glm::vec3 moveObjectInXAxis(GLFWwindow* window, const glm::vec3& objectPosition, const glm::vec3& cameraOrientation, const glm::vec3& cameraPosition) {
	if (!isMouseDragging) {
		return objectPosition;
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glm::vec2 currentMousePosition(xpos, ypos);

	double deltaX = currentMousePosition.x - initialMousePosition.x;
	float sensitivity = 0.0011f;

	float distance = glm::distance(objectPosition, cameraPosition);

	sensitivity *= distance;

	glm::vec3 updatedObjectPosition = objectPosition;
	updatedObjectPosition.x += static_cast<float>(deltaX * sensitivity);

	initialMousePosition = currentMousePosition;

	return updatedObjectPosition;
}

glm::vec3 moveObjectInZAxis(GLFWwindow* window, const glm::vec3& objectPosition, const glm::vec3& cameraOrientation, const glm::vec3& cameraPosition) {
	if (!isMouseDragging) {
		return objectPosition;
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glm::vec2 currentMousePosition(xpos, ypos);

	glm::vec2 deltaXY = currentMousePosition - initialMousePosition;
	float sensitivity = 0.0011f;

	glm::mat4 cameraRotation = glm::mat4(glm::quat(glm::radians(cameraOrientation)));

	glm::vec3 localDelta = glm::vec3(deltaXY.x, deltaXY.y, 0.0f) * sensitivity;
	glm::vec3 globalDelta = glm::vec3(cameraRotation * glm::vec4(localDelta, 0.0f));

	glm::vec3 cameraForward = glm::normalize(cameraPosition - objectPosition);
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraForward, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 cameraUp = glm::cross(cameraRight, cameraForward);

	float elevationAngle = glm::degrees(std::asin(cameraUp.y)); 

	glm::vec2 projectedDelta(glm::dot(deltaXY, glm::vec2(cameraRight.x, cameraUp.x)),
		glm::dot(deltaXY, glm::vec2(cameraRight.y, cameraUp.y)));

	projectedDelta.y *= std::cos(glm::radians(elevationAngle)); 

	float distance = glm::distance(objectPosition, cameraPosition);
	sensitivity *= distance;

	glm::vec3 delta = globalDelta.x * cameraRight + globalDelta.y * cameraUp;
	delta.z = -glm::dot(deltaXY, glm::normalize(glm::vec2(cameraForward.x, cameraForward.y))) * sensitivity;

	glm::vec3 updatedObjectPosition = objectPosition + delta;

	initialMousePosition = currentMousePosition;

	return updatedObjectPosition;
}





glm::vec3 moveObjectInYAxis(GLFWwindow* window, const glm::vec3& objectPosition, const glm::vec3& cameraOrientation, const glm::vec3& cameraPosition) {
	if (!isMouseDragging) {
		return objectPosition;
	}

	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glm::vec2 currentMousePosition(xpos, ypos);

	double deltaY = currentMousePosition.y - initialMousePosition.y;
	float sensitivity = 0.0011f;

	float distance = glm::distance(objectPosition, cameraPosition);

	sensitivity *= distance;

	glm::vec3 updatedObjectPosition = objectPosition;
	updatedObjectPosition.y -= static_cast<float>(deltaY * sensitivity);

	initialMousePosition = currentMousePosition;

	return updatedObjectPosition;
}



#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>




int main()
{
	
	
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
	float highCameraSpeed = myData.data[9];
	float cameraNormalSpeed = myData.data[10];
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
	float avgShadow = myData.data[25];

	bool BPL_LIGHTING = myData.data[26];
	
	bool no_resize = true;
	bool no_move = true;
	setup.SETUP_GLFW();
	

	
	
	//GLFWwindow* window = glfwCreateWindow(width, height, "Caliber window", glfwGetPrimaryMonitor(), NULL);
	
	
	GLFWwindow* window = glfwCreateWindow(width, height, "Loading Caliber Engine...", NULL, NULL);
	//glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1; 
	}
	glfwMakeContextCurrent(window);
	glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
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



	
	int wid, hei;
	int channels;
	unsigned char* pixels = stbi_load("MainIcon.png", &wid, &hei, &channels, 4);

	GLFWimage images[1];
	images[0].width = wid;
	images[0].height = hei;
	images[0].pixels = pixels;
	glfwSetWindowIcon(window, 1, images);
	
	gladLoadGL();
	glViewport(0, 0, width, height);

	console.log(reinterpret_cast<const char*>(glGetString(GL_VERSION)));


	std::string rendererString = "Using : ";
	rendererString += reinterpret_cast<const char*>(glGetString(GL_RENDERER));
	console.log(rendererString.c_str());

	setup.SETUP_IMGUI(window);
	Shader shaderProgram(vert.Default, frag.Default);
	Shader unlitProgram(vert.Default, frag.Unlit);
	Shader framebufferProgram(vert.Frame, frag.Frame);
	Shader shadowMapProgram(vert.ShadowMap, frag.NONE);
	Shader UniversalDepthProgram(vert.Default, frag.NONE);
	Shader blurProgram(vert.Frame, frag.Blur);
	Shader skyboxShader(vert.Skybox, frag.Skybox);

	Shader outlineShader(vert.Outline, frag.Ouline);


	

	glm::vec4 lightColor = glm::vec4(1.0f, 0.8f, 0.6f, 1.0f) * 3.0f;

	blurProgram.Activate();
	glUniform1f(glGetUniformLocation(blurProgram.ID, "spreadBlur"), BloomSpreadBlur);

	glm::vec3 lightPos = glm::vec3(0.5f, 1.0f, 0.0f);
	unlitProgram.Activate();
	glUniform4f(glGetUniformLocation(unlitProgram.ID, "color"), 0, 1, 0, 1);

	shaderProgram.Activate();
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "avgShadow"), avgShadow);
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "near"), fogNear);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "far"), viewFarPlane);

	glUniform1i(glGetUniformLocation(shaderProgram.ID, "BPL_Lighting"), BPL_LIGHTING);


	glUniform1f(glGetUniformLocation(shaderProgram.ID, "worldRadius"), WorldRadius);

	glUniform1f(glGetUniformLocation(shaderProgram.ID, "bias1"), DepthBias1);
	glUniform1f(glGetUniformLocation(shaderProgram.ID, "bias2"), DepthBias2);


	glUniform1i(glGetUniformLocation(shaderProgram.ID, "sampleRadius"), shadowSampleRadius);
	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);


	

	framebufferProgram.Activate();
	glUniform1i(glGetUniformLocation(framebufferProgram.ID, "screenTexture"), 0);
	glUniform1i(glGetUniformLocation(framebufferProgram.ID, "bloomTexture"), 1);
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "gamma"), gamma);
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "exposure"), exposure);
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "minEdgeContrast"), FXAA_REDUCE_MIN);
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "subPixelAliasing"), FXAA_REDUCE_MUL);
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "maximumEdgeDetection"), FXAA_SPAN_MAX);

	glUniform2f(glGetUniformLocation(framebufferProgram.ID, "resolution"), width, height);

	blurProgram.Activate();
	glUniform1i(glGetUniformLocation(blurProgram.ID, "screenTexture"), 0);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	Camera camera(width, height, glm::vec3(0, 0, 0.0f));
	//camera stacking
	//Camera camera2(width, height, glm::vec3(22.0f, 15.0, 0.0f));

	std::vector<Model> sceneObjects;
	Model gird("models/grid/scene.gltf");

	std::ifstream calibFile("world.caliber");
	std::string line;
	while (std::getline(calibFile, line)) {
		std::string path = "models/cube/scene.gltf";
		glm::vec3 pos;
		glm::vec3 rot;
		glm::vec3 sca;
		std::istringstream iss(line);

		std::string token;
		std::getline(iss, token, ',');
		pos.x = std::stof(token);
		std::getline(iss, token, ',');
		pos.y = std::stof(token);
		std::getline(iss, token, ',');
		pos.z = std::stof(token);

		std::getline(iss, token, ',');
		rot.x = std::stof(token);
		std::getline(iss, token, ',');
		rot.y = std::stof(token);
		std::getline(iss, token, ',');
		rot.z = std::stof(token);

		std::getline(iss, token, ',');
		sca.x = std::stof(token);
		std::getline(iss, token, ',');
		sca.y = std::stof(token);
		std::getline(iss, token, ',');
		sca.z = std::stof(token);
		std::getline(iss, token, ',');
		path = token.c_str();

		Model obj = Model(path.c_str());


		obj.translation = pos;
		obj.rotation = rot;
		obj.scale = sca;
		sceneObjects.push_back(obj);
	}

	Model GizmosBoundry = ("models/Gizmos/BoundSphere/scene.gltf");
	Model GizmosSphere = ("models/Gizmos/Sphere/scene.gltf");


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



	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;

	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);




	unsigned int framebufferTexture;
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, msaaSamples, GL_RGB16F, width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture, 0);

	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaaSamples, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

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

	unsigned int postProcessingFBO;
	glGenFramebuffers(1, &postProcessingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);

	unsigned int postProcessingTexture;
	glGenTextures(1, &postProcessingTexture);
	glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);

	unsigned int bloomTexture;
	glGenTextures(1, &bloomTexture);
	glBindTexture(GL_TEXTURE_2D, bloomTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bloomTexture, 0);

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Post-Processing Framebuffer error: " << fboStatus << std::endl;


	unsigned int shadowMapFBO;
	glGenFramebuffers(1, &shadowMapFBO);
	
	unsigned int shadowMap;
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 orthgonalProjection = glm::ortho(-WorldRadius, WorldRadius, -WorldRadius, WorldRadius, 0.1f, viewFarPlane);
	glm::mat4 lightView = glm::lookAt(20.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightProjection = orthgonalProjection * lightView;
	
	DefaultTheme();
	


	
	
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


	std::string facesCubemap[6] =
	{
		"skybox/deafult/right.jpg",
		"skybox/deafult/left.jpg",
		"skybox/deafult/top.jpg",
		"skybox/deafult/bottom.jpg",
		"skybox/deafult/front.jpg",
		"skybox/deafult/back.jpg"
	};

	unsigned int cubemapTexture;
	glGenTextures(1, &cubemapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
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





	
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

	


	btCollisionShape* sphereShape = new btBoxShape(btVector3(0.3, 0.3, 0.3));
	btDefaultMotionState* sphereMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))); 
	
	btVector3 sphereInertia(0, 0, 0);
	sphereShape->calculateLocalInertia(1, sphereInertia);
	btRigidBody::btRigidBodyConstructionInfo sphereRigidBodyCI(1, sphereMotionState, sphereShape, sphereInertia);
	btRigidBody* sphereRigidBody = new btRigidBody(sphereRigidBodyCI);
	dynamicsWorld->addRigidBody(sphereRigidBody); // replace boxRigidBody with sphereRigidBody
	sphereRigidBody->setSleepingThresholds(0,0);
	sphereRigidBody->setActivationState(DISABLE_DEACTIVATION);


	const int substep = 10;
	Model PhysicsCube("models/cube/scene.gltf");
	/*
	PhysicsCube.position = glm::vec3(0,0,0);
	PhysicsCube.scale = glm::vec3(4, 4, 4);
	PhysicsCube.BindPhysics(dynamicsWorld, objectWorldMult, true);
	*/

	bool bake = true;

	GLuint UniversalDepthframebuffer;
	glGenFramebuffers(1, &UniversalDepthframebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, UniversalDepthframebuffer);

	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	const float fixed_timestep = 1.0f / 60.0;



	float pixelColor[3];
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_HOME))
	{
		glfwGetWindowSize(window, &width, &height);
		if (!run) {
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

			int width, height;
			glfwGetWindowSize(window, &width, &height);

			int glX = static_cast<int>(xpos);
			int glY = height - static_cast<int>(ypos) - 1;

			
			glReadPixels(glX, glY, 1, 1, GL_RGB, GL_FLOAT, pixelColor);

			float red = pixelColor[0];
			float green = pixelColor[1];
			float blue = pixelColor[2];

			//std::cout << "RGB: " << red << ", " << green << ", " << blue << std::endl;
			std::string rgbOutput = "RGB: " + std::to_string(red) + ", " + std::to_string(green) + ", " + std::to_string(blue);
			//console.log(rgbOutput.c_str());

			
		}


		glfwSwapInterval(vsync);
		if (bakeShadows && bake)
		{
			renderShadows = true;
			bake = false;
		}
		else if ((bakeShadows && !bake)) {
			renderShadows = false;
		}


		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			run = false;
		}
		if (wireframe == true && !run) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			
		}

		

		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		camera.run = run;

		camera.TrackBallMouse(window);

		camera.Mouse(window);


		if (timeDiff >= fixed_timestep) {
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "Caliber Universal Editor- " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());

			prevTime = crntTime;
			counter = 0;
			
			
			camera.Inputs(window, cameraNormalSpeed * fixed_timestep, highCameraSpeed * fixed_timestep);
			camera.Trackaballmovement(window, cameraNormalSpeed * fixed_timestep, highCameraSpeed * fixed_timestep);
			
			

			if (run) {

				btTransform currentTransform;
				sphereRigidBody->getMotionState()->getWorldTransform(currentTransform);
				btVector3 currentPosition = currentTransform.getOrigin();

				glm::vec3 desiredCameraPosition = camera.Position;

				btVector3 desiredPosition = btVector3(desiredCameraPosition.x, desiredCameraPosition.y, desiredCameraPosition.z);
				btVector3 velocity = (desiredPosition - currentPosition) / (fixed_timestep);

				sphereRigidBody->setLinearVelocity(velocity);
				sphereRigidBody->setAngularVelocity(btVector3(0, 0, 0));
				sphereRigidBody->setAngularFactor(btVector3(0, 0, 0));

				sphereRigidBody->setGravity(btVector3(0, 0, 0));


				dynamicsWorld->stepSimulation(fixed_timestep, substep);

				btTransform updatedTransform;
				sphereRigidBody->getMotionState()->getWorldTransform(updatedTransform);
				camera.Position = glm::vec3(updatedTransform.getOrigin().getX(), updatedTransform.getOrigin().getY(), updatedTransform.getOrigin().getZ());

			}

			if (glm::length(camera.Position - glm::vec3(0)) > WorldRadius  - WorldRadius / 100000000000000000) {
				glm::vec3 direction = glm::normalize(camera.Position - glm::vec3(0));
				camera.Position = glm::vec3(0) + direction * (WorldRadius - WorldRadius / 100000000000000000);
			}

			GizmosBoundry.scale = glm::vec3(WorldRadius);
		}


		
		
		


		
		glEnable(GL_DEPTH_TEST);
		
		if (renderShadows) {
			shadowMapProgram.Activate();
			glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
			glViewport(0, 0, shadowMapWidth, shadowMapHeight);
			glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);

			for (int i = 0; i < sceneObjects.size(); i++)
			{
				sceneObjects[i].Draw(shadowMapProgram, camera, objectWorldMult);
			}

		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		

		shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

		if (renderShadows) {
			glActiveTexture(GL_TEXTURE0 + 2);

			glBindTexture(GL_TEXTURE_2D, shadowMap);

			glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowMap"), 2);
		}
		glEnable(GL_DEPTH_TEST);
		


		glClearColor(pow(0.05f, gamma), pow(0.05f, gamma), pow(0.05f, gamma), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		camera.updateMatrix3D(60.0f, 0.1f, viewFarPlane);

		glBindFramebuffer(GL_FRAMEBUFFER, UniversalDepthframebuffer);
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < sceneObjects.size(); i++)
		{
			sceneObjects[i].Draw(UniversalDepthProgram, camera, objectWorldMult);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		
		for (int i = 0; i < sceneObjects.size(); i++)
		{
			sceneObjects[i].Draw(shaderProgram, camera, objectWorldMult);
		
			if (!run) {
				GizmosSphere.Draw(unlitProgram, camera, 1, glm::vec3(sceneObjects[i].translation.x - 1.0f, sceneObjects[i].translation.y, sceneObjects[i].translation.z), Deg(glm::vec3(90, -90, 0)), glm::vec3(0.4));
				glDisable(GL_DEPTH_TEST);
				GizmosSphere.Draw(unlitProgram, camera, 1, glm::vec3(sceneObjects[i].translation.x - 1.0f, sceneObjects[i].translation.y, sceneObjects[i].translation.z), Deg(glm::vec3(90, -90, 0)), glm::vec3(0.4));
				if (checkMouseOverObject(glm::vec3(sceneObjects[i].translation.x - 1.0f, sceneObjects[i].translation.y, sceneObjects[i].translation.z), camera.Position, camera.Orientation, width, height, window) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
				{

					sceneObjects[i].translation.x = moveObjectInXAxis(window, sceneObjects[i].translation, camera.Orientation, camera.Position).x;

				};
				glEnable(GL_DEPTH_TEST);
				glUniform4f(glGetUniformLocation(unlitProgram.ID, "color"), 0, 0, 1, 1);

				GizmosSphere.Draw(unlitProgram, camera, 1, glm::vec3(sceneObjects[i].translation.x, sceneObjects[i].translation.y, sceneObjects[i].translation.z + 1.0f), Deg(glm::vec3(-90, 0, 0)), glm::vec3(0.4));
				glDisable(GL_DEPTH_TEST);
				GizmosSphere.Draw(unlitProgram, camera, 1, glm::vec3(sceneObjects[i].translation.x, sceneObjects[i].translation.y, sceneObjects[i].translation.z + 1.0f), Deg(glm::vec3(-90, 0, 0)), glm::vec3(0.4));
				glUniform4f(glGetUniformLocation(unlitProgram.ID, "color"), 1, 0, 0, 1);
				if (checkMouseOverObject(glm::vec3(sceneObjects[i].translation.x, sceneObjects[i].translation.y, sceneObjects[i].translation.z + 1.0f), camera.Position, camera.Orientation, width, height, window) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
				{

					sceneObjects[i].translation.z = moveObjectInZAxis(window, sceneObjects[i].translation, camera.Orientation, camera.Position).z;

				};
				glEnable(GL_DEPTH_TEST);

				glUniform4f(glGetUniformLocation(unlitProgram.ID, "color"), 0, 1, 0, 1);

				GizmosSphere.Draw(unlitProgram, camera, 1, glm::vec3(sceneObjects[i].translation.x, sceneObjects[i].translation.y + 1.0f, sceneObjects[i].translation.z), Deg(glm::vec3(0)), glm::vec3(0.4));
				glDisable(GL_DEPTH_TEST);
				GizmosSphere.Draw(unlitProgram, camera, 1, glm::vec3(sceneObjects[i].translation.x, sceneObjects[i].translation.y + 1.0f, sceneObjects[i].translation.z), Deg(glm::vec3(0)), glm::vec3(0.4));
				glUniform4f(glGetUniformLocation(unlitProgram.ID, "color"), 1, 0, 0, 1);
				if (checkMouseOverObject(glm::vec3(sceneObjects[i].translation.x, sceneObjects[i].translation.y + 1.0f, sceneObjects[i].translation.z), camera.Position, camera.Orientation, width, height, window) && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
				{

					sceneObjects[i].translation.y = moveObjectInYAxis(window, sceneObjects[i].translation, camera.Orientation, camera.Position).y;

				};
				glEnable(GL_DEPTH_TEST);
				GizmosBoundry.Draw(shaderProgram, camera, 1);
				gird.Draw(shaderProgram, camera, 1, glm::vec3(0), glm::vec3(0), glm::vec3(10));
			}
		}

		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(5.0f);
		

		for (size_t i = 0; i < sceneObjects.size(); i++)
		{

			PhysicsCube.position = glm::vec3(sceneObjects[i].translation.x, sceneObjects[i].translation.y + 1, sceneObjects[i].translation.z);
			PhysicsCube.translation = glm::vec3(sceneObjects[i].translation.x, sceneObjects[i].translation.y, sceneObjects[i].translation.z);
			PhysicsCube.scale = sceneObjects[i].scale;
			PhysicsCube.BindPhysics(dynamicsWorld, objectWorldMult, true);

			PhysicsCube.Draw(unlitProgram, camera, objectWorldMult);
			PhysicsCube.PhysicsUpdate(true);
			PhysicsCube.PHYSICS_SETUP();
		}




		

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		projection = glm::perspective(glm::radians(60.0f), (float)width / height, 0.1f, viewFarPlane);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(1.0f);
		
		if (enableskybox && run) {
			glDepthFunc(GL_LEQUAL);

			skyboxShader.Activate();

			glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);

			glDepthFunc(GL_LESS);
		}
		
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		//House.Draw(shaderProgram, camera, glm::vec3(0), euler_to_quat(0, 0, 0), glm::vec3(10));
		
		//sphere.Draw(shaderProgram, camera, glm::vec3(0), euler_to_quat(0, 0, 0), glm::vec3(10));
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		outlineShader.Activate();
		glUniform1f(glGetUniformLocation(outlineShader.ID, "outlining"), 0.05f);
		//House.Draw(outlineShader, camera, glm::vec3(0), euler_to_quat(0, 0, 0), glm::vec3(10));
		//sphere.Draw(outlineShader, camera, glm::vec3(0), euler_to_quat(0, 0, 0), glm::vec3(10));
		// Enable modifying of the stencil buffer
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);


		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFBO);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		bool horizontal = true, first_iteration = true;
		int Blur_amount = bloom;
		blurProgram.Activate();

		for (unsigned int i = 0; i < Blur_amount; i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
			glUniform1i(glGetUniformLocation(blurProgram.ID, "horizontal"), horizontal);

			if (first_iteration)
			{
				glBindTexture(GL_TEXTURE_2D, bloomTexture);
				first_iteration = false;
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
			}

			glBindVertexArray(rectVAO);
			glDisable(GL_DEPTH_TEST);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			horizontal = !horizontal;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		framebufferProgram.Activate();
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
		glActiveTexture(GL_TEXTURE1);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[!horizontal]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		if (strcmp(console.input_buf, "quit") == 0 && glfwGetKey(window, GLFW_KEY_ENTER))
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		else if (strcmp(console.input_buf, "clear") == 0 && glfwGetKey(window, GLFW_KEY_ENTER))
		{
			console.CLEAR_CONSOLE();
		}
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		console.Draw(no_resize,no_move);


		if (run == false) {
			ImGui::Begin("Viewport", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0));
			{
				if (ImGui::Button("Run"))
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

				ImGui::EndTabBar();
			}
			ImGui::End();

			SETUI(no_resize, no_move, run, postProcessingTexture, shadowMap, depthTexture, colorChoice, msaaSamples, FXAA_SPAN_MAX, FXAA_REDUCE_MIN, FXAA_REDUCE_MUL, framebufferProgram,
				gamma, exposure, bloom, BloomSpreadBlur, blurProgram, shadowMapWidth, shadowMapHeight, bakeShadows, renderShadows, shadowSampleRadius, avgShadow, DepthBias1, DepthBias2, shaderProgram,
				fogNear, viewFarPlane, enableskybox, vsync, highCameraSpeed, cameraNormalSpeed, wireframe, BPL_LIGHTING);
			glUniform1f(glGetUniformLocation(framebufferProgram.ID, "minEdgeContrast"), FXAA_REDUCE_MIN);
			glUniform1f(glGetUniformLocation(framebufferProgram.ID, "subPixelAliasing"), FXAA_REDUCE_MUL);
			glUniform1f(glGetUniformLocation(framebufferProgram.ID, "maximumEdgeDetection"), FXAA_SPAN_MAX);
			glUniform1f(glGetUniformLocation(framebufferProgram.ID, "gamma"), gamma);
			glUniform1f(glGetUniformLocation(framebufferProgram.ID, "exposure"), exposure);
			glUniform1f(glGetUniformLocation(blurProgram.ID, "spreadBlur"), BloomSpreadBlur);
			glUniform1f(glGetUniformLocation(shaderProgram.ID, "near"), fogNear);

			ImGui::Begin("Scene Hierarchy", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0));
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("Object Settings");

				
			}
			if (ImGui::BeginPopup("Object Settings"))
			{
				char objName[128];
				ImGui::InputText("File Path", objName, ImGuiInputTextFlags_EnterReturnsTrue);
				
				if (glfwGetKey(window, GLFW_KEY_ENTER)) {
					sceneObjects.push_back(Model(objName));
				}
				ImGui::EndPopup();
			}
			
			for (size_t i = 0; i < sceneObjects.size(); i++)
			{
				ImGui::Separator();
				float T[3] = { sceneObjects[i].translation.x, sceneObjects[i].translation.y, sceneObjects[i].translation.z };
				ImGui::InputFloat3(("Position##" + std::to_string(i)).c_str(), T);
				sceneObjects[i].translation = glm::vec3(T[0], T[1], T[2]);

				float S[3] = { sceneObjects[i].scale.x, sceneObjects[i].scale.y, sceneObjects[i].scale.z };
				ImGui::InputFloat3(("Scale##" + std::to_string(i)).c_str(), S);
				sceneObjects[i].scale = glm::vec3(S[0], S[1], S[2]);


				float F[3] = { sceneObjects[i].rotation.x, sceneObjects[i].rotation.y, sceneObjects[i].rotation.z };
				ImGui::InputFloat3(("Rotation##" + std::to_string(i)).c_str(), F);
				sceneObjects[i].rotation = glm::vec3(F[0], F[1], F[2]);
				ImGui::Columns(1, nullptr, true);
				ImGui::Separator();


			}
		}
		

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		if (glfwGetKey(window, GLFW_KEY_0)) {
			glFinish(); 
			saveScreenshot(window, "screenshot.png");
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	
		

	}


	std::ofstream Caliboutfile("world.caliber");
	for (const auto& obj : sceneObjects) {
		Caliboutfile << obj.translation.x << "," << obj.translation.y << "," << obj.translation.z << ","
			<< obj.rotation.x << "," << obj.rotation.y << "," << obj.rotation.z << ","
			<< obj.scale.x << "," << obj.scale.y << "," << obj.scale.z << "," << obj.file
			<< "\n";
	}
	Caliboutfile.close();


	myData.data = { float(vsync), float(renderShadows), float(msaaSamples), float(bloom), float(wireframe),
					float(width), float(height), gamma, exposure, highCameraSpeed, cameraNormalSpeed, float(enableskybox),
					fogNear, viewFarPlane, float(bakeShadows), float(colorChoice), FXAA_SPAN_MAX, FXAA_REDUCE_MIN, FXAA_REDUCE_MUL, BloomSpreadBlur, 
	float(shadowMapWidth), float(shadowMapHeight), float(shadowSampleRadius), DepthBias1, DepthBias2, avgShadow, float(BPL_LIGHTING) };
	
	myData.saveData();
	
	
	
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
	shaderProgram.Delete();
	framebufferProgram.Delete();
	shadowMapProgram.Delete();
	blurProgram.Delete();
	skyboxShader.Delete();
	outlineShader.Delete();

	glDeleteFramebuffers(1, &FBO);
	glDeleteFramebuffers(1, &postProcessingFBO);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();


	
	glfwDestroyWindow(window);
	glfwTerminate();

	



	return 0;

	
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}










