#pragma once

class CameraBase;
class Camera;

class Shader;
class RenderData;
class Texture;
class Material;
class Transform;
class RenderableObject;
class Light;
class RenderTarget;
class ParticleSystem;

#include "ApplicationBase.h"
#include <vector>
#include <glm\vec3.hpp>

class ApplicationDemo : public ApplicationBase
{
public:
	ApplicationDemo();
	~ApplicationDemo();

	int Start();
	int Shutdown();
	int FixedUpdate(double _deltaTime);
	int Update(double _deltaTime);
	int Draw();
	int GUIDraw();

protected:
	bool m_doBloomEffect;
	bool m_doDistortEffect;
	unsigned int m_drawCalls;
	unsigned int m_renderTargetCalls;

	Camera* m_camera1;

	Shader* m_tintTexShader;
	Shader* m_phongShader;
	Shader* m_depthTargetShader;
	Shader* m_bloomShader;
	Shader* m_distortShader;

	Texture* m_whiteTex;
	Texture* m_groundDiffuseTex;
	Texture* m_groundSpecularTex;
	Texture* m_groundNormalTex;
	Texture* m_signDiffuseTex;
	Texture* m_spearDiffuseTex;
	Texture* m_spearSpecularTex;
	Texture* m_spearNormalTex;

	RenderTarget* m_renderTarget1;
	RenderTarget* m_renderTarget2;

	RenderData* m_groundRenderData;
	RenderData* m_signRenderData;
	std::vector<RenderData*> m_spearRenderData;
	RenderData* m_screenRenderData;
	RenderData* m_sphereRenderData;

	Material* m_groundMat;
	Material* m_signMat;
	Material* m_spearMat;
	Material* m_screenMat1;
	Material* m_screenMat2;
	Material* m_depthTargetMat;

	RenderableObject* m_groundObj;
	RenderableObject* m_signObj;
	RenderableObject* m_spearObj;
	RenderableObject* m_screenObj;
	RenderableObject* m_lightObj;

	ParticleSystem* m_emitter1;

	Light* m_light1;
	glm::vec3 m_ambientLighting;
	int m_specPowerLighting;
};
