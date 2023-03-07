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
            OSWsave[i].translation = Model::From_string(line).translation;
            OSWsave[i].scale = Model::From_string(line).scale;
            OSWsave[i].rotation = Model::From_string(line).rotation;
            OSWsave[i].ID = glm::vec3(i, i + 1, i + 2);
            OSWsave[i] = Model::From_string(line);
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

void::Component::TRY_DRAWING(int objects, Model OSWsave[], Shader shader, Camera camera, float worldSize)
{
    for (int i = 0; i < objects + 1; i++)
    {
        OSWsave[i].Draw(shader,camera, worldSize);
    }
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
