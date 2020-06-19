#ifndef _UTILITIES_INSTANCE_COUNTER_H_
#define _UTILITIES_INSTANCE_COUNTER_H_

#include "Types.h"

template<typename I>
class InstanceCounter
{
	I* m_object;
	uint32 m_refCount;

public:
	InstanceCounter(I* obj)
		: m_object(obj)
		, m_refCount(1)
	{
	}

	~InstanceCounter()
	{
		m_object = nullptr;
		m_refCount = 0;
	}

    InstanceCounter(const InstanceCounter& other) = delete;
    InstanceCounter& operator=(const InstanceCounter& other) = delete;

	uint32 getRefCount() const { return m_refCount; }
	I* getObjectPtr() const { return m_object; }
    const I& getObjectRef() const { return *m_object; }

    bool hasRef() const
    {
        return m_refCount > 0;
    }

	void AddRef()
	{
		++m_refCount;
	}

	void RemoveRef()
	{
		--m_refCount;
	}
};


#endif 

