class MouseRotation
{
public:
    MouseRotation() : m_lastX(0), m_offset(0), m_rotation(1, 0, 0, 0), m_centerSet(false), m_velocity(0) {}

    void processMouseMovement(GLFWwindow* window, double deltaTime)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (!m_centerSet)
        {
            m_lastX = xpos;
            glfwSetCursorPos(window, m_lastX, ypos);
            m_centerSet = true;
        }

        double xoffset = xpos - m_lastX;

        double sensitivity = 0.1 * deltaTime;
        m_offset += xoffset * sensitivity;

        m_offset *= 0.9;

        glm::quat yawQuat = glm::angleAxis((float)m_offset, glm::vec3(0.0f, 1.0f, 0.0f));

        m_rotation = m_rotation * yawQuat;

        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glfwSetCursorPos(window, width / 2, height / 2);

        m_lastX = width / 2;
    }


    glm::quat getRotation() const { return m_rotation; }
private:
    double m_lastX;
    double m_offset;
    glm::quat m_rotation;
    bool m_centerSet;
    double m_velocity;
};