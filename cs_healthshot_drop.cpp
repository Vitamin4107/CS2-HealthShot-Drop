/*
 * CS2 HealthShot Drop
 * Copyright (c) 2025 Vitamin
 *
 * This file is part of the CS2 HealthShot Drop project.
 * Licensed under the BSD-3-Clause License. See LICENSE file in the project root for full license information.
 */

#include "cs_healthshot_drop.hpp"

#include <stdio.h>

#include <metamod_oslink.h>
#include <sh_memory.h>
#include <xbyak/xbyak.h>

#include "utils/mem_utils.hpp"
#include "utils/detour_manager.hpp"

CServerPlugin g_ServerPlugin;
PLUGIN_EXPOSE(CServerPlugin, g_ServerPlugin);

CInlineDetourManager g_detourManager;

bool CServerPlugin::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();

	ISource2Server* pSource2Server = nullptr;
	GET_V_IFACE_ANY(GetServerFactory, pSource2Server, ISource2Server, SOURCE2SERVER_INTERFACE_VERSION);

	return ApplyInlineDetour(pSource2Server, error, maxlen);
}

bool CServerPlugin::Unload(char *error, size_t maxlen)
{
	if (!g_detourManager.RemoveAllDetours())
	{
		strcpy_s(error, maxlen, "Some detours failed to free memory");
		return false;
	}

	return true;
}

bool CServerPlugin::ApplyInlineDetour(const void* pLibrary, char* error, size_t maxlen)
{
	CMemoryUtils pMemoryUtils;
	if (!pMemoryUtils.SetupLibrary(pLibrary))
	{
		sprintf_s(error, maxlen, "Library not setup, please contact with %s.", PLUGIN_AUTHOR);
		return false;
	}

	const std::string sigStr = "48 8B 47 30 84 DB";
	void* pDetourAddr = pMemoryUtils.FindSignature(sigStr);
	if (!pDetourAddr)
	{
		sprintf_s(error, maxlen, "Signature not found, please contact with %s.", PLUGIN_AUTHOR);
		return false;
	}

	const size_t requiredSize = 0x20;
	void* pTrampoline = CInlineDetourManager::AllocateMemoryNearAddress(pDetourAddr, requiredSize);
	if (!pTrampoline)
	{
		sprintf_s(error, maxlen, "Not allocate memory for detour, please contact with %s.", PLUGIN_AUTHOR);
		return false;
	}

	Xbyak::CodeGenerator code(requiredSize, pTrampoline);

	auto& rax = code.rax;
	auto& ecx = code.ecx;
	auto& rdi = code.rdi;
	auto& rdx = code.rdx;
	auto& r8 = code.r8;
	auto& bl = code.bl;

	code.cmp(ecx, 0xFFFFFFFD);
	code.mov(rax, code.ptr[rdi + 0x30]);
	code.jnz("original");
	code.or_(rdx, 0x01);
	code.jmp("ret_bypass");

	code.L("original");
	code.test(bl, bl);

	code.L("ret_bypass");
	code.mov(r8, reinterpret_cast<uintptr_t>(pDetourAddr) + 0x06);
	code.jmp(r8);

	if (!g_detourManager.InstallDetour(pDetourAddr, pTrampoline))
	{
		sprintf_s(error, maxlen, "Detour hook not installed, please contact with %s", PLUGIN_AUTHOR);
		return false;
	}

	return true;
}