
namespace sol {
    class state;
}

namespace FMOD{
    namespace Studio{
        class System;
    }
}


class SoundSystem
{
private:
    static FMOD::Studio::System *loaded_system;

public:
    static FMOD::Studio::System *Setup(sol::state& lua);
    static void Update();
};