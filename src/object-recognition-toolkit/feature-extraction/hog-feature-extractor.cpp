#include "precomp.h"
#include <object-recognition-toolkit/feature-extraction/hog-feature-extractor.h>

#include <omp.h>

namespace object_recognition_toolkit {
	namespace feature_extraction {


		HogFeatureExtractor::HogFeatureExtractor()
			: winSize_{ 64, 128 }
			, blockSize_{ 16, 16 }
			, blockStride_{ 8, 8 }
			, nBins_{ 9 }
		{
		}

		HogFeatureExtractor::HogFeatureExtractor(cv::Size winSize, cv::Size blockSize, cv::Size blockStride, cv::Size cellSize, int nBins)
			: winSize_{ winSize }
			, blockSize_{ blockSize }
			, blockStride_{ blockStride }
			, nBins_{ nBins }
		{

		}


		HogFeatureExtractor::~HogFeatureExtractor()
		{
		}


		boost::shared_ptr<FeatureExtractor> HogFeatureExtractor::Clone() const
		{
			std::stringstream ss;
			core::oarchive oa(ss);			
			oa << this;

			core::iarchive ia(ss);
			HogFeatureExtractor* ptr;
			ia >> ptr;
			return boost::shared_ptr<FeatureExtractor>(ptr);
		}

		core::FeatureVector HogFeatureExtractor::Compute(const cv::Mat& image) const
		{
			std::vector<float> feats;
			hog_.compute(image, feats);
			return core::FeatureVector(feats, true).reshape(1, 1);
		}

		void HogFeatureExtractor::ComputeMulti(std::vector<core::Matrix> const& images, core::Matrix& features) const
		{
			int num_images = (int)images.size( );
			
			int num_threads = omp_get_max_threads();
			std::vector<boost::shared_ptr<FeatureExtractor>> extractors_pool;
			for ( int i = 0; i < num_threads; i++ )
				extractors_pool.push_back(this->Clone( ));

			#pragma omp parallel for
			for( int i = 0; i < num_images; i++ ) {
				int thread_id = omp_get_thread_num();
				features.row(i) = extractors_pool[thread_id]->Compute(images[i]);
			}
		}


		int HogFeatureExtractor::Lenght() const
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