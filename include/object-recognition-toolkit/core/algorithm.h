#pragma once
#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <string>
#include <Poco\RefCountedObject.h>
#include <Poco\DynamicFactory.h>

namespace object_recognition_toolkit {
	namespace core {

		// # Big issue here, I'f i dont define the inline c-tor and d-tor lnk 2019
		class Algorithm
			: public Poco::RefCountedObject
		{
		public:
			Algorithm( );
			virtual ~Algorithm( ) = 0;
			virtual const std::string& name( ) const = 0;
		};

#if(1)
		using AlgorithmFactory = Poco::DynamicFactory<Algorithm>;
#endif


	}
}
#endif // ALGORITHM_H_