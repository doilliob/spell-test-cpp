#pragma once
#include <iostream>
#include "Common.h"
#define MIN(x,y) ((x) < (y) ? (x) : (y))

namespace spellchecker
{
	class CompareAlgorithm
	{
	public:
		// Amount of edits between words
		int EditsCount(std::string word, std::string other);
	private:
		// Distance matrix
		int m_dist[MAX_WORD_LENGTH][MAX_WORD_LENGTH];
	};

}

