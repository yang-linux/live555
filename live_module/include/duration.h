#ifndef DURATION_H
#define DURATION_H
#include <sys/time.h>

class CDuration
{
protected:
    timeval m_Start;
    timeval m_Stop;
public:
    CDuration(void)
    {
        gettimeofday( &m_Start , NULL);
        gettimeofday( &m_Stop , NULL);
    }
    void Start(void)
    {
        gettimeofday( &m_Start , NULL);
    }
    void Stop(void)
    {
        gettimeofday( &m_Stop , NULL);
    }
    int GetDuration(void) const
    {
        return (1000000*(m_Stop.tv_sec-m_Start.tv_sec) + m_Stop.tv_usec-m_Start.tv_usec)/1000.0;
    }
};

#endif
