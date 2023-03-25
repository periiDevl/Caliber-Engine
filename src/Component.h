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

    void TRY_DRAWING(int objects, Model OSWsave[], Shader shader, Camera camera, float worldSize);
    void CLEAR();
    Model OSWsave[1];
    
private:
    void Error();
};
