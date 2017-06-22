//#pragma once
//#include <map>
//
//class IShaderBase
//{
//public:
//	friend class ShaderProgram;
//
//	IShaderBase(unsigned int _shaderType, const char* const _source);
//	virtual ~IShaderBase();
//
//	unsigned int m_shaderID;
//
//protected:
//	void AttachLinkShaders(ShaderProgram* _container);
//	void DeleteComponent();
//};
//
//struct VertexShader : public IShaderBase {
//	VertexShader(const char* const _source);
//	~VertexShader();
//};
//
//struct FragmentShader : public IShaderBase {
//	FragmentShader(const char* const _source);
//	~FragmentShader();
//};
//
//class ShaderProgram
//{
//public:
//	//ShaderProgram(IShaderBase** _shaders, unsigned int _shadersCount, const char** const _uniforms = nullptr, const unsigned int _uniformsCount = 0);
//	ShaderProgram(VertexShader* _vertex, FragmentShader* _fragment, const char* const _uniforms[] = nullptr, const unsigned int _uniformsCount = 0);
//
//	~ShaderProgram();
//
//	void UseShader(bool _status = true);
//
//	unsigned int m_programID;
//	IShaderBase** m_shaders;
//	unsigned int m_shadersCount;
//	std::map<int, const char*> m_uniforms;
//
//protected:
//	void SetupShaderProgram();
//
//	void AssignUniforms(const char* const _uniforms[], const unsigned int _uniformsCount);
//	void CreateProgramID();
//	void AttachLinkShaders();
//	void TestCompilation();
//	void DeleteComponents();
//};
