#pragma once

#include <xhash>

namespace fs
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t id);
		UUID(const UUID& other ) = default;

		operator uint64_t() const
		{
			return m_UUID;
		}
	private:
		uint64_t m_UUID; //一般来说UUID是128位,但是随机数最多64位,拼接随机数不太好,而且64位UUID够用了
	};

}

namespace std
{
	template<>
	struct hash<fs::UUID> //std::hash 
	{
		std::size_t operator()(const fs::UUID& id) const
		{
			return hash<uint64_t>()((uint64_t)id); //一个哈希映射,接受键 返回10位hash值
			//c14 中 相同的键返回相同的hash值
		}
	};
}