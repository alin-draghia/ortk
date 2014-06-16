#pragma once
#ifndef PYTHON_EXT_HEADER_INCLUDED_
#define PYTHON_EXT_HEADER_INCLUDED_

#include <sstream>
#pragma warning(push)
#pragma warning(disable: 4273 4244 4267 4503)
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#pragma warning(pop)
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

				const char* tpn = typeid(T).name();
				std::ostringstream os;
				core::oarchive oa(os); 
				oa << item;
				
				return boost::python::make_tuple(os.str());
			}

			static void setstate(T& item, boost::python::tuple state)
			{
				const char* tpn = typeid(T).name();
				using namespace boost::python;
				str data = extract<str>(state[0]);
				std::string temp(extract<const char*>(data), len(data));
				std::istringstream is(temp);
				core::iarchive ia(is);
				ia >> item;
				const char* tpn2 = typeid(T).name();

				int igfasg = 0;
			}

		};

	}
}

#endif