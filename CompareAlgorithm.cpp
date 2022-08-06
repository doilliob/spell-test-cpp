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
		for (j = 1; j <= len_word; j++) {
			for (i = 1; i <= len_other; i++) {
				if (word[j - 1] == other[i - 1]) {
					track = 0;
				}
				else {
					track = 1;
				}
				t = MIN((m_dist[i - 1][j] + 1), (m_dist[i][j - 1] + 1));
				m_dist[i][j] = MIN(t, (m_dist[i - 1][j - 1] + track));
			}
		}
		return m_dist[len_other][len_word];
	}
}