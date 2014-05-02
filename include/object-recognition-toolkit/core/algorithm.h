#pragma once
#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <string>


#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/export.hpp>

#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>

#include <opencv2/opencv.hpp>

#include <object-recognition-toolkit/core/core.h>


namespace object_recognition_toolkit {
	namespace core {


		class ORT_API Algorithm
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

			BOOST_SERIALIZATION_SPLIT_MEMBER()

			template<class Archive>
			void save(Archive& ar, const unsigned int version) const;

			template<class Archive>
			void load(Archive& ar, const unsigned int version);
		};


	}
}


BOOST_CLASS_EXPORT_KEY(object_recognition_toolkit::core::Algorithm);

#pragma warning(pop)

#endif // ALGORITHM_H_