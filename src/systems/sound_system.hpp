#include <fmod/fmod_studio.hpp>

class SoundSystem
{
private:
    static FMOD::Studio::System *loaded_system;

public:
    static FMOD::Studio::System *Setup();
    static void Update();
};