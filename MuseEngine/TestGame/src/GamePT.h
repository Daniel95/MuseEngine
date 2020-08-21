#pragma once

#include "Core/Application.h"
#include "glm/glm.hpp"

#include <vector>
#include <array>
#include "Core/Utilities/Utilities.h"
#include "Core/Renderer/RayTracer/RayHitData.h"
#include "Core/Renderer/RayTracer/GetColorParameters.h"

namespace Muse
{
    class RenderComponent;
    struct Ray;
    class Shader;
}

class GamePT : public Muse::Application
{
public:
    GamePT() = default;
    virtual ~GamePT() = default;

protected:
    virtual void OnStart() override;
    virtual void OnUpdate(float a_DeltaTime) override;
    virtual void OnFixedUpdate() override;
    virtual void OnRender() override;
    virtual void OnImGuiRender() override;

private:
    std::vector<float> m_ScreenData;
    std::vector<float> m_Buffer;
    unsigned int m_Width, m_Height;
    glm::vec3 m_BackgroundColor = glm::vec3(0.1f, 0.1f, 0.25f);
    Muse::RayHitData m_RayHitData;
    Muse::GetColorParameters m_GetColorParameters;
    bool m_Hit = false;
    int m_Frames = 0;
    const int m_FrameRayMax = 5;
    int m_FrameRayCount = 0;
    glm::mat4 cameraTransform;
    std::vector<std::shared_ptr<Muse::RenderComponent>> m_LightsRenderComponents;

    void Resize(unsigned int a_Width, unsigned int a_Height);
    glm::vec3 Sample(const Muse::Ray& a_Ray);
    glm::vec3 SampleNew(const Muse::Ray& a_Ray);
    glm::vec3 SampleIS(const Muse::Ray& a_Ray);
    glm::vec3 SampleNEE(const Muse::Ray& a_Ray, bool a_LastSpecular);
    glm::vec3 SampleNEEIS(const Muse::Ray& a_Ray, bool a_LastSpecular);
    std::shared_ptr<Muse::RenderComponent> m_Light;

    glm::mat4 MakeRotationMat(const glm::vec3& a_Forward, const glm::vec3& a_WorldUp = glm::vec3(0, 1, 0), const glm::vec3& a_WorldRight = glm::vec3(1, 0, 0))
    {
        glm::vec3 right;
        glm::vec3 up;

        if(a_Forward != a_WorldUp)
        {
            right = glm::normalize(glm::cross(a_WorldUp, a_Forward));
            up = glm::normalize(glm::cross(a_Forward, right));
        }
        else 
        {
            up = glm::normalize(glm::cross(a_WorldRight, a_Forward));
            right = glm::normalize(glm::cross(a_Forward, up));
        }

        glm::mat3 rotationMat =
        {
            right.x, right.y, right.z,
            up.x, up.y, up.z,
            a_Forward.x, a_Forward.y, a_Forward.z
        };

        return rotationMat;
    }

    glm::vec3 CosineWeightedDiffuseReflection() const
    {
        float r0 = Muse::Random(), r1 = Muse::Random();
        float r = sqrt(r0);
        float theta = 2 * PI * r1;
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        return glm::vec3(x, y, sqrt(1 - r0));
    }

    glm::vec3 find_orthogonal_unit_vector(glm::vec3 a_Normal) const
    {
        return a_Normal.x == 0 ? glm::vec3(1, 0, 0) : glm::normalize(glm::cross(glm::vec3(0, 1, 0), a_Normal));
    }

    glm::vec3 TransformToDirection(const glm::vec3& a_Direction, const glm::vec3& a_Vector) const
    {
        glm::vec3 difference = a_Vector - (a_Direction * -1.f);
        //glm::vec3 differenceT = a_Vector - (glm::vec3(0, 1, 0));
        glm::vec3 result = a_Direction - difference;

        return result;
    }

    glm::vec3 ToTangent(const glm::vec3& a_Normal, const glm::vec3& a_Vector) const
    {
        glm::vec3 tangent = find_orthogonal_unit_vector(a_Normal);
        glm::vec3 bitangent = glm::cross(tangent, a_Normal);

        return a_Vector.x * tangent + a_Vector.y * a_Normal + a_Vector.z * bitangent;
    }

    glm::vec3 TransformToTangent(const glm::vec3& normal, const glm::vec3& vector) const
    {
        const glm::vec3 w = abs(normal.x > 0.99f) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
        const glm::vec3 t = normalize(cross(normal, w));
        const glm::vec3 b = cross(t, normal);
        return vector.x * t + vector.y * b + vector.z * normal;
    }

    glm::vec3 getBRDFRay(glm::vec3 position, glm::vec3 normal, glm::vec3 incident)
    {
        glm::vec3 ref = glm::reflect(incident, normal);
        glm::vec3 offset = glm::mix(ref, normal, 1.f);
        //return normalize(RandomUnitSphere(seed) * material.roughness + offset);
        return glm::normalize(Muse::RandomDirectionInHemisphere(normal)) * 1.f + offset;
    }

    /*
    glm::vec3 cosineDirection(glm::vec3 a_Normal)
    {
        // compute basis from normal
        // see http://orbit.dtu.dk/fedora/objects/orbit:113874/datastreams/file_75b66578-222e-4c7d-abdf-f7e255100209/content
        // (link provided by nimitz)

        glm::vec3 tc = glm::vec3(1.0f + a_Normal.z - (a_Normal.x + a_Normal.y) * (a_Normal.x + a_Normal.y), -a_Normal.x * a_Normal.y);// / (1.0f + a_Normal.z);
        glm::vec3 uu = glm::vec3(tc.x, tc.z, -a_Normal.x);
        glm::vec3 vv = glm::vec3(tc.z, tc.y, -a_Normal.y);

        return a_Normal * (glm::sqrt(Muse::Random()) * (cos(Muse::Random()) * uu + sin(Muse::Random()) * vv) + sqrt(1.0f - Muse::Random()));
    }
    */
};
