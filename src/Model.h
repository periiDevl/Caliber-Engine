#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>
#include"Mesh.h"
#include<bullet/btBulletDynamicsCommon.h>
#include"Functions.h"
using json = nlohmann::json;


class Model
{
public:
	float tintMult = 1;
	bool staticBody = false;
	bool deleted = false;
	bool draw = true;
	bool bindedphysics = true;
	bool semi_transparent = false;
	btRigidBody* boxRigidBody = nullptr;
	glm::vec3 translation = glm::vec3(0);
	glm::vec3 rotation = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(0);
	glm::vec3 ID = glm::vec3(0, 0, 0);
	std::string file = "models/rocket/scene.gltf";
	glm::vec4 tint = glm::vec4(1);

	Model(const char* fl = "models/rocket/scene.gltf",
		glm::vec3 trans = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 rot = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 sca = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3 id = glm::vec3(0, 0, 0));

	void Draw
	(
		Shader& shader,
		Camera& camera,
		float worldSize
	);
	void Draw
	(
		Shader& shader,
		Camera& camera,
		float worldSize, glm::vec3 position, glm::vec3 rot, glm::vec3 scale
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




	void BindPhysics(btDynamicsWorld* dynaWorld, float objectWorldMult)
	{
		if (!rigidBodyCreated) {
			btCollisionShape* boxShape = new btBoxShape(btVector3(scale.x / objectWorldMult, scale.y / objectWorldMult, scale.z / objectWorldMult));
			btScalar mass = staticBody ? 0 : 1;

			Functions func;
			glm::quat rotationQuat = func.Euler_to_quat(rotation.x, rotation.y, rotation.z);
			btDefaultMotionState* boxMotionState = new btDefaultMotionState(btTransform(btQuaternion(rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w), btVector3(translation.x, translation.y, translation.z)));
			btVector3 boxInertia(0, 0, 0);
			boxShape->calculateLocalInertia(mass, boxInertia);

			boxRigidBody = new btRigidBody(mass, boxMotionState, boxShape, boxInertia);
			dynaWorld->addRigidBody(boxRigidBody);

			boxRigidBody->setSleepingThresholds(0, 0);
			boxRigidBody->setActivationState(DISABLE_DEACTIVATION);

			rigidBodyCreated = true;
		}


	}
	btTransform phys;
	void PHYSICS_SETUP(float objectWorldMult, bool run)
	{
		Functions func;
		
		if (staticBody)
		{
			boxRigidBody->setWorldTransform(phys);
			boxRigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
		}
		
		boxRigidBody->setGravity(btVector3(0, -9.81 * 10, 0));
		boxRigidBody->getMotionState()->getWorldTransform(phys);
		if (!staticBody)
		{
			translation = glm::vec3(phys.getOrigin().getX(), phys.getOrigin().getY() - 1, phys.getOrigin().getZ());
		}

		phys.setOrigin(btVector3(translation.x, translation.y + 1, translation.z));
		



		if (!staticBody) {
			btQuaternion rotationT = phys.getRotation();
			rotation = func.Quat_to_euler(glm::quat(rotationT.getX(), rotationT.getY(), rotationT.getZ(), rotationT.getW()));
		}
		else {
			float radiansX = btRadians(rotation.x);
			float radiansY = btRadians(rotation.y);
			float radiansZ = btRadians(rotation.z);
			btQuaternion quaternionRotation;
			quaternionRotation.setEulerZYX(radiansZ, radiansY, radiansX);
			phys.setRotation(quaternionRotation);
		}

		btCollisionShape* boxShape = new btBoxShape(btVector3(scale.x / objectWorldMult, scale.y / objectWorldMult, scale.z / objectWorldMult));
		boxRigidBody->setCollisionShape(boxShape);

	}





	std::string To_string()
	{
		std::stringstream ss;
		ss << "Model(" << this->file << ", " << std::to_string(ID.x) << " " << std::to_string(ID.y) << " " << std::to_string(ID.z) << ", "
			<< std::to_string(translation.x) << " " << std::to_string(translation.y) << " " << std::to_string(translation.z) << ", "
			<< std::to_string(rotation.x) << " " << std::to_string(rotation.y) << " " << std::to_string(rotation.z) << " " << std::to_string(rotation.z) << ", "
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
		model.scale = scale;


		return model;
	}

private:


	// Create a flag to keep track of whether the rigid body has been created.
	bool rigidBodyCreated = false;


	const char* changeFileExtension(const char* file) {
		static char newFile[100];
		strcpy(newFile, file);
		size_t len = strlen(newFile);
		strcpy(&newFile[len - 5], ".bin");
		return newFile;
	}
	std::vector<unsigned char> data;
	json JSON;

	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;

	void loadMesh(unsigned int indMesh);

	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	std::vector<unsigned char> getData();
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();

	std::vector<Vertex> assembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};
#endif