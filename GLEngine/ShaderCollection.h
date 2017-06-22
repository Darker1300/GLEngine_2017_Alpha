//#pragma once
//#include <glm\glm.hpp>
//
//namespace gl {
//
//	struct SHADER_BASE
//	{
//		virtual ~SHADER_BASE();
//
//		const static unsigned int uniformsCount;
//		const static char* const uniforms[];
//
//		const static char* const vertexSource;
//		const static char* const fragmentSource;
//
//		struct Vertex {
//			glm::vec4 position;
//		};
//	};
//
//	const struct SHADER_STD : SHADER_BASE
//	{
//		virtual ~SHADER_STD();
//
//		const static unsigned int uniformsCount;
//		const static char* const uniforms[];
//
//		const static char* const vertexSource;
//		const static char* const fragmentSource;
//
//		struct Vertex : SHADER_BASE::Vertex {
//			glm::vec4 colour;
//		};
//	};
//
//	const struct SHADER_TEX : SHADER_STD
//	{
//		virtual ~SHADER_TEX();
//
//		const static unsigned int uniformsCount;
//		const static char* const uniforms[];
//
//		const static char* const vertexSource;
//		const static char* const fragmentSource;
//
//		struct Vertex : SHADER_STD::Vertex {
//			glm::vec4 colour;
//			glm::vec2 texCoord;
//		};
//	};
//
//};
