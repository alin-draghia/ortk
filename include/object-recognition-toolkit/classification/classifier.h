#pragma once
#ifndef CLASSIFIER_H_
#define CLASSIFIER_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include <string>
#include <vector>

#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#include <object-recognition-toolkit/core/algorithm.h>

namespace object_recognition_toolkit
{
	namespace classification
	{
		class ORT_API Classifier
			: public object_recognition_toolkit::core::Algorithm
		{
		public:
			Classifier();
			Classifier(const std::string& name);
			virtual ~Classifier();

			virtual double PredictConf(const std::vector<float>& instance) const;

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
BOOST_CLASS_EXPORT_KEY(object_recognition_toolkit::classification::Classifier);

#pragma warning(pop)

#endif // CLASSIFIER_H_