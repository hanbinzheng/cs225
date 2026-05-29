/**
 * @file cartalk_puzzle.cpp
 * Holds the function which solves a CarTalk puzzler.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include <fstream>
#include <iostream>

#include "cartalk_puzzle.h"

using namespace std;

/**
 * Solves the CarTalk puzzler described here:
 * http://www.cartalk.com/content/wordplay-anyone.
 * @return A vector of (string, string, string) tuples
 * Returns an empty vector if no solutions are found.
 * @param d The PronounceDict to be used to solve the puzzle.
 * @param word_list_fname The filename of the word list to be used.
 */
vector<std::tuple<std::string, std::string, std::string>> cartalk_puzzle(PronounceDict d,
                                    const string& word_list_fname)
{
   	vector<std::tuple<std::string, std::string, std::string>> ret;

    	/* Your code goes here! */
    	ifstream wordsFile(word_list_fname);
	std::string word;
	if (wordsFile.is_open()) {
    		while (getline(wordsFile, word)) {
			if (word.length() <= 2) {
				continue; /* in case that the word has less that 2 char */
			}

			/* get substring that minus the first and the second character */
			std::string _first = word.substr(1);
			std::string _second = word.substr(0, 1) + word.substr(2);

			if (d.homophones(word, _first) && d.homophones(word, _second)) {
				ret.emplace_back(word, _first, _second);
			}
    		}
	} 

    	return ret;
}
