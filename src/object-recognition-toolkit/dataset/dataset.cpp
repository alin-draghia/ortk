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


		void LoadDatasetFiles(const std::vector<std::string>& filenames, Dataset& dataset)
		{
			for (const std::string& filename : filenames)
			{				
				dataset.images.emplace_back(filename);
			}
		}

		void ExtractImages(const Dataset& dataset, std::vector<core::Matrix>& images, const std::string& label, bool color)
		{
			static const std::string FULL_IMAGE = "FULL_IMAGE";
			static const std::string DONT_CARE = "DONT_CARE";

			for (auto& dataset_image : dataset.images)
			{
				
				const std::string& filename = dataset_image.filename;
				core::Matrix full_image = core::imread(filename, color);

				if (label == FULL_IMAGE) {
					images.push_back(full_image);
					continue;
				}
				
				for (auto& box : dataset_image.boxes) {

					if ((label != DONT_CARE) && (box.label != label)) {
						// ignore if it does not have the right label
						continue;
					}

					// crop the image;
					const dlib::rectangle& box_rect = box.rect;
					int x = (int)box_rect.left();
					int y = (int)box_rect.top();
					int w = (int)box_rect.width();
					int h = (int)box_rect.height();

					core::Box roi(x, y, w, h);

					core::Matrix crop_image;
					full_image(roi).copyTo(crop_image);

					images.push_back(crop_image);
					
				}
			}
		}
	}
}