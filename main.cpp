#include <iostream>

#include "error.h"
#include "eval_env.h"
#include "parser.h"
#include "rjsj_test.hpp"
#include "tokenizer.h"
#include "value.h"

struct TestCtx {
    EvalEnv env;
    std::string eval(std::string input) {
        try {
            auto tokens = Tokenizer::tokenize(input);
            Parser parser(std::move(tokens));
            auto value = parser.parse();
            auto result = env.eval(value);
            return result->toString();
        } catch (const std::exception& e) {
            return "ERROR: " + std::string(e.what());
        }
    }
};

int main() {
    RJSJ_TEST(TestCtx, Lv2, Lv3,Lv4);

    EvalEnv env;
    while (true) {
        try {
            std::cout << ">>> ";
            std::string line;
            if (!std::getline(std::cin, line)) break;

            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));
            auto value = parser.parse();
            auto result = env.eval(value);
            if (!result->isNil()) {
                std::cout << result->toString() << std::endl;
            }
        } catch (const SyntaxError& e) {
            std::cerr << "Syntax error: " << e.what() << std::endl;
        } catch (const LispError& e) {
            std::cerr << "Evaluation error: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    return 0;
}
