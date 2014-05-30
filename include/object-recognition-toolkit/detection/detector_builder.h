#pragma once
#ifndef DETECTOR_BUILDER_H_INCLUDED_
#define DETECTOR_BUILDER_H_INCLUDED_


#pragma warning(push)
#pragma warning(disable: 4251)

#include "./detector.h"


namespace object_recognition_toolkit
{
	namespace detection
	{

		class PUBLIC_API DetectorBuilder
		{
		public:
			virtual ~DetectorBuilder() = 0;

		public:
			virtual Detector* Build() const = 0;

		public:
			void PutPyramidBuilder(std::shared_ptr<pyramid::PyramidBuilder> pyramidBuilder);
			void PutImageScanner(std::shared_ptr<image_scanning::ImageScanner> imageScanner);
			void PutFeatureExtractor(std::shared_ptr<feature_extraction::FeatureExtractor> featureExtractor);
			void PutClassifier(std::shared_ptr<classification::Classifier> classifier);
			void PutNonMaximaSuppressor(std::shared_ptr<non_maxima_suppression::NonMaximaSuppressor> nonMaximaSuppressor);
			
		protected:
			std::shared_ptr<pyramid::PyramidBuilder> pyramidBuilder_;
			std::shared_ptr<image_scanning::ImageScanner> imageScanner_;
			std::shared_ptr<feature_extraction::FeatureExtractor> featureExtractor_;
			std::shared_ptr<classification::Classifier> classifier_;
			std::shared_ptr<non_maxima_suppression::NonMaximaSuppressor> nonMaximaSuppressor_;
		};

		class PUBLIC_API  DetectorBase_Builder
			: public DetectorBuilder
		{
		public:			
			virtual ~DetectorBase_Builder();

		public:
			virtual Detector* Build() const;
		};

		class PUBLIC_API  DetectorBaseMT_Builder
			: public DetectorBuilder
		{
		public:
			virtual ~DetectorBaseMT_Builder();

		public:
			virtual Detector* Build() const;
		};

	}
}

#pragma warning(pop)

#endif // DETECTOR_BUILDER_H_INCLUDED_