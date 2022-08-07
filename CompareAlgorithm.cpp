#include "CompareAlgorithm.h"
namespace spellchecker
{
	/**
	* Implementation of counting Levenshtein m_distance
	* See https://en.wikipedia.org/wiki/Levenshtein_m_distance
	* 
	* Returns amount of edits between two words
	*/
	int CompareAlgorithm::EditsCount(std::string word, std::string other)
	{
		int i, j, t, track;
		int len_word = word.length();
		int len_other = other.length();
		
		for (i = 0; i <= len_word; i++) {
			m_dist[0][i] = i;
		}
		for (j = 0; j <= len_other; j++) {
			m_dist[j][0] = j;
		}
		bool was_insertion = false;
		bool was_deletion = false;
		for (j = 1; j <= len_word; j++) {
			for (i = 1; i <= len_other; i++) {
				if (word[j - 1] == other[i - 1]) {
					track = 0;
				}
				else {
					// Substitution is not allowed by task conditions
					track = MAX_WORD_LENGTH + 1;
				}
				auto delete_cost = m_dist[i - 1][j] + 1;
				auto insert_cost = m_dist[i][j - 1] + 1;
				t = std::min(delete_cost, insert_cost);
				if (delete_cost < insert_cost) {
					if (was_deletion) {
						t = MAX_WORD_LENGTH + 1; // Adjacent deletions are not allowed by task conditions
					}
					was_insertion = false;
					was_deletion = true;
				} else if (insert_cost < delete_cost) {
					if (was_insertion) {
						t = MAX_WORD_LENGTH + 1; // Adjacent insertions are not allowed by task conditions
					}
					was_deletion = false;
					was_insertion = true;
				} else {
					was_deletion = was_insertion = false;
				}

				m_dist[i][j] = std::min(t, m_dist[i - 1][j - 1] + track);
			}
		}
		return m_dist[len_other][len_word];
	}
}