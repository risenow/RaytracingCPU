#include "PPM.h"
#include <iostream>

void DemoSamplerPPMHelper(int width, int height, int x, int y, int& r, int& g, int& b){
    r = static_cast<int>(255.99 * ((double)x / width));
    g = static_cast<int>(255.99 * ((double)y / height));
    b = static_cast<int>((0.25) * 255.99);
}

//auto GetSampleFunc(int width, int height, void (*supsampler)(int width, int height, int x, int y, int& r, int& g, int& b)) 



void WritePPM(int width, int height, const std::function<void(int, int, int&, int&, int&)>& sampler) {
    std::cout << "P3\n" << width << ' ' << height << "\n255\n";

    int iR = 0, iG = 0, iB = 0;

    for (int i = height - 1; i >= 0; i--)
        for (int j = 0; j < width; j++) {
            sampler(j, i, iR, iG, iB);

            std::cout << iR << ' ' << iG << ' ' << iB << '\n';
        }
}