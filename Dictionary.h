#pragma once
#include "IDictionary.h"
#include "CompareAlgorithm.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>


namespace spellchecker
{
	class Dictionary : public IDictionary
	{
	public:
		Dictionary();
		~Dictionary();
		// Adds word to dictionary
		virtual void Add(const std::string word) override;

		// Searches word in dictionary and return possible corrections
		virtual SearchResult Search(const std::string word) override;
	private:
		// Helper for lowerCase
		std::string toLowerCase(const std::string word);
		// Comparation algorithm
		CompareAlgorithm m_algo_processor;
		
		// Caching
		// Response Cache (for repeated words)
		std::map<std::string, SearchResult> m_searchCache;

		// Indexing
		// 2 x Deletion/Insertion for adjacent characters is impossible
		// When inserting at the beginning of a word, the second letter of the word will be the index, 
		// and when deleting, the first.
		// This reduces amount of dictionary words to check.
		// E.g. "cat"
		//      | Operation         |  Letter in word   |  Letter in dict word |
		//      | Normal  (cat)     |  1                |  1                   |    
		//      | Insert  (bcat)    |  2                |  1                   |
		//      | Delete  (at)      |  1                |  2                   |
		//      | Subs (I+D) (bat)  |  2                |  2                   |
		typedef std::shared_ptr<std::string> SharedString; // To avoid copy of string while searching
		typedef std::vector<SharedString> VectorOfSharedStrings;
		std::map<char, VectorOfSharedStrings> m_firstLetterIndex;
		std::map<char, VectorOfSharedStrings> m_secondLetterIndex;
	};
}


