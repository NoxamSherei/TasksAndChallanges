#include "pch.h"
//todo: fix linker issiue
#include <UniqueWordCounterAlgoImpl.cpp>

class UniqueWordAlgoTestFixture : public ::testing::Test {
public:
    std::vector<std::string> testFilesPath = {
        "Lorem.txt",
        "Test1.txt",
        "Test2.txt",
        "Test3.txt"
    };
};

TEST_F(UniqueWordAlgoTestFixture, CheckLoremText) {
    std::string path = testFilesPath[0];
    UniqueWordCounterAlgoImpl counter;

    int _uniqueWords = counter.countUniqueWordsInFile(path);

    EXPECT_EQ(_uniqueWords,224);
}

TEST_F(UniqueWordAlgoTestFixture, CheckApostrophe) {
    std::string path = testFilesPath[1];
    UniqueWordCounterAlgoImpl counter;

    int _uniqueWords = counter.countUniqueWordsInFile(path);

    EXPECT_EQ(_uniqueWords,10);
}

TEST_F(UniqueWordAlgoTestFixture, RunLongTextInSingleThread) {
    std::string path = testFilesPath[2];
    UniqueWordCounterAlgoImpl counter;

    int _uniqueWords = counter.countUniqueWordsInFile(path);

    EXPECT_LT(counter.getLastCountOfThreads(), std::thread::hardware_concurrency());
    EXPECT_EQ(_uniqueWords, 3001);
}

TEST_F(UniqueWordAlgoTestFixture, RunLongTextInMultipleThread) {
    std::string path = testFilesPath[2];
    UniqueWordCounterAlgoImpl counter(1);

    int _uniqueWords = counter.countUniqueWordsInFile(path);

    EXPECT_EQ(_uniqueWords,3001);
    EXPECT_EQ(counter.getLastCountOfThreads(), std::thread::hardware_concurrency());
    ASSERT_TRUE(counter.wordExist("TESTNOX"));
}

TEST_F(UniqueWordAlgoTestFixture, RunLongWordsInMultipleThread) {
    std::string path = testFilesPath[3];
    UniqueWordCounterAlgoImpl counter(1);

    int _uniqueWords = counter.countUniqueWordsInFile(path);

    EXPECT_EQ(_uniqueWords,5);
    EXPECT_LT(counter.getLastCountOfThreads(), std::thread::hardware_concurrency());
    ASSERT_TRUE(counter.wordExist("TESTNOX"));
}