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
    
    Component()
    {

        
    }

    void TRY_OBJ_RECOVERING_TEST(int objects, Model OSWsave[]);
    void TRY_OBJ_SORTER_TEST(int objects, Model OSWsave[]);
    void SuffleObjectsID(int objects, Model OSWsave[]);
    void TRY_DRAWING(int objects, Model OSWsave[], Shader shader, Camera camera, float worldSize);
    void CLEAR();
    
private:
    Model OSWsave[1];
    void Error();
};
