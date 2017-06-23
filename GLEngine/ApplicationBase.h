#pragma once

class ApplicationBase;
namespace GLE { extern ApplicationBase* APP; }

struct GLFWwindow;
#include <string>

class ApplicationBase
{
public:
	ApplicationBase(std::string _title, int _width, int _height);
	virtual ~ApplicationBase();

	void Run();

	int GetWindowWidth();
	int GetWindowHeight();
	GLFWwindow* GetWindow();
	void GetOGLVersion(int& _outMajor, int& _outMinor);
	///<summary>Frames Per a Second.</summary>
	int GetFPS();
	///<summary>Fixed Updates Per a Second.</summary>
	int GetFUPS();
	void SetBackgroundColor(float _r, float _g, float _b, float _a = 0.0f);

protected:
	// Run Calls
	///<summary>Occurs once before Initialise.</summary>
	virtual int PreInitialise();
	///<summary>Occurs once before Start. OpenGL is Initialised.</summary>
	virtual int Initialise();
	///<summary>Occurs once before FixedUpdate, before main loop.</summary>
	virtual int Start();
	///<summary>Occurs before Update, Restricted to a consistent framerate. eg 60 FPS.</summary>
	virtual int FixedUpdate(double _deltaTime);
	///<summary>Occurs before LateUpdate.</summary>
	virtual int Update(double _deltaTime);
	///<summary>Occurs before Draw.</summary>
	virtual int LateUpdate(double _deltaTime);
	///<summary>Occurs before GUIDraw.</summary>
	virtual int Draw();
	///<summary>Occurs at the end of main loop.</summary>
	virtual int GUIDraw();
	///<summary>Occurs once before Finalise, after main loop has ended.</summary>
	virtual int Shutdown();
	///<summary>Occurs once after Shutdown. OpenGL is Finalised.</summary>
	virtual int Finalise();

private:
	bool InitialiseOGLFunctions();
	bool CreateOGLWindow();
	void DestroyOGLWindow();

	std::string m_windowTitle;
	int m_windowWidth;
	int m_windowHeight;
	///<summary>Frames Per a Second.</summary>
	int m_FPS;
	///<summary>Fixed Updates Per a Second.</summary>
	int m_FUPS;
	double m_targetFPS;
	double m_fixedDTInterval;
	GLFWwindow* m_window;
};
