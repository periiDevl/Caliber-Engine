#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>
#include"Mesh.h"
#include<bullet/btBulletDynamicsCommon.h>
using json = nlohmann::json;


class Model
{
public:
	btRigidBody* boxRigidBody;
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 ID = glm::vec3(0, 0, 0);
	const char* file = "models/rocket/scene.gltf";

	// Loads in a model from a file and stores tha information in 'data', 'JSON', and 'file'
	Model(const char* fl = "models/rocket/scene.gltf",
		glm::vec3 trans = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 sca = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 id = glm::vec3(0, 0, 0));

	void Draw
	(
		Shader& shader,
		Camera& camera,
		float worldSize
	);

	btTriangleMesh* getVerticesFromFile(const char* filePath, float divideValue)
	{
		btTriangleMesh* triangleMesh = new btTriangleMesh();

		std::ifstream file(filePath, std::ios::binary);
		if (file.is_open())
		{
			while (!file.eof())
			{
				glm::vec3 vertex;
				file.read(reinterpret_cast<char*>(&vertex), sizeof(glm::vec3));
				btVector3 btVertex(vertex.x / divideValue, vertex.y / divideValue, vertex.z / divideValue);
				triangleMesh->addTriangle(btVertex, btVertex, btVertex);
			}

			file.close();
		}

		return triangleMesh;
	}



	void BindPhysics(btDynamicsWorld* dynaWorld, float objectWorldMult, bool Static)
	{
		btCollisionShape* boxShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z) / objectWorldMult);
		btScalar mass = 0;

		if (Static) {
			btTriangleMesh* triangleMesh = getVerticesFromFile(changeFileExtension(file), objectWorldMult);
			btCollisionShape* boxShape = new btConvexTriangleMeshShape(triangleMesh, true);
			btScalar mass = 0;
		}
		

		btDefaultMotionState* boxMotionState = new btDefaultMotionState(btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w), btVector3(translation.x, translation.y, translation.z)));
		btVector3 boxInertia(0, 0, 0);
		boxShape->calculateLocalInertia(mass, boxInertia);
		boxRigidBody = new btRigidBody(mass, boxMotionState, boxShape, boxInertia);
		if (Static) {
			boxRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
		}
		dynaWorld->addRigidBody(boxRigidBody);
	}

	btTransform trn;
	void UpdatePhysics()
	{
		boxRigidBody->setGravity(btVector3(0, 0, 0));
		boxRigidBody->getMotionState()->getWorldTransform(trn);
		translation = glm::vec3(trn.getOrigin().getX(), trn.getOrigin().getY() - scale.y, trn.getOrigin().getZ());
		rotation = glm::quat(trn.getRotation().getX(), trn.getRotation().getY(), trn.getRotation().getZ(), trn.getRotation().getW());
		boxRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);


	}




	std::string To_string()
	{
		std::stringstream ss;
		ss << "Model(" << this->file << ", " << std::to_string(ID.x) << " " << std::to_string(ID.y) << " " << std::to_string(ID.z) << ", "
			<< std::to_string(translation.x) << " " << std::to_string(translation.y) << " " << std::to_string(translation.z) << ", "
			<< std::to_string(rotation.x) << " " << std::to_string(rotation.y) << " " << std::to_string(rotation.z) << " " << std::to_string(rotation.w) << ", "
			<< std::to_string(scale.x) << " " << std::to_string(scale.y) << " " << std::to_string(scale.z) << ")";
		return ss.str();
	}

	static Model From_string(const std::string& str)
	{
		std::stringstream ss(str);
		std::string file;
		glm::vec3 ID;
		glm::vec3 translation;
		glm::quat rotation;
		glm::vec3 scale;
		char comma1, comma2;
		ss >> comma1 >> file >> comma2 >> ID.x >> ID.y >> ID.z >> comma2 >> translation.x >> translation.y >> translation.z >> rotation.x >> rotation.y >> rotation.z >> rotation.w >> scale.x >> scale.y >> scale.z;

		Model model;
		model.file = file.c_str();
		model.translation = translation;
		model.ID = ID;
		model.rotation = rotation;
		model.scale = scale;


		return model;
	}

private:


	const char* changeFileExtension(const char* file) {
		static char newFile[100];
		strcpy(newFile, file);
		size_t len = strlen(newFile);
		strcpy(&newFile[len - 5], ".bin");
		return newFile;
	}
	// Variables for easy access
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
	void loadMesh(unsigned int indMesh);

	// Traverses a node recursively, so it essentially traverses all connected nodes
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	// Gets the binary data from a file
	std::vector<unsigned char> getData();
	// Interprets the binary data into floats, indices, and textures
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();

	// Assembles all the floats into vertices
	std::vector<Vertex> assembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	// Helps with the assembly from above by grouping floats
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};
#endif