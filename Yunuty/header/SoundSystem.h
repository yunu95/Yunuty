#include <iostream>
#include <unordered_map>
#include <unordered_set>

#pragma once
#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

using namespace std;

namespace FMOD
{
    class System;
    class Sound;
    class Channel;
}

namespace YunutyEngine
{
    class SoundChannel;
    class YUNUTY_API SoundSystem
    {
    public:
        static SoundChannel PlaySoundfile(string soundPath);
        // Music�� ���� ä�ο��� �����Ѵ�? �̰� ������� �����, ������ ������ �����ϱ� ���ؼ��� AudioSource ��ü���� ä�� ��Ʈ���� �����ϰ� �ؾ��Ѵ�.
        static SoundChannel PlayMusic(string soundPath);
        static SoundChannel StopMusic(double fadeLength=0);
    private:
        static SoundSystem* soundInstance;
        static SoundSystem* GetInstance();
        SoundSystem();
        ~SoundSystem();
        SoundChannel mPlaySound(string soundPath);
        // Music�� ���� ä�ο��� �����Ѵ�? �̰� ������� �����, ������ ������ �����ϱ� ���ؼ��� AudioSource ��ü���� ä�� ��Ʈ���� �����ϰ� �ؾ��Ѵ�.
        SoundChannel mPlayMusic(string soundPath);
        SoundChannel mStopMusic(double fadeLength);
        // fire and forget channels
        FMOD::Channel* channels[64] = { nullptr };
        FMOD::Channel* bgmChannel = nullptr;
        FMOD::System* fmodSystem = nullptr;
        void* extradriverdata = 0;
        unordered_map<string, FMOD::Sound*> sounds;
    };
};
