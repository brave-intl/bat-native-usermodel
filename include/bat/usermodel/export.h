#ifndef BAT_USERMODEL_EXPORT_H_
#define BAT_USERMODEL_EXPORT_H_

#if defined(STANDALONE_BUILD)
#if defined(WIN32)

#if defined(BASE_IMPLEMENTATION)
#define USERMODEL_EXPORT __declspec(dllexport)
#else
#define USERMODEL_EXPORT __declspec(dllimport)
#endif  // defined(BASE_IMPLEMENTATION)

#else  // defined(WIN32)
#define USERMODEL_EXPORT __attribute__((visibility("default")))
#endif

#else  // defined(STANDALONE_BUILD)
#define USERMODEL_EXPORT
#endif

#endif  // BAT_USERMODEL_EXPORT_H_
