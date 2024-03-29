/* Copyright 2008-2011 The MathWorks, Inc. */

#ifndef SIMULINK_SPEC_H
#define SIMULINK_SPEC_H

#ifdef SUPPORTS_PRAGMA_ONCE
#pragma once
#endif

#ifdef BUILDING_LIBMWSIMULINK

#define SIMULINK_EXPORT_CLASS DLL_EXPORT_SYM
#define SIMULINK_EXPORT_FRIEND DLL_EXPORT_SYM
#define SIMULINK_EXPORT_FCN DLL_EXPORT_SYM
#define SIMULINK_EXPORT_TEMPLATE DLL_EXPORT_TEMPLATE

#define SIMULINK_EXPORT_EXTERN_C extern "C" DLL_EXPORT_SYM

#define SIMULINK_EXPORT_VAR extern
#define SIMULINK_EXPORT_VAR_DEF DLL_EXPORT_SYM

#define SIMULINK_EXPORT DLL_EXPORT_SYM

#elif defined(DLL_IMPORT_SYM)

#define SIMULINK_EXPORT_CLASS DLL_IMPORT_SYM
#define SIMULINK_EXPORT_FRIEND DLL_IMPORT_SYM
#define SIMULINK_EXPORT_FCN DLL_IMPORT_SYM
#define SIMULINK_EXPORT_TEMPLATE DLL_EXPORT_TEMPLATE

#ifdef __cplusplus
#define SIMULINK_EXPORT_EXTERN_C extern "C" DLL_IMPORT_SYM
#else
#define SIMULINK_EXPORT_EXTERN_C extern DLL_IMPORT_SYM
#endif

#define SIMULINK_EXPORT_VAR extern DLL_IMPORT_SYM

#define SIMULINK_EXPORT DLL_IMPORT_SYM

#else

#define SIMULINK_EXPORT_CLASS extern
#define SIMULINK_EXPORT_FCN extern
#define SIMULINK_EXPORT_TEMPLATE extern

#ifdef __cplusplus
#define SIMULINK_EXPORT_EXTERN_C extern "C"
#else
#define SIMULINK_EXPORT_EXTERN_C extern
#endif

#define SIMULINK_EXPORT extern

#endif

#endif

