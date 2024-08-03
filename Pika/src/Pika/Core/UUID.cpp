#include "pkpch.h"
#include "UUID.h"

namespace Pika {

	UUID::UUID()
	{
		GUID ID;
		HRESULT HardwareResult = CoCreateGuid(&ID);
		if (FAILED(HardwareResult)) {
			PK_CORE_ERROR("UUID : Fail to generate UUID !");
			return;
		}
		m_UUID = ID;
	}

	UUID::UUID(const GUID& vUUID)
		: m_UUID{ vUUID }
	{
	}

	UUID::UUID(const std::string& vUUID)
	{
		GUID ID;
		std::wstring UUIDWString(vUUID.begin(), vUUID.end());
		HRESULT HardwareResult = CLSIDFromString(UUIDWString.c_str(), &ID);
		if (FAILED(HardwareResult)) {
			PK_CORE_ERROR("UUID : Fail to generate UUID !");
			return;
		}
		m_UUID = ID;
	}

	void UUID::setUUID(const GUID& vUUID)
	{
		m_UUID = vUUID;
	}

	void UUID::setUUID(const std::string& vUUID)
	{
		GUID ID;
		std::wstring UUIDWString(vUUID.begin(), vUUID.end());
		HRESULT HardwareResult = CLSIDFromString(UUIDWString.c_str(), &ID);
		if (FAILED(HardwareResult)) {
			PK_CORE_ERROR("UUID : Fail to set UUID !");
			return;
		}
		m_UUID = ID;
	}

	std::string UUID::toString() const
	{
		wchar_t UUIDWChar[39];  // GUID长为38位"{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}",加上终止符共39位
		HRESULT HardwareResult = StringFromGUID2(m_UUID, UUIDWChar, 39);          // 将UUID转为wchar_t
		if (FAILED(HardwareResult)) {
			PK_CORE_ERROR("UUID : Fail to convert UUID to wstring !");
			return "";
		}
		char UUIDChar[39];
		int Length = WideCharToMultiByte(CP_UTF8, 0, UUIDWChar, -1, UUIDChar, sizeof(UUIDChar), nullptr, nullptr);
		if (!Length) {
			PK_CORE_ERROR("UUID : Fail to convert UUID to string !");
			return "";
		}
		return std::string(UUIDChar);
	}

}