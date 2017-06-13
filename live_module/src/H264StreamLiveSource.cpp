
#include <H264StreamLiveSource.hh>
#include <InputFile.hh>
#include <GroupsockHelper.hh>

std::set< H264StreamLiveSource * > H264StreamLiveSource::m_SourceList;
Thread::Lock::CCriticalLock H264StreamLiveSource::m_SourceListLock;

H264StreamLiveSource*H264StreamLiveSource::createNew(UsageEnvironment& env, unsigned preferredFrameSize, unsigned playTimePerFrame) 
{
    H264StreamLiveSource* newSource = new H264StreamLiveSource(env, preferredFrameSize, playTimePerFrame);
    return newSource;
}

void H264StreamLiveSource::seekToByteAbsolute(u_int64_t byteNumber, u_int64_t numBytesToStream) 
{
    fNumBytesToStream = numBytesToStream;
    fLimitNumBytesToStream = fNumBytesToStream > 0;
}

void H264StreamLiveSource::seekToByteRelative(int64_t offset, u_int64_t numBytesToStream) 
{
    fNumBytesToStream = numBytesToStream;
    fLimitNumBytesToStream = fNumBytesToStream > 0;
}

void H264StreamLiveSource::seekToEnd() 
{
}

H264StreamLiveSource::H264StreamLiveSource(UsageEnvironment& env, unsigned preferredFrameSize, unsigned playTimePerFrame)
                                                    :FramedSource(env), fPreferredFrameSize(preferredFrameSize),
                                                     fPlayTimePerFrame(playTimePerFrame), fLastPlayTime(0), fHaveStartedReading(False), 
                                                     fLimitNumBytesToStream(False), fNumBytesToStream(0), m_VideoQueue( 1024 * 1024 * 12 ) 
{
    m_VideoQueue.Initialize();    
    Thread::Lock::CLock _Lock( &m_SourceListLock );    
    m_SourceList.insert( this );
}

H264StreamLiveSource::~H264StreamLiveSource() 
{
    Thread::Lock::CLock _Lock( &m_SourceListLock );    
    m_SourceList.erase(this);
}

void H264StreamLiveSource::doGetNextFrame() 
{
    doReadFromFile();
}

void H264StreamLiveSource::AddVideoData( const unsigned char * pData , unsigned int length )
{
    Thread::Lock::CLock _Lock( &m_SourceListLock );
    for( std::set< H264StreamLiveSource * >::iterator xun = m_SourceList.begin();
         xun != m_SourceList.end(); ++xun )
    {
        (*xun)->m_VideoQueue.Put( pData , length );
		//printf("queue size:%u\r\n",(*xun)->m_VideoQueue.GetDataLen());
    }
}


void H264StreamLiveSource::doReadFromFile() 
{
    if (fLimitNumBytesToStream && fNumBytesToStream < (u_int64_t)fMaxSize) 
    {
        fMaxSize = (unsigned)fNumBytesToStream;
    }
    
    if (fPreferredFrameSize > 0 && fPreferredFrameSize < fMaxSize) 
    {
        fMaxSize = fPreferredFrameSize;
    }
    
    fFrameSize = m_VideoQueue.Get( fTo, fMaxSize);
    
    if (fPlayTimePerFrame > 0 && fPreferredFrameSize > 0) 
    {
        if (fPresentationTime.tv_sec == 0 && fPresentationTime.tv_usec == 0) 
        {
            gettimeofday(&fPresentationTime, NULL);
        } 
        else 
        {
            //unsigned uSeconds	= fPresentationTime.tv_usec + fLastPlayTime;
            //fPresentationTime.tv_sec += uSeconds/1000000;
            //fPresentationTime.tv_usec = uSeconds%1000000;
            gettimeofday(&fPresentationTime, NULL);
        }

        fLastPlayTime = (fPlayTimePerFrame*fFrameSize)/fPreferredFrameSize;
        fDurationInMicroseconds = fLastPlayTime;
    } 
    else 
    {
        gettimeofday(&fPresentationTime, NULL);
    }

    nextTask() = envir().taskScheduler().scheduleDelayedTask(1000, (TaskFunc*)FramedSource::afterGetting, this);
}
