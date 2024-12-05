#include "FragmentShader.h"
#include "Shader.h"

color4_t FragmentShader::Process(const fragment_input_t& fragment)
{
	//float average = (fragment.color.r + fragment.color.g + fragment.color.b) / 3.0f;
	
	//transform light to view space
	glm::vec3 light_position = Shader::uniforms.view * glm::vec4{ Shader::uniforms.light.position, 1 };
	glm::vec3 light_direction = Shader::uniforms.view * glm::vec4{ -Shader::uniforms.light.direction, 0 };

	//glm::vec3 vposition = mv * glm::vec4{ fragment.position, 1 };

	glm::vec3 light_dir = glm::normalize(light_position - fragment.position); // point
	//glm::vec3 light_dir = glm::normalize(light_direction); // directional

	//diffuse
	float intensity = std::max(0.0f, glm::dot(light_dir, fragment.normal));
	color3_t diffuse = Shader::uniforms.light.color * intensity;

	glm::vec3 specular{ 0 };

	if (intensity > 0)
	{
		glm::vec3 reflection = glm::reflect(-light_dir, fragment.normal); // Compute reflection vector
		glm::vec3 view_direction = glm::normalize(-fragment.position); // Calculate view direction
		intensity = std::max(glm::dot(reflection, view_direction), 0.0f); // Compute intensity of reflection
		intensity = std::pow(intensity, Shader::uniforms.material.shininess); // Apply shininess factor
		specular = Shader::uniforms.material.specular * intensity; // Scale specular by intensity and material property
	}


	color3_t color = ((Shader::uniforms.ambient + diffuse) * Shader::uniforms.material.albedo) + specular;


	return { color, 1 };
}


