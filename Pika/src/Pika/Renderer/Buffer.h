#pragma once

namespace Pika
{

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;
		virtual void bind() = 0;
		virtual void unbind() = 0;

		static std::unique_ptr<VertexBuffer> create(uint32_t vSize);
		static std::unique_ptr<VertexBuffer> create(float* vVertices, uint32_t vSize);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;
		virtual void bind() = 0;
		virtual void unbind() = 0;

		static std::unique_ptr<IndexBuffer> create(uint32_t vSize);
		static std::unique_ptr<IndexBuffer> create(uint32_t* vIndices, uint32_t vSize);
	};

}

