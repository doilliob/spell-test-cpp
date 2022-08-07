#pragma once
#include<iostream>
#include<vector>
#include<memory>

namespace spellchecker 
{

	// Correction entity
	class Correction{
	public:
		// Fixed word after correction
		std::string word;
		// Count of edits in word
		int edits;
		// Original index in dictionary
		std::size_t dict_index;
	};

	// Result of the search word in the dictionary
	class SearchResult {
	public:
		SearchResult() : found(false) {}

		// Is word found in dictionary
		bool found;

		// Possible corrections
		std::vector<Correction> corrections;
	};

	// Dictionary interface
	class IDictionary {
	public:
		// Adds word to dictionary
		// Better to pass const std::string by reference to avoid unneeded sopies
		virtual void Add(const std::string& word) = 0;

		// Searches word in dictionary and return possible corrections
		// Here too (use const reference)
		virtual SearchResult Search(const std::string& word) = 0;
	};

	typedef std::shared_ptr<IDictionary> IDictionaryPtr;
}
