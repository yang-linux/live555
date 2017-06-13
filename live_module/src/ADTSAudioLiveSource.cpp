
#include <ADTSAudioLiveSource.hh>
#include <InputFile.hh>
#include <GroupsockHelper.hh>

std::set< ADTSAudioLiveSource * > ADTSAudioLiveSource::m_SourceList;
Thread::Lock::CCriticalLock ADTSAudioLiveSource::m_SourceListLock;

static unsigned const samplingFrequencyTable[16] = 
{
  96000, 88200, 64000, 48000,
  44100, 32000, 24000, 22050,
  16000, 12000, 11025,  8000,
   7350,      0,     0,     0
};

ADTSAudioLiveSource*ADTSAudioLiveSource::createNew(UsageEnvironment& env) 
{
    return new ADTSAudioLiveSource(env, 1, 4, 1);
}

void ADTSAudioLiveSource::AddAudioData( const unsigned char * pData , unsigned int length )
{
    Thread::Lock::CLock _Lock( &m_SourceListLock );
    
    for(std::set< ADTSAudioLiveSource * >::iterator xun = m_SourceList.begin();xun != m_SourceList.end(); ++xun )
    {
        (*xun)->m_AudioQueue.Put( pData , length );
    }
}

ADTSAudioLiveSource::ADTSAudioLiveSource(UsageEnvironment& env, u_int8_t profile, u_int8_t samplingFrequencyIndex, u_int8_t channelConfiguration)
                                                    :FramedSource(env), m_AudioQueue( 1024 * 1024 * 12 ) 
{
    m_AudioQueue.Initialize();
    Thread::Lock::CLock _Lock( &m_SourceListLock );
    m_SourceList.insert( this );

    fSamplingFrequency  = samplingFrequencyTable[samplingFrequencyIndex];
    fNumChannels        = channelConfiguration == 0 ? 2 : channelConfiguration;
    fuSecsPerFrame      = (1024 * 1000000) / fSamplingFrequency;

    unsigned char audioSpecificConfig[2];
    u_int8_t const audioObjectType  = profile + 1;
    
    audioSpecificConfig[0]          = (audioObjectType<<3) | (samplingFrequencyIndex>>1);
    audioSpecificConfig[1]          = (samplingFrequencyIndex<<7) | (channelConfiguration<<3);
    sprintf(fConfigStr, "%02X%02x", audioSpecificConfig[0], audioSpecificConfig[1]);
    
}

ADTSAudioLiveSource::~ADTSAudioLiveSource() 
{
    Thread::Lock::CLock _Lock( &m_SourceListLock );
    m_SourceList.erase(this);
}

void ADTSAudioLiveSource::doGetNextFrame() 
{
    unsigned char headers[7];
    int numBytesRead = 0;
    
    numBytesRead = m_AudioQueue.Get( headers, sizeof headers);
    if (numBytesRead < 0) 
    {
        numBytesRead = 0;
        return;
    }

    //Boolean protection_absent = headers[1] & 0x01;
    u_int16_t frame_length    = ((headers[3] & 0x03) << 11) | (headers[4] << 3) | ((headers[5] & 0xE0) >> 5);
    unsigned numBytesToRead   = frame_length > sizeof headers ? frame_length - sizeof headers : 0;
    #if 0
    if (!protection_absent) 
    {
        unsigned char ucTmp[2];
        numBytesRead = m_AudioQueue.Get( ucTmp, sizeof ucTmp);
        
        if (numBytesRead < 0) 
        {
            numBytesRead = 0;
            return;
        }
        numBytesToRead = numBytesToRead > 2 ? numBytesToRead - 2 : 0;
    }
    #endif    

    if (numBytesToRead > fMaxSize) 
    {
        fNumTruncatedBytes = numBytesToRead - fMaxSize;
        numBytesToRead = fMaxSize;
    }
    
    numBytesRead = m_AudioQueue.Get( fTo, numBytesToRead);
    
    if (numBytesRead < 0) 
    {
        numBytesRead = 0;
        return;
    }
    
    fFrameSize          = numBytesRead;
    fNumTruncatedBytes += numBytesToRead - numBytesRead;

    if (fPresentationTime.tv_sec == 0 && fPresentationTime.tv_usec == 0) 
    {
        gettimeofday(&fPresentationTime, NULL);
    } 
    else 
    {
        unsigned uSeconds           = fPresentationTime.tv_usec + fuSecsPerFrame;
        fPresentationTime.tv_sec   += uSeconds / 1000000;
        fPresentationTime.tv_usec   = uSeconds % 1000000;
    }

    fDurationInMicroseconds = fuSecsPerFrame;

    nextTask() = envir().taskScheduler().scheduleDelayedTask(1000, (TaskFunc*)FramedSource::afterGetting, this);
}
