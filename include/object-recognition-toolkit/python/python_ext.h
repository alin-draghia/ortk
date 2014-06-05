#pragma once
#ifndef PYTHON_EXT_HEADER_INCLUDED_
#define PYTHON_EXT_HEADER_INCLUDED_

#include <sstream>
#include <boost/python.hpp>

#include "../core/serialization.h"

namespace object_recognition_toolkit
{
	namespace python_ext
	{

		template<typename T>
		struct serialize_pickle
			: boost::python::pickle_suite
		{

			static boost::python::tuple getstate(const T& item)
			{
				std::ostringstream os;
				core::oarchive oa(os); 
				oa << item;
				return boost::python::make_tuple(os.str());
			}

			static void setstate(T& item, boost::python::tuple state)
			{
				using namespace boost::python;
				str data = extract<str>(state[0]);
				std::string temp(extract<const char*>(data), len(data));
				std::istringstream is;
				core::iarchive ia(is);
				ia >> item;
			}

		};

	}
}

#endif