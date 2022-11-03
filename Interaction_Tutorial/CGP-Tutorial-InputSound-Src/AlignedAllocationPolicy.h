#pragma once

// for warning C4316
// 위 경고는 __declspec(align(byte))로 정렬되어야 하는 데이터를 member로 가지거나, 
// 할당할 때 생기는 경고임. For example, using XMMATRIX as a class member. 
template<size_t T>
class AlignedAllocationPolicy
{
public:
	static void* operator new(size_t size)
	{
		return _aligned_malloc(size, T);
	}

	static void operator delete(void* memory)
	{
		_aligned_free(memory);
	}
};
