#include "lights/PointLight.h"

void PointLight::setUniforms(int id, Shader& shader) {
	shader.setVec3(conc(m_name, id, "pos"), m_lightPos);
	shader.setVec3(conc(m_name, id, "col"), m_col);
	shader.setFloat(conc(m_name, id, "Kc"), m_Kc);
	shader.setFloat(conc(m_name, id, "Kl"), m_Kl);
	shader.setFloat(conc(m_name, id, "Ke"), m_Ke);
}

void PointLight::draw(Shader& shader) {
	cube.draw(shader);
}