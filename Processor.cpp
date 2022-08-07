#include <numeric>
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
				{ // << this `{` is for keeping original formatting to not produce big diffs
					decltype(result.corrections) filtered_corrections;
					// Filter only suitable corrections and count one edit corrections
					int num_one_edits = 0;
					std::copy_if(result.corrections.begin(), result.corrections.end(), std::back_inserter(filtered_corrections),
						[&num_one_edits](const Correction& c) { 
							if(c.edits == 1) {
								num_one_edits++;
							}
							return c.edits == 1 || c.edits == 2;
						});
					
					// Sort corrections by edits count
					std::sort(filtered_corrections.begin(), filtered_corrections.end(),
						[](const Correction& lhs, const Correction& rhs) { return lhs.edits < rhs.edits; });

					// If there are some corrections exist...
					if (!filtered_corrections.empty())
					{
						// One correction or only one with one edit - print as is from dictionary
						if (filtered_corrections.size() == 1 || num_one_edits == 1)
						{
							std::cout << filtered_corrections.front().word;
						}
						else
						{
							// Sort corrections by dictionary order
							std::sort(filtered_corrections.begin(), filtered_corrections.end(),
							[](const Correction& lhs, const Correction& rhs) {
								return lhs.dict_index < rhs.dict_index;
							});

							// Join corrections and print with delimiters
							std::string variants = "{" + filtered_corrections.front().word;
							for (int i = 1; i < filtered_corrections.size(); i++)
							{
								variants.append(" " + filtered_corrections[i].word);
							}
							variants.append("}");
							std::cout << variants;
							
						}
					}
					else
					{
						// No corrections found
						std::cout << "{" << word << "?}";
					}
				}
			}

		} // while
	
	}
}
