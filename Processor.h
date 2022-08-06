#pragma once
#include "IDictionary.h"
#include <vector>
#include <algorithm>

namespace spellchecker
{
	// Dictionary/Text split pattern
	const std::string SPLIT_PATTERN = "===";

	class Processor
	{
	public:
		Processor(std::shared_ptr<IDictionary> dict);
		void Start();
	private:
		std::shared_ptr<IDictionary> m_dictionary;
	};
}

