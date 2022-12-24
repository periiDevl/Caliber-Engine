#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>

// A flight controller class that manages the position and orientation of an object in 3D space
class FlightController {
public:
    // Constructor
    FlightController() : pos_(0.0f, 0.0f, 0.0f), rot_(1.0f, 0.0f, 0.0f, 0.0f) {}

    // Update the position and orientation of the object based on user input
    void update(float elapsedTime, GLFWwindow* window) {
        // Compute the pitch, yaw, and roll values based on the WASD input
        
                // Compute the pitch, yaw, and roll values based on the WASD and arrow key input
        float pitch = 0.0f, yaw = 0.0f, roll = 0.0f;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) pitch += 0.1f;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) pitch -= 0.1f;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) yaw += 0.1f;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) yaw -= 0.1f;
        

        // Update the orientation of the object using the pitch, yaw, and roll values
        rot_ = glm::quat(glm::vec3(0.0f, yaw, 0.0f)) * rot_;
        rot_ = rot_ * glm::quat(glm::vec3(pitch, 0.0f, 0.0f));

        // To get the forward vector, we first need to create a transformation matrix
        // using the position and orientation
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos_) * glm::mat4_cast(rot_);
        

        // The forward vector is the negative Z-axis of the object's local coordinate system,
        // so we can extract it from the transformation matrix like this:
        glm::vec3 forward = -glm::vec3(transform[2]);

        // Update the position of the object by moving it in the forward direction
        // based on the elapsed times
        pos_ += forward * elapsedTime;
    }

    // Get the current position of the object
    glm::vec3 getPosition() const { return pos_; }

    // Get the current orientation of the object
    glm::quat getOrientation() const { return rot_; }

private:
    // The position and orientation of the object
    glm::vec3 pos_;
    glm::quat rot_;
};