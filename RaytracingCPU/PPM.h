#pragma once

#include <functional>

inline auto GetSampleFunc(int width, int height, void (*supsampler)(int width, int height, int x, int y, int& r, int& g, int& b))
{
    return [width, height, supsampler](int x, int y, int& r, int& g, int& b) {supsampler(width, height, x, y, r, g, b); };
}

template<int width, int height>
void DemoSamplerPPM(int x, int y, int& r, int& g, int& b)
{
    DemoSamplerPPMHelper(width, height, x, y, r, g, b);
}

void DemoSamplerPPMHelper(int width, int height, int x, int y, int& r, int& g, int& b);

//void WritePPM(int width, int height, void (*sampler)(int x, int y, int& r, int& g, int& b)); 
void WritePPM(int width, int height, const std::function<void (int, int, int&, int&, int&)>& sampler);//void (*sampler)(int x, int y, int& r, int& g, int& b));