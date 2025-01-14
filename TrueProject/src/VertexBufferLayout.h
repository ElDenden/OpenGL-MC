#pragma once
#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

//Info about attribute
struct VertexBufferElement
{
	unsigned int count;
	unsigned int type;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE : return 1;
		}
		ASSERT(false);

		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

	;
public:
	VertexBufferLayout(){}

	template<typename T>
	void push(unsigned int count)
	{
		ASSERT(false);
	}
	template<>
	void push<float>(unsigned int count)
	{
		m_Elements.push_back({count, GL_FLOAT,GL_FALSE});
		m_Stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_UNSIGNED_INT,GL_FALSE});
		m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT)* count;
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_UNSIGNED_BYTE,GL_TRUE});
		m_Stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement> getElements() const { return m_Elements; }

	inline const unsigned int getStride() const { return m_Stride; }

	;
};