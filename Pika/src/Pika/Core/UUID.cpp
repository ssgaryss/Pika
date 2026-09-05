#include "pkpch.h"
#include "UUID.h"

#include <random>

namespace Pika {

	namespace {

		uint8_t HexValue(char vChar)
		{
			if (vChar >= '0' && vChar <= '9') return static_cast<uint8_t>(vChar - '0');
			if (vChar >= 'a' && vChar <= 'f') return static_cast<uint8_t>(vChar - 'a' + 10);
			if (vChar >= 'A' && vChar <= 'F') return static_cast<uint8_t>(vChar - 'A' + 10);
			return 0xFF; // invalid
		}

	}

	UUID::UUID()
	{
		// Seed a fresh engine per UUID so back-to-back creations don't share a
		// deterministic stream (std::random_device is the entropy source).
		static std::random_device s_RandomDevice;
		std::seed_seq Seed{ s_RandomDevice(), s_RandomDevice(), s_RandomDevice() };
		std::mt19937_64 Engine(Seed);

		uint64_t Part1 = Engine();
		uint64_t Part2 = Engine();
		for (int i = 0; i < 8; ++i)
		{
			m_Data[i] = static_cast<uint8_t>((Part1 >> (8 * i)) & 0xFF);
			m_Data[i + 8] = static_cast<uint8_t>((Part2 >> (8 * i)) & 0xFF);
		}

		// Set the version (4) and RFC 4122 variant bits.
		m_Data[6] = static_cast<uint8_t>((m_Data[6] & 0x0F) | 0x40);
		m_Data[8] = static_cast<uint8_t>((m_Data[8] & 0x3F) | 0x80);

		m_IsValid = true;
	}

	UUID::UUID(const std::string& vString)
	{
		*this = fromString(vString);
	}

	UUID UUID::fromString(const std::string& vString)
	{
		UUID Result;
		Result.m_IsValid = false;
		Result.m_Data.fill(0);

		uint8_t ByteIndex = 0;
		uint8_t HighNibble = 0;
		bool HasHigh = false;

		for (char c : vString)
		{
			uint8_t Value = HexValue(c);
			if (Value == 0xFF)
			{
				if (c == '-')
					continue;
				return Result; // invalid character
			}

			if (!HasHigh)
			{
				HighNibble = Value;
				HasHigh = true;
			}
			else
			{
				Result.m_Data[ByteIndex++] = static_cast<uint8_t>((HighNibble << 4) | Value);
				HasHigh = false;
			}

			if (ByteIndex > 16)
				return Result; // too long
		}

		if (ByteIndex != 16 || HasHigh)
			return Result;

		Result.m_IsValid = true;
		return Result;
	}

	std::string UUID::toString() const
	{
		static const char* HexChars = "0123456789abcdef";
		std::string Result;
		Result.reserve(36);
		for (int i = 0; i < 16; ++i)
		{
			if (i == 4 || i == 6 || i == 8 || i == 10)
				Result += '-';
			uint8_t Byte = m_Data[i];
			Result += HexChars[(Byte >> 4) & 0x0F];
			Result += HexChars[Byte & 0x0F];
		}
		return Result;
	}

}

namespace std {

	size_t hash<Pika::UUID>::operator()(const Pika::UUID& vUUID) const noexcept
	{
		// FNV-1a over the 16 bytes.
		const size_t Offset = 14695981039346656037ull;
		const size_t Prime = 1099511628211ull;
		size_t Hash = Offset;
		for (uint8_t Byte : vUUID.getData())
		{
			Hash ^= Byte;
			Hash *= Prime;
		}
		return Hash;
	}

}
