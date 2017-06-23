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

	m_drawCalls = 0;
	m_renderTargetCalls = 0;
	m_doBloomEffect = false;
	m_doDistortEffect = false;

	// Set up Camera
	m_camera = new Camera();
	m_camera->position += Vector3::backward * 18;
	m_camera->position += Vector3::up * 5;
	m_camera->yaw += glm::radians(180.f);

	// Shaders
	m_primativeShader = new Shader("./shaders/basic.vert", "./shaders/basic.frag");
	m_basicShader = new Shader("./shaders/basicOBJ.vert", "./shaders/basicOBJ.frag");
	m_tintTexShader = new Shader("./shaders/ian.vert", "./shaders/ian.frag");
	m_phongShader = new Shader("./shaders/phongOBJ.vert", "./shaders/phongOBJ.frag");
	m_depthTargetShader = new Shader("./shaders/effects/depthTarget.vert", "./shaders/effects/depthTarget.frag");
	m_bloomShader = new Shader("./shaders/effects/bloom.vert", "./shaders/effects/bloom.frag");
	m_distortShader = new Shader("./shaders/effects/distortWave.vert", "./shaders/effects/distortWave.frag");

	// Textures
	m_texWhite = new Texture("./textures/white.png");

	m_texGroundDiffuse = new Texture("./textures/metal1/161_diffuse.png");
	m_texGroundSpecular = new Texture("./textures/metal1/161_spec.png");
	m_texGroundNormal = new Texture("./textures/metal1/161_norm.png");

	m_texSignDiffuse = new Texture("./textures/matt.png");
	m_texSpearDiffuse = new Texture("./models/soulspear/soulspear_diffuse.tga");
	m_texSpearSpecular = new Texture("./models/soulspear/soulspear_specular.tga");
	m_texSpearNormal = new Texture("./models/soulspear/soulspear_normal.tga");

	m_renderTarget1 = new RenderTarget();
	m_renderTarget1->Generate(GLE::APP->GetWindowWidth(), GLE::APP->GetWindowHeight());

	m_renderTarget2 = new RenderTarget();
	m_renderTarget2->Generate(GLE::APP->GetWindowWidth(), GLE::APP->GetWindowHeight());

	// RenderData
	m_groundRenderData = GeometryHelper::CreatePlane(10, 10, 10, 10);
	m_signRenderData = GeometryHelper::CreatePlane(2, 2, 4, 2);
	m_spearRenderData = GeometryHelper::LoadOBJFromDisk("./models/soulspear/soulspear.obj"); //stanford/dragon
	//m_mirrorRenderData = GeometryHelper::CreatePlane(2, 2, 4, 4);
	m_sphereRenderData = GeometryHelper::CreateSphere(0.5f, 16, 16);

	// fullscreen quad
	m_screenRenderData = GeometryHelper::CreateFullscreenQuad((float)GLE::APP->GetWindowWidth(), (float)GLE::APP->GetWindowHeight());

	// Materials
	m_groundMat = new Material(m_phongShader);
	m_spearMat = new Material(m_phongShader);
	m_signMat = new Material(m_tintTexShader);
	m_screenMat1 = new Material(m_bloomShader);
	m_screenMat2 = new Material(m_distortShader);
	m_lightObjMat = new Material(m_depthTargetShader);

	// Set Textures
	m_groundMat->m_textures["normalMap"] = m_texGroundDiffuse;
	m_groundMat->m_textures["diffuseMap"] = m_texGroundSpecular;
	m_groundMat->m_textures["specularMap"] = m_texGroundNormal;

	m_signMat->m_textures["diffuseMap"] = m_texSignDiffuse;

	m_spearMat->m_textures["normalMap"] = m_texSpearNormal;
	m_spearMat->m_textures["diffuseMap"] = m_texSpearDiffuse;
	m_spearMat->m_textures["specularMap"] = m_texSpearSpecular;

	m_screenMat1->m_textures["depthMap"] = m_renderTarget1->m_depth;
	m_screenMat1->m_textures["colourMap"] = m_renderTarget1->m_colour;

	m_screenMat2->m_textures["depthMap"] = m_renderTarget2->m_depth;
	m_screenMat2->m_textures["colourMap"] = m_renderTarget2->m_colour;

	m_lightObjMat->m_textures["depthMap"] = m_renderTarget1->m_depth;
	m_lightObjMat->m_textures["colourMap"] = m_renderTarget1->m_colour;

	// RenderableObjects
	m_ground = new RenderableObject(m_groundMat, std::vector<RenderData*>{ m_groundRenderData });
	m_sign = new RenderableObject(m_signMat, std::vector<RenderData*>{ m_signRenderData });
	m_spear = new RenderableObject(m_spearMat, m_spearRenderData);
	m_screenObject = new RenderableObject(m_screenMat1, std::vector<RenderData*>{m_screenRenderData});
	m_light = new RenderableObject(m_lightObjMat, std::vector<RenderData*>{m_sphereRenderData});

	// Misc
	m_lightAlpha = new Light(m_light->m_transform);

	m_emitter = new ParticleSystem();
	m_emitter->Initalise(
		100000,
		0.1f, 5.0f,
		5, 20,
		1, 0.1f,
		glm::vec4(1, 0, 1, 1), glm::vec4(0, 0, 1, 0));

	// Set Transforms
	m_spear->m_transform.position += Vector3::up * 1;

	m_ground->m_transform.AddPitch(3.14159265f * 0.5f);

	m_sign->m_transform.AddYaw(3.14159265f * 1.0f);
	m_sign->m_transform.position += Vector3::up * 10;

	m_screenObject->m_transform.position += Vector3::up * 2;
	m_screenObject->m_transform.position += Vector3::right * 2;

	float currentTime = (float)glfwGetTime();
	m_lightAlpha->m_transform->position.x = sinf(currentTime) * 5;
	m_lightAlpha->m_transform->position.z = cosf(currentTime) * 5;
	m_lightAlpha->m_transform->position.y = 3;

	m_emitter->m_position.z = 6;

	return 0;
}

int ApplicationDemo::Shutdown()
{
	if (ApplicationBase::Shutdown()) return -1;

	delete m_lightAlpha;

	delete m_light;
	delete m_screenObject;
	delete m_spear;
	delete m_sign;
	delete m_ground;
	delete m_emitter;

	delete m_lightObjMat;
	delete m_screenMat2;
	delete m_screenMat1;
	delete m_signMat;
	delete m_spearMat;
	delete m_groundMat;

	delete m_sphereRenderData;
	delete m_screenRenderData;
	//delete m_mirrorRenderData;
	for (auto data : m_spearRenderData) delete data;
	delete m_signRenderData;
	delete m_groundRenderData;

	delete m_renderTarget1;

	delete m_texGroundDiffuse;
	delete m_texGroundSpecular;
	delete m_texGroundNormal;

	delete m_texSpearNormal;
	delete m_texSpearSpecular;
	delete m_texSpearDiffuse;

	delete m_texSignDiffuse;

	delete m_texWhite;

	delete m_distortShader;
	delete m_bloomShader;
	delete m_depthTargetShader;
	delete m_phongShader;
	delete m_tintTexShader;
	delete m_basicShader;
	delete m_primativeShader;

	delete m_camera;
	return 0;
}

int ApplicationDemo::FixedUpdate(double _deltaTime)
{
	if (ApplicationBase::FixedUpdate(_deltaTime)) return -1;

	// Camera controls
	m_camera->UpdateFly(GetWindow(), (float)_deltaTime, 3, 1);

	// Transformations
	m_spear->m_transform.AddYaw(3.14159265f * (float)_deltaTime * -0.001f);

	m_light->m_transform.position.x = sinf((float)glfwGetTime()) * 5;
	m_light->m_transform.position.z = cosf((float)glfwGetTime()) * 5;
	m_light->m_transform.AddYaw(3.14159265f * (float)_deltaTime * 0.8f);
	m_light->m_transform.AddRoll(3.14159265f * (float)_deltaTime * 0.25f);

	m_emitter->m_position.x = sinf((float)glfwGetTime()) * -10;
	m_emitter->m_position.y = cosf((float)glfwGetTime()) * -10;

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
	glm::mat4 projView = m_camera->GetProjectionViewMatrix();
	Camera::Frustum frustum = m_camera->GetFrustum();

	// Light Getters
	glm::vec3 lightPos = m_light->m_transform.position;
	glm::vec3 lightDir = m_lightAlpha->GetDirection(m_camera->position);

	// Bind RenderTarget
	if (m_doBloomEffect || m_doDistortEffect) {
		m_renderTarget1->Bind();
		m_renderTargetCalls++;
	}

	if (m_ground->FrustrumCollision(frustum)) {
		m_drawCalls++;
		// Update material
		m_ground->Bind();
		//m_groundMat->ApplyUniformVec3("ambientMat", { 0, 0, 0 }); // { 0.0f, 0.4f, 0.0f });
		m_groundMat->ApplyUniformMat4("projectionViewMatrix", projView);
		m_groundMat->ApplyUniformMat4("modelMatrix", m_ground->m_transform.GetLocalMatrix());
		// Lighting
		m_groundMat->ApplyUniformVec3("cameraPos", m_camera->position);
		m_groundMat->ApplyUniformVec3("lightDir", lightDir);
		// Render
		m_ground->Render();
		// Unbind
		m_ground->Unbind();
	}

	// Sphere
	//m_groundMat->Bind();
	//m_groundMat->ApplyUniformMat4("projectionViewMatrix", projView);
	//m_groundMat->ApplyUniformMat4("modelMatrix", m_lightAlpha->m_transform.GetLocalMatrix());
	//// Lighting
	//m_groundMat->ApplyUniformVec3("cameraPos", m_camera->position);
	//m_groundMat->ApplyUniformVec3("lightDir", lightDir);
	//m_sphereRenderData->Render();
	// Unbind
	//m_sphereRenderData->Unbind();

	if (m_sign->FrustrumCollision(frustum)) {
		m_drawCalls++;
		// Update material
		m_sign->Bind();
		//m_signMat->ApplyUniformVec3("ambientMat", { 0, 0, 0 }); // { 0, 0.5f, 1 });
		m_signMat->ApplyUniformMat4("projectionViewMatrix", projView);
		m_signMat->ApplyUniformMat4("modelMatrix", m_sign->m_transform.GetLocalMatrix());
		// Render
		m_sign->Render();
		// Unbind
		m_sign->Unbind();
	}

	if (m_light->FrustrumCollision(frustum)) {
		m_drawCalls++;
		// Render for Light
		m_light->Bind();
		m_groundMat->Bind();
		m_groundMat->ApplyUniformMat4("projectionViewMatrix", projView);
		m_groundMat->ApplyUniformMat4("modelMatrix", m_light->m_transform.GetLocalMatrix());
		// Render
		m_light->Render();
		// Unbind
		m_light->Unbind();
	}

	if (m_spear->FrustrumCollision(frustum)) {
		m_drawCalls++;
		// Update material
		m_spear->Bind();
		//m_spearMat->ApplyUniformVec3("ambientMat", { 0, 0, 0 });
		m_spearMat->ApplyUniformMat4("projectionViewMatrix", projView);
		m_spearMat->ApplyUniformMat4("modelMatrix", m_spear->m_transform.GetLocalMatrix());
		// Lighting
		m_spearMat->ApplyUniformVec3("cameraPos", m_camera->position);
		m_spearMat->ApplyUniformVec3("lightDir", lightDir);
		// Render
		m_spear->Render();
		// Unbind
		m_spear->Unbind();
	}

	m_emitter->Draw((float)glfwGetTime(),
		m_camera->GetLocalMatrix(),
		projView);
	m_drawCalls++;

	//m_mirrorMat->ApplyUniformVec3("ambientMat", { 1, 1, 1 });
	//m_mirrorMat->ApplyUniformMat4("projectionViewMatrix", projView);
	//m_mirrorMat->ApplyUniformMat4("modelMatrix", m_mirror->m_transform.GetLocalMatrix());
	//float move = (float)glfwGetTime();// / 1000.0 * 2 * 3.14159 * .75;
	//m_mirrorMat->ApplyUniformFloat("distortTime", move);

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

		m_screenObject->m_material = m_screenMat1;
		m_screenObject->Bind();
		m_screenObject->Render();
		m_drawCalls++;
		m_screenObject->Unbind();

		if (bothEffects) {
			m_renderTarget2->Unbind();

			m_screenObject->m_material = m_screenMat2;
			m_screenObject->Bind();
			m_screenObject->Render();
			m_drawCalls++;
			m_screenObject->Unbind();
		}



		if (m_doBloomEffect) {

		}

		if (m_doDistortEffect) {
			// Update Distort Material parameters

		}



		//if (!(m_doBloomEffect && m_doDistortEffect)) {
		//	if (m_doBloomEffect) {

		//	}
		//	else {

		//	}
		//}
		//else {

		//}

		//// Render
		//if (m_doDistortEffect) {
		//	m_screenObject->m_material->m_shader = m_distortShader;
		//	m_screenObject->Bind();
		//	float move = (float)glfwGetTime();// / 1000.0 * 2 * 3.14159 * .75;
		//	m_screenObject->m_material->ApplyUniformFloat("distortTime", move);
		//	m_screenObject->Render();
		//	m_drawCalls++;
		//	// Unbind
		//	m_screenObject->Unbind();
		//}
		//// Render
		//if (m_doBloomEffect) {
		//	m_screenObject->m_material->m_shader = m_bloomShader;
		//	m_screenObject->Bind();
		//	m_screenObject->Render();
		//	m_drawCalls++;
		//	// Unbind
		//	m_screenObject->Unbind();
		//}
	}

	return 0;
}

int ApplicationDemo::GUIDraw()
{
	ImGui::Text((std::string("Draw Calls: ") + std::to_string(m_drawCalls)).c_str());
	ImGui::Text((std::string("RenderTargets: ") + std::to_string(m_renderTargetCalls)).c_str());
	ImGui::Checkbox("Bloom", &m_doBloomEffect);
	ImGui::Checkbox("Distort", &m_doDistortEffect);

	//ImGui::Button("Hello", { 60, 30 });
	return 0;
}
