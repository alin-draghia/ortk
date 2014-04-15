#pragma once
#ifndef ALGORITHM_H_
#define ALGORITHM_H_

#include <string>
#include <vector>
#include <typeinfo>
#include <map>

//#include <Poco/Any.h>
#include <Poco/DynamicAny.h>
#include <opencv2/opencv.hpp>

#include "dynamic-holders.h"

#include <object-recognition-toolkit/core/core.h>

namespace object_recognition_toolkit {
	namespace core {

		class ORT_API Algorithm
		{
		public:
			virtual ~Algorithm( ) = 0;

			//
			virtual const std::string& name( ) const = 0;
			
			virtual std::vector<std::string> getParamList( ) const;
			virtual const std::type_info& getParamType(const std::string& paramName) const;
			virtual const std::string& getParamHelp(const std::string& paramName) const;
			virtual Poco::DynamicAny getParam(const std::string& paramName) const;
			virtual void setParam(const std::string& paramName, Poco::DynamicAny paramVal);

		protected:
			virtual void addParam(const std::string& paramName, Poco::DynamicAny paramVal, const std::string paramHelp);

			struct Param
			{
				std::string name;
				std::string help;
				Poco::DynamicAny val;
			};

			virtual Param& getParam_(const std::string& paramName);
			virtual const Param& getParam_(const std::string& paramName) const;

		
		private:
			std::map < std::string, Param> params_;

		};

	}
}

#endif // ALGORITHM_H_