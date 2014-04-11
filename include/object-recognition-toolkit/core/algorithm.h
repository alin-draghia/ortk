#pragma once
#ifndef _ORT_ALGORITHM_HEADER_INCLUDED_
#define _ORT_ALGORITHM_HEADER_INCLUDED_

#include <string>
#include <Poco\RefCountedObject.h>
#include <Poco\DynamicFactory.h>

namespace object_recognition_toolkit {
	namespace core {

		// # Big issue here, I'f i dont define the inline c-tor and d-tor lnk 2019
		class Algorithm
			: public Poco::RefCountedObject {
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
#endif //_ORT_ALGORITHM_HEADER_INCLUDED_