#include "ParticleSystem.h"

#include "DEBUG_NEW_LEAK_DETECT.h"
#include "DEBUG_WINDOWS_ERR_MSG.h"

#include <gl_core_4_4.h>
#include "Shader.h"

ParticleSystem::ParticleSystem()
	: m_particles(nullptr)
	, m_maxParticles(0)
	, m_position(0, 0, 0)
	, m_drawShaderID(0)
	, m_updateShaderID(0)
	, m_lastDrawTime(0)
{
	m_vao[0] = 0;
	m_vao[1] = 0;
	m_vbo[0] = 0;
	m_vbo[1] = 0;
}


ParticleSystem::~ParticleSystem()
{
	delete[] m_particles;
	glDeleteVertexArrays(2, m_vao);
	glDeleteBuffers(2, m_vbo);
	// delete the shaders
	glDeleteProgram(m_drawShaderID);
	glDeleteProgram(m_updateShaderID);
}

void ParticleSystem::Initalise(unsigned int a_maxParticles, float a_lifespanMin, float a_lifespanMax, float a_velocityMin, float a_velocityMax, float a_startSize, float a_endSize, const glm::vec4 & a_startColour, const glm::vec4 & a_endColour)
{
	// store all variables passed in
	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_startSize = a_startSize;
	m_endSize = a_endSize;
	m_velocityMin = a_velocityMin;
	m_velocityMax = a_velocityMax;
	m_lifespanMin = a_lifespanMin;
	m_lifespanMax = a_lifespanMax;
	m_maxParticles = a_maxParticles;

	// create particle array
	m_particles = new Particle[a_maxParticles];
	// set our starting ping-pong buffer
	m_activeBuffer = 0;
	CreateBuffers();
	CreateUpdateShader();
	CreateDrawShader();
}

void ParticleSystem::Draw(float time, const glm::mat4 & a_cameraTransform, const glm::mat4 & a_projectionView)
{
	// update the particles using transform feedback
	glUseProgram(m_updateShaderID);

	// bind time information
	int location = glGetUniformLocation(m_updateShaderID, "time");
	if (location == -1) LOG_ERROR("Uniform failed.", location);
	glUniform1f(location, time);

	float deltaTime = time - m_lastDrawTime;
	m_lastDrawTime = time;

	location = glGetUniformLocation(m_updateShaderID, "deltaTime");
	if (location == -1) LOG_ERROR("Uniform failed.", location);
	glUniform1f(location, deltaTime);

	// bind emitter's position
	location = glGetUniformLocation(m_updateShaderID, "emitterPosition");
	if (location == -1) LOG_ERROR("Uniform failed.", location);
	glUniform3fv(location, 1, &m_position[0]);

	// disable rasterisation
	glEnable(GL_RASTERIZER_DISCARD);

	// bind the buffer we will update
	glBindVertexArray(m_vao[m_activeBuffer]);

	// work out the "other" buffer
	unsigned int otherBuffer = (m_activeBuffer + 1) % 2;

	// bind the buffer we will update into as points
	// and begin transform feedback
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[otherBuffer]);
	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	// disable transform feedback and enable rasterization again
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	// draw the particles using the Geometry SHader to billboard them
	glUseProgram(m_drawShaderID);

	location = glGetUniformLocation(m_drawShaderID, "projectionView");
	glUniformMatrix4fv(location, 1, false, &a_projectionView[0][0]);

	location = glGetUniformLocation(m_drawShaderID, "cameraTransform");
	glUniformMatrix4fv(location, 1, false, &a_cameraTransform[0][0]);

	// draw particles in the "other" buffer
	glBindVertexArray(m_vao[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, m_maxParticles);

	// swap for next frame
	m_activeBuffer = otherBuffer;

}

void ParticleSystem::CreateBuffers()
{
	// create opengl buffers
	glGenVertexArrays(2, m_vao);
	glGenBuffers(2, m_vbo);

	// setup the first buffer
	glBindVertexArray(m_vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles *
		sizeof(Particle), m_particles, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // velocity
	glEnableVertexAttribArray(2); // lifetime
	glEnableVertexAttribArray(3); // lifespan
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(Particle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(Particle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,
		sizeof(Particle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
		sizeof(Particle), ((char*)0) + 28);

	// setup the second buffer
	glBindVertexArray(m_vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles *
		sizeof(Particle), 0, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // velocity
	glEnableVertexAttribArray(2); // lifetime
	glEnableVertexAttribArray(3); // lifespan
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		sizeof(Particle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		sizeof(Particle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,
		sizeof(Particle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE,
		sizeof(Particle), ((char*)0) + 28);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::CreateUpdateShader()
{
	// create a vertex shader handle

	unsigned int vs = Shader::MakeShader(GL_VERTEX_SHADER, "./shaders/particles/particleUpdate.vert");

	m_updateShaderID = glCreateProgram();
	glAttachShader(m_updateShaderID, vs);
	// specify the data that we will stream back
	const char* varyings[] = { "position", "velocity",
		"lifetime", "lifespan" };
	glTransformFeedbackVaryings(m_updateShaderID, 4, varyings,
		GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(m_updateShaderID);

	// remove unneeded handles
	glDeleteShader(vs);
	// bind the shader so that we can set some
	// uniforms that don't change per-frame
	glUseProgram(m_updateShaderID);
	// bind lifetime minimum and maximum
	int location = glGetUniformLocation(m_updateShaderID, "lifeMin");
	if (location == -1) LOG_ERROR("Uniform failed.", location);
	glUniform1f(location, m_lifespanMin);

	location = glGetUniformLocation(m_updateShaderID, "lifeMax");
	if (location == -1) LOG_ERROR("Uniform failed.", location);
	glUniform1f(location, m_lifespanMax);
}

void ParticleSystem::CreateDrawShader()
{
	// load shaders from files and turn into GL shader handles
	unsigned int vs = Shader::MakeShader(GL_VERTEX_SHADER, "./shaders/particles/particle.vert");
	unsigned int gs = Shader::MakeShader(GL_GEOMETRY_SHADER, "./shaders/particles/particle.geom");
	unsigned int fs = Shader::MakeShader(GL_FRAGMENT_SHADER, "./shaders/particles/particle.frag");

	m_drawShaderID = glCreateProgram();
	glAttachShader(m_drawShaderID, vs);
	glAttachShader(m_drawShaderID, fs);
	glAttachShader(m_drawShaderID, gs);
	glLinkProgram(m_drawShaderID);

	// remove unneeded handles
	glDeleteShader(vs);
	glDeleteShader(gs);
	glDeleteShader(fs);

	// bind the shader so that we can set
	// some uniforms that don't change per-frame
	glUseProgram(m_drawShaderID);

	// bind size information for interpolation that won’t change
	int location = glGetUniformLocation(m_drawShaderID, "sizeStart");
	glUniform1f(location, m_startSize);
	location = glGetUniformLocation(m_drawShaderID, "sizeEnd");
	glUniform1f(location, m_endSize);

	// bind colour information for interpolation that wont change
	location = glGetUniformLocation(m_drawShaderID, "colourStart");
	glUniform4fv(location, 1, &m_startColour[0]);
	location = glGetUniformLocation(m_drawShaderID, "colourEnd");
	glUniform4fv(location, 1, &m_endColour[0]);

}
