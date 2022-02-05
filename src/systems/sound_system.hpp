#include <fmod/fmod_studio.hpp>
#include <ecs/ECS.h>

class SoundSystem : public System
{
private:
    FMOD::Studio::System *loaded_system;

public:
    FMOD::Studio::System *Setup();
    void Update();
};