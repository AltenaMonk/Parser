#ifndef SERVERBAR_MODULE_VERSION_H__INCLUDE_GUARD
#define SERVERBAR_MODULE_VERSION_H__INCLUDE_GUARD

#include <Library/String.h>

namespace Version
{

#ifdef UNITTEST

Library::String const applicationName="Parser";
Library::String const applicationVersion="UNITTEST";
Library::String const applicationDate="1986.01.04";

#else

Library::String const applicationName="Parser";
Library::String const applicationVersion="0.0.1";
Library::String const applicationDate="2014.10.06";

#endif//UNITTEST

} // namespace Version

#endif//SERVERBAR_MODULE_VERSION_H__INCLUDE_GUARD
