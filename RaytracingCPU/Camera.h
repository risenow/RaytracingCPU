#pragma once
#include "vec3.h"
#include "ray.h"

class Camera {
public:
    Camera(vec3 origin = vec3(0.0, 0.0, 0.0), double aspectRatio = 16.0/9.0, double focalLen = 1.0)
    {
        m_ViewportHeight = 2.0;
        m_ViewportWidth = m_ViewportHeight * aspectRatio;

        m_FocalLength = focalLen;

        m_VerDir = vec3(0.0, m_ViewportHeight, 0.0);
        m_HorDir = vec3(m_ViewportWidth, 0.0, 0.0);

        m_Origin = origin;
        m_BotLeftCorner = m_Origin - vec3(0, 0, m_FocalLength) - 0.5 * m_HorDir - 0.5 * m_VerDir;
    }

    Ray GetRay(double u, double v) const {
        return Ray(m_Origin, m_BotLeftCorner + u * m_HorDir + v * m_VerDir - m_Origin);
    }
private:
    double m_FocalLength;
    double m_ViewportHeight;
    double m_ViewportWidth;

    point3 m_BotLeftCorner;
    vec3 m_VerDir;
    vec3 m_HorDir;
    vec3 m_Origin;
};