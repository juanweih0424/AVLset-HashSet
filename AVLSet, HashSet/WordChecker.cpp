// WordChecker.cpp
//
// ICS 46 Spring 2021
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"
#include <iostream>
#include <string>


WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    // Nonsensical code because the compiler requires the member variables
    // 'words' to be used somewhere, or else it becomes a warning (which
    // turns into an error).
    words.isImplemented();
    std::vector<std::string> suggestions;
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    //technique 1: swap each adjacent pair of characters
    for (int i=0;i<word.size()-1;++i)
    {
        std::string s1 = word;
        std::swap(s1[i], s1[i+1]);
        if (wordExists(s1))
            {
                suggestions.push_back(s1);
            }
    }
    
    //technique 2: insert every character in between
    for (int i=0;i<word.size();++i)
    {
        for (int y=0;y<alphabet.size();++y)
        {
            std::string s2 = word;
            s2.insert(i, alphabet.substr(y, 1));
            if (wordExists(s2))
            {
                suggestions.push_back(s2);
            }
        }
    }
    for (auto character: alphabet)
    {
        std::string temp = word;
        temp = word + character;
        if (wordExists(temp))
        {
            suggestions.push_back(temp);
        }
    }

    

    //technique 3: deleting each character from the word
    for (int i=0;i<word.size();++i)
    {
        std::string s3 = word;
        s3 = s3.erase(i,1);
        if (wordExists(s3))
        {
            suggestions.push_back(s3);
        }

    }

    //technique 4: replace every character with each letter
    for (int i=0;i<word.size();++i)
    {
        for (auto character: alphabet)
        {
            std::string s4 = word;
            s4.at(i) = character;
            if (wordExists(s4))
            {
                suggestions.push_back(s4);
            }
        }
    }

    //techinique 5: adding a space in between each adjacent pair of characters in the word.
    for (int i=0;i<word.size();++i)
    {
        std::string s5 = word;
        s5 = s5.insert(i, " ");
        if (wordExists(s5))
        {
            suggestions.push_back(s5);
        }
    } 

    return suggestions;
}

