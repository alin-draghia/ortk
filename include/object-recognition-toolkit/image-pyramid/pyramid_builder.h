#pragma once
#ifndef IMAGE_PYRAMID_H_
#define IMAGE_PYRAMID_H_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "../core/public-api.h"
#include "../core/core.h"
#include "../core/named.h"
#include "../core/serialization.h"
#include "./pyramid-level.h"

namespace object_recognition_toolkit
{
	namespace pyramid
	{
		class PUBLIC_API PyramidBuilder
			: public core::Named
		{
		public:
			virtual ~PyramidBuilder() = 0;

		public:
			virtual std::vector<PyramidLevel> Build(const core::Matrix& image) const = 0;
			
		private:
			friend class boost::serialization::access;
			void serialize(core::iarchive& ar, const unsigned int version);
			void serialize(core::oarchive& ar, const unsigned int version);
		};
	}
}

#pragma warning(pop)

#endif // IMAGE_PYRAMID_H_