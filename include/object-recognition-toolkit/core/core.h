#pragma once
#ifndef CORE_H_
#define CORE_H_

#if (defined WIN32 || defined _WIN32) 
#	if defined BUILDING_THE_DLL
#		define ORT_API __declspec(dllexport)
#	else
#		define ORT_API __declspec(dllimport)
#	endif
#endif

#endif // CORE_H_