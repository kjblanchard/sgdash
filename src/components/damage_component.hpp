#pragma once

struct DamageComponent
{
    DamageComponent(bool friendly = false) : is_friendly{friendly} {}
    bool is_friendly;
};