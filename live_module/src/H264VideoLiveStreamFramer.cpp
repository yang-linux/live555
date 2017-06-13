
#include <H264VideoLiveStreamFramer.hh>

H264VideoLiveStreamFramer* H264VideoLiveStreamFramer::createNew(UsageEnvironment& env, FramedSource* inputSource, Boolean includeStartCodeInOutput) 
{
    return new H264VideoLiveStreamFramer(env, inputSource, True, includeStartCodeInOutput);
}

H264VideoLiveStreamFramer::H264VideoLiveStreamFramer(UsageEnvironment& env, FramedSource* inputSource, Boolean createParser, Boolean includeStartCodeInOutput)
                                                                :H264or5VideoStreamFramer(264, env, inputSource, createParser, includeStartCodeInOutput) 
{

}

H264VideoLiveStreamFramer::~H264VideoLiveStreamFramer() 
{

}

Boolean H264VideoLiveStreamFramer::isH264VideoStreamFramer() const 
{
    return True;
}
