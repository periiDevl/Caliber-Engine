#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>

class FlightController {
public:
    FlightController() : pos_(0.0f, 0.0f, 0.0f), rot_(1.0f, 0.0f, 0.0f, 0.0f) {}

    void update(float elapsedTime, GLFWwindow* window) {
        
        float pitch = 0.0f, yaw = 0.0f, roll = 0.0f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pitch += 0.1f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pitch -= 0.1f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) yaw += 0.1f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) yaw -= 0.1f;
        

        rot_ = glm::quat(glm::vec3(0.0f, yaw, 0.0f)) * rot_;
        rot_ = rot_ * glm::quat(glm::vec3(pitch, 0.0f, 0.0f));

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos_) * glm::mat4_cast(rot_);
        
        glm::vec3 forward = -glm::vec3(transform[2]);
        pos_ += forward * elapsedTime;
    }
    glm::vec3 getPosition() const { return pos_; }

    glm::quat getOrientation() const { return rot_; }

private:
    glm::vec3 pos_;
    glm::quat rot_;
};