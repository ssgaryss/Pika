#include "pkpch.h"
#include "OpenGLTextureFormat.h"

namespace Pika {

	namespace Utils {

		GLenum PikaTextureFormatToGLDataFormat(TextureFormat vFormat) {
			switch (vFormat)
			{
			case Pika::TextureFormat::R8:                return GL_RED;
			case Pika::TextureFormat::R16F:              return GL_RED;
			case Pika::TextureFormat::R32F:              return GL_RED;
			case Pika::TextureFormat::R8I:               return GL_RED_INTEGER;
			case Pika::TextureFormat::R16I:              return GL_RED_INTEGER;
			case Pika::TextureFormat::R32I:              return GL_RED_INTEGER;
			case Pika::TextureFormat::RG8:               return GL_RG;
			case Pika::TextureFormat::RG16F:             return GL_RG;
			case Pika::TextureFormat::RG32F:             return GL_RG;
			case Pika::TextureFormat::RGB8:              return GL_RGB;
			case Pika::TextureFormat::SRGB8:             return GL_RGB;
			case Pika::TextureFormat::RGB16F:            return GL_RGB;
			case Pika::TextureFormat::RGB32F:            return GL_RGB;
			case Pika::TextureFormat::RGBA8:             return GL_RGBA;
			case Pika::TextureFormat::RGBA16F:           return GL_RGBA;
			case Pika::TextureFormat::RGBA32F:           return GL_RGBA;
			case Pika::TextureFormat::DEPTH16:           return GL_DEPTH_COMPONENT;
			case Pika::TextureFormat::DEPTH32F:           return GL_DEPTH_COMPONENT;
			case Pika::TextureFormat::DEPTH24STENCIL8:   return GL_DEPTH_STENCIL;
			}
			PK_CORE_ERROR(R"(TextureFormat : Unknown Pika format!)");
			return 0;
		}

		GLenum PikaTextureFormatToGLInternalFormat(TextureFormat vFormat) {
			switch (vFormat)
			{
			case Pika::TextureFormat::R8:                return GL_R8;
			case Pika::TextureFormat::R16F:              return GL_R16F;
			case Pika::TextureFormat::R32F:              return GL_R32F;
			case Pika::TextureFormat::R8I:               return GL_R8I;
			case Pika::TextureFormat::R16I:              return GL_R16I;
			case Pika::TextureFormat::R32I:              return GL_R32I;
			case Pika::TextureFormat::RG8:               return GL_RG8;
			case Pika::TextureFormat::RG16F:             return GL_RG16F;
			case Pika::TextureFormat::RG32F:             return GL_RG32F;
			case Pika::TextureFormat::RGB8:              return GL_RGB8;
			case Pika::TextureFormat::SRGB8:             return GL_SRGB8;
			case Pika::TextureFormat::RGB16F:            return GL_RGB16F;
			case Pika::TextureFormat::RGB32F:            return GL_RGB32F;
			case Pika::TextureFormat::RGBA8:             return GL_RGBA8;
			case Pika::TextureFormat::RGBA16F:           return GL_RGBA16F;
			case Pika::TextureFormat::RGBA32F:           return GL_RGBA32F;
			case Pika::TextureFormat::DEPTH16:           return GL_DEPTH_COMPONENT16;
			case Pika::TextureFormat::DEPTH32F:          return GL_DEPTH_COMPONENT32F;
			case Pika::TextureFormat::DEPTH24STENCIL8:   return GL_DEPTH24_STENCIL8;
			}
			PK_CORE_ERROR(R"(TextureFormat : Unknown Pika format!)");
			return 0;
		}

		GLenum PikaTextureFormatToGLDataType(TextureFormat vFormat)
		{
			switch (vFormat)
			{
			case Pika::TextureFormat::R8:                return GL_UNSIGNED_BYTE;
			case Pika::TextureFormat::R16F:              return GL_HALF_FLOAT;
			case Pika::TextureFormat::R32F:              return GL_FLOAT;
			case Pika::TextureFormat::R8I:               return GL_BYTE;
			case Pika::TextureFormat::R16I:              return GL_SHORT;
			case Pika::TextureFormat::R32I:              return GL_INT;
			case Pika::TextureFormat::RG8:               return GL_UNSIGNED_BYTE;
			case Pika::TextureFormat::RG16F:             return GL_HALF_FLOAT;
			case Pika::TextureFormat::RG32F:             return GL_FLOAT;
			case Pika::TextureFormat::RGB8:              return GL_UNSIGNED_BYTE;
			case Pika::TextureFormat::SRGB8:             return GL_UNSIGNED_BYTE;
			case Pika::TextureFormat::RGB16F:            return GL_HALF_FLOAT;
			case Pika::TextureFormat::RGB32F:            return GL_FLOAT;
			case Pika::TextureFormat::RGBA8:             return GL_UNSIGNED_BYTE;
			case Pika::TextureFormat::RGBA16F:           return GL_HALF_FLOAT;
			case Pika::TextureFormat::RGBA32F:           return GL_FLOAT;
			case Pika::TextureFormat::DEPTH16:           return GL_UNSIGNED_SHORT;
			case Pika::TextureFormat::DEPTH32F:          return GL_FLOAT;
			case Pika::TextureFormat::DEPTH24STENCIL8:   return GL_UNSIGNED_INT_24_8;
			}
			PK_CORE_ERROR(R"(TextureFormat : Unknown Pika format!)");
			return 0;
		}

	}

}