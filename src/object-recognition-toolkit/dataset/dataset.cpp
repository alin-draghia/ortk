#include "object-recognition-toolkit/dataset/dataset.h"

#include "./imglab/convert_pascal_v1.h"
//#include "./imglab/convert_pascal_xml.h"

#include <filesystem>

namespace object_recognition_toolkit
{
	namespace dataset
	{

		void LoadDatasetDlib(const std::string& filename, Dataset& dataset)
		{
			dlib::image_dataset_metadata::load_image_dataset_metadata(dataset, filename);
		}

		void SaveDatasetDlib(const std::string& filename, const Dataset& dataset)
		{
			dlib::image_dataset_metadata::save_image_dataset_metadata(dataset, filename);
		}

		void LoadDatasetPascalV1(const std::vector<std::string>& filenames, Dataset& dataset, const std::string& basepath)
		{
			using namespace std::tr2::sys;
			convert_pascal_v1(filenames, dataset);
			if (!basepath.empty()) {

				const path base_path_ = path(basepath);

				for (auto& im : dataset.images) {
					path full_image_path = base_path_ / path(im.filename);
					im.filename = full_image_path; 
				}

			}
		}

		void LoadDatasetPascalXml(const std::vector<std::string>& filenames, Dataset& dataset)
		{

		}


		void LoadDatasetFiles(const std::vector<std::string>& filenames, Dataset& dataset, const std::string& label)
		{
			for (const std::string& filename : filenames)
			{				
				dataset.images.emplace_back(filename);
				dataset.images.back().boxes.emplace_back();
				dataset.images.back().boxes.back().label = label;
			}
		}
	}
}