#pragma once
#ifndef WINDOW_HEADER_INCLUDED_
#define WINDOW_HEADER_INCLUDED_

#include "../core/public-api.h"
#include "../core/core.h"
#include "../core/named.h"
#include "../core/serialization.h"

namespace object_recognition_toolkit
{
	namespace image_scanning
	{
		struct Window
		{		
			core::Matrix image;
			core::Box box;

			bool operator==(Window const& w) const {
				return w.box == this->box;
			}

			bool operator!=(Window const& w) const { 
				return w.box != this->box;
			}
		};

	}
}

#endif // WINDOW_HEADER_INCLUDED_