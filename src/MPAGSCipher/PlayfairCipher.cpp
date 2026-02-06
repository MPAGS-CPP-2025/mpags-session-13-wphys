#include "PlayfairCipher.hpp"

#include <algorithm>
#include <iostream>
#include <string>
/**
 * \file PlayfairCipher.cpp
 * \brief Contains the implementation of the PlayfairCipher class
 */

PlayfairCipher::PlayfairCipher(const std::string& key)
{
    this->setKey(key);
}

void PlayfairCipher::setKey(const std::string& key)
{
    // Store the original key
    key_ = key;

    // Append the alphabet to the key
    key_ += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Make sure the key is upper case
    std::transform(std::begin(key_), std::end(key_), std::begin(key_),
                   ::toupper);

    // Remove non-alphabet characters
    key_.erase(std::remove_if(std::begin(key_), std::end(key_),
                              [](char c) { return !std::isalpha(c); }),
               std::end(key_));

    // Change J -> I
    std::transform(std::begin(key_), std::end(key_), std::begin(key_),
                   [](char c) { return (c == 'J') ? 'I' : c; });

    // Remove duplicated letters
    std::string lettersFound{""};
    auto detectDuplicates = [&](char c) {
        if (lettersFound.find(c) == std::string::npos) {
            lettersFound += c;
            return false;
        } else {
            return true;
        }
    };
    key_.erase(
        std::remove_if(std::begin(key_), std::end(key_), detectDuplicates),
        std::end(key_));

    // Store the coordinates of each letter
    // (at this point the key length must be equal to the square of the grid dimension)
    for (std::size_t i{0}; i < keyLength_; ++i) {
        std::size_t row{i / gridSize_};
        std::size_t column{i % gridSize_};

        auto coords = std::make_pair(row, column);

        charLookup_[key_[i]] = coords;
        coordLookup_[coords] = key_[i];
    }
}

std::string PlayfairCipher::applyCipher(const std::string& inputText,
                                        const CipherMode cipherMode) const
{
    // Create the output string, initially a copy of the input text
    std::string outputText{inputText};

    if (cipherMode == CipherMode::Encrypt) {
        // Change J -> I
        std::transform(std::begin(outputText), std::end(outputText),
                       std::begin(outputText),
                       [](char c) { return (c == 'J') ? 'I' : c; });
        // Find repeated characters and add an X (or a Q for repeated X's)
        for (std::size_t i{0}; i < outputText.size() - 1; i += 2) {
            if (outputText[i] == outputText[i + 1]) {
                char insertChar = (outputText[i] == 'X') ? 'Q' : 'X';
                outputText.insert(i + 1, 1, insertChar);
            }
        }
        // If the size of the input is odd, add a trailing Z
        if (outputText.size() % 2 != 0) {
            outputText += (outputText.back() == 'Z') ? 'X' : 'Z';
        }
        // Loop over the input bigrams
        for (std::size_t i{0}; i < outputText.size() - 1; i += 2) {
            // - Find the coordinates in the grid for each bigram
            auto [row1, column1] = charLookup_.at(outputText[i]);
            auto [row2, column2] = charLookup_.at(outputText[i + 1]);
            // - Apply the rules to these coords to get new coords
            if (row1 == row2) {
                // Same row: shift columns to the right
                column1 = (column1 + 1) % gridSize_;
                column2 = (column2 + 1) % gridSize_;
            } else if (column1 == column2) {
                // Same column: shift rows down
                row1 = (row1 + 1) % gridSize_;
                row2 = (row2 + 1) % gridSize_;
            } else {
                // Rectangle: swap columns
                std::swap(column1, column2);
            }
            // - Find the letters associated with the new coords
            char newChar1 = coordLookup_.at({row1, column1});
            char newChar2 = coordLookup_.at({row2, column2});
            // - Make the replacements
            outputText[i] = newChar1;
            outputText[i + 1] = newChar2;
        }
        
    } else {
        for (std::size_t i{0}; i < outputText.size() - 1; i += 2) {
            // - Find the coordinates in the grid for each bigram
            auto [row1, column1] = charLookup_.at(outputText[i]);
            auto [row2, column2] = charLookup_.at(outputText[i + 1]);
            // - Apply the rules to these coords to get new coords
            if (row1 == row2) {
                // Same row: shift columns to the right
                column1 = (gridSize_ + column1 - 1) % gridSize_;
                column2 = (gridSize_ + column2 - 1) % gridSize_;
            } else if (column1 == column2) {
                // Same column: shift rows down
                row1 = (gridSize_ + row1 - 1) % gridSize_;
                row2 = (gridSize_ + row2 - 1) % gridSize_;
            } else {
                // Rectangle: swap columns
                std::swap(column1, column2);
            }
            // - Find the letters associated with the new coords
            char newChar1 = coordLookup_.at({row1, column1});
            char newChar2 = coordLookup_.at({row2, column2});
            // - Make the replacements
            outputText[i] = newChar1;
            outputText[i + 1] = newChar2;
        }
    }

    // Return the output text
    return outputText;
}
