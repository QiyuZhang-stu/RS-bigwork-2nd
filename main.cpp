#include <iostream>

#include "error.h"
#include "eval_env.h"
#include "forms.h"
#include "parser.h"
#include "rjsj_test.hpp"
#include "tokenizer.h"
#include "value.h"

struct TestCtx {
    // 使用安全的工厂方法创建环境
    std::shared_ptr<EvalEnv> env;

    TestCtx() {
        // 正确创建全局环境
        env = EvalEnv::createGlobal();
    }

    std::string eval(std::string input) {
        try {
            auto tokens = Tokenizer::tokenize(input);
            Parser parser(std::move(tokens));
            auto value = parser.parse();
            auto result = env->eval(value);
            return result->toString();
        } catch (const std::exception& e) {
            return "ERROR: " + std::string(e.what());
        }
    }
};

// 测试框架代码保持不变

int main() {
    // 创建测试环境
    auto ctx = TestCtx();

    // 运行所有测试
    try {
        RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5);
    } catch (const std::exception& e) {
        std::cerr << "测试错误: " << e.what() << std::endl;
    }

    // 创建全局环境
    auto env = EvalEnv::createGlobal();

    // REPL 循环
    while (true) {
        try {
            std::cout << ">>> ";
            std::string line;
            if (!std::getline(std::cin, line)) break;
            if (line.empty()) continue;

            // 特殊命令：退出
            if (line == "exit") {
                std::cout << "再见!" << std::endl;
                break;
            }

            // 特殊命令：重置环境
            if (line == "reset") {
                env = EvalEnv::createGlobal();
                std::cout << "环境已重置" << std::endl;
                continue;
            }

            // 特殊命令：查看符号表
            if (line == "symbols") {
                // 这里只是示例，实际实现需要访问内部符号表
                std::cout << "当前符号: + - * / lambda define" << std::endl;
                continue;
            }

            // 解析并求值
            auto tokens = Tokenizer::tokenize(line);
            Parser parser(std::move(tokens));
            auto value = parser.parse();
            auto result = env->eval(value);

            // 输出结果（忽略 nil 结果）
            if (!result->isNil()) {
                std::cout << result->toString() << std::endl;
            }
        } catch (const SyntaxError& e) {
            std::cerr << "语法错误: " << e.what() << std::endl;
        } catch (const LispError& e) {
            std::cerr << "求值错误: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "未知错误: " << e.what() << std::endl;
        }
    }

    return 0;
}
