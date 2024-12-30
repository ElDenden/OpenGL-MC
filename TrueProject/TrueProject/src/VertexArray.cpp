#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID););
    GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
	vb.Bind();
    const auto& elements = layout.getElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset)); //Links vertex buffer to vertex array with the specified attribute
        //glVertexAttribPointer --> specifies how the data of our buffer is laid out
        //Now we need to enable the attribute pointer (can also do it before the attribute pointer)
        GLCall(glEnableVertexAttribArray(i));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
