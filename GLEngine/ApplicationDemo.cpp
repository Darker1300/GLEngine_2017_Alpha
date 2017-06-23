#include "DEBUG_NEW_LEAK_DETECT.h"
#define USE_CONSOLE_LOG true
#include "DEBUG_WINDOWS_ERR_MSG.h"

#include "ApplicationDemo.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <gl_core_4_4.h>
#include <GlFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Texture.h"
#include "Shader.h"
#include "RenderData.h"
#include "GeometryHelper.h"
#include "Transform.h"
#include "Material.h"
#include "RenderableObject.h"
#include "Camera.h"
#include "Light.h"
#include "RenderTarget.h"
#include "ParticleSystem.h"
#include <imgui\imgui.h>

ApplicationDemo::ApplicationDemo()
	: ApplicationBase("Game Engine Demo", 1280, 720) {}

ApplicationDemo::~ApplicationDemo() {}

int ApplicationDemo::Start()
{
	if (ApplicationBase::Start()) return -1;

	// Set Background color
	SetBackgroundColor(0.25f, 0.25f, 0.25f);

	// Set DrawMode:
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Turn on Wireframe:
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	m_ambientLighting = glm::vec3(1.f, 1.f, 1.f);
	m_specPowerLighting = 16;
	m_drawCalls = 0;
	m_renderTargetCalls = 0;
	m_doBloomEffect = false;
	m_doDistortEffect = false;

	// Camera
	m_camera1 = new Camera();

	// Shaders
	m_tintTexShader = new Shader("./shaders/tintTex.vert", "./shaders/tintTex.frag");
	m_phongShader = new Shader("./shaders/phongOBJ.vert", "./shaders/phongOBJ.frag");
	m_depthTargetShader = new Shader("./shaders/effects/depthTarget.vert", "./shaders/effects/depthTarget.frag");
	m_bloomShader = new Shader("./shaders/effects/bloom.vert", "./shaders/effects/bloom.frag");
	m_distortShader = new Shader("./shaders/effects/distortWave.vert", "./shaders/effects/distortWave.frag");

	// Textures
	m_whiteTex = new Texture("./textures/white.png");

	m_groundDiffuseTex = new Texture("./textures/metal1/161_diffuse.png");
	m_groundSpecularTex = new Texture("./textures/metal1/161_spec.png");
	m_groundNormalTex = new Texture("./textures/metal1/161_norm.png");

	m_signDiffuseTex = new Texture("./textures/matt.png");
	m_spearDiffuseTex = new Texture("./models/soulspear/soulspear_diffuse.tga");
	m_spearSpecularTex = new Texture("./models/soulspear/soulspear_specular.tga");
	m_spearNormalTex = new Texture("./models/soulspear/soulspear_normal.tga");

	// Render Targets
	m_renderTarget1 = new RenderTarget();
	m_renderTarget1->Generate(GLE::APP->GetWindowWidth(), GLE::APP->GetWindowHeight());

	m_renderTarget2 = new RenderTarget();
	m_renderTarget2->Generate(GLE::APP->GetWindowWidth(), GLE::APP->GetWindowHeight());

	// RenderData
	m_groundRenderData = GeometryHelper::CreatePlane(10, 10, 10, 10);
	m_signRenderData = GeometryHelper::CreatePlane(2, 2, 4, 2);
	m_spearRenderData = GeometryHelper::LoadOBJFromDisk("./models/soulspear/soulspear.obj"); //stanford/dragon
	m_sphereRenderData = GeometryHelper::CreateSphere(0.5f, 16, 16);

	// fullscreen quad
	m_screenRenderData = GeometryHelper::CreateFullscreenQuad((float)GLE::APP->GetWindowWidth(), (float)GLE::APP->GetWindowHeight());

	// Materials
	m_groundMat = new Material(m_phongShader);
	m_spearMat = new Material(m_phongShader);
	m_signMat = new Material(m_tintTexShader);
	m_screenMat1 = new Material(m_bloomShader);
	m_screenMat2 = new Material(m_distortShader);
	m_depthTargetMat = new Material(m_depthTargetShader);

	// Set Textures
	m_groundMat->m_textures["normalMap"] = m_groundNormalTex;
	m_groundMat->m_textures["diffuseMap"] = m_groundDiffuseTex;
	m_groundMat->m_textures["specularMap"] = m_groundSpecularTex;

	m_signMat->m_textures["diffuseMap"] = m_signDiffuseTex;

	m_spearMat->m_textures["normalMap"] = m_spearNormalTex;
	m_spearMat->m_textures["diffuseMap"] = m_spearDiffuseTex;
	m_spearMat->m_textures["specularMap"] = m_spearSpecularTex;

	m_screenMat1->m_textures["depthMap"] = m_renderTarget1->m_depth;
	m_screenMat1->m_textures["colourMap"] = m_renderTarget1->m_colour;

	m_screenMat2->m_textures["depthMap"] = m_renderTarget2->m_depth;
	m_screenMat2->m_textures["colourMap"] = m_renderTarget2->m_colour;

	m_depthTargetMat->m_textures["depthMap"] = m_renderTarget1->m_depth;
	m_depthTargetMat->m_textures["colourMap"] = m_renderTarget1->m_colour;

	// RenderableObjects
	m_groundObj = new RenderableObject(m_groundMat, std::vector<RenderData*>{ m_groundRenderData });
	m_signObj = new RenderableObject(m_signMat, std::vector<RenderData*>{ m_signRenderData });
	m_spearObj = new RenderableObject(m_spearMat, m_spearRenderData);
	m_screenObj = new RenderableObject(m_screenMat1, std::vector<RenderData*>{m_screenRenderData});
	m_lightObj = new RenderableObject(m_groundMat, std::vector<RenderData*>{m_sphereRenderData});

	// Light
	m_light1 = new Light(m_lightObj->m_transform);

	// Particles
	m_emitter1 = new ParticleSystem();
	m_emitter1->Initalise(
		100000,
		0.1f, 5.0f,
		5, 20,
		1, 0.1f,
		glm::vec4(1, 0, 1, 1), glm::vec4(0, 0, 1, 0));

	// ---- Set Transforms ----
	// Camera Transform
	m_camera1->position += Vector3::backward * 18;
	m_camera1->position += Vector3::up * 5;
	m_camera1->yaw += glm::radians(180.f);

	// Spear Transform
	m_spearObj->m_transform.position += Vector3::up * 1;

	// Ground Plane Transform
	m_groundObj->m_transform.AddPitch(3.14159265f * 0.5f);

	// Sign Plane Transform
	m_signObj->m_transform.AddYaw(3.14159265f * 1.0f);
	m_signObj->m_transform.position += Vector3::up * 10;

	// Light Transform
	float currentTime = (float)glfwGetTime();
	m_lightObj->m_transform.position.x = sinf(currentTime) * 5;
	m_lightObj->m_transform.position.z = cosf(currentTime) * 5;
	m_lightObj->m_transform.position.y = 3;

	// Particle Emitter Transform
	m_emitter1->m_position.z = 6;

	return 0;
}

int ApplicationDemo::Shutdown()
{
	if (ApplicationBase::Shutdown()) return -1;

	delete m_light1;

	delete m_lightObj;
	delete m_screenObj;
	delete m_spearObj;
	delete m_signObj;
	delete m_groundObj;

	delete m_emitter1;

	delete m_depthTargetMat;
	delete m_screenMat2;
	delete m_screenMat1;
	delete m_signMat;
	delete m_spearMat;
	delete m_groundMat;

	delete m_sphereRenderData;
	delete m_screenRenderData;
	for (auto data : m_spearRenderData) delete data;
	delete m_signRenderData;
	delete m_groundRenderData;

	delete m_renderTarget2;
	delete m_renderTarget1;

	delete m_groundDiffuseTex;
	delete m_groundSpecularTex;
	delete m_groundNormalTex;

	delete m_spearNormalTex;
	delete m_spearSpecularTex;
	delete m_spearDiffuseTex;

	delete m_signDiffuseTex;

	delete m_whiteTex;

	delete m_distortShader;
	delete m_bloomShader;
	delete m_depthTargetShader;
	delete m_phongShader;
	delete m_tintTexShader;

	delete m_camera1;
	return 0;
}

int ApplicationDemo::FixedUpdate(double _deltaTime)
{
	if (ApplicationBase::FixedUpdate(_deltaTime)) return -1;

	// Camera controls
	m_camera1->UpdateFly(GetWindow(), (float)_deltaTime, 3, 1);

	// Transformations
	m_spearObj->m_transform.AddYaw(3.14159265f * (float)_deltaTime * -0.001f);

	m_lightObj->m_transform.position.x = sinf((float)glfwGetTime()) * 5;
	m_lightObj->m_transform.position.z = cosf((float)glfwGetTime()) * 5;
	//m_light->m_transform.AddYaw(3.14159265f * (float)_deltaTime * 0.8f);
	//m_light->m_transform.AddRoll(3.14159265f * (float)_deltaTime * 0.25f);

	m_emitter1->m_position.x = sinf((float)glfwGetTime()) * -10;
	m_emitter1->m_position.y = cosf((float)glfwGetTime()) * -10;

	return 0;
}

int ApplicationDemo::Update(double _deltaTime)
{
	if (ApplicationBase::Update(_deltaTime)) return -1;

	return 0;
}

int ApplicationDemo::Draw()
{
	if (ApplicationBase::Draw()) return -1;

	// Reset DrawCall Counter
	m_drawCalls = 0;
	m_renderTargetCalls = 0;

	// Calculate Camera
	glm::mat4 projView = m_camera1->GetProjectionViewMatrix();
	Camera::Frustum frustum = m_camera1->GetFrustum();

	// Light Getters
	glm::vec3 lightPos = m_lightObj->m_transform.position;
	glm::vec3 lightDir = m_light1->GetDirection(m_camera1->position);



	// Bind RenderTarget
	if (m_doBloomEffect || m_doDistortEffect) {
		m_renderTarget1->Bind();
		m_renderTargetCalls++;
	}

	if (m_groundObj->FrustrumCollision(frustum)) {
		m_drawCalls++;
		// Update material
		m_groundObj->Bind();
		m_groundMat->ApplyUniformVec3("ambientLight", m_ambientLighting);
		m_groundMat->ApplyUniformFloat("specularLightPower", (float)m_specPowerLighting);
		m_groundMat->ApplyUniformMat4("projectionViewMatrix", projView);
		m_groundMat->ApplyUniformMat4("modelMatrix", m_groundObj->m_transform.GetLocalMatrix());
		// Lighting
		m_groundMat->ApplyUniformVec3("cameraPos", m_camera1->position);
		m_groundMat->ApplyUniformVec3("lightDir", lightDir);
		// Render
		m_groundObj->Render();
		// Unbind
		m_groundObj->Unbind();
	}

	if (m_signObj->FrustrumCollision(frustum)) {
		m_drawCalls++;
		// Update material
		m_signObj->Bind();
		//m_signMat->ApplyUniformVec3("ambientMat", { 0, 0, 0 }); // { 0, 0.5f, 1 });
		m_signMat->ApplyUniformMat4("projectionViewMatrix", projView);
		m_signMat->ApplyUniformMat4("modelMatrix", m_signObj->m_transform.GetLocalMatrix());
		// Render
		m_signObj->Render();
		// Unbind
		m_signObj->Unbind();
	}

	if (m_lightObj->FrustrumCollision(frustum)) {
		m_drawCalls++;
		m_lightObj->Bind();
		m_lightObj->m_material->ApplyUniformVec3("ambientLight", m_ambientLighting);
		m_lightObj->m_material->ApplyUniformFloat("specularLightPower", (float)m_specPowerLighting);
		m_lightObj->m_material->ApplyUniformMat4("projectionViewMatrix", projView);
		m_lightObj->m_material->ApplyUniformMat4("modelMatrix", m_lightObj->m_transform.GetLocalMatrix());
		// Render
		m_lightObj->Render();
		// Unbind
		m_lightObj->Unbind();
	}

	if (m_spearObj->FrustrumCollision(frustum)) {
		m_drawCalls++;
		// Update material
		m_spearObj->Bind();
		//m_spearMat->ApplyUniformVec3("ambientMat", { 0, 0, 0 });
		m_spearMat->ApplyUniformVec3("ambientLight", m_ambientLighting);
		m_spearMat->ApplyUniformFloat("specularLightPower", (float)m_specPowerLighting);
		m_spearMat->ApplyUniformMat4("projectionViewMatrix", projView);
		m_spearMat->ApplyUniformMat4("modelMatrix", m_spearObj->m_transform.GetLocalMatrix());
		// Lighting
		m_spearMat->ApplyUniformVec3("cameraPos", m_camera1->position);
		m_spearMat->ApplyUniformVec3("lightDir", lightDir);
		// Render
		m_spearObj->Render();
		// Unbind
		m_spearObj->Unbind();
	}

	// Particles
	m_emitter1->Draw((float)glfwGetTime(),
		m_camera1->GetLocalMatrix(),
		projView);
	m_drawCalls++;

	// Unbind RenderTarget
	if (m_doBloomEffect || m_doDistortEffect) {
		m_renderTarget1->Unbind();

		const bool bothEffects = m_doBloomEffect && m_doDistortEffect;

		if (bothEffects) {
			m_screenMat1->m_shader = m_distortShader;
			m_screenMat2->m_shader = m_bloomShader;
		}
		else {
			m_screenMat1->m_shader = m_doBloomEffect ? m_bloomShader : m_distortShader;
		}

		if (m_screenMat1->m_shader == m_distortShader) {
			m_screenMat1->Bind();
			m_screenMat1->ApplyUniformFloat("distortTime", (float)glfwGetTime());
			m_screenMat1->Unbind();
		}

		if (bothEffects) {
			m_renderTarget2->Bind();
			m_renderTargetCalls++;
		}

		m_screenObj->m_material = m_screenMat1;
		m_screenObj->Bind();
		m_screenObj->Render();
		m_drawCalls++;
		m_screenObj->Unbind();

		if (bothEffects) {
			m_renderTarget2->Unbind();

			m_screenObj->m_material = m_screenMat2;
			m_screenObj->Bind();
			m_screenObj->Render();
			m_drawCalls++;
			m_screenObj->Unbind();
		}
	}

	return 0;
}

int ApplicationDemo::GUIDraw()
{
	ImGui::Text("Controls:");
	ImGui::Text("WASD: Move Directionally. E/Q: Up/Down");
	ImGui::Text("Keypad 8/4/5/6, 7/9: Rotate");
	ImGui::Text("Camera Effects:");
	ImGui::Checkbox("Bloom", &m_doBloomEffect);
	ImGui::Checkbox("Distort", &m_doDistortEffect);
	ImGui::Text("Lighting:");
	ImGui::Text("Ambient:");
	ImGui::SliderFloat3("", &m_ambientLighting[0], 0.0f, 1.0f);
	ImGui::Text("Specular Power:");
	ImGui::SliderInt("", &m_specPowerLighting, 1, 128);
	ImGui::Text((std::string("Draw Calls: ") + std::to_string(m_drawCalls)).c_str());
	ImGui::Text((std::string("RenderTargets: ") + std::to_string(m_renderTargetCalls)).c_str());
	return 0;
}
