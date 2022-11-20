#pragma once

// Shared library definitions
#if defined WINDOWS || defined __CYGWIN__
  #define CORNICHON_IMPORT_HELPER __declspec(dllimport)
  #define CORNICHON_EXPORT_HELPER __declspec(dllexport)
  #define CORNICHON_LOCAL_HELPER
#else
  #if __GNUC__ >= 4
    #define CORNICHON_IMPORT_HELPER __attribute__ ((visibility ("default")))
    #define CORNICHON_EXPORT_HELPER __attribute__ ((visibility ("default")))
    #define CORNICHON_LOCAL_HELPER  __attribute__ ((visibility ("hidden")))
  #else
    #define CORNICHON_IMPORT_HELPER
    #define CORNICHON_EXPORT_HELPER
    #define CORNICHON_LOCAL_HELPER
  #endif
#endif

#ifdef CORNICHON_EXPORTS // defined when using
  #define CORNICHON_API CORNICHON_EXPORT_HELPER
#else
  #define CORNICHON_API CORNICHON_IMPORT_HELPER
#endif

#define CORNICHON_LOCAL CORNICHON_LOCAL_HELPER
