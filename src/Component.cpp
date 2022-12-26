#include "Component.h"

void::Component::AddObject(const char* location)
{
    OSWsave[objects] = Model(location, glm::vec3(0), glm::vec3(0), glm::quat(0, 0, 0, 0), glm::vec3(0));
    objects++;
    SuffleObjectsID();
}



void::Component::TRY_OBJ_RECOVERING_TEST()
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

void::Component::TRY_OBJ_SORTER_TEST()
{
    std::ofstream OSWFileWr("projectname[OSW].caliber");

    for (int i = 0; i < objects; i++)
    {
        std::string str = OSWsave[i].To_string();
        OSWFileWr << str << "\n";
    }
}

void::Component::SuffleObjectsID()
{
    std::vector<glm::vec3> allIDs;

    // Initialize allIDs with a list of all possible IDs
    for (int i = 0; i < objects; i++)
    {
        allIDs.push_back(glm::vec3(i, i, i));
    }

    // Shuffle the list of IDs
    std::random_device rd;
    std::mt19937 mt(rd());
    std::shuffle(allIDs.begin(), allIDs.end(), mt);

    for (int i = 0; i < objects; i++)
    {
        OSWsave[i].ID = allIDs[i];
    }
}


void::Component::TRY_SAFE_MODE()
{
    // Iterate through the objects in OSWsave
    for (int i = 0; i < OSWsave.size(); i++)
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
            SuffleObjectsID();
            i = -1; // Reset the loop counter to 0
        }
    }

    // If no duplicate IDs were found, print a message
    printf("[SAFE TEST PASSED SUCCESFULLY]");
}


Model Component::FindObjectID(GLFWwindow* window)
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
    for (int i = 0; i < objects; i++)
    {
        if (OSWsave[i].ID == ID)
        {
            return OSWsave[i];
        }
    }

    return Model("", glm::vec3(0), glm::vec3(0), glm::quat(0, 0, 0, 0), glm::vec3(0));
}


void::Component::Error()
{
    printf("[FAILED GENRATING/LOADING FILES]");
}
