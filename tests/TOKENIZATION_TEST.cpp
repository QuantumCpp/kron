#include <gtest/gtest.h>
#include "../include/core-hpp/tokenization.hpp"

//1. Identificacion del COMMAND 
//-> El primer token que no comienza con "-" es un comando
TEST(TokenizationSuite, IdentifyCommand){
  //Caso 1:
  std::vector<std::string> input = {"list"};
  auto result = tokenization(input);
  EXPECT_EQ(result[0].type, TypeToken::COMMAND);
  //Caso 2:
  input = {"--all", "list"};
  result = tokenization(input);
  for(const auto& test : result){
    EXPECT_NE(test.type,TypeToken::COMMAND);
  }
  //Caso 3:
  input = {"list", "inspect"};
  result = tokenization(input);
  EXPECT_EQ(result[0].type, TypeToken::COMMAND);
  EXPECT_EQ(result[1].type, TypeToken::LITERAL);

  //Caso 4:
  input = {};
  result = tokenization(input);
  ASSERT_EQ(result.empty(), true);

  //Caso 5:
  input = {""};
  result = tokenization(input);
  ASSERT_EQ(result.empty(), true);
}

//Clasificacion de opciones
TEST(TokenizationSuite, SortOptions){
  //Caso 1:
  std::vector<std::string> input = {"--all"};
  auto result = tokenization(input);
  EXPECT_EQ(result[0].type, TypeToken::OPTION_NOT_NORMALIZED);
  //Caso 2:
  input = {"-a"};
  result = tokenization(input);
  EXPECT_EQ(result[0].type,TypeToken::OPTION_NOT_NORMALIZED);
  //Caso 3:
  input = {"--sort=option"};
  result = tokenization(input);
  EXPECT_EQ(result[0].type, TypeToken::OPTION_NOT_NORMALIZED);

  //Caso 4:
  input = {"-"};
  result = tokenization(input);
  EXPECT_EQ(result[0].type, TypeToken::POSITIONAL);

  //Cas0 5:
  input = {"---triple"};
  result = tokenization(input);
  EXPECT_EQ(result[0].type, TypeToken::OPTION_NOT_NORMALIZED);
}

//El token de separacion
TEST(TokenizationSuite, SeparationToken){
  //Caso 1:
  std::vector<std::string> input = {"list", "--", "--all"};
  auto result = tokenization(input);
  EXPECT_EQ(result[0].type, TypeToken::COMMAND);
  EXPECT_EQ(result[1].type, TypeToken::POSITIONAL);

  //Caso 2:
  input = {"list", "--", "src", "--sort"};
  result = tokenization(input);
  EXPECT_EQ(result[0].type, TypeToken::COMMAND);
  EXPECT_EQ(result[1].type, TypeToken::POSITIONAL);
  EXPECT_EQ(result[2].type, TypeToken::POSITIONAL);

  //Caso 3:
  input = {"--"};
  result = tokenization(input);
  EXPECT_EQ(result.empty(), true);

  //Caso 4:
  input = {"list", "--", "--", "src"};
  result = tokenization(input);
  EXPECT_EQ(result[1].type, TypeToken::POSITIONAL);
}


//Strings vacios y espacios
TEST(TokenizationSuite, EmptyString){
  //Caso 1:
  std::vector<std::string> input = {"list", "--all"};
  auto result = tokenization(input);
  ASSERT_EQ(result.size() == 2, true);

  //Caso 2:
  input = {"", "sort"};
  result = tokenization(input);
  EXPECT_EQ(result[0].name, "sort");
  EXPECT_EQ(result[0].type, TypeToken::COMMAND);

  //Caso 3:
  input = {" "};
  result = tokenization(input);
  EXPECT_EQ(result.empty(), true);
}
