#include "VertexShader.h"
#include "Shader.h"

//#define GOURAUD

void VertexShader::Process(const vertex_t& ivertex, vertex_output_t& overtex)
{
	glm::mat4 mvp = Shader::uniforms.projection * Shader::uniforms.view * Shader::uniforms.model;
	overtex.position = mvp * glm::vec4(ivertex.position, 1);
	glm::mat4 mv = Shader::uniforms.view * Shader::uniforms.model;
	overtex.normal = glm::normalize(glm::mat3{ mv } * ivertex.normal);
	overtex.vposition = mv * glm::vec4(ivertex.position, 1);


#ifdef GOURAUD
	//transform light to view space
	glm::vec3 light_position = Shader::uniforms.view * glm::vec4{ Shader::uniforms.light.position, 1 };
	glm::vec3 light_direction = Shader::uniforms.view * glm::vec4{ -Shader::uniforms.light.direction, 0 };

	glm::vec3 vposition = mv * glm::vec4{ ivertex.position, 1 };

	//glm::vec3 light_dir = glm::normalize(light_position - vposition); // point
	glm::vec3 light_dir = glm::normalize(light_direction); // directional

	//diffuse
	float intensity = std::max(0.0f, glm::dot(light_dir, overtex.normal));
	color3_t diffuse = Shader::uniforms.light.color * intensity;

	glm::vec3 specular{ 0 };

	if (intensity > 0)
	{
		glm::vec3 reflection = glm::reflect(-light_direction, overtex.normal); // Compute reflection vector
		glm::vec3 view_direction = glm::normalize(-vposition); // Calculate view direction
		intensity = std::max(glm::dot(reflection, view_direction), 0.0f); // Compute intensity of reflection
		intensity = std::pow(intensity, Shader::uniforms.material.shininess); // Apply shininess factor
		specular = Shader::uniforms.material.specular * intensity; // Scale specular by intensity and material property
	}


	overtex.color = ((Shader::uniforms.ambient + diffuse) * Shader::uniforms.material.albedo) + specular;
#endif
}
