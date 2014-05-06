#pragma once
#ifndef PUBLIC_API_H_
#define PUBLIC_API_H_

#if (defined WIN32 || defined _WIN32) 
#	if defined BUILDING_THE_DLL
#		define PUBLIC_API __declspec(dllexport)
#	else
#		define PUBLIC_API __declspec(dllimport)
#	endif
#endif

#endif // PUBLIC_API_H_
