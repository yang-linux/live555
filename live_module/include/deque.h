#ifndef DEQUE_H
#define DEQUE_H
#include <deque>
#include "criticallock.h"

namespace Types
{
    template< class _TYPE >
	class CDeque : public Thread::Lock::CCriticalLock
	{
	public:
        typedef std::deque< _TYPE > TDeque;
        TDeque m_DequeData;
    public:
		CDeque()
		{
        }
		virtual ~CDeque()
		{
        }
		bool IsEmpty()
		{
			Thread::Lock::CLock _Lock( this );
			return m_DequeData.empty();
        }
        size_t GetSize()
		{
			Thread::Lock::CLock _Lock(this);
            return (size_t)m_DequeData.size();
        }
		long Add(const _TYPE & pData)
		{
			long _res;
			try
			{
				Thread::Lock::CLock _Lock(this);
				_res = m_DequeData.size();
				m_DequeData.push_back( pData );			//插入列队
			}
			catch (...)
			{
			}
			return _res;
		}
		long AddToHead(const _TYPE & pData)
		{
			long _res;
			try
			{
				Thread::Lock::CLock _Lock(this);
				_res = m_DequeData.size();
				m_DequeData.push_front( pData );			//插入列队
			}
			catch (...)
			{
				return 0;
			}
			return _res;
        }
        bool GetData(_TYPE & pData)
		{
			Thread::Lock::CLock _Lock(this);
			if (!m_DequeData.empty())
			{
				pData = m_DequeData[0];
				m_DequeData.pop_front();
				return 1;
			}
			return 0;
        }
		void Clear()
		{
			Thread::Lock::CLock _Lock(this);
			m_DequeData.clear();
		}
    private:
		CDeque & operator=(const CDeque &);
		CDeque(const CDeque &);
	};
}
#endif
