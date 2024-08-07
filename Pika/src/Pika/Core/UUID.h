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
		operator GUID() const { return m_UUID; }
	private:
		GUID m_UUID = {};  // Universe Unique Identifier
	};

}

namespace std {

	// �˴�ģ���ػ���Ϊ��unordered_map����UUIDΪ��
	template<>
	struct hash<Pika::UUID> {
		_NODISCARD size_t operator()(const Pika::UUID& vUUID) const noexcept {
			auto Value = static_cast<GUID>(vUUID);
			std::size_t Hash1 = std::hash<uint32_t>()(Value.Data1);
			std::size_t Hash2 = std::hash<uint16_t>()(Value.Data2);
			std::size_t Hash3 = std::hash<uint16_t>()(Value.Data3);
			std::size_t Hash4 = 0;
			for (int i = 0; i < 8; ++i) {
				Hash4 ^= std::hash<uint8_t>()(Value.Data4[i]) + 0x9e3779b9 + (Hash4 << 6) + (Hash4 >> 2); // 0x9e3779b9 ����һ������ĳ�������Ϊ�ƽ�ָ��ʵĵ�����0x9e3779b9 �� 2^32 ���� (sqrt(5) - 1) / 2 �Ľ������
			}
			return Hash1 ^ (Hash2 << 1) ^ (Hash3 << 2) ^ (Hash4 << 3);
		}
	};

}


