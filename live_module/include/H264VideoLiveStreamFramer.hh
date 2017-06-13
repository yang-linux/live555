
#ifndef _H264_VIDEO_LIVE_STREAM_FRAMER_HH
#define _H264_VIDEO_LIVE_STREAM_FRAMER_HH

#include <H264or5VideoStreamFramer.hh>

class H264VideoLiveStreamFramer: public H264or5VideoStreamFramer {
public:
  static H264VideoLiveStreamFramer* createNew(UsageEnvironment& env, FramedSource* inputSource, Boolean includeStartCodeInOutput = False);

protected:
  H264VideoLiveStreamFramer(UsageEnvironment& env, FramedSource* inputSource, Boolean createParser, Boolean includeStartCodeInOutput);
  virtual ~H264VideoLiveStreamFramer();
  virtual Boolean isH264VideoStreamFramer() const;
};

#endif
