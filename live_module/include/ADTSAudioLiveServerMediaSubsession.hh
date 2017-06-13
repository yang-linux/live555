
#ifndef _ADTS_AUDIO_LIVE_SERVER_MEDIA_SUBSESSION_HH
#define _ADTS_AUDIO_LIVE_SERVER_MEDIA_SUBSESSION_HH

#include <OnDemandServerMediaSubsession.hh>

class ADTSAudioLiveServerMediaSubsession: public OnDemandServerMediaSubsession{
public:
    static ADTSAudioLiveServerMediaSubsession*createNew(UsageEnvironment& env, Boolean reuseFirstSource);
protected:
    ADTSAudioLiveServerMediaSubsession(UsageEnvironment& env, Boolean reuseFirstSource);
    virtual ~ADTSAudioLiveServerMediaSubsession();
protected:
  virtual FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate);
  virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource);
};

#endif
