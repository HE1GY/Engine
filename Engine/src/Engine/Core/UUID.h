#pragma once

namespace Engine
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID& other) = default;

		operator uint64_t() const
		{
			return m_uuid;
		}
	private:
		uint64_t m_uuid;
	};

} // Engine

namespace std //for hashmap usage
{
	template<>
	struct hash<Engine::UUID>
	{
		std::size_t operator()(const Engine::UUID& uuid) const
		{
			return hash<uint64_t>()(uint64_t(uuid));
		}
	};

}
