#include "object-recognition-toolkit/core/algorithm.h"

namespace object_recognition_toolkit {
	namespace core {

		Algorithm::~Algorithm( ) { }

		std::vector<std::string> Algorithm::getParamList( ) const
		{
			std::vector<std::string> list;
			for ( auto& kv : params_ ) {
				list.push_back(kv.first);
			}
			return std::move(list);
		}

		const std::type_info& Algorithm::getParamType(const std::string& paramName) const
		{
			const Param& param = getParam_(paramName);
			return param.val.type( );
		}

		const std::string& Algorithm::getParamHelp(const std::string& paramName) const
		{
			const Param& param = getParam_(paramName);
			return param.help;
		}

		Poco::DynamicAny Algorithm::getParam(const std::string& paramName) const
		{
			const Param& param = getParam_(paramName);
			return param.val;
		}

		void Algorithm::setParam(const std::string& paramName, Poco::DynamicAny paramVal)
		{
			Param& param = getParam_(paramName);
			param.val = paramVal;
		}

		void Algorithm::addParam(const std::string& paramName, Poco::DynamicAny paramVal, const std::string paramHelp)
		{
			auto it = params_.find(paramName);
			if ( it != params_.end( ) ) {
				throw Poco::ExistsException(paramName);
			}
			params_.insert({ paramName, { paramName, paramHelp, paramVal } });
		}


		Algorithm::Param& Algorithm::getParam_(const std::string& paramName)
		{
			try {
				return params_.at(paramName);
			} catch ( std::out_of_range& ) {
				throw Poco::NotFoundException(paramName);
			}
		}

		const Algorithm::Param& Algorithm::getParam_(const std::string& paramName) const
		{
			try {
				return params_.at(paramName);
			} catch ( std::out_of_range& ) {
				throw Poco::NotFoundException(paramName);
			}
		}

	}
}