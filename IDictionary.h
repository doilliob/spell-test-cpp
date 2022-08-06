#pragma once
#include<iostream>
#include<vector>

namespace spellchecker 
{

	// Correction entity
	class Correction{
	public:
		// Fixed word after correction
		std::string word;
		// Count of edits in word
		int edits;
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
		virtual void Add(const std::string word) = 0;

		// Searches word in dictionary and return possible corrections
		virtual SearchResult Search(const std::string word) = 0;
	};

	typedef std::shared_ptr<IDictionary> IDictionaryPtr;
}