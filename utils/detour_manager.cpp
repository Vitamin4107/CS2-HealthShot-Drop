/*
 * CS2 HealthShot Drop
 * Copyright (c) 2025 Vitamin
 *
 * This file is part of the CS2 HealthShot Drop project.
 * Licensed under the BSD-3-Clause License. See LICENSE file in the project root for full license information.
 */

#include "detour_manager.hpp"

#include <Windows.h>
#include <xbyak/xbyak.h>
#include <sh_memory.h>

void* CInlineDetourManager::AllocateMemoryNearAddress(void* pTargetAddress, size_t size)
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	uintptr_t pageSize   = sysInfo.dwPageSize;
	uintptr_t minAddress = reinterpret_cast<uintptr_t>(sysInfo.lpMinimumApplicationAddress);
	uintptr_t maxAddress = reinterpret_cast<uintptr_t>(sysInfo.lpMaximumApplicationAddress);
	uintptr_t target     = reinterpret_cast<uintptr_t>(pTargetAddress);
	uintptr_t twoGB      = 0x80000000;

	uintptr_t searchStart = (target >= twoGB) ? (target - twoGB) : minAddress;
	uintptr_t searchEnd   = ((target + twoGB) < maxAddress) ? (target + twoGB) : maxAddress;

	for (uintptr_t current = searchStart; current < searchEnd; current += pageSize)
	{
		MEMORY_BASIC_INFORMATION memInfo = {};
		if (!VirtualQuery(reinterpret_cast<void*>(current), &memInfo, sizeof(memInfo)))
		{
			continue;
		}

		if (memInfo.State == MEM_FREE)
		{
			void* allocated = VirtualAlloc(	reinterpret_cast<void*>(current), size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			if (allocated)
			{
				uintptr_t allocAddr = reinterpret_cast<uintptr_t>(allocated);
				if (allocAddr >= searchStart && allocAddr < searchEnd)
				{
					return allocated;
				}

				VirtualFree(allocated, 0, MEM_RELEASE);
			}
		}
	}

	return nullptr;
}

bool CInlineDetourManager::InstallDetour(void* pTargetAddr, void* pTrampoline)
{
	using namespace std;
	using namespace Xbyak;

	const size_t patchSize = 5;

	vector<uint8_t> origBytes(patchSize);
	memcpy(origBytes.data(), pTargetAddr, patchSize);

	CodeGenerator patchJMP(patchSize, pTargetAddr);

	if (!patchJMP.setProtectModeRW())
	{
		VirtualFree(pTrampoline, 0, MEM_RELEASE);
		return false;
	}

	patchJMP.jmp(pTrampoline);

	if (!patchJMP.setProtectModeRE())
	{
		VirtualFree(pTrampoline, 0, MEM_RELEASE);
		return false;
	}

	InlineDetour entry;
	entry.m_pTargetAddr = pTargetAddr;
	entry.m_vecOrigBytes = move(origBytes);
	entry.m_pTrampoline = pTrampoline;
	m_detours.push_back(entry);

	return true;
}

bool CInlineDetourManager::RemoveDetour(void* pTargetAddr)
{
	using namespace SourceHook;

	for (auto it = m_detours.begin(); it != m_detours.end(); ++it)
	{
		if (it->m_pTargetAddr == pTargetAddr)
		{
			if (!MakePageWritable(pTargetAddr))
			{
				return false;
			}

			memcpy(pTargetAddr, it->m_vecOrigBytes.data(), it->m_vecOrigBytes.size());
			if (!SetMemAccess(pTargetAddr, it->m_vecOrigBytes.size(), SH_MEM_READ | SH_MEM_EXEC))
			{
				return false;
			}

			VirtualFree(it->m_pTrampoline, 0, MEM_RELEASE);
			m_detours.erase(it);
			return true;
		}
	}

	return false;
}

bool CInlineDetourManager::RemoveAllDetours()
{
	bool success = true;
	while (!m_detours.empty())
	{
		if (RemoveDetour(m_detours.front().m_pTargetAddr))
		{
			continue;
		}

		success = false;
	}

	return success;
}