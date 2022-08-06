#include <algorithm>
#include <cctype>
#include <string>
#include "Dictionary.h"

namespace spellchecker
{
	Dictionary::Dictionary()
	{
	}

	Dictionary::~Dictionary()
	{
		// Empty cache and index
		m_searchCache.clear();
		m_firstLetterIndex.clear();
		m_secondLetterIndex.clear();
	}

	void Dictionary::Add(const std::string word)
	{
		if (word.size() > 0)
		{
			SharedString shared(new std::string(word));

			// First lowercases letter
			char first = std::tolower(word[0]);
			if (m_firstLetterIndex.empty() ||
				m_firstLetterIndex.find(first) == m_firstLetterIndex.end())
			{
				m_firstLetterIndex[first] = VectorOfSharedStrings();
			}
			m_firstLetterIndex[first].push_back(shared);

			// Second letter
			if (word.size() > 1)
			{
				char second = std::tolower(word[1]);
				if (m_secondLetterIndex.empty() ||
					m_secondLetterIndex.find(second) == m_secondLetterIndex.end())
				{
					m_secondLetterIndex[second] = VectorOfSharedStrings();
				}
				m_secondLetterIndex[second].push_back(shared);
			}
		}
	}

	std::string Dictionary::toLowerCase(const std::string word)
	{
		// Convert search word to lowercase
		std::string word_lowercase = word;
		std::transform(word_lowercase.begin(), word_lowercase.end(), word_lowercase.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return word_lowercase;
	}

	SearchResult Dictionary::Search(const std::string word)
	{
		// Empty string
		if (word.size() == 0)
		{
			return SearchResult();
		}

		// Convert base word to lowercase
		auto word_lowercase = toLowerCase(word);

		// Get response from cache if exists
		if (!m_searchCache.empty())
		{
			auto found = m_searchCache.find(word_lowercase);
			if (found != m_searchCache.end())
			{
				return m_searchCache[word_lowercase];
			}
		}

		// Returned result
		SearchResult result;

		// Search words in storage by first/second letter
		std::set<SharedString> fromIndex; // to avoid duplicates from different indexes
		auto getIndexed = [this](char letter, std::set<SharedString> *storage) {
			// Extract first letter values
			if (!m_firstLetterIndex.empty() &&
				m_firstLetterIndex.find(letter) != m_firstLetterIndex.end())
			{
				for (auto shared : m_firstLetterIndex[letter])
				{
					storage->insert(shared);
				}
			}
			// Extract second letter values
			if (!m_secondLetterIndex.empty() &&
				m_secondLetterIndex.find(letter) != m_secondLetterIndex.end())
			{
				for (auto shared : m_secondLetterIndex[letter])
				{
					storage->insert(shared);
				}
			}
		};
		char first = word_lowercase[0];
		getIndexed(first, &fromIndex);
		if (word_lowercase.size() > 1)
		{
			char second = word_lowercase[1];
			getIndexed(second, &fromIndex);
		}

		// Search in filtered values
		for (const auto sharedWord : fromIndex)
		{	
			// Convert testing word to lowercase
			const auto dict_lowercase = toLowerCase(*sharedWord);

			// Direct coincidence
			if (dict_lowercase == word_lowercase)
			{
				result.found = true;
				// Corrections can be found earlier
				// E.g. 'fall', 'falls'
				if (result.corrections.size() > 0)
				{
					result.corrections.clear();
				}
				return result;
			}

			// Get edits
			int edits = m_algo_processor.EditsCount(word_lowercase, dict_lowercase);
			
			// Maximum allowed 2 edits, else not found
			if (edits <= 2)
			{
				// Else add correction to result
				Correction corr = { *sharedWord, edits};
				result.corrections.push_back(corr);
			}

		}
		// Add response to cache
		m_searchCache[word_lowercase] = result;

		return result;
	}
}