#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_IndexCount;

	;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const void* data, unsigned int size, unsigned int count);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int getCount() const { return m_IndexCount; }

	void refill(const void* data, unsigned int size, unsigned int indexcount);
	;
};