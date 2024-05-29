#pragma once

#include <initializer_list>

namespace Pika
{

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat3, Mat4, Bool
	};

	static uint32_t getShaderDataTypeSize(ShaderDataType vType) {
		switch (vType)
		{
		case Pika::ShaderDataType::Float:
			return 4;
		case Pika::ShaderDataType::Float2:
			return 4 * 2;
		case Pika::ShaderDataType::Float3:
			return 4 * 3;
		case Pika::ShaderDataType::Float4:
			return 4 * 4;
		case Pika::ShaderDataType::Int:
			return 4;
		case Pika::ShaderDataType::Int2:
			return 4 * 2;
		case Pika::ShaderDataType::Int3:
			return 4 * 3;
		case Pika::ShaderDataType::Int4:
			return 4 * 4;
		case Pika::ShaderDataType::Mat3:  //3 * float3
			return 4 * 3 * 3;
		case Pika::ShaderDataType::Mat4:  //4 * float4 
			return 4 * 4 * 4;
		case Pika::ShaderDataType::Bool:
			return 1;
		}
		PK_ASSERT(false, "ShaderDataType : unknown type error!");
		return 0;
	}

	struct BufferElement
	{
		std::string m_Name;       // variable name
		ShaderDataType m_Type;    // variable type
		uint32_t m_Size;          // variable byte size
		size_t m_Offset;          // element offset
		bool m_Normalized;        // normalized or not

		BufferElement() = default;
		BufferElement(ShaderDataType vType, const std::string& vName, bool vNormalized = false)
			: m_Name{ vName }, m_Type{ vType }, m_Size{ getShaderDataTypeSize(vType) }, m_Offset{ 0 }, m_Normalized{ vNormalized }
		{
		}

		[[nodiscard]] uint32_t getComponentCount() const
		{
			switch (m_Type)
			{
			case Pika::ShaderDataType::Float:
				return 1;
			case Pika::ShaderDataType::Float2:
				return 2;
			case Pika::ShaderDataType::Float3:
				return 3;
			case Pika::ShaderDataType::Float4:
				return 4;
			case Pika::ShaderDataType::Int:
				return 1;
			case Pika::ShaderDataType::Int2:
				return 2;
			case Pika::ShaderDataType::Int3:
				return 3;
			case Pika::ShaderDataType::Int4:
				return 4;
			case Pika::ShaderDataType::Mat3:
				return 3 * 3;
			case Pika::ShaderDataType::Mat4:
				return 4 * 4;
			case Pika::ShaderDataType::Bool:
				return 1;
			}
			PK_ASSERT(false, "ShaderDataType : unknown type error!");
			return 0;
		}

	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(std::initializer_list<BufferElement> vList)
			: m_Elements{ vList }
		{
			calculateStrideAndOffset();
		}
		inline uint32_t getStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& getElements() const { return m_Elements; }

		inline std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		inline std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		inline std::vector<BufferElement>::reverse_iterator rbegin() { return m_Elements.rbegin(); }
		inline std::vector<BufferElement>::reverse_iterator rend() { return m_Elements.rend(); }
		inline std::vector<BufferElement>::const_iterator begin() const { return m_Elements.cbegin(); }
		inline std::vector<BufferElement>::const_iterator end() const { return m_Elements.cend(); }
		inline std::vector<BufferElement>::const_reverse_iterator rbegin() const { return m_Elements.crbegin(); }
		inline std::vector<BufferElement>::const_reverse_iterator rend() const { return m_Elements.crend(); }
	private:
		void calculateStrideAndOffset() {
			uint32_t Stride = 0;
			uint32_t Count = 0;
			size_t Offset = 0;
			for (auto& element : m_Elements) {
				Count += element.getComponentCount();
				Stride += element.m_Size;
				element.m_Offset = Offset;
				Offset += element.m_Size;
			}
			m_Count = Count;
			m_Stride = Stride;
		}
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Count = 0;  // nums of parameters 
		uint32_t m_Stride = 0; // bytes size of the entire Vertex Buffer
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const BufferLayout& getLayout() const = 0;
		virtual void setLayout(const BufferLayout& vLayout) = 0;

		static VertexBuffer* Create(uint32_t vSize);
		static VertexBuffer* Create(float* vVertices, uint32_t vSize);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;
		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		static IndexBuffer* Create(uint32_t vCount);
		static IndexBuffer* Create(uint32_t* vIndices, uint32_t vCount);
	};

}

