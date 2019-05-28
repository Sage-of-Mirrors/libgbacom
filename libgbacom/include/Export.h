#pragma once

#ifdef LIBGBACOM_EXPORTS
#define LIBGBACOM_API __declspec(dllexport)
#else
#define LIBGBACOM_API __declspec(dllimport)
#endif