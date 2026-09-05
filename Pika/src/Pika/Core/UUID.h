#pragma once

#include <array>
#include <cstdint>
#include <string>

namespace Pika {

	// Cross-platform 128-bit universally unique identifier.
	//
	// Replaces the previous Windows-only GUID / CoCreateGuid implementation so
	// the identifier works identically on Windows and macOS. It is stored as 16
	// raw bytes and rendered in the standard RFC 4122 string form
	// "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx".
	class UUID
	{
	public:
		using Data = std::array<uint8_t, 16>;

		UUID();                                              // generate a random (version 4) UUID
		UUID(const Data& vBytes) : m_Data{ vBytes }, m_IsValid{ true } {}
		explicit UUID(const std::string& vString);           // parse from string form
		UUID(const UUID&) = default;
		UUID& operator=(const UUID&) = default;

		static UUID fromString(const std::string& vString);

		const Data& getData() const { return m_Data; }
		std::string toString() const;

		bool isValid() const { return m_IsValid; }

		operator std::string() const { return toString(); }

		bool operator==(const UUID& vOther) const { return m_Data == vOther.m_Data; }
		bool operator!=(const UUID& vOther) const { return !(*this == vOther); }
		bool operator<(const UUID& vOther) const { return m_Data < vOther.m_Data; }

	private:
		Data m_Data{};
		bool m_IsValid = false;
	};

}

namespace std {

	template<>
	struct hash<Pika::UUID>
	{
		size_t operator()(const Pika::UUID& vUUID) const noexcept;
	};

}
