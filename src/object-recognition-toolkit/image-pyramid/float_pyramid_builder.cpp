#include "object-recognition-toolkit/image-pyramid/float_pyramid_builder.h"

namespace object_recognition_toolkit
{
	namespace pyramid
	{

		FloatPyramidBuilder::FloatPyramidBuilder()
		{
		}

		FloatPyramidBuilder::FloatPyramidBuilder(double scaleFactor, cv::Size minSize, cv::Size maxSize)
			: scaleFactor_{ scaleFactor }
			, minSize_{ minSize }
			, maxSize_{ maxSize }
		{
		}


		FloatPyramidBuilder::~FloatPyramidBuilder()
		{
		}

		const std::string& FloatPyramidBuilder::name() const
		{
			static const std::string name = "FloatImagePyramid";
			return name;
		}

		core::Clonable* FloatPyramidBuilder::Clone()
		{
			return new FloatPyramidBuilder(*this);
		}
		
		double FloatPyramidBuilder::GetScaleFactor() const
		{
			return scaleFactor_;
		}


		cv::Size FloatPyramidBuilder::GetMinSize() const
		{
			return minSize_;
		}


		cv::Size FloatPyramidBuilder::GetMaxSize() const
		{
			return maxSize_;
		}


		Pyramid FloatPyramidBuilder::Build(const core::Matrix& image) const
		{
			Pyramid pyramid;
			

			for (int i = 0; true; i++) {
				double scale = 1.0 / std::pow(scaleFactor_, i);
				int width = cvRound(image.cols * scale);
				int height = cvRound(image.rows * scale);

				if (maxSize_.width != 0 && maxSize_.height != 0) {
					if (width > maxSize_.width || height > maxSize_.height) {
						continue;
					}
				}

				if (width < minSize_.width || height < minSize_.height) {
					break;
				}

				if (width == 0 || height == 0) {
					break;
				}

				cv::Mat image0;
				cv::resize(image, image0, cv::Size(), scale, scale, cv::INTER_LINEAR);
				

				pyramid.AddLevel({ image0, scale });
			}
			
			return std::move(pyramid);
		}

		void FloatPyramidBuilder::serialize(core::iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<PyramidBuilder>(*this);
			ar >> scaleFactor_; 
			ar >> minSize_;
			ar >> maxSize_;
		}

		void FloatPyramidBuilder::serialize(core::oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<PyramidBuilder>(*this);
			ar << scaleFactor_;
			ar << minSize_;
			ar << maxSize_;
		}

	}
}

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(object_recognition_toolkit::pyramid::FloatPyramidBuilder);