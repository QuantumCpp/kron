#include <gtest/gtest.h>
#include "../include/core-hpp/tokenization.hpp"
#include "../include/core-hpp/parsing.hpp"
#include "../include/core-hpp/validator.hpp"
#include "../include/command/command-implementation.hpp"
#include "option/option-implementation.hpp"

class ValidatorSuite : public ::testing::Test {
protected:
  static void SetUpTestSuite() {
    CreatedOptionData();
    CreatedCommandData();
  }
};

TEST_F(ValidatorSuite, PositionalValidation){
  //Caso 1 : List sin posicionales
  std::vector<std::string> input = {"list"};
  auto result_tokenization = tokenization(input);
  auto result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), true);

  input = {"list", "~/.config"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), true);

  input = {"list", "~/.config", "~/Documents/"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), false);
}

TEST_F(ValidatorSuite, AvailableOptionsForCommands){
   //Caso 1 : List sin posicionales
  std::vector<std::string> input = {"list", "--all"};
  auto result_tokenization = tokenization(input);
  auto result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), true);

  input = {"list", "--recursive"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), true);

  input = {"list", "--force"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), true);
}

TEST_F(ValidatorSuite, ConflictOptions){
   //Caso 1 : List sin posicionales
  std::vector<std::string> input = {"--force", "--no-overwrite"};
  auto result_tokenization = tokenization(input);
  auto result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), false);

  input = {"--force", "--skip-existing"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), false);

  input = {"--quiet", "--verbose"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), false);
}

TEST_F(ValidatorSuite, OptionRequieresValue){
   //Caso 1 : List sin posicionales
  std::vector<std::string> input = {"list","--depth", "--recursive"};
  auto result_tokenization = tokenization(input);
  auto result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), false);

  input = {"list","--depth=2", "--recursive"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), true);
}

TEST_F(ValidatorSuite, ValidationTypeDataOption){
   //Caso 1 : List sin posicionales
  std::vector<std::string> input = {"list","--sort=size"};
  auto result_tokenization = tokenization(input);
  auto result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), true);

  input = {"list","--modified-after=2024-01-01"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), true);

  input = {"list","--modified-after=no-es-fecha"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), false);

  input = {"list","--modified-after=2024-13-01"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), false);

  input = {"list","--size-gt=10MB"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), true);

  input = {"list","--size-gt=10XB"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), false);

  input = {"list","--size-gt=MB"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), false);

  input = {"list","--filter=*.cpp"};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), true);


  input = {"list","--filter="};
  result_tokenization = tokenization(input);
  result_parsing = parsing(result_tokenization);
  EXPECT_EQ(ValidationGroupToken(result_parsing), false);

}
