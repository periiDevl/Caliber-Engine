#pragma once
#include <iostream>
#include <fstream>
#include <chrono>
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
    
    // Default constructor for the Component class
    Component()
    {
        // Initialize the object counter to 0
        //objects = 0;

        // Initialize all elements in the OSWsave array to default values
        
    }

    void TRY_OBJ_RECOVERING_TEST(int objects, Model OSWsave[]);
    void TRY_OBJ_SORTER_TEST(int objects, Model OSWsave[]);
    void SuffleObjectsID(int objects, Model OSWsave[]);
    void TRY_DRAWING(int objects, Model OSWsave[], Shader shader, Camera camera);
    void CLEAR();
    
private:
    Model OSWsave[1];
    void Error();
};
