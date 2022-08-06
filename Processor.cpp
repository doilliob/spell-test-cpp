#include "Processor.h"

namespace spellchecker
{
	Processor::Processor(std::shared_ptr<IDictionary> dict)
		: m_dictionary(dict)
	{
	}

	void Processor::Start()
	{
		std::string word;

		// Load dictionary
		while (std::cin >> word && word != SPLIT_PATTERN)
		{
			m_dictionary.get()->Add(word);
		}

		// Processing the text
		bool is_first_word = true;
		while (std::cin >> word && word != SPLIT_PATTERN)
		{
			// Add space before the word
			if (is_first_word)
			{
				is_first_word = false;
			}
			else 
			{
				std::cout << " ";
			}

			SearchResult result = m_dictionary.get()->Search(word);
			
			// Word found in dictionary
			if (result.found)
			{
				// As in input
				std::cout << word;
			}
			// Not found
			else {
				// No corrections
				if (result.corrections.size() == 0)
				{
					std::cout << "{" << word << "?}";
				}
				// Corrections
				else
				{
					// Filter corrections with one edit
					std::vector<Correction> one_edit_corr;
					std::copy_if(result.corrections.begin(), result.corrections.end(), std::back_inserter(one_edit_corr),
						[](Correction c) { return c.edits == 1; });

					// If corrections with one edit
					if (one_edit_corr.size() > 0)
					{
						// One correction with one edit - print as is from dictionary
						if (one_edit_corr.size() == 1)
						{
							std::cout << one_edit_corr[0].word;
						}
						else
						{
							// Join corrections and print with delimiters
							std::string variants = "{" + one_edit_corr[0].word;
							for (int i = 1; i < one_edit_corr.size(); i++)
							{
								variants.append(" " + one_edit_corr[i].word);
							}
							variants.append("}");
							std::cout << variants;
							
						}
					}
					else
					{
						// Do nothing with 2-edits corrections?
					}
				}
			}

		} // while
	
	}
}