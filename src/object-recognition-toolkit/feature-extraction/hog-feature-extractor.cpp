#include "precomp.h"
#include <object-recognition-toolkit/feature-extraction/hog-feature-extractor.h>


namespace object_recognition_toolkit {
	namespace feature_extraction {


		HogFeatureExtractor::HogFeatureExtractor()
			: winSize_{ 64, 128 }
			, blockSize_{ 16, 16 }
			, blockStride_{ 8, 8 }
			, nBins_{ 9 }
		{
		}


		HogFeatureExtractor::~HogFeatureExtractor()
		{
		}

		const std::string& HogFeatureExtractor::name() const
		{
			static const std::string name = "HogFeatureExtractor";
			return name;
		}

		core::Clonable* HogFeatureExtractor::Clone()
		{
			std::stringstream ss;
			core::oarchive oa(ss);			
			oa << this;

			core::iarchive ia(ss);
			HogFeatureExtractor* ptr;
			ia >> ptr;
			return ptr;
		}

		core::FeatureVector HogFeatureExtractor::compute(const cv::Mat& image) const
		{
			std::vector<float> feats;
			hog_.compute(image, feats);
			return core::FeatureVector(feats, true);
		}


		int HogFeatureExtractor::lenght() const
		{
			return (int)hog_.getDescriptorSize();
		}


		void HogFeatureExtractor::serialize(core::iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<FeatureExtractor>(*this);
			ar >> winSize_;
			ar >> blockSize_;
			ar >> blockStride_;
			ar >> nBins_;
		}

		void HogFeatureExtractor::serialize(core::oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<FeatureExtractor>(*this);
			ar << winSize_;
			ar << blockSize_;
			ar << blockStride_;
			ar << nBins_;
		}

	}
}

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(object_recognition_toolkit::feature_extraction::HogFeatureExtractor);