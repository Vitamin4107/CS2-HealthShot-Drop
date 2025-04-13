#pragma once
#define PLUGIN_DESCRIPTION "Allows players to drop the healthshot from their inventory"
#define PLUGIN_VER_MAJOR 1
#define PLUGIN_VER_MINOR 0
#define PLUGIN_VER_BUILD 0
#define PLUGIN_VER_REVISION 0
#define STRINGIFY(x) #x
#define PLUGIN_VERSION_STR_HELER(major, minor, build, rev)  STRINGIFY(major) "." STRINGIFY(minor) "." STRINGIFY(build) "." STRINGIFY(rev)
#define PLUGIN_VERSION_STR PLUGIN_VERSION_STR_HELER(PLUGIN_VER_MAJOR, PLUGIN_VER_MINOR, PLUGIN_VER_BUILD, PLUGIN_VER_REVISION)
#define IDR_VERSIONINFO 1
