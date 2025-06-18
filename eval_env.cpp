#include "eval_env.h"

#include <optional>

#include "error.h"
#include "builtins.h"

EvalEnv::EvalEnv() {
    initializeBuiltins();  // 可以在这里初始化一些内置函数
}


void EvalEnv::initializeBuiltins() {
    // 算术运算
    symbolTable_["+"] = std::make_shared<BuiltinProcValue>(add, "+");
    symbolTable_["-"] = std::make_shared<BuiltinProcValue>(subtract, "-");
    symbolTable_["*"] = std::make_shared<BuiltinProcValue>(multiply, "*");
    symbolTable_["/"] = std::make_shared<BuiltinProcValue>(divide, "/");

    // 输出
    symbolTable_["print"] = std::make_shared<BuiltinProcValue>(print, "print");
    symbolTable_["display"] =
        std::make_shared<BuiltinProcValue>(display, "display");
    symbolTable_["newline"] =
        std::make_shared<BuiltinProcValue>(newline, "newline");

    // 类型检查
    symbolTable_["number?"] =
        std::make_shared<BuiltinProcValue>(isNumber, "number?");
    symbolTable_["boolean?"] =
        std::make_shared<BuiltinProcValue>(isBoolean, "boolean?");
    symbolTable_["string?"] =
        std::make_shared<BuiltinProcValue>(isString, "string?");
    symbolTable_["symbol?"] =
        std::make_shared<BuiltinProcValue>(isSymbol, "symbol?");
    symbolTable_["list?"] = std::make_shared<BuiltinProcValue>(isList, "list?");
    symbolTable_["null?"] = std::make_shared<BuiltinProcValue>(isNull, "null?");
    symbolTable_["pair?"] = std::make_shared<BuiltinProcValue>(isPair, "pair?");
    symbolTable_["procedure?"] =
        std::make_shared<BuiltinProcValue>(isProcedure, "procedure?");

    // 列表操作
    symbolTable_["car"] = std::make_shared<BuiltinProcValue>(car, "car");
    symbolTable_["cdr"] = std::make_shared<BuiltinProcValue>(cdr, "cdr");
    symbolTable_["cons"] = std::make_shared<BuiltinProcValue>(cons, "cons");
    symbolTable_["length"] =
        std::make_shared<BuiltinProcValue>(length, "length");
    symbolTable_["list"] = std::make_shared<BuiltinProcValue>(list, "list");

    // 系统操作
    symbolTable_["exit"] = std::make_shared<BuiltinProcValue>(exitFunc, "exit");
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
        if (!expr->isPair()) {
            throw LispError("Expected a list for evaluation");
        }
        
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

                if (auto name = list[1]->asSymbol()) {
                    
                    ValuePtr value = eval(list[2]);
                    symbolTable_[*name] = value;
                    return std::make_shared<NilValue>();
                } else if (auto pair =
                               std::dynamic_pointer_cast<PairValue>(list[1])) {
                    // 处理函数定义 (define (f x) ...)
                    throw LispError("Function definition not implemented");
                } else {
                    throw LispError(
                        "Malformed define: expected symbol as variable name");
                }
            }
        }

       
        ValuePtr proc = eval(list[0]);
        std::vector<ValuePtr> args;
        for (size_t i = 1; i < list.size(); i++) {
            args.push_back(eval(list[i]));
        }

        return apply(proc, args);
    } catch (const std::exception& e) {
        throw LispError(std::string("Evaluation error: ") + e.what());
    }
}


std::vector<ValuePtr> EvalEnv::evalList(ValuePtr expr) {
    std::vector<ValuePtr> result;
    if (expr->isNil()) return result;

    if (auto pair = std::dynamic_pointer_cast<PairValue>(expr)) {
        result.push_back(eval(pair->getCar()));
        auto rest = evalList(pair->getCdr());
        result.insert(result.end(), rest.begin(), rest.end());
    } else {
        throw LispError("Malformed list");
    }
    return result;
}

ValuePtr EvalEnv::apply(ValuePtr proc, std::vector<ValuePtr> args) {
    if (auto builtin = dynamic_cast<BuiltinProcValue*>(proc.get())) {
        // 关键修复：实际调用内置过程
        return builtin->getFunc()(args);
    } else {
        throw LispError("Unsupported procedure type");
    }
}
