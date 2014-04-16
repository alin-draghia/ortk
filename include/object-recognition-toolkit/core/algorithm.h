#pragma once
#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <string>

#include <opencv2/opencv.hpp>

#include <object-recognition-toolkit/core/core.h>

namespace object_recognition_toolkit {
	namespace core {


		class ORT_API Algorithm
		{
		public:
			Algorithm(const std::string& name);
			virtual ~Algorithm( );
			const std::string& name( ) const;			
		private:
			std::string name_;
		};


	}
}

#endif // ALGORITHM_H_