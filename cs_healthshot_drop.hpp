/*
 * CS2 HealthShot Drop
 * Copyright (c) 2025 Vitamin
 *
 * This file is part of the CS2 HealthShot Drop project.
 * Licensed under the BSD-3-Clause License. See LICENSE file in the project root for full license information.
 */

#pragma once

#pragma warning(disable: 4100)

class ISource2Server;

class CPlayerSlot
{
public:
	CPlayerSlot(int slot): m_Data(slot)
	{
	}
	bool operator==(const CPlayerSlot &other) const
	{
		return other.m_Data == m_Data;
	}

private:
	int m_Data;
};

#include <cstdint>
#include <ISmmPlugin.h>
#include <version.h>

#define PLUGIN_AUTHOR "Vitamin"
#define SOURCE2SERVER_INTERFACE_VERSION "Source2Server001"

class CServerPlugin: public ISmmPlugin
{
public:
	bool Load(PluginId, ISmmAPI*, char*, size_t, bool) override;
	bool Unload(char*, size_t) override;
	const char* GetAuthor() override
	{
		return PLUGIN_AUTHOR;
	}
	const char* GetDate() override
	{
		return __DATE__;
	}
	const char* GetDescription() override
	{
		return PLUGIN_DESCRIPTION;
	}
	const char* GetLicense() override
	{
		return "BSD 3-Clause";
	}
	const char* GetLogTag() override
	{
		return "HealthShot";
	}
	const char* GetName() override
	{
		return PLUGIN_NAME;
	}
	const char* GetURL() override
	{
		return "https://gunserver.ru";
	}
	const char* GetVersion() override
	{
		return PLUGIN_VERSION_STR;
	}

public:
	bool ApplyInlineDetour(const void*, char*, size_t);
};

extern CServerPlugin g_ServerPlugin;