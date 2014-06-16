#pragma once
#ifndef DETECTOR_BUILDER_H_INCLUDED_
#define DETECTOR_BUILDER_H_INCLUDED_


#pragma warning(push)
#pragma warning(disable: 4251)

#include "./detector.h"

#include <boost/shared_ptr.hpp>

namespace object_recognition_toolkit
{
	namespace detection
	{

		class PUBLIC_API DetectorBuilder
		{
		public:
			virtual ~DetectorBuilder() = 0;

		public:
			virtual boost::shared_ptr<Detector> Build() const = 0;

		public:
			void PutPyramidBuilder(boost::shared_ptr<pyramid::PyramidBuilder> pyramidBuilder);
			void PutImageScanner(boost::shared_ptr<image_scanning::ImageScanner> imageScanner);
			void PutFeatureExtractor(boost::shared_ptr<feature_extraction::FeatureExtractor> featureExtractor);
			void PutClassifier(boost::shared_ptr<classification::Classifier> classifier);
			void PutNonMaximaSuppressor(boost::shared_ptr<non_maxima_suppression::NonMaximaSuppressor> nonMaximaSuppressor);
			
		protected:
			boost::shared_ptr<pyramid::PyramidBuilder> pyramidBuilder_;
			boost::shared_ptr<image_scanning::ImageScanner> imageScanner_;
			boost::shared_ptr<feature_extraction::FeatureExtractor> featureExtractor_;
			boost::shared_ptr<classification::Classifier> classifier_;
			boost::shared_ptr<non_maxima_suppression::NonMaximaSuppressor> nonMaximaSuppressor_;
		};

		class PUBLIC_API  DetectorBase_Builder
			: public DetectorBuilder
		{
		public:			
			virtual ~DetectorBase_Builder();

		public:
			virtual boost::shared_ptr<Detector> Build() const;
		};

		class PUBLIC_API  DetectorBaseMT_Builder
			: public DetectorBuilder
		{
		public:
			virtual ~DetectorBaseMT_Builder();

		public:
			virtual boost::shared_ptr<Detector> Build() const;
		};

	}
}

#pragma warning(pop)

#endif // DETECTOR_BUILDER_H_INCLUDED_