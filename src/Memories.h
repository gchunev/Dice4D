#pragma once

#include <string>
#include <vector>

#include "Arbiter.h"

#include <glm/glm.hpp>

////////////////////////////////////////////////////////////////////////////////
class MemoryChunk;

////////////////////////////////////////////////////////////////////////////////
class IMemorable
{
public:
	virtual ~IMemorable();

	virtual void Impress(MemoryChunk& chunk) const = 0;

	virtual void Comprehend(MemoryChunk& chunk) const = 0;

	virtual void Evoke(const MemoryChunk& chunk) = 0;

	virtual void Commence(const MemoryChunk& chunk) = 0;
};

////////////////////////////////////////////////////////////////////////////////
#define MEMORY_CHUNK_DATA_TYPES_ITERATOR(MACRO)\
	MACRO(int, Int)\
	MACRO(std::int64_t, Int64)\
	MACRO(float, Float)\
	MACRO(glm::vec3, Vec3)\
	MACRO(glm::vec4, Vec4)\
	MACRO(glm::mat3, Mat3)\
	MACRO(glm::mat4, Mat4)\
	MACRO(ArbitersMap, Arbiters)\

////////////////////////////////////////////////////////////////////////////////
#define MEMORY_CHUNK_DECLARE_DATA_TYPE_METHODS(type, Name)\
	void Set##Name(const std::string& key, const type& value);\
	const type& Get##Name(const std::string& key) const;

////////////////////////////////////////////////////////////////////////////////
#define MEMORY_CHUNK_DECLARE_DATA_TYPE_STORAGE(type, Name)\
	typedef std::pair<std::string, type> Name##Data;\
	std::vector<Name##Data> m_##Name##Data;

////////////////////////////////////////////////////////////////////////////////
class MemoryChunk
{
public:
	MEMORY_CHUNK_DATA_TYPES_ITERATOR(MEMORY_CHUNK_DECLARE_DATA_TYPE_METHODS);

private:
	MEMORY_CHUNK_DATA_TYPES_ITERATOR(MEMORY_CHUNK_DECLARE_DATA_TYPE_STORAGE);
};

////////////////////////////////////////////////////////////////////////////////
class Memories
{
public:
	static Memories& GetInstance();

	void Add(IMemorable* subject);
	void Forget(IMemorable* subject);

	void SetCapacity(int maxStoredMoments);

	void Wipe();

	void Impress();
	void Comprehend();

	void Cease();
	void Commence();

	void Begining();
	void End();

	void Back();
	void Forward();

private:
	Memories();

	void ForgetFuture();
	void Evoke(int i);

	struct Memory
	{
		Memory();

		IMemorable* m_Subject;
		MemoryChunk m_Data;
	};
	typedef std::vector<Memory> MomentSnapshot;
	typedef std::vector<MomentSnapshot> TimeContinuum;
	TimeContinuum m_Past;

	typedef std::vector<IMemorable*> Memorables;
	Memorables m_Memorables;

	int m_TimeHand;

	int m_MaxStoredMoments;
	bool m_IsStorageFilled;
};

#define GetMemories() (Memories::GetInstance())

////////////////////////////////////////////////////////////////////////////////
