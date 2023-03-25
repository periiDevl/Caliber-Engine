#pragma once
#include <fstream>
#include <glm/glm.hpp>
class Save {
public:
    void saveVec3(const glm::vec3& v, const std::string& filename)
    {
        std::ofstream file(filename, std::ios::app);
        file << v.x << " " << v.y << " " << v.z << "\n";
        file.close();
    }


    void saveVec4(const glm::quat& q, const std::string& filename)
    {
        std::ofstream file(filename, std::ios::app);
        file << q.w << " " << q.x << " " << q.y << " " << q.z << "\n";
        file.close();
    }
    glm::vec3 loadVec3(const std::string& filename, int lineNum)
    {
        std::ifstream file(filename);
        glm::vec3 v;
        std::string line;
        for (int i = 1; i <= lineNum; i++) {
            if (!std::getline(file, line)) {
                // If the specified line number is out of range,
                // return a zero vector.
                return glm::vec3(0.0f);
            }
        }
        std::stringstream ss(line);
        ss >> v.x >> v.y >> v.z;
        file.close();
        return v;
    }


    glm::quat loadVec4(const std::string& filename, int lineNum)
    {
        std::ifstream file(filename);
        glm::quat q;
        std::string line;
        for (int i = 1; i <= lineNum; i++) {
            if (!std::getline(file, line)) {
                // If the specified line number is out of range,
                // return a zero quaternion.
                return glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
            }
        }
        std::stringstream ss(line);
        ss >> q.w >> q.x >> q.y >> q.z;
        file.close();
        return q;
    }


};