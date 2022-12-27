#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <array>
#include "Model.h"
#include <set>
#include <algorithm>
#include <array>
#include <random>

class Component
{
public:
    std::array<Model, 1000> OSWsave;
    int objects;

    // Default constructor for the Component class
    Component()
    {
        // Initialize the object counter to 0
        objects = 0;

        // Initialize all elements in the OSWsave array to default values
        for (int i = 0; i < 1000; i++)
        {
            OSWsave[i] = Model();
        }
    }

    void AddObject(const char* location);
    void RemoveObject(glm::vec3 ID);
    void TRY_OBJ_RECOVERING_TEST();
    void TRY_OBJ_SORTER_TEST();
    void SuffleObjectsID();
    void TRY_SAFE_MODE();
    Model FindObjectID(GLFWwindow* window);

private:
    void Error();
};
