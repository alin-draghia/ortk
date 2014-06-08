#include "precomp.h"
#include "object-recognition-toolkit/core/core.h"

namespace object_recognition_toolkit
{
	namespace core
	{
		Matrix imread(const std::string& filename, bool color)
		{
			if (color) {
				return cv::imread(filename, cv::IMREAD_COLOR);
			} else {
				return cv::imread(filename, cv::IMREAD_GRAYSCALE);
			}
		}


		void imwrite(const std::string& filename, const Matrix& image)
		{
			cv::imwrite(filename, image);
		}

		void imshow(const Matrix& image, const std::string& name)
		{
			cv::imshow(name, image);
		}

		int show(int delay)
		{
			return cv::waitKey(delay);
		}
	}
}

#define NO_MAKEFILE
#include <dlib/data_io.h>