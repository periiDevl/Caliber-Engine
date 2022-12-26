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
class Object
{
public:
    
    std::array<Model, 1000> OSWsave;


    

    void AddObject(const char* location);

    void RemoveObject(glm::vec3 ID);

    void TRY_OBJ_RECOVERING_TEST();

    void TRY_OBJ_SORTER_TEST();

    void SuffleObjectsID();

    void TRY_SAFE_MODE();

    Model FindObjectID(GLFWwindow* window);



private:
    void Error();
    int objects;
    
};

