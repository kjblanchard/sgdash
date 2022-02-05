#pragma once
#include <iostream>
#include <sol/sol.hpp>
#include <essential/logger.hpp>

class utilities
{

public:
    static sol::table load_lua_table(sol::state &lua, const std::string &file_location, const std::string& table_name);
};