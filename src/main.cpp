// Copyright (c) 2014-2015 Jae-jun Kang
// See the file LICENSE for details.

#include "x2boost/pre.hpp"

#if defined(_WIN32) && defined(X2BOOST_EXPORTS)

BOOL APIENTRY DllMain(HMODULE /*hModule*/,
                      DWORD ul_reason_for_call,
                      LPVOID /*lpReserved*/) {
  switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break;
  }
  return TRUE;
}

#endif

// EOF main.cpp
