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
			Pyramid(const Pyramid&) = default;
			Pyramid(Pyramid&&);

		public:
			int AddLevel(const PyramidLevel& level);
			void Clear();
			int GetNumLevels() const;
			const PyramidLevel& GetLevel(int index) const;

		private:
			std::vector<PyramidLevel> levels_;
		};

		
	}
}

namespace object_recognition_toolkit
{
	namespace pyramid
	{
		inline
		Pyramid::Pyramid(Pyramid&& other)
		{
			this->levels_ = std::move(other.levels_);
		}

		inline
		int Pyramid::AddLevel(const PyramidLevel& level)
		{
			this->levels_.push_back(level);
			return static_cast<int>(this->levels_.size() - 1);
		}

		inline
		void Pyramid::Clear()
		{
			this->levels_.clear();
		}

		inline
		int Pyramid::GetNumLevels() const
		{
			return static_cast<int>(this->levels_.size());
		}

		inline
		const PyramidLevel& Pyramid::GetLevel(int index) const
		{
			return this->levels_.at(index);
		}

	}
}
#endif // PYRAMID_HEADER_INCLUDED_