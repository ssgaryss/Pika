#include "pkpch.h"
#include "OpenGLTexture.h"
#include "OpenGLTextureFormat.h"
#include "OpenGLFramebuffer.h"
#include "OpenGLShader.h"
#include "OpenGLRendererAPI.h"
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Pika {

	namespace Utils {

		static void FlipImageVertically(unsigned char* vData, int vWidth, int vHeight, int vChannels) // 竖直方向翻转
		{
			for (int j = 0; j < vHeight / 2; ++j) {
				for (int i = 0; i < vWidth * vChannels; ++i) {
					std::swap(vData[j * vWidth * vChannels + i], vData[(vHeight - 1 - j) * vWidth * vChannels + i]);
				}
			}
		}

		static bool IsHDR(const std::filesystem::path& vPath) {
			return vPath.extension().string() == ".hdr";
		}

		static bool IsLDR(const std::filesystem::path& vPath) {
			return vPath.extension().string() == ".png"
				|| vPath.extension().string() == ".jpg"; // TODO :Others
		}
	}

	////////////////////////////////// Texture2D ///////////////////////////////////
	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& vTextureSpecification)
		: m_Width{ vTextureSpecification.m_Width }, m_Height{ vTextureSpecification.m_Height },
		m_RequiredMips{ vTextureSpecification.m_RequiredMips }
	{
		m_Format = vTextureSpecification.m_Format;
		m_InternalFormat = Utils::PikaTextureFormatToGLInternalFormat(vTextureSpecification.m_Format);
		m_DataFormat = Utils::PikaTextureFormatToGLDataFormat(vTextureSpecification.m_Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		// TODO : Assign Texture Wrap
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		if (m_RequiredMips) {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateTextureMipmap(m_RendererID);
		}
		else {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& vPath, bool vRequiredMips)
		: m_Path{ vPath }, m_RequiredMips{ vRequiredMips }
	{
		PK_PROFILE_FUNCTION();

		try
		{
			loadTexture(vPath);
		}
		catch (const std::runtime_error& e)
		{
			PK_CORE_WARN(e.what());
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		PK_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::setData(void* vData, uint32_t vSize)
	{
		PK_PROFILE_FUNCTION();
		if (vData == nullptr || vSize == 0) {
			PK_CORE_WARN("OpenGLTexture2D : Set an empty texture data!");
			return;
		}
		uint32_t BPP = 0; //bytes per pixel
		if (m_DataFormat == GL_RED) BPP = 1;
		else if (m_DataFormat == GL_RGB) BPP = 3;
		else if (m_DataFormat == GL_RGBA) BPP = 4;
		PK_ASSERT(vSize == m_Width * m_Height * BPP, "Data must be entire texture!");
		auto DataType = Utils::PikaTextureFormatToGLDataType(m_Format);
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, DataType, vData);
	}

	void OpenGLTexture2D::bind(uint32_t vSlot) const
	{
		PK_PROFILE_FUNCTION();
		glBindTextureUnit(vSlot, m_RendererID);
	}

	void OpenGLTexture2D::unbind(uint32_t vSlot) const
	{
		PK_PROFILE_FUNCTION();
		glBindTextureUnit(vSlot, 0);
	}

	void OpenGLTexture2D::loadTexture(const std::filesystem::path& vPath)
	{
		PK_PROFILE_FUNCTION();

		int Width, Height, Channels;
		stbi_set_flip_vertically_on_load(true);

		if (!Utils::IsLDR(vPath))
			throw std::runtime_error(std::format(R"(OpenGLTexture2D : Fail to load the texture with unsupported format at "{0}".)", vPath.string()));

		stbi_uc* Data = stbi_load(vPath.string().c_str(), &Width, &Height, &Channels, 0); // 0 means desired channels = Channels
		if (!Data)
			throw std::runtime_error(std::format(R"(OpenGLTexture2D : Fail to load the texture at "{0}".)", vPath.string()));
		m_Width = Width;
		m_Height = Height;

		if (Channels == 4)
			m_Format = TextureFormat::RGBA8;
		else if (Channels == 3)
			m_Format = TextureFormat::RGB8;
		else if (Channels == 1)
			m_Format = TextureFormat::R8;

		m_InternalFormat = Utils::PikaTextureFormatToGLInternalFormat(m_Format);
		m_DataFormat = Utils::PikaTextureFormatToGLDataFormat(m_Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (m_RequiredMips)
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (m_RequiredMips)
			glGenerateMipmap(GL_TEXTURE_2D);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, Data);
		stbi_image_free(Data);
		PK_CORE_INFO("OpenGLTexture2D : Success to load a texture at {0}.", vPath.string());
		m_IsLoaded = true;
	}
	////////////////////////////////// Texture2D ///////////////////////////////////


	/////////////////////////////////// Cubemap ////////////////////////////////////
	OpenGLCubemap::OpenGLCubemap(const TextureSpecification& vTextureSpecification)
		: m_Width{ vTextureSpecification.m_Width }, m_Height{ vTextureSpecification.m_Height },
		m_RequiredMips{ vTextureSpecification.m_RequiredMips }
	{
		m_Format = vTextureSpecification.m_Format;
		m_InternalFormat = Utils::PikaTextureFormatToGLInternalFormat(vTextureSpecification.m_Format);
		m_DataFormat = Utils::PikaTextureFormatToGLDataFormat(vTextureSpecification.m_Format);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		for (uint32_t i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat,
				m_Width, m_Height, 0, m_DataFormat, Utils::PikaTextureFormatToGLDataType(m_Format), nullptr);
		}
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // 通常对于 Cubemap 纹理，使用 GL_CLAMP_TO_EDGE 会更合适，避免在面之间出现边缘接缝
		if (m_RequiredMips) {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateTextureMipmap(m_RendererID);
		}
		else {
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	OpenGLCubemap::OpenGLCubemap(const std::filesystem::path& vPath, bool vRequiredMips)
		: m_Path{ vPath }, m_RequiredMips{ vRequiredMips }
	{
		PK_PROFILE_FUNCTION();

		try
		{
			loadCubemap(vPath);
		}
		catch (const std::runtime_error& e)
		{
			PK_CORE_WARN(e.what());
		}
	}

	OpenGLCubemap::~OpenGLCubemap()
	{
		PK_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLCubemap::setData(void* vData, uint32_t vSize)
	{
		// TODO !
	}

	void OpenGLCubemap::bind(uint32_t vSlot) const
	{
		PK_PROFILE_FUNCTION();
		glBindTextureUnit(vSlot, m_RendererID);
	}

	void OpenGLCubemap::unbind(uint32_t vSlot) const
	{
		PK_PROFILE_FUNCTION();
		glBindTextureUnit(vSlot, 0);
	}

	void OpenGLCubemap::loadCubemap(const std::filesystem::path& vPath)
	{
		PK_PROFILE_FUNCTION();
		if (Utils::IsHDR(vPath))
			loadHDR(vPath);
		else if (Utils::IsLDR(vPath))
			loadLDR(vPath);
		else
			throw std::runtime_error(std::format(R"(OpenGLCubemap : Fail to load the cubemap with unsupported format at "{0}".)", vPath.string()));
	}

	void OpenGLCubemap::loadHDR(const std::filesystem::path& vPath)
	{
		PK_PROFILE_FUNCTION();

		int Width, Height, Channels;

		if (!Utils::IsHDR(vPath))
			throw std::runtime_error(std::format(R"(OpenGLCubemap : Fail to load the texture with unsupported format at "{0}".)", vPath.string()));

		float* Data = stbi_loadf(vPath.string().c_str(), &Width, &Height, &Channels, 0); // 0 means desired channels = Channels
		if (!Data)
			throw std::runtime_error(std::format(R"(OpenGLCubemap : Fail to load cubemap at "{0}".)", vPath.string()));
		m_Width = Width;
		m_Height = Height;

		if (Channels == 4)
			m_Format = TextureFormat::RGBA32F;
		else if (Channels == 3)
			m_Format = TextureFormat::RGB32F;
		else if (Channels == 1)
			m_Format = TextureFormat::R32F;

		m_InternalFormat = Utils::PikaTextureFormatToGLInternalFormat(m_Format);
		m_DataFormat = Utils::PikaTextureFormatToGLDataFormat(m_Format);

		TextureSpecification TS{ m_Width, m_Height, m_Format, false };
		Ref<OpenGLTexture2D> PanoramaHDRTexture = CreateRef<OpenGLTexture2D>(TS);
		PanoramaHDRTexture->setData(Data, sizeof(Data));

		stbi_image_free(Data);
		PK_CORE_INFO("OpenGLCubemap : Success to load a cubemap at {0}.", vPath.string());
		m_IsLoaded = true;

		convertPanoramaToCubemap(PanoramaHDRTexture);
	}

	void OpenGLCubemap::loadLDR(const std::filesystem::path& vPath)
	{
		PK_PROFILE_FUNCTION();

		int Width, Height, Channels;
		stbi_uc* Data = stbi_load(vPath.string().c_str(), &Width, &Height, &Channels, 0); // 0 means desired channels = Channels
		if (!Data)
			throw std::runtime_error(std::format(R"(OpenGLCubemap : Fail to load cubemap at "{0}".)", vPath.string()));
		m_Width = Width;
		m_Height = Height;


		if (Channels == 4)
			m_Format = TextureFormat::RGBA8;
		else if (Channels == 3)
			m_Format = TextureFormat::RGB8;
		else if (Channels == 1)
			m_Format = TextureFormat::R8;

		m_InternalFormat = Utils::PikaTextureFormatToGLInternalFormat(m_Format);
		m_DataFormat = Utils::PikaTextureFormatToGLDataFormat(m_Format);

		uint32_t FaceWidth = m_Width / 4;
		uint32_t FaceHeight = m_Height / 3; // 暂时支持十字型Cubemap贴图

		stbi_uc* FaceData[6];
		FaceData[0] = Data + (1 * FaceHeight * m_Width + 3 * FaceWidth) * Channels; // +X
		FaceData[1] = Data + (1 * FaceHeight * m_Width + 1 * FaceWidth) * Channels; // -X
		FaceData[2] = Data + (0 * FaceHeight * m_Width + 1 * FaceWidth) * Channels; // +Y
		FaceData[3] = Data + (2 * FaceHeight * m_Width + 1 * FaceWidth) * Channels; // -Y
		FaceData[4] = Data + (1 * FaceHeight * m_Width + 2 * FaceWidth) * Channels; // +Z
		FaceData[5] = Data + (1 * FaceHeight * m_Width + 0 * FaceWidth) * Channels; // -Z

		// 定义每个面的偏移
		struct FaceOffset {
			int m_X, m_Y;
		};
		static FaceOffset Offsets[6] = {
			{2, 1},  // +X (右)
			{0, 1},  // -X (左)
			{1, 2},  // +Z (上)
			{1, 0},  // -Z (下)
			{1, 1},  // +Y (前)
			{3, 1},  // -Y (后)
		};

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		for (uint32_t i = 0; i < 6; ++i) {
			int XOffset = Offsets[i].m_X * FaceWidth;
			int YOffset = Offsets[i].m_Y * FaceHeight;
			std::vector<stbi_uc> FaceData(FaceWidth * FaceHeight * Channels);
			for (uint32_t Row = 0; Row < FaceHeight; ++Row) {
				//stbi_uc* SrcPtr = Data + ((YOffset + (FaceHeight - 1 - Row)) * m_Width + XOffset) * Channels;
				stbi_uc* SrcPtr = Data + ((YOffset + Row) * m_Width + XOffset) * Channels;
				stbi_uc* DstPtr = FaceData.data() + (Row * FaceWidth * Channels);
				std::memcpy(DstPtr, SrcPtr, FaceWidth * Channels);
			}
			Utils::FlipImageVertically(FaceData.data(), FaceWidth, FaceHeight, Channels);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat,
				FaceWidth, FaceHeight, 0, m_DataFormat, GL_UNSIGNED_BYTE, FaceData.data());
		}
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // 通常对于 Cubemap 纹理，使用 GL_CLAMP_TO_EDGE 会更合适，避免在面之间出现边缘接缝
		if (m_RequiredMips)
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (m_RequiredMips)
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		stbi_image_free(Data);
		PK_CORE_INFO("OpenGLCubemap : Success to load a cubemap at {0}.", vPath.string());
		m_IsLoaded = true;
	}

	void OpenGLCubemap::convertPanoramaToCubemap(const Ref<OpenGLTexture2D>& vPanorama)
	{
		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
		uint32_t FaceWidth = m_Width / 4;
		uint32_t FaceHeight = m_Height / 2;
		PK_ASSERT(FaceHeight == FaceWidth, "FaceWidth != FaceHeight");
		for (uint32_t i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat,
				FaceWidth, FaceHeight, 0, m_DataFormat, GL_FLOAT, nullptr);
		}
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // 通常对于 Cubemap 纹理，使用 GL_CLAMP_TO_EDGE 会更合适，避免在面之间出现边缘接缝
		if (m_RequiredMips)
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (m_RequiredMips)
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		// 渲染Panorama到Cubemap
		GLuint Buffer;
		Ref<OpenGLShader> Shader = CreateRef<OpenGLShader>(s_PanoramaToCubemapShaderPath);
		glCreateFramebuffers(1, &Buffer);
		glBindFramebuffer(GL_FRAMEBUFFER, Buffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_RendererID, 0);
		GLenum Buffers = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, &Buffers);
		glViewport(0, 0, FaceWidth, FaceHeight);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Shader->setInt("u_Panorama", 0);
		Shader->bind();
		vPanorama->bind(0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		vPanorama->unbind(0);
		Shader->unbind();
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
			PK_CORE_INFO("OpenGLCubemap : Success to convert panorama to cubemap!");
		else
			PK_CORE_INFO("OpenGLCubemap : Fail to convert panorama to cubemap!");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &Buffer);
	}
	/////////////////////////////////// Cubemap ////////////////////////////////////

}