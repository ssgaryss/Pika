#pragma once
#include "Pika/Renderer/TextureFormat.h"
#include <glad/glad.h>

namespace Pika {

	namespace Utils {

		GLenum PikaTextureFormatToGLDataFormat(TextureFormat vFormat);
		GLenum PikaTextureFormatToGLInternalFormat(TextureFormat vFormat);

	}

}
