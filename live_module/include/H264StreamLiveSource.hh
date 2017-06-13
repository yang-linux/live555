
#ifndef _H264_STREAM_LIVE_SOURCE_HH
#define _H264_STREAM_LIVE_SOURCE_HH

#include <FramedSource.hh>
#include <videoqueue.h>
#include <set>
#include <criticallock.h>

class H264StreamLiveSource: public FramedSource {
public:
  static H264StreamLiveSource* createNew(UsageEnvironment& env, unsigned preferredFrameSize = 0, unsigned playTimePerFrame = 0);
  void seekToByteAbsolute(u_int64_t byteNumber, u_int64_t numBytesToStream = 0);
  void seekToByteRelative(int64_t offset, u_int64_t numBytesToStream = 0);
  void seekToEnd();
  static void AddVideoData( const unsigned char * pData , unsigned int length );

protected:
  H264StreamLiveSource(UsageEnvironment& env, unsigned preferredFrameSize, unsigned playTimePerFrame);
  virtual ~H264StreamLiveSource();
  void doReadFromFile();

private:
  virtual void doGetNextFrame();

private:
  unsigned fPreferredFrameSize;
  unsigned fPlayTimePerFrame;
  Boolean fFidIsSeekable;
  unsigned fLastPlayTime;
  Boolean fHaveStartedReading;
  Boolean fLimitNumBytesToStream;
  u_int64_t fNumBytesToStream; // used iff "fLimitNumBytesToStream" is True
  VideoQueue m_VideoQueue;

public:
      static Thread::Lock::CCriticalLock m_SourceListLock;
      static std::set< H264StreamLiveSource * > m_SourceList;

};

#endif
