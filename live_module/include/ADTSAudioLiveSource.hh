
#ifndef _ADTS_AUDIO_LIVE_SOURCE_HH
#define _ADTS_AUDIO_LIVE_SOURCE_HH

#include <FramedSource.hh>
#include <videoqueue.h>
#include <set>
#include <criticallock.h>

class ADTSAudioLiveSource: public FramedSource {
public:
    static ADTSAudioLiveSource* createNew(UsageEnvironment& env);
    unsigned samplingFrequency() const { return fSamplingFrequency; }
    unsigned numChannels() const { return fNumChannels; }
    char const* configStr() const { return fConfigStr; }
    static void AddAudioData( const unsigned char * pData , unsigned int length );
private:
    ADTSAudioLiveSource(UsageEnvironment& env, u_int8_t profile, u_int8_t samplingFrequencyIndex, u_int8_t channelConfiguration);
    virtual ~ADTSAudioLiveSource();
private:
  virtual void doGetNextFrame();
private:
  unsigned fSamplingFrequency;
  unsigned fNumChannels;
  unsigned fuSecsPerFrame;
  char fConfigStr[5];
  VideoQueue m_AudioQueue;
public:
    static Thread::Lock::CCriticalLock m_SourceListLock;
    static std::set< ADTSAudioLiveSource * > m_SourceList;
};

#endif
