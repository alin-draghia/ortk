#pragma once
#ifndef C8847D2C_5C87_4BFD_A58F_118E4BCD9A43
#define C8847D2C_5C87_4BFD_A58F_118E4BCD9A43

#include "feature-extractor.h"

namespace object_recognition_toolkit
{
	namespace feature_extraction
	{
		class DallalTriggsHog
			: public object_recognition_toolkit::feature_extraction::FeatureExtractor
		{
		public:
			DallalTriggsHog();
			virtual ~DallalTriggsHog();
			virtual const std::string& name() const;

		private:

		};

	}
}

#endif //C8847D2C_5C87_4BFD_A58F_118E4BCD9A43