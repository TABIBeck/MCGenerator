# include "mc.hh"

// Takes a number of MCs to be made and one or more text files and trains a
// random text generation algorithm on them

int main (int argc, char* argv[]) {
    if (argc < 3) { // Makes sure the nubmer of mcs to be generated and at least
        // one input file were provided in the command line
        std::cerr << "No Number/File Provided\n";
        return 1;
    }
    unsigned mcNum = atoi(argv[1]);    // First argument is the number of mcs to be made
    std::vector<std::filesystem::path> inputFiles;
        // Vector to hold all the input files
    for (int f = 2; f < argc; ++f) {    // Adds a path to each input file to the inputFiles vector
        std::filesystem::path file {argv[f]}; // puts a path to the file in the heap
        inputFiles.push_back(file); // Puts a unique pointer to it in the list
    }
    Library library = Library();
    for (std::filesystem::path file : inputFiles) {
        // reads the data from each file
        std::ifstream readStream {file};
        if (!readStream) {
            std::cerr << "Invalid File Name: " << file.string() << "\n";
            return 1;
        }
        library.readFile(&readStream);
    }
    std::ofstream outputFile = std::ofstream("newmc.txt");
    for (unsigned i = 1; i <= mcNum; ++i) {
        outputFile << i << ". ";    // Puts the mc number before the MC is printed
        generateMc(&outputFile, &library); // Writes one new MC to the output file
    }
    return 0;
}
