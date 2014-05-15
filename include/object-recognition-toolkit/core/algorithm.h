#pragma once
#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <string>

#include <boost/serialization/access.hpp>
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>

#include <opencv2/opencv.hpp>

#include "./public-api.h"
#include "./serialization.h"



namespace object_recognition_toolkit {
	namespace core {


		class PUBLIC_API Algorithm
		{
		public:
			Algorithm();
			Algorithm(const std::string& name);
			virtual ~Algorithm( );
			const std::string& name( ) const;			
		private:
			std::string name_;

		private:			
			friend class boost::serialization::access;
			void serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version);
			void serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version);
		};


	}
}

#pragma warning(pop)

#endif // ALGORITHM_H_