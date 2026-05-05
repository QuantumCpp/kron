#include <gtest/gtest.h>
#include "../include/core-hpp/tokenization.hpp"
#include "../include/core-hpp/parsing.hpp"
#include "../include/command/command-implementation.hpp"
#include "core-hpp/validator.hpp"
#include "option/option-implementation.hpp"
#include "../include/core-hpp/executor.hpp"

class ExecutorSuite : public ::testing::Test {
protected:
  static void SetUpTestSuite() {
    CreatedOptionData();
    CreatedCommandData();
  }

  std::string CaptureOutput(const std::string& cmd_str) {
    testing::internal::CaptureStdout();
    std::vector<std::string> input = {cmd_str, "--help"};
    auto tokens = tokenization(input);
    auto parsed = parsing(tokens);
    ValidationGroupToken(parsed);
    executor(parsed);
    return testing::internal::GetCapturedStdout();
  }
};

TEST_F(ExecutorSuite, HelpFlagCallsHelpHandler) {
  // 1. Preparar el input
  std::vector<std::string> input = {"list", "--help"};
  
  // 2. Correr el pipeline completo
  auto tokens  = tokenization(input);
  auto parsed  = parsing(tokens);
  ValidationGroupToken(parsed);
  
  // 3. Capturar lo que imprima executor
  testing::internal::CaptureStdout();
  executor(parsed);
  std::string output = testing::internal::GetCapturedStdout();
  
  // 4. Verificar
  EXPECT_FALSE(output.empty());
  EXPECT_NE(output.find("kron list [path]"), std::string::npos);
}

