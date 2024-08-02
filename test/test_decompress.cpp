#include <gtest/gtest.h>
#include "GzipDecompressor.h"
#include "ZstdDecompressor.h"
#include "LocalFileRead.h"


TEST(GzipDecompressorTest, BasicReadTest)
{
    LocalFileReader fileReader("./test_gz_zstd.txt.gz");
    GzipDecompressor decompressor(fileReader);

    char buffer[1024];
    size_t bytesRead = decompressor.read(buffer, sizeof(buffer));
    EXPECT_GT(bytesRead, 0);
    std::string expectedContent = "This is a test file for gzip and zstd compression";
    EXPECT_EQ(std::string(buffer, bytesRead), expectedContent);
}

TEST(ZstdDecompressorTest, BasicReadTest)
{
    LocalFileReader fileReader("./test_gz_zstd.txt.zst");
    ZstdDecompressor decompressor(fileReader);
    char buffer[1024];
    size_t bytesRead = decompressor.read(buffer, sizeof(buffer));
    EXPECT_GT(bytesRead, 0);
    std::string expectedContent = "This is a test file for gzip and zstd compression";
    EXPECT_EQ(std::string(buffer, bytesRead), expectedContent);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
