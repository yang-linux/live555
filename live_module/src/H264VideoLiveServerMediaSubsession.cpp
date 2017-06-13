
#include <H264VideoLiveServerMediaSubsession.hh>
#include <H264VideoRTPSink.hh>
#include <H264StreamLiveSource.hh>
#include <H264VideoLiveStreamFramer.hh>

H264VideoLiveServerMediaSubsession*H264VideoLiveServerMediaSubsession::createNew(UsageEnvironment& env, Boolean reuseFirstSource) 
{
    return new H264VideoLiveServerMediaSubsession(env, reuseFirstSource);
}

H264VideoLiveServerMediaSubsession::H264VideoLiveServerMediaSubsession(UsageEnvironment& env, Boolean reuseFirstSource)
                                                                                       :OnDemandServerMediaSubsession(env, reuseFirstSource),
                                                                                        fAuxSDPLine(NULL), fDoneFlag(0), fDummyRTPSink(NULL) 
{

}

H264VideoLiveServerMediaSubsession::~H264VideoLiveServerMediaSubsession() 
{
    delete[] fAuxSDPLine;
}

static void afterPlayingDummy(void* clientData) 
{
    H264VideoLiveServerMediaSubsession* subsess = (H264VideoLiveServerMediaSubsession*)clientData;
    subsess->afterPlayingDummy1();
}

void H264VideoLiveServerMediaSubsession::afterPlayingDummy1() 
{
    envir().taskScheduler().unscheduleDelayedTask(nextTask());
    setDoneFlag();
}

static void checkForAuxSDPLine(void* clientData) 
{
    H264VideoLiveServerMediaSubsession* subsess = (H264VideoLiveServerMediaSubsession*)clientData;
    subsess->checkForAuxSDPLine1();
}

void H264VideoLiveServerMediaSubsession::checkForAuxSDPLine1() 
{
    nextTask() = NULL;

    char const* dasl;
    if (fAuxSDPLine != NULL) 
    {
        setDoneFlag();
    }
    else if (fDummyRTPSink != NULL && (dasl = fDummyRTPSink->auxSDPLine()) != NULL) 
    {
        fAuxSDPLine = strDup(dasl);
        fDummyRTPSink = NULL;
        setDoneFlag();
    } 
    else if (!fDoneFlag) 
    {
        int uSecsToDelay = 100000; // 100 ms
        nextTask() = envir().taskScheduler().scheduleDelayedTask(uSecsToDelay, (TaskFunc*)checkForAuxSDPLine, this);
    }
}

char const* H264VideoLiveServerMediaSubsession::getAuxSDPLine(RTPSink* rtpSink, FramedSource* inputSource) 
{
    if (fAuxSDPLine != NULL) return fAuxSDPLine;

    if (fDummyRTPSink == NULL) 
    { 
        fDummyRTPSink = rtpSink;
        fDummyRTPSink->startPlaying(*inputSource, afterPlayingDummy, this);
        checkForAuxSDPLine(this);
    }

    envir().taskScheduler().doEventLoop(&fDoneFlag);

    return fAuxSDPLine;
}

FramedSource* H264VideoLiveServerMediaSubsession::createNewStreamSource(unsigned /*clientSessionId*/, unsigned& estBitrate) 
{
  estBitrate = 500;

  H264StreamLiveSource* fileSource = H264StreamLiveSource::createNew(envir());
  
  if (fileSource == NULL) return NULL;
  
  return H264VideoLiveStreamFramer::createNew(envir(), fileSource);
}

RTPSink* H264VideoLiveServerMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* /*inputSource*/) 
{
    return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
}
