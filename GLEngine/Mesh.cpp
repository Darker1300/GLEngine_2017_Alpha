//#include "DEBUG_NEW_LEAK_DETECT.h"
//#include "DEBUG_WINDOWS_ERR_MSG.h"
//
//#include "Mesh.h"
//
////#include "ShaderProgram.h"
////#include <gl_core_4_4.h>
////#include <glm\ext.hpp>
////#include "VertexArray.h"
////#include "Texture.h"
////Mesh::Mesh()
////	:// m_VertexArrayObj(-1), m_VertexBufferObj(-1), m_IndexBufferObj(-1),
////	m_vertexCount(), m_indexCount(), m_geometryType(GL_TRIANGLES), m_vertexArray(nullptr)
////{
////}
////
////Mesh::~Mesh()
////{
////	if (m_vertexArray != nullptr) delete m_vertexArray;
////}
////
////void Mesh::UpdateUniforms(ShaderProgram* _shader, glm::mat4 * _projectionViewMatrix)
////{
////	_shader->UseShader();
////	// Get projectionViewGlobalMatrix uniform location
////	unsigned int PV_UniformLocation = glGetUniformLocation(_shader->m_programID, "projectionViewWorldMatrix");
////
////	// Testing
////	if (PV_UniformLocation < 0) {
////		LOG_ERROR("glGetUniformLocation( projectionViewWorldMatrix ) failed.")
////	}
////
////	// Apply matrix uniform CPU-->GPU
////	glUniformMatrix4fv(PV_UniformLocation, 1, false, glm::value_ptr(*_projectionViewMatrix));
////
////	_shader->UseShader(false);
////}
////
////void Mesh::UpdateUniforms(ShaderProgram* _shader, glm::mat4 * _projectionViewMatrix, gl::Texture * _texture)
////{
////	_shader->UseShader();
////	// Get projectionViewGlobalMatrix uniform location
////	unsigned int projectionViewUniformLocation = glGetUniformLocation(_shader->m_programID, "projectionViewWorldMatrix");
////	if (projectionViewUniformLocation < 0) {
////		LOG_ERROR("glGetUniformLocation( projectionViewWorldMatrix ) failed.")
////	}
////
////	// set texture slot
////	glActiveTexture(GL_TEXTURE0);
////	glBindTexture(GL_TEXTURE_2D, _texture->m_id);
////
////	// tell the shader where it is 
////	unsigned int textureUniformLocation = glGetUniformLocation(_shader->m_programID, "diffuse");
////	if (projectionViewUniformLocation < 0) {
////		LOG_ERROR("glGetUniformLocation( diffuse ) failed.")
////	}
////
////	glUniform1i(textureUniformLocation, 0);
////
////	// Apply matrix uniform CPU-->GPU
////	glUniformMatrix4fv(projectionViewUniformLocation, 1, false, glm::value_ptr(*_projectionViewMatrix));
////
////	_shader->UseShader(false);
////}
////
////void Mesh::DrawMesh(ShaderProgram * _shader, glm::mat4 * _projectionViewMatrix)
////{
////	DrawMesh(this, _shader, _projectionViewMatrix);
////}
////void Mesh::DrawMesh(ShaderProgram * _shader, glm::mat4 * _projectionViewMatrix, gl::Texture * _texture)
////{
////	DrawMesh(this, _shader, _projectionViewMatrix, _texture);
////}
////
////void Mesh::DrawMesh(Mesh * _model, ShaderProgram * _shader, glm::mat4 * _projectionViewMatrix)
////{
////	// Uniforms
////	UpdateUniforms(_shader, _projectionViewMatrix);
////
////	// Bind
////	_shader->UseShader();
////	_model->m_vertexArray->bind();
////	// Draw
////	glDrawElements(_model->m_geometryType, _model->m_indexCount, GL_UNSIGNED_INT, 0);
////	// Unbind
////	_model->m_vertexArray->bind(false);
////	_shader->UseShader(false);
////}
////
////void Mesh::DrawMesh(Mesh * _model, ShaderProgram * _shader, glm::mat4 * _projectionViewMatrix, gl::Texture * _texture)
////{
////	// Uniforms
////	UpdateUniforms(_shader, _projectionViewMatrix, _texture);
////
////	// Bind
////	_shader->UseShader();
////	_model->m_vertexArray->bind();
////	// Draw
////	glDrawElements(_model->m_geometryType, _model->m_indexCount, GL_UNSIGNED_INT, 0);
////	// Unbind
////	_model->m_vertexArray->bind(false);
////	_shader->UseShader(false);
////}
//
//
