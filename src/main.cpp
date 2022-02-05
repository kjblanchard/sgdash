#include <essential/world.hpp>

int main(int argc, char *argv[])
{
    auto world = World();
    world.Initialize();
    world.Run();
    return 0;
}
