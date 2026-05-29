/**
 * @file anagram_dict.cpp
 * Implementation of the AnagramDict class.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "anagram_dict.h"

#include <algorithm> /* I wonder why this is included... */
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;

static inline std::string get_unique_name(const std::string &in) {
	std::string out = in;
	std::transform(out.begin(), out.end(), out.begin(), ::tolower);
	std::sort(out.begin(), out.end());
	return out;
}

void process_words(const std::vector<std::string> &words, 
	std::map<std::string, std::vector<std::string>> &dest_map)
{
	for (const std::string &word : words) {
		std::string name = get_unique_name(word);
			if (dest_map.find(name) == dest_map.end()) {
				std::vector<std::string> val = {word};
				dest_map[name] = val;
			} else {
				dest_map[name].emplace_back(word);
			}
    	}
}

/**
 * Constructs an AnagramDict from a filename with newline-separated
 * words.
 * @param filename The name of the word list file.
 */
AnagramDict::AnagramDict(const string& filename)
{
	/* Your code goes here! */

	/* get words */
	std::vector<std::string> words;
	std::ifstream file(filename);
	std::string word;
	if (file.is_open()) {
		while (file >> word) {
			words.emplace_back(word);
		}
	}

	/* process words */
	process_words(words, dict);
}

/**
 * Constructs an AnagramDict from a vector of words.
 * @param words The vector of strings to be used as source words.
 */
AnagramDict::AnagramDict(const vector<string>& words)
{
    	/* Your code goes here! */
	process_words(words, dict);
}

/**
 * @param word The word to find anagrams of.
 * @return A vector of strings of anagrams of the given word. Empty
 * vector returned if no anagrams are found or the word is not in the
 * word list.
 */
vector<string> AnagramDict::get_anagrams(const string& word) const
{
    	/* Your code goes here! */
	std::string name = get_unique_name(word);
	std::map<std::string, std::vector<std::string>>::const_iterator it = dict.find(name);
	if (it == dict.end()) {
		return std::vector<std::string>();
	} else {
		return it->second;
	}
}

/**
 * @return A vector of vectors of strings. Each inner vector contains
 * the "anagram siblings", i.e. words that are anagrams of one another.
 * NOTE: It is impossible to have one of these vectors have less than
 * two elements, i.e. words with no anagrams are ommitted.
 */
vector<vector<string>> AnagramDict::get_all_anagrams() const
{
    	/* Your code goes here! */
	std::vector<std::vector<std::string>> out;
	for (const std::pair<const std::string, std::vector<std::string>> &key_val : dict) {
		if (key_val.second.size() > 1) {
			out.emplace_back(key_val.second);
		}
	}
	return out;
}
