#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <sstream>
#include <vector>

class UniqueWordCounterAlgo {
private:
	std::mutex mutex;
	std::unordered_set<std::string> uniqueWords;
private:
	void countUniqueWordsInLine(std::string line) {
		std::unordered_set<std::string> uniqueWordsInLine;
		std::stringstream sstream(line);
		std::string word;

		while (sstream >> word) {
			uniqueWordsInLine.insert(word);
		}

		std::lock_guard<std::mutex> lock(mutex);
		uniqueWords.insert(uniqueWordsInLine.begin(), uniqueWordsInLine.end());
	}
public:
	int countUniqueWordsInFile(std::string file_path) {
		std::ifstream file(file_path);
		if (!file) {
			std::cerr << "Missing file:" << file_path << std::endl;
			return 1;
		}

		try {
			std::string line;
			std::vector<std::thread> threads;
			while (std::getline(file, line)) {
				if (line.empty()) {
					continue;
				}
				threads.emplace_back(&UniqueWordCounterAlgo::countUniqueWordsInLine, this, line);
			}
			file.close();

			for (auto& t : threads) {
				t.join();
			}
			return uniqueWords.size();
		}
		catch (std::exception e) {
			std::cout << e.what();
		}
		if (file.is_open()) {
			file.close();
		}
		return 0;
	}
};

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Missing Argument: <FilePath>" << std::endl;
		return 1;
	}

	std::string file_path = argv[1];

	int uniqueWords = UniqueWordCounterAlgo().countUniqueWordsInFile(file_path);

	std::cout << uniqueWords << std::endl;
}