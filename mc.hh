# include <string>
# include <vector>
# include <iostream>
# include <filesystem>
# include <fstream>
# include <cassert>
# include <random>

// A Bigram holds onto a string for a word and another string for the following word
class Bigram {
public:
    Bigram() = delete;   // You can't create a blank Bigram
    Bigram(std::string prev, std::string follower)
        // Prev is the previous word, follower is the word that followers it
        :prev_(prev), follower_(follower)
        {}
    ~Bigram() = default;
    const std::string getPrev() {return prev_;};
    const std::string getFollower() {return follower_;};
private:
    std::string prev_;
    std::string follower_;
};

// A Trigram holds onto a string for a word, another string for a word, and a third for
// a word that follows the other two
class Trigram {
public:
    Trigram() = delete;   // You can't create a blank Trigram
    Trigram(std::string prev2, std::string prev1, std::string follower)
        // Prev 2 and 1 are the previous two words, follower is the word that follows them
        :prev2_(prev2), prev1_(prev1), follower_(follower)
        {}
    ~Trigram() = default;
    const std::string getPrev2() {return prev2_;};
    const std::string getPrev1() {return prev1_;};
    const std::string getFollower() {return follower_;};
private:
    std::string prev2_;
    std::string prev1_;
    std::string follower_;
};

class Library {
public:
    Library() {};

    // Takes an input file stream. Reads each character from it into a string until it hits
    // a white space character or a line ender (: or \n, counted as another word).
    // After reading a word, adds a bigram of the last two word to its list of bigrams
    // and a trigram of the last three to its list of trigrams
    void readFile(std::istream* file);

    // Helper function used by generateMC, generates one word given the two previous ones,
    // using the library's bigrams and trigrams. Trigrams are given priority if any of them
    // fit
    std::string generateWord(std::string prev2, std::string prev1) const;

private:
    std::vector<Bigram> bigramVec;  // Since these are only ever going to be added to and
        // exhaustively searched, there isn't really a point to making them something faster
        // like a search tree. Still, there might be other options to consider for optimization
    std::vector<Trigram> trigramVec;
};

// Takes an output stream. Loops looking at the previous two words (or one if there
// is only one) and searching for trigrams with the two as keys. If it finds some it
// picks one randommly. If it doesn't it does the same for bigrams. Does this until it
// Has printed 4-9 new line characters. Uses generate word
void generateMc(std::ofstream* file, const Library* lib);
