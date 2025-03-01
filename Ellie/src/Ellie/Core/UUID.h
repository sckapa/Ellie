#pragma once


namespace Ellie {
	
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t id);
		UUID(const UUID&) = default;

		operator uint64_t() const { return m_UUID; }

	private:
		uint64_t m_UUID;
	};

}

namespace std {

	template<>
	struct hash<Ellie::UUID>
	{
		std::size_t operator()(const Ellie::UUID& uuid) const
		{
			return hash<uint64_t>()((uint64_t)uuid);
		}
	};

}