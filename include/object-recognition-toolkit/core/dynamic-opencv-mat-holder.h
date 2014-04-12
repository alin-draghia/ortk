#pragma once
#ifndef DYNAMIC_OPENCV_MAT_HOLDER_H_
#define DYNAMIC_OPENCV_MAT_HOLDER_H_

#include <sstream>

#include <opencv2/opencv.hpp>

#include <Poco/SharedPtr.h>
#include <Poco/DynamicAny.h>

namespace Poco {
	namespace Dynamic {

		template<>
		class VarHolderImpl<cv::Mat>
			: public VarHolder
		{
		public:
			VarHolderImpl(cv::Mat mat) : mat_(mat) { }
			~VarHolderImpl( ) { }

			virtual VarHolder* clone(Placeholder<VarHolder>* pHolder = 0) const
			{
				return cloneHolder(pHolder, mat_);
			}

			const cv::Mat& value() const
			{
				return mat_;
			}
			
			const std::type_info& type( ) const
			{
				return typeid(cv::Mat);
			}

			virtual void convert(bool& val) const
			{
				val = !mat_.empty( );
			}

			virtual void convert(std::string& val) const
			{
				std::stringstream ss;
				ss << cv::format(mat_, "NUMPY");
				val = ss.str( );
			}

		private:
			cv::Mat mat_;
		};

	}
}

#endif DYNAMIC_OPENCV_MAT_HOLDER_H_