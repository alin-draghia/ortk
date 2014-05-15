#include <object-recognition-toolkit/feature-extraction/hog-feature-extractor.h>


namespace object_recognition_toolkit {
	namespace feature_extraction {


		HogFeatureExtractor::HogFeatureExtractor() : 
			FeatureExtractor("HogFeatureExtractor"), 
			winSize_{ 64, 128 }, 
			blockSize_{ 16, 16 }, 
			blockStride_{ 8, 8 }, 
			nBins_{ 9 }
		{
		}


		HogFeatureExtractor::~HogFeatureExtractor()
		{
		}


		std::vector<float> HogFeatureExtractor::compute(const cv::Mat& image) const
		{
			std::vector<float> feats;
			hog_.compute(image, feats);
			return std::move(feats);
		}


		int HogFeatureExtractor::lenght() const
		{
			return (int)hog_.getDescriptorSize();
		}


		void HogFeatureExtractor::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<FeatureExtractor>(*this);
			ar >> winSize_;
			ar >> blockSize_;
			ar >> blockStride_;
			ar >> nBins_;
		}

		void HogFeatureExtractor::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
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