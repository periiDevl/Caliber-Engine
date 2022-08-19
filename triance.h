#ifndef TRIANCE_CLASS_H
#define TRIANCE_CLASS_H

#include<json/json.h>
#include"Mesh.h"

using json = nlohmann::json;


class TRI_Model
{
public:
	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
	TRI_Model(const char* file);

	void TRI_Draw
	(
		Shader& shader,
		Camera& camera,
		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f)
	);

private:
	// Variables for easy access
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	// All the meshes and transformations
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	// Prevents textures from being loaded twice
	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;

	// Loads a single mesh by its index
	void TRI_loadMesh(unsigned int indMesh);

	// Traverses a node recursively, so it essentially traverses all connected nodes
	void TRI_traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	// Gets the binary data from a file
	std::vector<unsigned char> TRI_getData();
	// Interprets the binary data into floats, indices, and textures
	std::vector<float> TRI_getFloats(json accessor);
	std::vector<GLuint> TRI_getIndices(json accessor);
	std::vector<Texture> TRI_getTextures();

	// Assembles all the floats into vertices
	std::vector<Vertex> TRI_assembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	// Helps with the assembly from above by grouping floats
	std::vector<glm::vec2> TRI_groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> TRI_groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> TRI_groupFloatsVec4(std::vector<float> floatVec);
};
#endif