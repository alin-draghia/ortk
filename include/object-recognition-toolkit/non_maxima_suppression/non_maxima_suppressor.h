#pragma once
#ifndef NON_MAXIMA_SUPPRESSOR_H_INCLUDED_
#define NON_MAXIMA_SUPPRESSOR_H_INCLUDED_

#pragma warning(push)
#pragma warning(disable: 4251)

#include "../core/public-api.h"
#include "../core/core.h"
#include "../core/named.h"
#include "../core/clonable.h"
#include "../core/serialization.h"

#include <boost/shared_ptr.hpp>

namespace object_recognition_toolkit
{
	namespace non_maxima_suppression
	{

		class PUBLIC_API NonMaximaSuppressor
		{
		public:			
			virtual ~NonMaximaSuppressor();

			virtual boost::shared_ptr<NonMaximaSuppressor> Clone() const;
	
		public:
			virtual void suppress(std::vector<core::Box>& detections, std::vector<double>& confidences) const;

		private:
			DECLARE_SERIALIZABLE();
		};

	}
}

#pragma warning(pop)

#endif // NON_MAXIMA_SUPPRESSOR_H_INCLUDED_