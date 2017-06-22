//#include "VertexArray.h"
//
//#include <gl_core_4_4.h>
//#include <climits>
//
//gl::VertexArray::VertexArray()
//	: m_B_Vertex_ID(MAXUINT)
//	, m_B_Index_ID(MAXUINT)
//	, m_A_Vertex_ID(MAXUINT)
//{
//}
//
//gl::VertexArray::~VertexArray()
//{
//	if (m_B_Vertex_ID != MAXUINT)
//		glDeleteBuffers(1, &m_B_Vertex_ID);
//	if (m_B_Index_ID != MAXUINT)
//		glDeleteBuffers(1, &m_B_Index_ID);
//	if (m_A_Vertex_ID != MAXUINT)
//		glDeleteBuffers(1, &m_A_Vertex_ID);
//}
//
//void gl::VertexArray::bind(bool _status)
//{
//	glBindVertexArray(_status ? m_A_Vertex_ID : 0);
//}
//
