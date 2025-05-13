/*
 * CS2 HealthShot Drop
 * Copyright (c) 2025 YourName
 *
 * This file is part of the CS2 HealthShot Drop project.
 * Licensed under the BSD-3-Clause License. See LICENSE file in the project root for full license information.
 */

#pragma once

#include <sstream>
#include <vector>

#define WILDCARD_MARKER 0x2A

namespace MemUtils
{
	struct Signature
	{
		std::vector<uint8_t> bytes;
	};

	static Signature ParseSignature(const std::string &sigStr)
	{
		using namespace std;

		Signature sig;
		istringstream iss(sigStr);
		string byteStr;

		while (iss >> byteStr)
		{
			if (byteStr == "?" || byteStr == "??" || byteStr == "*")
			{
				sig.bytes.push_back(WILDCARD_MARKER);
			}
			else
			{
				uint32_t byteValue;
				istringstream converter(byteStr);
				converter >> hex >> byteValue;
				sig.bytes.push_back(static_cast<uint8_t>(byteValue));
			}
		}

		return sig;
	}
}

class CMemoryUtils
{
public:
	CMemoryUtils(): m_pBase(nullptr), m_uSize(0)
	{
	}

	void* FindSignature(const std::string& sigStr) const
	{
		using namespace MemUtils;

		Signature sig = ParseSignature(sigStr);
		const size_t patternSize = sig.bytes.size();

		if (!m_pBase || sig.bytes.empty() || m_uSize < patternSize)
		{
			return nullptr;
		}

		const uint8_t* haystack = static_cast<const uint8_t*>(m_pBase);

		for (size_t i = 0; i <= m_uSize - patternSize; i++)
		{
			bool match = true;
			for (size_t j = 0; j < patternSize; j++)
			{
				if (sig.bytes[j] == WILDCARD_MARKER)
				{
					continue;
				}

				if (haystack[i + j] != sig.bytes[j])
				{
					match = false;
					break;
				}
			}

			if (!match)
			{
				continue;
			}

			return const_cast<void*>(static_cast<const void*>(haystack + i));
		}

		return nullptr;
	}
	bool SetupLibrary(const void* pLibrary)
	{
		if (!pLibrary)
		{
			return false;
		}

		MEMORY_BASIC_INFORMATION memInfo{};

		if (!VirtualQuery(pLibrary, &memInfo, sizeof(memInfo)))
		{
			return false;
		}

		uint8_t* pBaseAddr = static_cast<uint8_t*>(memInfo.AllocationBase);

		auto pDOS = reinterpret_cast<IMAGE_DOS_HEADER*>(pBaseAddr);
		if (pDOS->e_magic != IMAGE_DOS_SIGNATURE)
		{
			return false;
		}

		auto pNT = reinterpret_cast<IMAGE_NT_HEADERS*>(pBaseAddr + pDOS->e_lfanew);
		if (pNT->Signature != IMAGE_NT_SIGNATURE)
		{
			return false;
		}

		IMAGE_FILE_HEADER& pFile = pNT->FileHeader;
		IMAGE_OPTIONAL_HEADER& pOptional = pNT->OptionalHeader;

		if (pOptional.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC ||
			!(pFile.Characteristics & IMAGE_FILE_DLL))
		{
			return false;
		}

		m_pBase = static_cast<void*>(pBaseAddr);
		m_uSize = pOptional.SizeOfImage;

		return true;
	}

private:
	void*	m_pBase;
	size_t	m_uSize;
};