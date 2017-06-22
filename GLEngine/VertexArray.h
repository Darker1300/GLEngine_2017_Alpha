//#pragma once
//#include <map>
//
//namespace gl {
//
//	class VertexArray
//	{
//	public:
//		VertexArray();
//		~VertexArray();
//
//		template<class vertexType>
//		static gl::VertexArray* Create(vertexType* _vertexFirst, unsigned int _vertexSize, unsigned int* _indexFirst, unsigned int _indexSize)
//		{
//			gl::VertexArray* arr = new VertexArray();
//			// Vertex
//			glGenBuffers(1, &arr->m_B_Vertex_ID);
//			glBindBuffer(GL_ARRAY_BUFFER, arr->m_B_Vertex_ID);
//			glBufferData(GL_ARRAY_BUFFER, _vertexSize * sizeof(vertexType), _vertexFirst, GL_STATIC_DRAW);
//
//			// Index
//			glGenBuffers(1, &arr->m_B_Index_ID);
//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arr->m_B_Index_ID);
//			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indexSize * sizeof(unsigned int), _indexFirst, GL_STATIC_DRAW);
//
//			// VA
//			glGenVertexArrays(1, &arr->m_A_Vertex_ID);
//			glBindVertexArray(arr->m_A_Vertex_ID);
//
//			// Bind VB & IB to VA
//			glBindBuffer(GL_ARRAY_BUFFER, arr->m_B_Vertex_ID);
//			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, arr->m_B_Index_ID);
//
//			// Unbind VA
//			glBindVertexArray(0);
//
//			return arr;
//		}
//
//		void bind(bool _status = true);
//
//		union {
//			struct {
//				unsigned int m_B_Vertex_ID;
//				unsigned int m_B_Index_ID;
//				unsigned int m_A_Vertex_ID;
//			};
//			unsigned int m_IDs[3];
//		};
//		std::map<int, const char*> m_attributePTRs;
//	};
//}
