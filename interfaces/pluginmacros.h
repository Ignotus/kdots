#include <kdemacros.h>
#include <kpluginfactory.h>
#include <kexportplugin.h>

#define KDOTS_PLUGIN_EXPORT(c) \
  K_PLUGIN_FACTORY(KDotsPluginFactory, registerPlugin<c>(); ) \
  K_EXPORT_PLUGIN(KDotsPluginFactory("c"))

#ifndef KDOTS_EXPORT
# if defined(MAKE_KDOTS_LIB)
// We are building this library
# define KDOTS_EXPORT KDE_EXPORT
# else
// We are using this library
# define KDOTS_EXPORT KDE_IMPORT
# endif
#endif