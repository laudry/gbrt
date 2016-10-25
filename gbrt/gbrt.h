// Copyright (c) 2016 ¸Ê²¨¡£All rights reserved¡£

#if !defined (_GB_RT_H_GB_20161025_)
#define _GB_RT_H_GB_20161025_

#pragma once

#include "gbdef.h"

#if !defined (GBAPI)
	#define GBAPI __declspec(dllexport)
	#define GBVARIABLE __declspec(dllexport)
	#define GBCLASS __declspec(dllexport)
	#if defined (_WIN32)
		#if !defined (_UNICODE)
			#if !defined (_DEBUG)
				#pragma comment(lib, "gbrt32.lib")
				#pragma message("import \"gbrt32.lib\".")
			#else
				#pragma comment(lib, "gbrt32D.lib")
				#pragma message("import \"gbrt32D.lib\".")
			#endif
		#else
			#if !defined (_DEBUG)
				#pragma comment(lib, "gbrt32u.lib")
				#pragma message("import \"gbrt32u.lib\".")
			#else
				#pragma comment(lib, "gbrt32uD.lib")
				#pragma message("import \"gbrt32uD.lib\".")
			#endif
		#endif
	#elif defined (_WIN64)
		#if !defined (_UNICODE)
			#if !defined (_DEBUG)
				#pragma comment(lib, "gbrt64.lib")
				#pragma message("import \"gbrt64.lib\".")
			#else
				#pragma comment(lib, "gbrt64D.lib")
				#pragma message("import \"gbrt64D.lib\".")
			#endif
		#else
			#if !defined (_DEBUG)
				#pragma comment(lib, "gbrt64u.lib")
				#pragma message("import \"gbrt64u.lib\".")
			#else
				#pragma comment(lib, "gbrt64uD.lib")
				#pragma message("import \"gbrt64uD.lib\".")
			#endif
		#endif
	#else
		#error Platform is not supported.
	#endif
#endif

#endif
