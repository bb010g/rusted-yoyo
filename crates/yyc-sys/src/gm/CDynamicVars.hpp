#pragma once
#include "Common.hpp"
#include "YYObjectBase.hpp"

template<typename T>
struct CDynamicArray
{
	int m_arrayLength;
	T* Elements;
};

template<typename T>
struct CDynamicArrayRef
{
	int m_refCount;
	CDynamicArray<T>* Array;
	RValue* pOwner;

    #if 0
	static CDynamicArrayRef<T>* Assign(CDynamicArrayRef<T>* _Other)
	{
		if (_Other)
		{
			_Other->m_refCount++;
		}

		return _Other;
	}

	static CDynamicArrayRef<T>* Remove(CDynamicArrayRef<T>* _Other)
	{
		if (_Other)
		{
			_Other->m_refCount--;
		}

		return nullptr;
	}
    #endif
};
