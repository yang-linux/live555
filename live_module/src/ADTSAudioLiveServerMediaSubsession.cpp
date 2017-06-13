
#include <ADTSAudioLiveServerMediaSubsession.hh>
#include <ADTSAudioLiveSource.hh>
#include <MPEG4GenericRTPSink.hh>

ADTSAudioLiveServerMediaSubsession*ADTSAudioLiveServerMediaSubsession::createNew(UsageEnvironment& env, Boolean reuseFirstSource) 
{
    return new ADTSAudioLiveServerMediaSubsession(env, reuseFirstSource);
}

ADTSAudioLiveServerMediaSubsession::ADTSAudioLiveServerMediaSubsession(UsageEnvironment& env, Boolean reuseFirstSource)
                                                                                        :OnDemandServerMediaSubsession(env, reuseFirstSource) 
{

}

ADTSAudioLiveServerMediaSubsession::~ADTSAudioLiveServerMediaSubsession() 
{

}

FramedSource* ADTSAudioLiveServerMediaSubsession::createNewStreamSource(unsigned, unsigned& estBitrate) 
{
    estBitrate = 96;
    return ADTSAudioLiveSource::createNew(envir());
}

RTPSink* ADTSAudioLiveServerMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource) 
{
  ADTSAudioLiveSource* adtsSource = (ADTSAudioLiveSource*)inputSource;
  return MPEG4GenericRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic, adtsSource->samplingFrequency(), "audio", "AAC-hbr", adtsSource->configStr(), adtsSource->numChannels());
}
