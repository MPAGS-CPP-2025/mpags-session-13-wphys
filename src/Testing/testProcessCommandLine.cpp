//! Unit Tests for MPAGSCipher processCommandLine interface
#include "gtest/gtest.h"

#include "ProcessCommandLine.hpp"

TEST(CommandLine, HelpFoundCorrectly)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine{"mpags-cipher", "--help"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_TRUE(res);
    EXPECT_TRUE(settings.helpRequested);
}

TEST(CommandLine, VersionFoundCorrectly)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine{"mpags-cipher", "--version"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_TRUE(res);
    EXPECT_TRUE(settings.versionRequested);
}

TEST(CommandLine, EncryptModeActivated)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine{"mpags-cipher", "--encrypt"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_TRUE(res);
    EXPECT_EQ(settings.cipherMode, CipherMode::Encrypt);
}

TEST(CommandLine, DecryptModeActivated)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine{"mpags-cipher", "--decrypt"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_TRUE(res);
    EXPECT_EQ(settings.cipherMode, CipherMode::Decrypt);
}

TEST(CommandLine, KeyEnteredWithoutSpecification)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine = {"mpags-cipher", "-k"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_FALSE(res);
}

TEST(CommandLine, KeyEnteredAndSpecified)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine = {"mpags-cipher", "-k", "4"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_TRUE(res);
    EXPECT_EQ(settings.cipherKey, "4");
}

TEST(CommandLine, InputFileWithoutArg)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine = {"mpags-cipher", "-i"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_FALSE(res);
}

TEST(CommandLine, InputFileDeclared)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine = {"mpags-cipher", "-i",
                                              "input.txt"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_TRUE(res);
    EXPECT_EQ(settings.inputFile, "input.txt");
}

TEST(CommandLine, OutputFileWithoutArg)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine = {"mpags-cipher", "-o"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_FALSE(res);
}

TEST(CommandLine, OutputFileDeclared)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine = {"mpags-cipher", "-o",
                                              "output.txt"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_TRUE(res);
    EXPECT_EQ(settings.outputFile, "output.txt");
}

TEST(CommandLine, CipherTypeWithoutArg)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine{"mpags-cipher", "-c"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_FALSE(res);
}

TEST(CommandLine, CipherTypeUnknown)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine{"mpags-cipher", "-c", "rubbish"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_FALSE(res);
}

TEST(CommandLine, CipherTypeCaesar)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine{"mpags-cipher", "-c", "caesar"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_TRUE(res);
    EXPECT_EQ(settings.cipherType, CipherType::Caesar);
}

TEST(CommandLine, CipherTypePlayfair)
{
    ProgramSettings settings;
    const std::vector<std::string> cmdLine{"mpags-cipher", "-c", "playfair"};
    const bool res{processCommandLine(cmdLine, settings)};

    EXPECT_TRUE(res);
    EXPECT_EQ(settings.cipherType, CipherType::Playfair);
}
