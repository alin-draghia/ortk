#pragma once
#ifndef FLOAT_PYRAMID_H_
#define FLOAT_PYRAMID_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "./pyramid_builder.h"

namespace object_recognition_toolkit
{
	namespace pyramid
	{
		class PUBLIC_API FloatPyramidBuilder
			: public PyramidBuilder
		{
			FloatPyramidBuilder();
		public:
			FloatPyramidBuilder(double scaleFactor, cv::Size minSize, cv::Size maxSize);
			virtual ~FloatPyramidBuilder();

		public:
			virtual const std::string& name() const;

		public:
			virtual Pyramid Build(const core::Matrix& image) const;

		public:
			double GetScaleFactor() const;
			cv::Size GetMinSize() const;
			cv::Size GetMaxSize() const;

		private:
			double scaleFactor_;
			cv::Size minSize_;
			cv::Size maxSize_;

		private:
			friend class boost::serialization::access;
			void serialize(core::iarchive& ar, const unsigned int version);
			void serialize(core::oarchive& ar, const unsigned int version);
		};
	}
}

#pragma warning(pop)

#endif // FLOAT_PYRAMID_H_
