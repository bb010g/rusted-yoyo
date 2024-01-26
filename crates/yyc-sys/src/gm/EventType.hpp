#pragma once
#include "Common.hpp"

enum class EventType : uint64_t
{
	CODE_EXECUTE = (1 << 0),					// The event represents a Code_Execute() call.
	YYERROR = (1 << 1),							// The event represents a YYError() call.
	ENDSCENE = (1 << 2),						// The event represents an LPDIRECT3DDEVICE9::EndScene() call.
	PRESENT = (1 << 3),							// The event represents an IDXGISwapChain::Present() call.
	RESIZEBUFFERS = (1 << 4),					// The event represents an IDXGISwapChain::ResizeBuffers() call.
	WNDPROC = (1 << 5),							// The event represents a window procedure call.
	DOCALLSCRIPT = (1 << 6),					// The event represents a DoCallScript() call.
};
