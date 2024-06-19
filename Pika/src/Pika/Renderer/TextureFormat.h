#pragma once

namespace Pika {

	enum class TextureFormat {
		None = 0,
		R8,
		R16F,
		R32F,
		R8I,
		R16I,
		R32I,
		RG8,
		RG16F,
		RG32F,
		RGB8,
		SRGB8,
		RGB16F,
		RGB32F,
		RGBA8,
		RGBA16F,
		RGBA32F,
		// Depth/Stencil
		DEPTH16,
		DEPTH32F,
		DEPTH24STENCIL8,
	};

}
