#pragma once

namespace sol {
    class state;
}

namespace FMOD{
    namespace Studio{
        class System;
        class EventInstance;
    }
}


class SoundSystem
{
private:
    static FMOD::Studio::System *loaded_system;
    static FMOD::Studio::EventInstance *current_music;

public:
    static FMOD::Studio::System *Setup();
    static void play_sfx_oneshot();
    static void Update();
    static void stop_music_with_fadeout();

};