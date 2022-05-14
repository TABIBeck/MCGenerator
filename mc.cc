# include "mc.hh"

static std::random_device rd;  // Static so we don't initialize every time
static std::default_random_engine g(rd());

// Helper function for readFile. Reads one word in from the input file. Counts
// the characters \n and : as their own words. Otherwise a word ends at a whitespace character.
// Characters other than numbers, letters, apostrophes and those listed above are not recorded
std::string readWord(std::istream* file) {
    std::string word = "";
    char c;
    file->get(c);
    while (true) {
        // Since there are many stop conditions, and they all require different
        // action directly after stopping, it's easier to just put them in if's
        // with returns inside and have the while loop go until a return triggers
        if (file->eof()) { // if end of file reached
            // Checks if there is anything in the word. If so, returns the word.
            // If not, returns the end of file symbol
            if (word == "") {
                c = 3;
                word += c;
            }
            return word;
        }
        if (((c == 9) || (c == ' ')) && (word != "")) {   // if character is a tab or space
            // this means the end of the word has been reached and the word
            // can be returned. However, it's important to also check that the word is
            // not empty, because otherwise we might hit double spaces and get blank
            // words.
            return word;
        }
        if ((c == ':') || (c == '\n')) {  // if character is a semicolon or newline
            // Both of these are counted as their own words, but we can't return two words here.
            // So instead we check some things. If there's already things in the word,
            // that means this is being read at the end of something else, so we put the character
            // back in the stream and return the previous word. IF the word is empty, it means
            // we just did that, and we can return the character as it's own word
            if (word == "") {
                word += c;
            } else {
                file->putback(c);    // Returns the semicolon to the stream
            }
            return word;
        }
        c = std::tolower(c);    // Make sure its lower case
        if ((c >= 96) && (c <= 122)) {    // If it's not
            // a special case character, makes sure its something to be copied, otherwise
            // ignores it.
            // Only copies down letters and apostrophes
            word += c;
        }
        file->get(c);  // Loads the next character
    }
}

// Takes an input file stream. Reads each character from it into a string until it hits
// a white space character or a line ender (: or \n, counted as another word).
// After reading a word, adds a bigram of the last two word to its list of bigrams
// and a trigram of the last three to its list of trigrams
void Library::readFile(std::istream* file) {
    std::string prev2;
    std::string prev = "\n";
    std::string current = readWord(file);
    assert(current.length() > 0); // Make sure we actually have a word
    if (current[0] == 3) {  // If the end of the file has been reached
        return;
    }
    bigramVec.push_back(Bigram(prev, current));
    prev2 = prev;
    prev = current;
    current = readWord(file);
    assert(current.length() > 0); // Make sure we actually have a word
    while (current[0] != 3) {   // Needed the previous entry for the start when you can't
        // have a trigram, now can just loop through the rest easily
        bigramVec.push_back(Bigram(prev, current));
        trigramVec.push_back(Trigram(prev2, prev, current));
        prev2 = prev;
        prev = current;
        current = readWord(file);
        assert(current.length() > 0); // Make sure we actually have a word
    }
    return; // Full file analyzed, good to continue
}



// Helper function used by generateMC, generates one word given the two previous ones,
// using the library's bigrams and trigrams. Trigrams are given priority if any of them
// fit
std::string Library::generateWord(std::string prev2, std::string prev1) const {
    std::vector<std::string> options;   // A vector of the potential new words, pulled
        // from trigrams if there are any or bigrams if there aren't
    if (prev2 != "") {  // Doesn't look for trigrams if this is generating the first word
        for (Trigram t : trigramVec) {
            if ((t.getPrev2() == prev2) && (t.getPrev1()) == prev1) {
                options.push_back(t.getFollower());
            }
        }
    }
    if (options.size() < 1) {   // If there are no trigrams we go for bigrams
        for (Bigram b : bigramVec) {
            if (b.getPrev() == prev1) {
                options.push_back(b.getFollower());
            }
        }
    }
    return options[g() % options.size()];   // Selects a random follower from the
        // list of potential followers using the random generator g() defind at top of file
}

// Takes an output stream. Loops looking at the previous two words (or one if there
// is only one) and searching for trigrams with the two as keys. If it finds some it
// picks one randommly. If it doesn't it does the same for bigrams. Does this until it
// Has printed 5-7 new line characters
void generateMc(std::ofstream* file, const Library* lib) {
    unsigned numLines = (g() % 6) + 3;  // MC's have varying line counts from 4-9
    std::string prev2 = "";
    std::string prev1 = "\n";
    std::string current = "";
    unsigned i = 0; // Number of lines printed so far
    while (i < numLines) {
        current = lib->generateWord(prev2, prev1);
        (*file) << current;    // Sends the current word to the file
        if (current == "\n") {  // Marks the line counter if printing a new line character
            ++i;
        } else {
            (*file) << " "; // prints a space between words
        }
        prev2 = prev1;
        prev1 = current;
    }
    (*file) << '\n' << '\n';   // Prints two new lines after the MC
    return;
}
