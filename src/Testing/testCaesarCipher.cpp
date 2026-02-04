//! Unit Tests for MPAGSCipher CaesarCipher Class
#include "gtest/gtest.h"

#include "CaesarCipher.hpp"

TEST(CaesarCipher, Encrypt)
{
    CaesarCipher cc{10};
    EXPECT_EQ(cc.applyCipher("HELLOWORLD", CipherMode::Encrypt), "ROVVYGYBVN");
}

TEST(CaesarCipher, Decrypt)
{
    CaesarCipher cc{10};
    EXPECT_EQ(cc.applyCipher("ROVVYGYBVN", CipherMode::Decrypt), "HELLOWORLD");
}
