#pragma once
#ifndef DATASET_H_INCLUDED_
#define DATASET_H_INCLUDED_

#include <dlib/data_io.h>


#include "../core/public-api.h"
#include "../core/core.h"


namespace object_recognition_toolkit
{
	namespace dataset
	{
		using Rectangle = dlib::rectangle;
		using Box = dlib::image_dataset_metadata::box;
		using Image = dlib::image_dataset_metadata::image;
		using Dataset = dlib::image_dataset_metadata::dataset;

		void PUBLIC_API LoadDatasetDlib(const std::string& filename, Dataset& dataset);
		void PUBLIC_API SaveDatasetDlib(const std::string& filename, const Dataset& dataset);

		void PUBLIC_API LoadDatasetPascalV1(const std::vector<std::string>& filenames, Dataset& dataset, const std::string& basepath = std::string());
		void PUBLIC_API LoadDatasetPascalXml(const std::vector<std::string>& filenames, Dataset& dataset);

		void PUBLIC_API LoadDatasetFiles(const std::vector<std::string>& filenames, Dataset& dataset);


		/*
		use label="FULL_IMAGE" to extract full images and ignore labels;
		use label="DONT_CARE" to extract all annotated boxes;
		*/
		void PUBLIC_API ExtractImages(const Dataset& dataset, std::vector<core::Matrix>& images, const std::string& label="FULL_IMAGE", bool color=false);

	}
}

#endif // DATASET_H_INCLUDED_