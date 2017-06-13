
#ifndef _H264_VIDEO_LIVE_SERVER_MEDIA_SUBSESSION_HH
#define _H264_VIDEO_LIVE_SERVER_MEDIA_SUBSESSION_HH

#include <FileServerMediaSubsession.hh>

class H264VideoLiveServerMediaSubsession: public OnDemandServerMediaSubsession {
public:
  static H264VideoLiveServerMediaSubsession*createNew(UsageEnvironment& env, Boolean reuseFirstSource);
  void checkForAuxSDPLine1();
  void afterPlayingDummy1();

protected:
  H264VideoLiveServerMediaSubsession(UsageEnvironment& env, Boolean reuseFirstSource);
  virtual ~H264VideoLiveServerMediaSubsession();
  void setDoneFlag() { fDoneFlag = ~0; }

protected:
  virtual char const* getAuxSDPLine(RTPSink* rtpSink, FramedSource* inputSource);
  virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate);
  virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource);

private:
  char* fAuxSDPLine;
  char fDoneFlag; // used when setting up "fAuxSDPLine"
  RTPSink* fDummyRTPSink; // ditto
};

#endif
