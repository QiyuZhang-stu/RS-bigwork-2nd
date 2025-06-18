// builtins.cpp
#include "builtins.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "error.h"

// 辅助函数
double asNumber(ValuePtr arg) {
    if (auto num = std::dynamic_pointer_cast<NumericValue>(arg)) {
        return num->getValue();
    }
    throw LispError("Expected a number");
}

// ===== 数值操作 =====
ValuePtr add(const std::vector<ValuePtr>& args) {
    double result = 0.0;
    for (const auto& arg : args) {
        result += asNumber(arg);
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr subtract(const std::vector<ValuePtr>& args) {
    if (args.empty()) throw LispError("- requires at least one argument");

    double result = asNumber(args[0]);
    if (args.size() == 1) return std::make_shared<NumericValue>(-result);

    for (size_t i = 1; i < args.size(); i++) {
        result -= asNumber(args[i]);
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr multiply(const std::vector<ValuePtr>& args) {
    double result = 1.0;
    for (const auto& arg : args) {
        result *= asNumber(arg);
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr divide(const std::vector<ValuePtr>& args) {
    if (args.empty()) throw LispError("/ requires at least one argument");

    double result = asNumber(args[0]);
    if (args.size() == 1) return std::make_shared<NumericValue>(1.0 / result);

    for (size_t i = 1; i < args.size(); i++) {
        double divisor = asNumber(args[i]);
        if (divisor == 0) throw LispError("Division by zero");
        result /= divisor;
    }
    return std::make_shared<NumericValue>(result);
}

// ===== 输出操作 =====
ValuePtr print(const std::vector<ValuePtr>& args) {
    for (const auto& arg : args) {
        std::cout << arg->toString() << " ";
    }
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr display(const std::vector<ValuePtr>& args) {
    if (args.empty()) return std::make_shared<NilValue>();

    if (auto str = std::dynamic_pointer_cast<StringValue>(args[0])) {
        std::cout << str->getValue();
    } else {
        std::cout << args[0]->toString();
    }

    if (args.size() > 1) {
        for (size_t i = 1; i < args.size(); i++) {
            std::cout << " " << args[i]->toString();
        }
    }
    return std::make_shared<NilValue>();
}

ValuePtr newline(const std::vector<ValuePtr>& args) {
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

// ===== 类型检查 =====
#define TYPE_PREDICATE(name, type)                                             \
    ValuePtr name(const std::vector<ValuePtr>& args) {                         \
        if (args.size() != 1) throw LispError(#name " requires one argument"); \
        return std::make_shared<BooleanValue>(                                 \
            bool(std::dynamic_pointer_cast<type>(args[0])));                   \
    }

TYPE_PREDICATE(isNumber, NumericValue)
TYPE_PREDICATE(isBoolean, BooleanValue)
TYPE_PREDICATE(isString, StringValue)
TYPE_PREDICATE(isSymbol, SymbolValue)
TYPE_PREDICATE(isList, PairValue)  // 简化实现，实际应检查列表结构
TYPE_PREDICATE(isNull, NilValue)
TYPE_PREDICATE(isPair, PairValue)
TYPE_PREDICATE(isProcedure, BuiltinProcValue)

#undef TYPE_PREDICATE

// ===== 列表操作 =====
ValuePtr car(const std::vector<ValuePtr>& args) {
    if (args.size() != 1) throw LispError("car requires one argument");
    if (auto pair = std::dynamic_pointer_cast<PairValue>(args[0])) {
        return pair->getCar();
    }
    throw LispError("Argument to car must be a pair");
}

ValuePtr cdr(const std::vector<ValuePtr>& args) {
    if (args.size() != 1) throw LispError("cdr requires one argument");
    if (auto pair = std::dynamic_pointer_cast<PairValue>(args[0])) {
        return pair->getCdr();
    }
    throw LispError("Argument to cdr must be a pair");
}

ValuePtr cons(const std::vector<ValuePtr>& args) {
    if (args.size() != 2) throw LispError("cons requires two arguments");
    return std::make_shared<PairValue>(args[0], args[1]);
}

ValuePtr length(const std::vector<ValuePtr>& args) {
    if (args.size() != 1) throw LispError("length requires one argument");

    int count = 0;
    auto current = args[0];
    while (!current->isNil()) {
        if (auto pair = std::dynamic_pointer_cast<PairValue>(current)) {
            count++;
            current = pair->getCdr();
        } else {
            throw LispError("Argument to length must be a list");
        }
    }
    return std::make_shared<NumericValue>(count);
}

ValuePtr list(const std::vector<ValuePtr>& args) {
    ValuePtr result = std::make_shared<NilValue>();
    for (auto it = args.rbegin(); it != args.rend(); ++it) {
        result = std::make_shared<PairValue>(*it, result);
    }
    return result;
}

// ===== 系统操作 =====
ValuePtr exitFunc(const std::vector<ValuePtr>& args) {
    int code = 0;
    if (!args.empty()) {
        code = static_cast<int>(asNumber(args[0]));
    }
    std::exit(code);
}
