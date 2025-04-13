#pragma once
#include "cs_healthshot_drop.hpp"

struct InlineDetour
{
    void* m_pTargetAddr;
    std::vector<uint8_t> m_vecOrigBytes;
    void* m_pTrampoline;
};

class CInlineDetourManager
{
public:
    bool InstallDetour(void*, void*);
    bool RemoveDetour(void* pTargetAddr);
    bool RemoveAllDetours();

    static void* AllocateMemoryNearAddress(void*, size_t);

private:
    std::vector<InlineDetour> m_detours;
};

extern CInlineDetourManager g_detourManager;