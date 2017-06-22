//#include "ShaderCollection.h"
//#include "DEBUG_NEW_LEAK_DETECT.h"
//
//#pragma region SHADER_BASE
//
//gl::SHADER_BASE::~SHADER_BASE() {
//	for (unsigned i = 0; i < uniformsCount; i++) delete uniforms[i]; delete[] uniforms;
//}
//
//const unsigned int	gl::SHADER_BASE::uniformsCount = 1;
//const char* const	gl::SHADER_BASE::uniforms[uniformsCount] = {
//	{"projectionViewWorldMatrix"}
//};
//const char* const			gl::SHADER_BASE::vertexSource =
//"\
//		#version 410 \n\n	\
//		layout(location=0) in vec4 position;	\
//		uniform mat4 projectionViewWorldMatrix;	\
//		void main()	\
//		{	\
//			gl_Position = projectionViewWorldMatrix * position;	\
//		}	\
//			\
//		";
//const char* const	gl::SHADER_BASE::fragmentSource =
//"\
//		#version 410 \n\n	\
//		out vec4 fragColour;	\
//		void main()	\
//		{	\
//			fragColour = vec4(0,0,0,1);	\
//		}	\
//			\
//		";
//#pragma endregion SHADER_BASE
//
//#pragma region SHADER_STD
//
//gl::SHADER_STD::~SHADER_STD() {
//	for (unsigned i = 0; i < uniformsCount; i++) delete uniforms[i]; delete[] uniforms;
//}
//
//const unsigned int	gl::SHADER_STD::uniformsCount = 1;
//const char* const	gl::SHADER_STD::uniforms[uniformsCount] = {
//	{ "projectionViewWorldMatrix" }
//};
//const char* const	gl::SHADER_STD::vertexSource =
//"\
//		#version 410 \n\n	\
//		layout(location=0) in vec4 position;	\
//		layout(location=1) in vec4 colour;	\
//		out vec4 vColour;	\
//		uniform mat4 projectionViewWorldMatrix;	\
//		void main()	\
//		{	\
//			vColour = colour;	\
//			gl_Position = projectionViewWorldMatrix * position;	\
//		}	\
//			\
//		";
//const char* const	gl::SHADER_STD::fragmentSource =
//"\
//		#version 410 \n\n	\
//		in vec4 vColour;	\
//		out vec4 fragColour;	\
//		void main()	\
//		{	\
//			fragColour = vColour;	\
//		}	\
//			\
//		";
//#pragma endregion SHADER_STD
//
//
//#pragma region SHADER_TEX
//
//gl::SHADER_TEX::~SHADER_TEX() {
//	for (unsigned i = 0; i < uniformsCount; i++) delete uniforms[i]; delete[] uniforms;
//}
//
//const unsigned int	gl::SHADER_TEX::uniformsCount = 1;
//const char* const	gl::SHADER_TEX::uniforms[uniformsCount] = {
//	{ "projectionViewWorldMatrix" }
//};
//const char* const	gl::SHADER_TEX::vertexSource =
//"\
//		#version 410 \n\n	\
//		layout(location=0) in vec4 position;	\
//		layout(location=1) in vec4 colour;	\
//		layout(location=2) in vec2 texCoord; \
//		out vec4 vColour;	\
//		out vec2 vTexCoord; \
//		uniform mat4 projectionViewWorldMatrix;	\
//		void main()	\
//		{	\
//			vColour = colour;	\
//			vTexCoord = texCoord; \
//			gl_Position = projectionViewWorldMatrix * position;	\
//		}	\
//			\
//		";
//const char* const	gl::SHADER_TEX::fragmentSource =
//"\
//#define GLM_SWIZZLE GLM_SWIZZLE_XYZW\
//		#version 410 \n\n	\
//		in vec4 vColour;	\
//		in vec2 vTexCoord; \
//		out vec4 fragColour;	\
//		uniform sampler2D diffuse; \
//		void main()	\
//		{	\
//			vec4 tex = texture(diffuse,vTexCoord);	\
//			if (tex.a > 0.1f) fragColour = tex;	\
//			else fragColour = vec4(vColour.rgb, 1);	\
//		}	\
//			\
//		";
//#pragma endregion SHADER_TEX
