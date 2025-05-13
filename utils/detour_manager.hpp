/*
 * CS2 HealthShot Drop
 * Copyright (c) 2025 YourName
 *
 * This file is part of the CS2 HealthShot Drop project.
 * Licensed under the BSD-3-Clause License. See LICENSE file in the project root for full license information.
 */

#pragma once

#include "cs_healthshot_drop.hpp"
#include <vector>

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
	bool RemoveDetour(void*);
	bool RemoveAllDetours();

	static void* AllocateMemoryNearAddress(void*, size_t);

private:
	std::vector<InlineDetour> m_detours;
};

extern CInlineDetourManager g_detourManager;