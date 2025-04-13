#pragma once

#pragma warning(disable: 4100)

#include <vector>

#include "metamod_helper.hpp"
#include "metamod_oslink.h"

#include "sh_memory.h"

#include "SDK/defs.hpp"
#include "SDK/eiface.hpp"

#include "metadata/version.h"

#include "xbyak/xbyak.h"

#define PLUGIN_AUTHOR "Vitamin"

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
		return "CS2 HealthShot Drop";
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
	bool ValidateTargetOpcodes(const void*);
};

extern CServerPlugin g_ServerPlugin;