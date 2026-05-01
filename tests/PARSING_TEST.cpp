#include <gtest/gtest.h>
#include "../include/core-hpp/tokenization.hpp"
#include "../include/core-hpp/parsing.hpp"
#include "../include/command/command-implementation.hpp"
#include "option/option-implementation.hpp"


//1. Resolucion de los comandos
TEST(ParsingSuite, CommandResolution){
  CreatedCommandData();
  CreatedOptionData();
  //Caso 1:
  std::vector<std::string> input = {"list"};
  auto result = tokenization(input);
  auto result_parsing = parsing(result);

  EXPECT_EQ(result_parsing.command.name, "list");
  EXPECT_EQ(result_parsing.command.type, TypeToken::COMMAND);
  EXPECT_EQ(GetCommandData(result_parsing.command.name) != nullptr, true);

  //Caso 2:
  input = {"no-existe"};
  result = tokenization(input);
  result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.empty(),true);

  //Caso 3:
  input = {"--all"};
  result = tokenization(input);
  result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.command.name, "");
}

//2. 
TEST(ParsingSuite, LongOptionWithoutEqual){
  CreatedCommandData();
  CreatedOptionData();
  
  //Caso 1:
  std::vector<std::string> input = {"--all"};
  auto result = tokenization(input);
  auto result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.options.front().name, "--all");
  EXPECT_EQ(result_parsing.options.front().value, "");

  //Caso 2:
  input = {"--sort", "size"};
  result = tokenization(input);
  result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.options.front().name, "--sort");
  EXPECT_EQ(result_parsing.options.front().value, "size");

  //Caso 3
  input = {"--sort"};
  result = tokenization(input);
  result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.empty(), true);

  //Caso 4
  input = {"--no-existe"};
  result = tokenization(input);
  result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.empty(), true);
}

//3. 
TEST(ParsingSuite,LongOptionWithEqual){
  CreatedCommandData();
  CreatedOptionData();
  
  //Caso 1:
  std::vector<std::string> input = {"--sort=size"};
  auto result = tokenization(input);
  auto result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.options.front().name, "--sort");
  EXPECT_EQ(result_parsing.options.front().value, "size");

  //Caso 2:
  input = {"--sort=",};
  result = tokenization(input);
  result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.options.front().name, "--sort");
  EXPECT_EQ(result_parsing.options.front().value, "");

  //Caso 3
  input = {"--fake=value"};
  result = tokenization(input);
  result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.empty(), true);
}

//4. 
TEST(ParsingSuite,ShortOption){
  CreatedCommandData();
  CreatedOptionData();
  
  //Caso 1:
  std::vector<std::string> input = {"-a"};
  auto result = tokenization(input);
  auto result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.options.front().name, "--all");
  EXPECT_EQ(result_parsing.options.front().value, "");

  //Caso 2:
  input = {"-l",};
  result = tokenization(input);
  result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.options.front().name, "--long");
  EXPECT_EQ(result_parsing.options.front().value, "");

  //Caso 3
  input = {"-la"};
  result = tokenization(input);
  result_parsing = parsing(result);
  ASSERT_EQ(result_parsing.options.size(), 2);
  EXPECT_EQ(result_parsing.options[0].name, "--long");
  EXPECT_EQ(result_parsing.options[1].name, "--all");

  //Caso 4:
  input = {"-o", "json"};
  result = tokenization(input);
  result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.options.front().name, "--output");
  EXPECT_EQ(result_parsing.options.front().value, "json");
  
  //Caso 5:
  input = {"-o=json"};
  result = tokenization(input);
  result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.options.front().name, "--output");
  EXPECT_EQ(result_parsing.options.front().value, "json");

}

//5. 
TEST(ParsingSuite, Positional){
  CreatedCommandData();
  CreatedOptionData();
  
  //Caso 1:
  std::vector<std::string> input = {"list", "./src"};
  auto result = tokenization(input);
  auto result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.positional.front().name, "./src");

  //Caso 2:
  input = {"list", "a", "b"};
  result = tokenization(input);
  result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.positional.size(), 2);
}

TEST(ParsingSuite, SeeTokenMechanism){
  CreatedCommandData();
  CreatedOptionData();
  
  //Caso 1:
  std::vector<std::string> input = {"--sort", "size"};
  auto result = tokenization(input);
  auto result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.positional.empty(), true);

  //Caso 2:
  input = {"--sort", "size", "size"};
  result = tokenization(input);
  result_parsing = parsing(result);
  EXPECT_EQ(result_parsing.positional.size(), 1);
}
