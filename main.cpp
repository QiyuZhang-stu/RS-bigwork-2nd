#include <iostream>

#include "parser.h"
#include "error.h"
#include "rjsj_test.hpp"
#include "tokenizer.h"
#include "value.h"

struct TestCtx {
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        return value->toString();
    }
};

int main() {
    RJSJ_TEST(TestCtx, Lv2, Lv2Only);
    while (true) {
        try {
            std::cout << ">>> ";
            std::string line;
            if (!std::getline(std::cin, line)) break;

            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));
            ValuePtr value = parser.parse();
            std::cout << value->toString() << std::endl;
        } catch (const SyntaxError& e) {
            std::cerr << "Syntax error: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    return 0;
}
