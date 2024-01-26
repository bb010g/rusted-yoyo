#pragma once
#include "Common.hpp"
#if 0
#include <d3d11.h>
#endif

#pragma pack(push, 1)

struct CGlobals
{
    union
    {
        YYObjectBase* object;
        CInstance* instance = nullptr;
    } global_object;

    CDynamicArray<CScript*>* global_scripts_array = nullptr;
    void* global_window_device = nullptr;
    HWND global_window_handle = nullptr;
    HMODULE global_main_module = nullptr;
    ID3D11RenderTargetView* global_render_view = nullptr;
    ID3D11DeviceContext* global_device_context = nullptr;
    IDXGISwapChain* global_swap_chain = nullptr;
    bool global_was_preloaded = false;
};

struct CFunctions
{
    FNCodeExecute Code_Execute = nullptr;
    FNCodeFunctionGetTheFunction Code_Function_GET_the_function = nullptr;
};

struct CAPIVars
{
    CGlobals Globals;
    CFunctions Functions;
};

#pragma pack(pop)
