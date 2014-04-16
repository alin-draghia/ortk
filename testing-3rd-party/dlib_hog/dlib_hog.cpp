// dlib_hog.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <dlib\array2d.h>
#include <dlib\image_keypoint.h>

int _tmain(int argc, _TCHAR* argv[])
{
	dlib::hog_image<8, 2, 8, 9, dlib::hog_unsigned_gradient, dlib::hog_no_interpolation> hog;

	dlib::array2d<dlib::uint8> im;
	im.set_size(128, 64);

	hog.load<decltype(im)>(im);

	auto dim = hog.get_num_dimensions();
	auto nr = hog.nr();
	auto nc = hog.nc();
	return 0;
}

