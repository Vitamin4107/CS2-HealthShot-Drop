/*
 * CS2 HealthShot Drop
 * Copyright (c) 2025 Vitamin
 *
 * This file is part of the CS2 HealthShot Drop project.
 * Licensed under the BSD-3-Clause License. See LICENSE file in the project root for full license information.
 */

#include "cs_healthshot_drop.hpp"

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

	META_CONPRINTF("Version: %s\n", PLUGIN_VERSION_STR);

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
	using namespace std;
	using namespace Xbyak;

	CMemoryUtils pMemoryUtils;
	if (!pMemoryUtils.SetupLibrary(pLibrary))
	{
		sprintf_s(error, maxlen, "Library not setup, please contact with %s.", PLUGIN_AUTHOR);
		return false;
	}

	const string sigStr = "48 89 74 24 20 57 41 54 41 57 48 83 EC 40 45 0F B6 F8";
	void* pSigAddr = pMemoryUtils.FindSignature(sigStr);
	if (!pSigAddr)
	{
		sprintf_s(error, maxlen, "Signature not found, please contact with %s.", PLUGIN_AUTHOR);
		return false;
	}

	void* pDetourAddr = reinterpret_cast<uint8_t*>(pSigAddr) + 0x1A8;
	if (!ValidateTargetOpcodes(pDetourAddr))
	{
		sprintf_s(error, maxlen, "Expected instructions do not match, please contact with %s", PLUGIN_AUTHOR);
		return false;
	}

	const size_t requiredSize = 0x20;
	void* pTrampoline = CInlineDetourManager::AllocateMemoryNearAddress(pDetourAddr, requiredSize);
	if (!pTrampoline)
	{
		sprintf_s(error, maxlen, "Not allocate memory for detour, please contact with %s.", PLUGIN_AUTHOR);
		return false;
	}

	CodeGenerator code(requiredSize, pTrampoline);

	auto& ecx = code.ecx;
	auto& rax = code.rax;
	auto& rsi = code.rsi;
	auto& rcx = code.rcx;
	auto& r11 = code.r11;
	auto& bl = code.bl;

	code.cmp(ecx, 0xFFFFFFFD);
	code.mov(rcx, code.ptr[rsi + 0x30]);
	code.jne("original");
	code.or_(rax, 1);
	code.jmp("ret_label");

	code.L("original");
	code.test(bl, bl);

	code.L("ret_label");
	code.mov(r11, reinterpret_cast<uintptr_t>(pDetourAddr) + 6);
	code.jmp(r11);

	if (!g_detourManager.InstallDetour(pDetourAddr, pTrampoline))
	{
		sprintf_s(error, maxlen, "Detour hook not installed, please contact with %s", PLUGIN_AUTHOR);
		return false;
	}

	return true;
}

bool CServerPlugin::ValidateTargetOpcodes(const void* pTargetAddr)
{
	const uint8_t expectedOpCodes[] = {0x48, 0x8B, 0x4E, 0x30, 0x84, 0xDB};
	const size_t opCodesSize = sizeof(expectedOpCodes);

	uint8_t foundOpCodes[opCodesSize] = {};
	memcpy(foundOpCodes, pTargetAddr, opCodesSize);

	if (memcmp(foundOpCodes, expectedOpCodes, opCodesSize) != 0)
	{
		return false;
	}

	return true;
}