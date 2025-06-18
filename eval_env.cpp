#include "eval_env.h"

#include <optional>

#include "error.h"

EvalEnv::EvalEnv() {
    // 可以在这里初始化一些内置函数
}

ValuePtr EvalEnv::eval(ValuePtr expr) {
    // 1. 自求值表达式
    if (expr->isSelfEvaluating()) {
        return expr;
    }

    // 2. 空表
    if (expr->isNil()) {
        throw LispError("Evaluating nil is prohibited.");
    }

    // 3. 符号查找
    if (auto name = expr->asSymbol()) {
        auto it = symbolTable_.find(*name);
        if (it != symbolTable_.end()) {
            return it->second;
        } else {
            throw LispError("Variable " + *name + " not defined.");
        }
    }

    // 4. 处理列表
    try {
        auto list = expr->toVector();
        if (list.empty()) {
            throw LispError("Empty list cannot be evaluated.");
        }

        // 处理特殊形式 define
        if (auto first = list[0]->asSymbol()) {
            if (*first == "define") {
                if (list.size() != 3) {
                    throw LispError(
                        "Malformed define: expected exactly 3 elements");
                }

                auto nameValue = list[1];
                if (auto name = nameValue->asSymbol()) {
                    ValuePtr value = eval(list[2]);
                    symbolTable_[*name] = value;
                    return std::make_shared<NilValue>();  // 返回空表表示成功
                } else {
                    throw LispError(
                        "Malformed define: expected symbol as variable name");
                }
            }
        }

        throw LispError("Unimplemented procedure call");
    } catch (const std::runtime_error& e) {
        throw LispError(e.what());
    }
}
