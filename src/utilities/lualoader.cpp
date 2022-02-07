#include <utilities/lualoader.hpp>

sol::table utilities::load_lua_table(sol::state &lua, const std::string &file_location, const std::string &table_name)
{

    sol::load_result script = lua.load_file(file_location);
    if (!script.valid())
    {
        sol::error err = script;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: " + errorMessage);
        return nullptr;
    }
    lua.script_file(file_location);

    return lua[table_name];
}
