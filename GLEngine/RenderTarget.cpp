#include "RenderTarget.h"

#include "DEBUG_NEW_LEAK_DETECT.h"
#include "DEBUG_WINDOWS_ERR_MSG.h"

#include <gl_core_4_4.h>
#include "ApplicationBase.h"
#include "Texture.h"


RenderTarget::RenderTarget()
	: m_frameBufferID(-1)
	, m_colour(new Texture())
	, m_depth(new Texture())
{
}


RenderTarget::~RenderTarget()
{
	delete m_colour;
	delete m_depth;
	if (m_frameBufferID != -1) glDeleteFramebuffers(1, &m_frameBufferID);
}

RenderTarget::RenderTarget(RenderTarget && _other)
{
	m_frameBufferID = _other.m_frameBufferID;
	delete m_colour;
	m_colour = _other.m_colour;
	delete m_depth;
	m_depth = _other.m_depth;
	_other.m_frameBufferID = -1;
	_other.m_depth = nullptr;
	_other.m_colour = nullptr;
}

void RenderTarget::Generate(unsigned int _width, unsigned int _height)
{
	m_colour->m_width = _width;
	m_colour->m_height = _height;
	m_depth->m_width = _width;
	m_depth->m_height = _height;

	CreateFBO(_width, _height);
}

void RenderTarget::Bind()
{
	glEnable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);
	//glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//glDrawBuffer(GL_NONE);
	glViewport(0, 0, m_colour->m_width, m_colour->m_height);
	// clear old capture
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderTarget::Unbind()
{
	// bind the back-buffer 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, GLE::APP->GetWindowWidth(), GLE::APP->GetWindowHeight());
}

bool RenderTarget::CreateFBO(const int _width, const int _height)
{
	bool result = false;
	//generate textures for FBO usage. You could use other formats here, e.g. GL_RGBA8 for color
	m_colour->CreateTexture(_width, _height, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	m_depth->CreateTexture(_width, _height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
	//generate and bind FBO
	glGenFramebuffers(1, &m_frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);
	//bind color and depth texture to FBO you could also use glFramebufferTexture2D with GL_TEXTURE_2D
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_colour->m_id, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth->m_id, 0);
	//check if FBO was created ok
	if (GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
		result = true;
	}
	else {
		LOG_ERROR("Render Target ", m_frameBufferID, " is set up incorrectly.");
	}
	//unbind FBO for now
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return result;
}
