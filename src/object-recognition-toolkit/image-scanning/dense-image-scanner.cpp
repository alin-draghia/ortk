#include <object-recognition-toolkit/image-scanning/dense-image-scanner.h>

namespace object_recognition_toolkit
{
	namespace image_scanning
	{
		DenseImageScanner::DenseImageScanner()
		{
		}

		DenseImageScanner::DenseImageScanner(cv::Size windowSize, cv::Size windowStep, cv::Size padding)
			: windowSize_{ windowSize }
			, windowStep_{ windowStep }
			, padding_{ padding }
		{
		}


		DenseImageScanner::~DenseImageScanner()
		{
		}

		const std::string& DenseImageScanner::name() const
		{
			static const std::string name = "DenseImageScanner";
			return name;
		}

		cv::Size DenseImageScanner::GetWindowSize() const
		{
			return windowSize_;
		}


		cv::Size DenseImageScanner::GetWindowStep() const
		{
			return windowStep_;
		}


		cv::Size DenseImageScanner::GetPadding() const
		{
			return padding_;
		}


		void DenseImageScanner::ScanImage(const cv::Mat& image, std::vector<cv::Mat>& windows, std::vector<cv::Rect>& boxes) const
		{
			windows.clear();
			boxes.clear();

			int left = padding_.width < 0 ? (image.cols % windowSize_.width) / 2 : padding_.width;
			int right = image.cols - windowSize_.width;
			int top = padding_.height < 0 ? (image.rows % windowSize_.height) / 2 : padding_.height;
			int bottom = image.rows - windowSize_.height;

			if (left < 0 || top < 0) {
				return;
			}

			for (int y = top; y < bottom; y += windowStep_.height) {
				for (int x = left; x < right; x += windowStep_.width) {
					cv::Rect box{ x, y, windowSize_.width, windowSize_.height };
					cv::Mat window = image(box).clone();
					boxes.push_back(box);
					windows.push_back(window);
				}
			}
		}

		void DenseImageScanner::serialize(boost::archive::polymorphic_iarchive& ar, const unsigned int version)
		{
			ar >> boost::serialization::base_object<ImageScanner>(*this);
			ar >> windowSize_;
			ar >> windowStep_;
			ar >> padding_;
		}

		void DenseImageScanner::serialize(boost::archive::polymorphic_oarchive& ar, const unsigned int version)
		{
			ar << boost::serialization::base_object<ImageScanner>(*this);
			ar << windowSize_;
			ar << windowStep_;
			ar << padding_;
		}

	}
}


#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(object_recognition_toolkit::image_scanning::DenseImageScanner);