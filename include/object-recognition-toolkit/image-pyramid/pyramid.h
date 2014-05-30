#pragma once
#ifndef PYRAMID_HEADER_INCLUDED_
#define PYRAMID_HEADER_INCLUDED_

#include <vector>

#include "../core/public-api.h"
#include "../core/core.h"
#include "../core/named.h"
#include "../core/serialization.h"
#include "./pyramid-level.h"

namespace object_recognition_toolkit
{
	namespace pyramid
	{
		class Pyramid
		{
		public:
			Pyramid() = default;
			~Pyramid() = default;
			Pyramid(Pyramid&) = default;

		public:
			int AddLevel(const PyramidLevel& level);
			void Clear();
			int GetNumLevels() const;
			const PyramidLevel& GetLevel(int index) const;
			
		private:
			std::vector<PyramidLevel> levels_;
		};


		int Pyramid::AddLevel(const PyramidLevel& level)
		{
			this->levels_.push_back(level);
			return static_cast<int>(this->levels_.size() - 1);
		}

		void Pyramid::Clear()
		{
			this->levels_.clear();
		}

		int Pyramid::GetNumLevels() const
		{
			return static_cast<int>(this->levels_.size());
		}

		const PyramidLevel& Pyramid::GetLevel(int index) const
		{
			return this->levels_.at(index);
		}
	}
}

#endif // PYRAMID_HEADER_INCLUDED_