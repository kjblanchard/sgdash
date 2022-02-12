#include <fmod/fmod_studio.hpp>

namespace sol {
    class state;
}


class SoundSystem
{
private:
    static FMOD::Studio::System *loaded_system;

public:
    static FMOD::Studio::System *Setup(sol::state& lua);
    static void Update();
};