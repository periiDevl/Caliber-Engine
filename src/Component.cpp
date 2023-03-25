#include "Component.h"




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
