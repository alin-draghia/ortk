#include <boost/python.hpp>

#include "object-recognition-toolkit/python/python_ext.h"
#include "object-recognition-toolkit/object_recognition_toolkit.h"


namespace object_recognition_toolkit
{
	namespace detection
	{
		namespace bp = boost::python;

		struct DetectorWrap
			: Detector
			, bp::wrapper < Detector >
		{
			void Detect(const core::Matrix& image, std::vector<core::Box>& detections, std::vector<double>& confidences, double treshold = 0.0) const
			{
				this->get_override("Detect")(image, detections, confidences, treshold);
			}

			pyramid::PyramidBuilder& GetPyramidBuilder() const
			{
				throw std::runtime_error("not implemented");
				//return this->get_override("GetPyramidBuilder")();
			}

			image_scanning::ImageScanner& GetImageScanner() const
			{
				throw std::runtime_error("not implemented");
				// return this->get_override("GetImageScanner")();
			}

			feature_extraction::FeatureExtractor& GetFeatureExtracotr() const
			{
				throw std::runtime_error("not implemented");
				// return this->get_override("GetFeatureExtracotr")();
			}

			classification::Classifier& GetClassifier() const
			{
				throw std::runtime_error("not implemented");
				//return this->get_override("GetClassifier")();
			}

			non_maxima_suppression::NonMaximaSuppressor& GetNonMaximaSuppressor() const
			{
				throw std::runtime_error("not implemented");
				//return this->get_override("GetNonMaximaSuppressor")();
			}

			const std::string& name() const
			{
				return this->get_override("name")();
			}

			Clonable* Clone()
			{
				return this->get_override("Clone")();
			}
		};

		void export_detection() {
			using namespace boost::python;



		}

	}
}