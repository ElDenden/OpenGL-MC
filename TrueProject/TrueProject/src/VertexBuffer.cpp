#include "VertexBuffer.h"
#include "Renderer.h"
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID)); //Generates buffer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));//Binds the buffer to the specific OpenGL buffer
    //To unbind buffer glBindBuffer(GL_ARRAY_BUFFER, 0)
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); //Populates the specified buffer with data
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size, unsigned int indexcount): m_IndexCount(indexcount)
{
    GLCall(glGenBuffers(1, &m_RendererID)); //Generates buffer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));//Binds the buffer to the specific OpenGL buffer
    //To unbind buffer glBindBuffer(GL_ARRAY_BUFFER, 0)
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); //Populates the specified buffer with data
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::refill(const void* data, unsigned int size, unsigned int indexcount)
{
    m_IndexCount = indexcount; //change the indexcount
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));//Binds the buffer to the specific OpenGL buffer
    //To unbind buffer glBindBuffer(GL_ARRAY_BUFFER, 0)
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW)); //Set to dynamic bc I'm assuming more changes in the data
}
