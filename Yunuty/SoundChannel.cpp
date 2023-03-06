#include "fmod.hpp"
#include "SoundChannel.h"

void YunutyEngine::SoundChannel::StopSound()
{
    channel->stop();
    //bool isPlaying;
    //channel->isPlaying(&isPlaying);
    //return channel->isPlaying();
}
YunutyEngine::SoundChannel::SoundChannel(FMOD::Channel* channel)
    :channel(channel)
{
}
