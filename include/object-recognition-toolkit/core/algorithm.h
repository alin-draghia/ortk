#pragma once
#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <string>


#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>


#include <opencv2/opencv.hpp>

#include <object-recognition-toolkit/core/core.h>


namespace object_recognition_toolkit {
	namespace core {


		class ORT_API Algorithm
		{
		public:
			Algorithm();
			Algorithm(const std::string& name);
			virtual ~Algorithm( ) = 0;
			const std::string& name( ) const;			
		private:
			std::string name_;

		private:
			friend class boost::serialization::access;		
			template<class Archive>
			void save(Archive & ar, const unsigned int version) const;
			template<class Archive>
			void load(Archive & ar, const unsigned int version);
			BOOST_SERIALIZATION_SPLIT_MEMBER()
		};


	}
}

#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/serialization/export.hpp>
BOOST_SERIALIZATION_ASSUME_ABSTRACT(object_recognition_toolkit::core::Algorithm);
//BOOST_CLASS_EXPORT_KEY(object_recognition_toolkit::core::Algorithm);

#pragma warning(pop)

#endif // ALGORITHM_H_