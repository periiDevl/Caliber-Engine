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
#include <vector>

template<typename T>
class Presave
{
public:
    std::vector<T> data;

    Presave()
    {
        std::ifstream input("project.caliber");
        if (input) {
            T value;
            while (input >> value) {
                data.push_back(value);
            }
            input.close();
        }
        else {
            std::cerr << "Error: Could not open file." << std::endl;
        }
    }

    void saveData()
    {
        std::ofstream output("project.caliber");
        printf("save info");
        if (output) {
            // Clear the file first
            output.clear();

            // Write new data to file
            for (const auto& value : data) {
                output << value << std::endl;
            }
            output.close();
        }
        else {
            std::cerr << "Error: Could not open file." << std::endl;
        }
    }


    void SafeOperation()
    {
        if (data.empty()) {
            for (int i = 0; i < 200; i++) {
                data.push_back(400);
            }
            saveData();
        }
    }


private:
};
