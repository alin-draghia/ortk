#pragma once
#ifndef CONFIGURABLE_HEADER_INCLUDED_
#define CONFIGURABLE_HEADER_INCLUDED_

#include <boost/variant.hpp>
#include "core.h"

namespace object_recognition_toolkit
{
	namespace core
	{

		class Field
		{
		public:
			enum Type
			{
				INTEGER,
				REAL,
				STRING,				
				MATRIX
			};

			Field(const std::string& name);

			Type type() const;
			
			const std::string& name() const;
			void name(const std::string& name);

			int get_integer() const;
			void set_integer(int val);

			double get_real() const;
			void set_real(double val);

			const std::string& get_string() const;
			void set_string(const std::string& val);

			const Matrix& get_matrix() const;
			void set_matrix(const Matrix& val);


		private:

			std::string name_;
			
			using value_type = boost::variant < int, double, std::string, core::Matrix > ;
			
		};

	}
}

#endif // CONFIGURABLE_HEADER_INCLUDED_