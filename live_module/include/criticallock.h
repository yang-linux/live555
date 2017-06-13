
#ifndef CRITICALLOCK_H
#define CRITICALLOCK_H

#include <time.h>
#include <string>

namespace Thread
{
	namespace Lock
    {
        class CLockBase
		{
		public:
			virtual void Lock() = 0;
			virtual void UnLock() = 0;
		};

		class CLock
		{
		public:
			CLock(CLockBase * _Lock)
				: m_pLock(_Lock)
			{
				m_pLock->Lock();
			}
			~CLock()
			{
				m_pLock->UnLock();
			}
		private:
			CLockBase * m_pLock;
		};

		class CCriticalLock : public CLockBase
		{
		private:
			CCriticalLock( const CCriticalLock & );
			CCriticalLock & operator=( const CCriticalLock & );
		public:
			CCriticalLock(void)
			{
                pthread_spin_init(&spinLock, 0);
			}
			virtual ~CCriticalLock(void)
			{
                pthread_spin_destroy(&spinLock);
            }
			virtual void Lock()
			{
                pthread_spin_lock(&spinLock);
            }
			virtual void UnLock()
			{
                pthread_spin_unlock(&spinLock);
			}
		private:
            pthread_spinlock_t spinLock;
		};
	}
}
#endif
