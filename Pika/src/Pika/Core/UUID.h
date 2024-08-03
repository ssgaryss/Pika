#pragma once
#include <string>

namespace Pika {

	class UUID
	{
	public:
		UUID();
		UUID(const GUID& vUUID);
		UUID(const std::string& vUUID);
		UUID(const UUID&) = default;

		void setUUID(const GUID& vUUID);
		void setUUID(const std::string& vUUID);
		std::string toString() const;
		operator std::string() const { return toString(); }
	private:
		GUID m_UUID = {};  // Universe Unique Identifier
	};

}


