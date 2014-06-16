#pragma once
#ifndef IMAGE_PYRAMID_H_
#define IMAGE_PYRAMID_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "../core/public-api.h"
#include "../core/core.h"
#include "../core/named.h"
#include "../core/clonable.h"
#include "../core/serialization.h"
#include "./pyramid-level.h"
#include "./pyramid.h"

#include <boost/shared_array.hpp>

namespace object_recognition_toolkit
{
	namespace pyramid
	{
		class PUBLIC_API PyramidBuilder
		{
		public:
			virtual ~PyramidBuilder();

		public:
			virtual boost::shared_ptr<PyramidBuilder> Clone() const;

		public:
			virtual Pyramid Build(core::Matrix const& image) const;
			
		private:
			DECLARE_SERIALIZABLE();
		};
	}
}

#pragma warning(pop)

#endif // IMAGE_PYRAMID_H_