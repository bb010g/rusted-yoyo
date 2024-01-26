#pragma once
#include "Common.hpp"

#if 0
#include "gui/gui.hpp"
#endif

using IO_UpdateM_t = void (*)();

#if 0
inline void hook_IO_UpdateM()
{
    if (!big::g_gui->is_open())
    {
        big::g_hooking->get_original<hook_IO_UpdateM>()();
    }
}
#endif
