#pragma once
#include "vec3.h"
#include "ray.h"

class Camera {
public:
    Camera(vec3 origin = vec3(0.0, 0.0, 0.0), vec3 lookat = vec3(0.0, 0.0, -1.0), vec3 up = vec3(0.0, 1.0, 0.0), double fovYdegrees = 90.0, double aspectRatio = 16.0 / 9.0, double focusDist = 1.0, double aperture = 0.0)
    {
        double h = tan(degrees2rads(fovYdegrees) / 2.0);

        m_ViewportHeight = 2.0*h;
        m_ViewportWidth = m_ViewportHeight * aspectRatio;

        m_FocusDist = focusDist;

        m_W = -normalize(lookat - origin); // forward dir
        m_U = normalize(cross(up, m_W)); // right dir
        m_V = normalize(cross(m_W, m_U)); // up dir
        vec3 upDir2 = (cross(m_W, m_U));

        m_VerDir = m_FocusDist * m_ViewportHeight * m_V;
        m_HorDir = m_FocusDist * m_ViewportWidth * m_U;

        m_Origin = origin;
        m_BotLeftCorner = m_Origin - m_FocusDist * m_W - 0.5 * m_HorDir - 0.5 * m_VerDir;

        m_LensRadius = aperture / 2;
    }

    Ray GetRay(double u, double v) const {
        vec3 pointOnLens = m_LensRadius * vec3::random_in_unit_disk();
        pointOnLens = m_Origin + m_U*pointOnLens[0] + m_V * pointOnLens[1];
        return Ray(pointOnLens, m_BotLeftCorner + u * m_HorDir + v * m_VerDir - pointOnLens);
    }
private:
    double m_LensRadius;
    double m_FocusDist;
    double m_ViewportHeight;
    double m_ViewportWidth;

    point3 m_BotLeftCorner;
    vec3 m_VerDir;
    vec3 m_HorDir;
    vec3 m_Origin;

    vec3 m_U;
    vec3 m_V;
    vec3 m_W;
};