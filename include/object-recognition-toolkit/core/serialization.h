#pragma once
#ifndef SERIALIZATION_H_INCLUDED_
#define SERIALIZATION_H_INCLUDED_

#include <memory>
#include <string>
#include <fstream>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>

#include <boost/serialization/split_free.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <opencv2/opencv.hpp>

#include "./public-api.h"

namespace object_recognition_toolkit
{
	namespace core
	{
		using iarchive = boost::archive::text_iarchive;
		using oarchive = boost::archive::text_oarchive;
	}
}

namespace boost
{
	namespace serialization
	{
		template<class Archive, class Tp>
		void serialize(Archive& ar, cv::Size_<Tp>& size, const unsigned int version)
		{
			ar & size.width;
			ar & size.height;
		}

		template<class Archive, class Tp>
		void serialize(Archive& ar, cv::Point_<Tp>& point, const unsigned int version)
		{
			ar & point.x;
			ar & point.y;
		}

		template<class Archive, class Tp>
		void serialize(Archive& ar, cv::Rect_<Tp>& rect, const unsigned int version)
		{
			ar & rect.x;
			ar & rect.y;
			ar & rect.width;
			ar & rect.height;
		}


		template<class Archive>
		void save(Archive& ar, const cv::Mat& mat, const unsigned int version)
		{
			cv::FileStorage fs("", cv::FileStorage::WRITE + cv::FileStorage::FORMAT_YAML + cv::FileStorage::MEMORY);
			fs << "mat_yaml" << mat;
			std::string buffer = fs.releaseAndGetString();
			ar << buffer;
		}

		template<class Archive>
		void load(Archive& ar, cv::Mat& mat, const unsigned int version)
		{
			std::string buffer;
			ar >> buffer;
			cv::FileStorage fs(buffer, cv::FileStorage::READ + cv::FileStorage::FORMAT_YAML + cv::FileStorage::MEMORY);
			ar["mat_yaml"] >> mat;
		}

		template<class Archive, class Tp>
		void save(Archive& ar, const std::unique_ptr<Tp>& uptr, const unsigned int version)
		{
			Tp* p = uptr.get();
			ar & p;
		}

		template<class Archive, class Tp>
		void load(Archive& ar, std::unique_ptr<Tp>& uptr, const unsigned int version)
		{
			Tp* p;
			ar & p;
			uptr.reset(p);
		}

		template<class Archive, class Tp>
		void serialize(Archive& ar, std::unique_ptr<Tp>& uptr, const unsigned int version)
		{
			boost::serialization::split_free(ar, uptr, version);
		}
	}
}


BOOST_SERIALIZATION_SPLIT_FREE(cv::Mat);


#endif // SERIALIZATION_H_INCLUDED_