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

	void Dictionary::Add(const std::string& word)
	{
		if (word.empty())
		{
			return; // sometimes it's more convinient to return early to avoid nested trivial `if` branches.
		}
		// I kept `{` and `}` only to retain original formatting. They can be safely removed.
		{
			// When constructing shared_ptr from object having public constructor it's usually better to use
			// std::make_shared(), because of more optimal memory usage.
			// Retain index in added shared word to use it later, in corrections.
			SharedString shared{std::make_shared<std::string>(word), m_nextIndex++};

			// First lowercases letter
			char first = std::tolower(word[0]);
			// this entire branch is not needed as the next operator[] + push_back will do the same job.
			/*
			if (m_firstLetterIndex.empty() ||
				m_firstLetterIndex.find(first) == m_firstLetterIndex.end())
			{
				m_firstLetterIndex[first] = VectorOfSharedStrings();
			}
			*/
			m_firstLetterIndex[first].push_back(shared);

			// Second letter
			if (word.size() > 1)
			{
				char second = std::tolower(word[1]);
				// this branch is not needed for the same reason as previous
				/*
				if (m_secondLetterIndex.empty() ||
					m_secondLetterIndex.find(second) == m_secondLetterIndex.end())
				{
					m_secondLetterIndex[second] = VectorOfSharedStrings();
				}
				*/
				m_secondLetterIndex[second].push_back(shared);
			}
		}
	}

	std::string Dictionary::toLowerCase(std::string word)
	{
		// Convert search word to lowercase
		// since `word` is passed by value it may be modified directly
		// std::string word_lowercase = word;
		std::transform(word.begin(), word.end(), word.begin(),
			[](unsigned char c) { return std::tolower(c); });
		return word;
	}

	SearchResult Dictionary::Search(const std::string& word)
	{
		// Empty string
		if (word.empty())
		{
			return SearchResult();
		}

		// Convert base word to lowercase
		auto word_lowercase = toLowerCase(word);

		// Get response from cache if exists
		// `find` will return `end()` if map is empty, so this if is not required
//		if (!m_searchCache.empty())
		{
			auto found = m_searchCache.find(word_lowercase);
			if (found != m_searchCache.end())
			{
				// the next call will perform `find()` again.
				// It's better to use iterator since it already points to the needed value here.
				// return m_searchCache[word_lowercase];
				return found->second;
			}
		}

		// Returned result
		SearchResult result;

		// Search words in storage by first/second letter
		std::set<SharedString> fromIndex; // to avoid duplicates from different indexes
		auto getIndexed = [this](char letter, std::set<SharedString> *storage) {
			// General issue: use already got iterators instead of `operator []`.
			// Extract first letter values
			auto itFirst = m_firstLetterIndex.find(letter);
			if (/*!m_firstLetterIndex.empty() && */ // < this is also not needed
				itFirst != m_firstLetterIndex.end())
			{
				// If possible, better use const references to avoid copies
				for (const auto& shared : itFirst->second)
				{
					storage->insert(shared);
				}
			}
			// Extract second letter values
			auto itSecond = m_secondLetterIndex.find(letter);
			if (/* !m_secondLetterIndex.empty() && */ // < and this too
				itSecond != m_secondLetterIndex.end())
			{
				for (const auto& shared : itSecond->second)
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
			const auto dict_lowercase = toLowerCase(*sharedWord.m_string);

			// Direct coincidence
			if (dict_lowercase == word_lowercase)
			{
				result.found = true;
				// Corrections can be found earlier
				// E.g. 'fall', 'falls'
				// It's not necessary to check the size here
				//if (result.corrections.size() > 0)
				//{
					result.corrections.clear();
				//}
				return result;
			}

			// Get edits
			int edits = m_algo_processor.EditsCount(word_lowercase, dict_lowercase);
			
			// Maximum allowed 2 edits, else not found
			if (edits <= 2)
			{
				// Else add correction to result
				Correction corr = { *sharedWord.m_string, edits, sharedWord.m_index};
				result.corrections.push_back(corr);
			}

		}
		// Add response to cache
		m_searchCache[word_lowercase] = result;

		return result;
	}
}
