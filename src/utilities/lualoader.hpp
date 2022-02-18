#pragma once
#include <string>

namespace sol
{
    class state;
}

class utilities
{

public:
    static sol::table load_lua_table(sol::state &lua, const std::string &file_location, const std::string &table_name);
};