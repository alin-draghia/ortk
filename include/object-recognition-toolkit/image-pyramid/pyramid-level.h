#pragma once
#ifndef PYRAMID_LEVEL_H_
#define PYRAMID_LEVEL_H_


#pragma warning(push)
#pragma warning(disable: 4251)

#include "../core/public-api.h"
#include "../core/core.h"

namespace object_recognition_toolkit
{
	namespace pyramid
	{
		class PyramidLevel
		{
		public:
			PyramidLevel(const core::Matrix& image, double scale);
			~PyramidLevel();

			double GetScale() const;

			const core::Matrix& GetImage() const;

			template<typename T>
			cv::Rect_<T> Transform(const cv::Rect_<T>& box) const;

			template<typename T>
			cv::Rect_<T> Invert(const cv::Rect_<T>& box) const;

			template<typename T>
			cv::Point_<T> Transform(const cv::Point_<T>& pt) const;

			template<typename T>
			cv::Point_<T> Invert(const cv::Point_<T>& pt) const;

		private:
			cv::Mat image_;
			double scale_;
		};


		inline
		PyramidLevel::PyramidLevel(const core::Matrix& image, double scale)
			: image_{ image }, scale_{ scale } {}

		inline
		PyramidLevel::~PyramidLevel() {}

		inline
		double PyramidLevel::GetScale() const
		{
			return scale_;
		}

		inline
		const core::Matrix& PyramidLevel::GetImage() const
		{
			return image_;
		}

		template<typename T> inline
		cv::Rect_<T> PyramidLevel::Transform(const cv::Rect_<T>& box) const
		{
			double f = scale_;
			T x = static_cast<T>(box.x * f);
			T y = static_cast<T>(box.y * f);
			T w = static_cast<T>(box.width * f);
			T h = static_cast<T>(box.height * f);
			return cv::Rect_<T>(x, y, w, h);
		}

		template<typename T> inline
		cv::Rect_<T> PyramidLevel::Invert(const cv::Rect_<T>& box) const
		{
			double f = 1.0 / scale_;
			T x = static_cast<T>(box.x * f);
			T y = static_cast<T>(box.y * f);
			T w = static_cast<T>(box.width * f);
			T h = static_cast<T>(box.height * f);
			return cv::Rect_<T>(x, y, w, h);
		}

		template<typename T> inline
		cv::Point_<T> PyramidLevel::Transform(const cv::Point_<T>& pt) const
		{
			double f = scale_;
			T x = static_cast<T>(pt.x * f);
			T y = static_cast<T>(pt.y * f);
			return cv::Point_<T>(x, y, w, h);
		}

		template<typename T> inline
		cv::Point_<T> PyramidLevel::Invert(const cv::Point_<T>& pt) const
		{
			double f = 1.0 / scale_;
			T x = static_cast<T>(pt.x * f);
			T y = static_cast<T>(pt.y * f);
			return cv::Point_<T>(x, y, w, h);
		}
	}
}

#pragma warning(pop)

#endif