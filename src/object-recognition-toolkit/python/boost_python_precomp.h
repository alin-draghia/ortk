#pragma once
#ifndef BOOST_PYTHON_PRECOMP_HEADER_INCLUDED_
#define BOOST_PYTHON_PRECOMP_HEADER_INCLUDED_


#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#pragma warning(push)
#pragma warning(disable: 4273 4244 4267 4503)
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/scope.hpp>
#pragma warning(pop)

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <opencv2/opencv.hpp>

#include <opencv2/core/core.hpp>

#define HAVE_ROUND
#include <Python.h>

//#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>


#include <memory>
#include <string>
#include <fstream>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/split_free.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#endif // BOOST_PYTHON_PRECOMP_HEADER_INCLUDED_