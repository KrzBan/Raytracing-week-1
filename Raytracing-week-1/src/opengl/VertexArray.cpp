#include "VertexArray.h"

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
	switch (type) {
	case ShaderDataType::Float:     return GL_FLOAT;
	case ShaderDataType::Float2:    return GL_FLOAT;	//represents Vectors of floats
	case ShaderDataType::Float3:    return GL_FLOAT;
	case ShaderDataType::Float4:    return GL_FLOAT;
	case ShaderDataType::Mat3:      return GL_FLOAT;
	case ShaderDataType::Mat4:      return GL_FLOAT;
	case ShaderDataType::Int:       return GL_INT;
	case ShaderDataType::Int2:      return GL_INT;
	case ShaderDataType::Int3:      return GL_INT;
	case ShaderDataType::Int4:      return GL_INT;
	case ShaderDataType::Bool:      return GL_BOOL;
	}
	assert(false && "Unknown ShaderDataType!\n");
	return 0;
}

VertexArray::VertexArray(){
	glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
	glBindVertexArray(m_RendererID);
	vertexBuffer->Bind();

	assert(vertexBuffer->GetLayout().GetElements().size() && "Vertex buffer has no layout!\n");

    uint32_t index = 0;
	const auto& layout = vertexBuffer->GetLayout();
    for (const auto& element : vertexBuffer->GetLayout()) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, element.GetElementCount(),
            ShaderDataTypeToOpenGLBaseType(element.Type),
            element.Normalized ? GL_TRUE : GL_FALSE,
            layout.GetStride(),
            reinterpret_cast<const void*>(element.Offset));
        ++index;
    }
	m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
	glBindVertexArray(m_RendererID);
	indexBuffer->Bind();

	m_IndexBuffer = indexBuffer;
}
