#include <gtest/gtest.h>
#include "../include/core-hpp/tokenization.hpp"

//Verifica que un comando simple se divida correctamente
TEST(TokenizationSuite, BasicCommands) {
  std::vector<std::string> input = {"kron", "list", "--all"};
    auto result = tokenization(input);

    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0].value, "kron");
    EXPECT_EQ(result[1].value, "list");
}

// Test de ejemplo: Verifica el manejo de espacios múltiples
TEST(TokenizationSuite, ExtraSpaces) {
  std::vector<std::string> input = {"kron"," ", "","list"};
    auto result = tokenization(input);

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[1].value, "list");
}
