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

	Camera* m_camera;

	Shader* m_primativeShader;
	Shader* m_basicShader;
	Shader* m_tintTexShader;
	Shader* m_phongShader;
	Shader* m_depthTargetShader;
	Shader* m_bloomShader;
	Shader* m_distortShader;

	Texture* m_texWhite;
	Texture* m_texGroundDiffuse;
	Texture* m_texGroundSpecular;
	Texture* m_texGroundNormal;
	Texture* m_texSignDiffuse;
	Texture* m_texSpearDiffuse;
	Texture* m_texSpearSpecular;
	Texture* m_texSpearNormal;

	RenderTarget* m_renderTarget1;
	RenderTarget* m_renderTarget2;

	RenderData* m_groundRenderData;
	RenderData* m_signRenderData;
	std::vector<RenderData*> m_spearRenderData;
	//RenderData* m_mirrorRenderData;
	RenderData* m_screenRenderData;
	RenderData* m_sphereRenderData;

	Material* m_groundMat;
	Material* m_signMat;
	Material* m_spearMat;
	Material* m_screenMat1;
	Material* m_screenMat2;
	Material* m_lightObjMat;

	RenderableObject* m_ground;
	RenderableObject* m_sign;
	RenderableObject* m_spear;
	RenderableObject* m_screenObject;
	RenderableObject* m_light;

	ParticleSystem* m_emitter;

	Light* m_lightAlpha;
};
