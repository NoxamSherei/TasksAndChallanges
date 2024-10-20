#include "pch.h"
#include "..\WordCounter\UniqueWordCounterAlgoImpl.cpp"

class UniqueWordCounterAlgoImplTestFixture : public ::testing::Test {
public:
    std::vector<std::string> testFilesPath = {
        "Lorem.txt",
        "Test1.txt",
        "Test2.txt",
        "Test3.txt"
    };
};

TEST_F(UniqueWordCounterAlgoImplTestFixture, TestNam2) {
    std::string path = testFilesPath[0];
    UniqueWordCounterAlgoImpl counter;

    int uniqueWords = counter.countUniqueWordsInFile(path);

    EXPECT_EQ(uniqueWords,224);
}

TEST_F(UniqueWordCounterAlgoImplTestFixture, TestName) {
    std::string path = testFilesPath[1];
    UniqueWordCounterAlgoImpl counter;

    int uniqueWords = counter.countUniqueWordsInFile(path);

    EXPECT_EQ(uniqueWords,10);
}

TEST_F(UniqueWordCounterAlgoImplTestFixture, TestName2SignleThread) {
    std::string path = testFilesPath[2];
    UniqueWordCounterAlgoImpl counter;

    int uniqueWords = counter.countUniqueWordsInFile(path);

    EXPECT_LT(counter.lastRunnedThreads, thread::hardware_concurrency());
    EXPECT_EQ(uniqueWords, 3001);
}

TEST_F(UniqueWordCounterAlgoImplTestFixture, TestName2MultipleThreads) {
    std::string path = testFilesPath[2];
    UniqueWordCounterAlgoImpl counter(1);

    int uniqueWords = counter.countUniqueWordsInFile(path);

    EXPECT_EQ(uniqueWords,3001);
    EXPECT_EQ(counter.lastRunnedThreads, thread::hardware_concurrency());
    ASSERT_TRUE(counter.wordExist("TESTNOX"));
}

TEST_F(UniqueWordCounterAlgoImplTestFixture, TestName2LongWords) {
    std::string path = testFilesPath[3];
    UniqueWordCounterAlgoImpl counter(1);

    int uniqueWords = counter.countUniqueWordsInFile(path);

    EXPECT_EQ(uniqueWords,5);
    EXPECT_LT(counter.lastRunnedThreads, thread::hardware_concurrency());
    ASSERT_TRUE(counter.wordExist("TESTNOX"));
}