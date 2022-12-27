#include "Component.h"





void::Component::TRY_OBJ_RECOVERING_TEST(int objects, Model OSWsave[])
{
    std::string line;
    std::ifstream OSWFile("projectname[OSW].caliber");
    int i = 0;

    while (std::getline(OSWFile, line)) // Use getline to check for the end of the file
    {
        try
        {
            OSWsave[i] = Model::From_string(line); // Convert the string to a model
            i++;
            objects++;
        }
        catch (const std::invalid_argument&)
        {
            // Handle the exception if the string cannot be converted to a float
            Error();
        }
    }

    std::cout << i + " OBJECTS RECOVERD" << std::endl;
}

std::ofstream OSWFileWr("projectname[OSW].caliber");
void::Component::TRY_OBJ_SORTER_TEST(int objects, Model OSWsave[])
{

    

    for (int i = 0; i < objects + 1; i++)
    {
        std::string str = OSWsave[i].To_string();
        OSWFileWr << str << "\n";
    }
}

void::Component::TRY_DRAWING(int objects, Model OSWsave[], Shader shader, Camera camera)
{
    for (int i = 0; i < objects + 1; i++)
    {
        OSWsave[i].Draw(shader,camera);
    }
}

void::Component::SuffleObjectsID(int objects , Model OSWsave[])
{
    // Initialize vectors of all possible values for the x, y, and z components
    std::vector<float> allXValues;
    std::vector<float> allYValues;
    std::vector<float> allZValues;
    for (float i = 0.0f; i <= 1.0f; i += 0.000001f) {
        allXValues.push_back(i);
        allYValues.push_back(i);
        allZValues.push_back(i);
    }

    // Shuffle the x, y, and z values for each object
    std::random_device rd;
    std::mt19937 mt(rd());
    for (int i = 0; i < objects; i++) {
        std::shuffle(allXValues.begin(), allXValues.end(), mt);
        std::shuffle(allYValues.begin(), allYValues.end(), mt);
        std::shuffle(allZValues.begin(), allZValues.end(), mt);
        OSWsave[i].ID = glm::vec3(allXValues[0], allYValues[0], allZValues[0]);
    }
}


void::Component::TRY_SAFE_MODE(int objects, Model OSWsave[1])
{
    // Iterate through the objects in OSWsave
    for (int i = 0; i < objects + 1; i++)
    {
        // Check if the current object's ID is already in use
        bool idInUse = false;
        for (int j = 0; j < i; j++)
        {
            if (OSWsave[j].ID == OSWsave[i].ID)
            {
                idInUse = true;
                break;
            }
        }

        if (idInUse)
        {
            // If the ID is already in use, shuffle the object IDs and start the loop over
            printf("[SAFE TEST PASSED POORLY WAIT UNTIL ERRORS ARE FIXED]");
            SuffleObjectsID(objects, OSWsave);
            i = -1; // Reset the loop counter to 0
        }
    }

    // If no duplicate IDs were found, print a message
    printf("[SAFE TEST PASSED SUCCESFULLY]");
}


Model Component::FindObjectID(GLFWwindow* window, int objects, Model OSWsave[])
{
    // Get the mouse position in screen coordinates
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Flip the y coordinate, since screen coordinates start at the top left,
    // but OpenGL coordinates start at the bottom left
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    mouseY = viewport[3] - mouseY;

    // Read the color values of the pixel under the mouse cursor
    GLubyte pixel[4];
    glReadPixels(mouseX, mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

    // The pixel array now contains the color values of the pixel under the mouse cursor
    // You can access the individual color components like this:
    float r = pixel[0] / 255.0f;
    float g = pixel[1] / 255.0f;
    float b = pixel[2] / 255.0f;

    // Convert the color values to an integer ID
    glm::vec3 ID = glm::vec3(r, g, b);

    // Search the objects in OSWsave for an object with the matching ID
    for (int i = 0; i < objects + 1; i++)
    {
        if (OSWsave[i].ID == ID)
        {
            return OSWsave[i];
        }
    }

    return Model("models/rocket/scene.gltf");
}


void::Component::CLEAR()
{
    //std::fstream file("projectname[OSW].caliber", std::fstream::out | std::fstream::trunc);
    //file.close();

    remove("projectname[OSW].caliber");
}

void::Component::Error()
{
    printf("[FAILED GENRATING/LOADING FILES]");
}
