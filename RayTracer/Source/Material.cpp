#include "Material.h"
#include "Random.h"

bool Lambertian::Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scatter) const
{
    scatter.origin = raycastHit.point;
    scatter.direction = raycastHit.normal + randomOnUnitSphere();

    attenuation = m_albedo;

    return true;
}

bool Metal::Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scatter) const
{
    
    glm::vec3 reflected = Reflect(ray.direction, raycastHit.normal);

    // set scattered ray from reflected ray + random point in sphere (fuzz = 0 no randomness, fuzz = 1 random reflected)
    // a mirror has a fuzz value of 0 and a diffused metal surface a higher value
    scatter = ray_t{ raycastHit.point, reflected + (randomOnUnitSphere() * m_fuzz) };
    attenuation = m_albedo;

    // check that reflected ray is going away from surface normal (dot product > 0)
    return glm::dot(scatter.direction, raycastHit.normal) > 0;
    
}

bool Dielectric::Scatter(const ray_t& ray, const raycastHit_t& raycastHit, color3_t& attenuation, ray_t& scatter) const
{
    glm::vec3 outNormal;
    float ni_over_nt;
    float cosine;

    //check if we are hitting from the outside
    if (glm::dot(ray.direction, raycastHit.normal) < 0)
    {
        outNormal = raycastHit.normal;
        ni_over_nt = 1.0f/m_refractiveIndex;
        cosine = -glm::dot(ray.direction, raycastHit.normal) / glm::length(ray.direction);
    }
    else  //hitting from the inside
    {
        outNormal = -raycastHit.normal;
        ni_over_nt = m_refractiveIndex;
        cosine = m_refractiveIndex * glm::dot(ray.direction, raycastHit.normal) / glm::length(ray.direction);
    }

    glm::vec3 refracted;
    float reflectProbability = 1.0f;

    if (Refract(ray.direction, outNormal, ni_over_nt, refracted))
    {
        reflectProbability = Schlick(cosine, m_refractiveIndex);
    }
    glm::vec3 reflected = Reflect(ray.direction, raycastHit.normal);

    scatter = (randomf() < reflectProbability) ? ray_t{ raycastHit.point, reflected } : ray_t{ raycastHit.point, refracted };

    attenuation = m_albedo;


    return true;
}
