#pragma once
#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <string>
#include <vector>
#include <typeinfo>
#include <map>
#include <opencv2/opencv.hpp>

#include <object-recognition-toolkit/core/core.h>

namespace object_recognition_toolkit {
	namespace core {


		class ORT_API Algorithm
		{
		public:
			virtual ~Algorithm( ) = 0;
			virtual const std::string& name( ) const = 0;			
		};


	}
}

#endif // ALGORITHM_H_