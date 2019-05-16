#include "Memories.h"

#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
#define MEMORY_CHUNK_DEFINE_DATA_TYPE_METHODS(type, Name)\
void MemoryChunk::Set##Name(const std::string& key, const type& value)\
{\
	auto it = std::find_if(m_##Name##Data.begin(), m_##Name##Data.end(), [&key](const auto& elem) { return elem.first == key; });\
	if(it == m_##Name##Data.end())\
	{\
		m_##Name##Data.emplace_back(key, value);\
	}\
	else\
	{\
		it->second = value;\
	}\
}\
const type& MemoryChunk::Get##Name(const std::string& key) const\
{\
	auto it = std::find_if(m_##Name##Data.begin(), m_##Name##Data.end(), [&key](const auto& elem) { return elem.first == key; });\
	if(it != m_##Name##Data.end())\
	{\
		return it->second;\
	}\
	static type invalidValue = type();\
	return invalidValue;\
}\

////////////////////////////////////////////////////////////////////////////////
MEMORY_CHUNK_DATA_TYPES_ITERATOR(MEMORY_CHUNK_DEFINE_DATA_TYPE_METHODS);

////////////////////////////////////////////////////////////////////////////////
IMemorable::~IMemorable()
{
	Memories::GetInstance().Forget(this);
}

////////////////////////////////////////////////////////////////////////////////
Memories::Memories()
	: m_TimeHand(0)
	, m_MaxStoredMoments(2000)
	, m_IsStorageFilled(false)
{
	m_Past.reserve(m_MaxStoredMoments);
}

////////////////////////////////////////////////////////////////////////////////
Memories& Memories::GetInstance()
{
	static Memories _instance;

	return _instance;
}

////////////////////////////////////////////////////////////////////////////////
void Memories::Add(IMemorable* subject)
{
	if (std::find(m_Memorables.begin(), m_Memorables.end(), subject) == m_Memorables.end())
	{
		m_Memorables.push_back(subject);
	}
}

////////////////////////////////////////////////////////////////////////////////
void Memories::SetCapacity(int maxStoredMoments)
{
	m_MaxStoredMoments = maxStoredMoments;

	m_Past.reserve(m_MaxStoredMoments);
}

////////////////////////////////////////////////////////////////////////////////
void Memories::Forget(IMemorable* subject)
{
	Wipe();
}

////////////////////////////////////////////////////////////////////////////////
void Memories::Wipe()
{
	m_Memorables.clear();
	m_Past.clear();
}

////////////////////////////////////////////////////////////////////////////////
void Memories::Impress()
{
	if ((int)m_Past.size() >= m_MaxStoredMoments)
	{
		return;
	}

	m_Past.emplace_back(MomentSnapshot());
	m_IsStorageFilled = false;

	auto& momentSnapshot = m_Past.back();
	momentSnapshot.reserve(m_Memorables.size());

	for (auto entry : m_Memorables)
	{
		momentSnapshot.emplace_back(Memory());
		auto& memory = momentSnapshot.back();

		memory.m_Subject = entry;
		entry->Impress(memory.m_Data);
	}
}

////////////////////////////////////////////////////////////////////////////////
void Memories::Comprehend()
{
	if (m_IsStorageFilled || m_Past.empty())
	{
		return;
	}

	if ((int)m_Past.size() >= m_MaxStoredMoments)
	{
		m_IsStorageFilled = true;
	}

	auto& momentSnapshot = m_Past.back();

	size_t momemntsCount = momentSnapshot.size();
	if (momemntsCount != m_Memorables.size())
	{
		return;
	}

	size_t i = 0;
	for (auto entry : m_Memorables)
	{
		auto& memory = momentSnapshot[i];

		memory.m_Subject = entry;
		entry->Comprehend(memory.m_Data);

		++i;
	}
}

////////////////////////////////////////////////////////////////////////////////
void Memories::Cease()
{
	m_TimeHand = std::max(0, (int)m_Past.size() - 1);

	Evoke(m_TimeHand);
}

////////////////////////////////////////////////////////////////////////////////
void Memories::Commence()
{
	if (m_TimeHand >= 0 && m_TimeHand < (int)m_Past.size())
	{
		auto& momentSnapshot = m_Past.at(m_TimeHand);

		for (auto& memory : momentSnapshot)
		{
			memory.m_Subject->Commence(memory.m_Data);
		}
	}

	ForgetFuture();
}

////////////////////////////////////////////////////////////////////////////////
void Memories::Begining()
{
	m_TimeHand = 0;

	Evoke(m_TimeHand);
}

////////////////////////////////////////////////////////////////////////////////
void Memories::End()
{
	m_TimeHand = m_Past.size() - 1;

	Evoke(m_TimeHand);
}

////////////////////////////////////////////////////////////////////////////////
void Memories::Back()
{
	m_TimeHand = std::max(0, m_TimeHand - 1);

	Evoke(m_TimeHand);
}

////////////////////////////////////////////////////////////////////////////////
void Memories::Forward()
{
	m_TimeHand = std::min((int)m_Past.size() - 1, m_TimeHand + 1);

	Evoke(m_TimeHand);
}

////////////////////////////////////////////////////////////////////////////////
void Memories::ForgetFuture()
{
	if (m_TimeHand >= 0 && m_TimeHand < (int)m_Past.size())
	{
		m_Past.erase(m_Past.begin() + m_TimeHand, m_Past.end());
	}
}

////////////////////////////////////////////////////////////////////////////////
void Memories::Evoke(int i)
{
	if (i < 0 || i >= (int)m_Past.size())
	{
		return;
	}

	auto& momentSnapshot = m_Past.at(i);

	for (auto& memory : momentSnapshot)
	{
		memory.m_Subject->Evoke(memory.m_Data);
	}
}

////////////////////////////////////////////////////////////////////////////////
Memories::Memory::Memory()
	: m_Subject(nullptr)
{}

////////////////////////////////////////////////////////////////////////////////
