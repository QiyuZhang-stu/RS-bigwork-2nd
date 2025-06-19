#include <fstream>
#include <iostream>
#include <sstream>

#include "error.h"
#include "eval_env.h"
#include "forms.h"
#include "parser.h"
#include "rjsj_test.hpp"
#include "tokenizer.h"
#include "value.h"

struct TestCtx {
    std::shared_ptr<EvalEnv> env;

    TestCtx() {
        // 使用工厂方法创建全局环境
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

struct ReplMode {
    void run(std::shared_ptr<EvalEnv> env) {
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
    }
};

struct FileMode {
    void run(std::shared_ptr<EvalEnv> env, const std::string& filename) {
        try {
            std::ifstream file(filename);
            if (!file) throw std::runtime_error("无法打开文件: " + filename);

            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string content = buffer.str();

            auto tokens = Tokenizer::tokenize(content);
            Parser parser(std::move(tokens));

            while (auto value = parser.parse()) {
                env->eval(value);  // 文件模式下不输出求值结果
            }
        } catch (const std::exception& e) {
            std::cerr << "文件错误: " << e.what() << std::endl;
        }
    }
};

int main(int argc, char* argv[]) {
    // 创建全局环境
    auto globalEnv = EvalEnv::createGlobal();

    // 测试模式
    if (argc == 1) {
        // 创建测试上下文
        TestCtx ctx;

        // 运行所有测试
        try {
            // 修复图片中的错误：移除多余的逗号
            RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5, Lv5Extra, Lv6, Lv7, Lv7Lib,Sicp);
        } catch (const std::exception& e) {
            std::cerr << "测试错误: " << e.what() << std::endl;
        }

        // 运行 REPL
        ReplMode repl;
        repl.run(globalEnv);
    }
    // 文件模式
    else if (argc == 2) {
        FileMode fileMode;
        fileMode.run(globalEnv, argv[1]);
    }
    // 错误用法
    else {
        std::cerr << "用法: " << argv[0] << " [文件名]" << std::endl;
        return 1;
    }

    return 0;
}
