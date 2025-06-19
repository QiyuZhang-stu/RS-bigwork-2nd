#include "builtins.h"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "error.h"

// ========== 辅助函数 ==========
double asNumber(ValuePtr arg) {
    if (!arg->isNumber()) {
        throw LispError("Expected a number");
    }
    return arg->asNumber();
}

// ========== 核心库 ==========
ValuePtr apply(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) throw LispError("apply requires two arguments");
    auto proc = args[0];
    auto argList = args[1]->toVector();
    return env.apply(proc, argList);
}

ValuePtr display(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.empty()) return std::make_shared<NilValue>();

    if (auto str = std::dynamic_pointer_cast<StringValue>(args[0])) {
        std::cout << str->getStringValue();
    } else {
        std::cout << args[0]->toString();
    }

    return std::make_shared<NilValue>();
}

ValuePtr displayln(const std::vector<ValuePtr>& args, EvalEnv& env) {
    display(args, env);
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr error(const std::vector<ValuePtr>& args, EvalEnv& env) {
    std::string message = "Error";
    if (!args.empty()) {
        message = args[0]->toString();
    }
    throw LispError(message);
}

ValuePtr eval(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("eval requires one argument");
    return env.eval(args[0]);
}

ValuePtr exitFunc(const std::vector<ValuePtr>& args, EvalEnv& env) {
    int code = 0;
    if (!args.empty()) {
        code = static_cast<int>(args[0]->asNumber());
    }
    std::exit(code);
}

ValuePtr newline(const std::vector<ValuePtr>& args, EvalEnv& env) {
    std::cout << std::endl;
    return std::make_shared<NilValue>();
}

ValuePtr print(const std::vector<ValuePtr>& args, EvalEnv& env) {
    for (auto& arg : args) {
        std::cout << arg->toString() << std::endl;
    }
    return std::make_shared<NilValue>();
}

// ========== 类型检查库 ==========
ValuePtr isAtom(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("atom? requires one argument");
    auto value = args[0];
    return std::make_shared<BooleanValue>(
        value->isBoolean() || value->isNumber() || value->isString() ||
        value->isSymbol() || value->isNil());
}

ValuePtr isBoolean(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("boolean? requires one argument");
    return std::make_shared<BooleanValue>(args[0]->isBoolean());
}

ValuePtr isInteger(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("integer? requires one argument");
    if (!args[0]->isNumber()) return std::make_shared<BooleanValue>(false);
    double num = args[0]->asNumber();
    return std::make_shared<BooleanValue>(std::floor(num) == num);
}

ValuePtr isList(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("list? requires one argument");
    return std::make_shared<BooleanValue>(args[0]->isList());
}

ValuePtr isNumber(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("number? requires one argument");
    return std::make_shared<BooleanValue>(args[0]->isNumber());
}

ValuePtr isNull(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("null? requires one argument");
    return std::make_shared<BooleanValue>(args[0]->isNil());
}

ValuePtr isPair(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("pair? requires one argument");
    return std::make_shared<BooleanValue>(args[0]->isPair());
}

ValuePtr isProcedure(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("procedure? requires one argument");
    return std::make_shared<BooleanValue>(args[0]->isProcedure());
}

ValuePtr isString(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("string? requires one argument");
    return std::make_shared<BooleanValue>(args[0]->isString());
}

ValuePtr isSymbol(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("symbol? requires one argument");
    return std::make_shared<BooleanValue>(args[0]->isSymbol());
}

// ========== 列表操作库 ==========
ValuePtr append(const std::vector<ValuePtr>& args, EvalEnv& env) {
    ValuePtr result = std::make_shared<NilValue>();
    std::vector<ValuePtr> elements;

    for (auto& list : args) {
        auto vec = list->toVector();
        elements.insert(elements.end(), vec.begin(), vec.end());
    }

    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        result = std::make_shared<PairValue>(*it, result);
    }
    return result;
}

ValuePtr car(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("car requires one argument");
    if (!args[0]->isPair()) {
        throw LispError("Argument to car must be a pair");
    }
    return args[0]->getCar();
}

ValuePtr cdr(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("cdr requires one argument");
    if (!args[0]->isPair()) {
        throw LispError("Argument to cdr must be a pair");
    }
    return args[0]->getCdr();
}

ValuePtr cons(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) throw LispError("cons requires two arguments");
    return std::make_shared<PairValue>(args[0], args[1]);
}

ValuePtr length(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("length requires one argument");

    int count = 0;
    auto current = args[0];
    while (!current->isNil()) {
        if (current->isPair()) {
            count++;
            current = current->getCdr();
        } else {
            throw LispError("Argument to length must be a list");
        }
    }
    return std::make_shared<NumericValue>(count);
}

ValuePtr list(const std::vector<ValuePtr>& args, EvalEnv& env) {
    ValuePtr result = std::make_shared<NilValue>();
    for (auto it = args.rbegin(); it != args.rend(); ++it) {
        result = std::make_shared<PairValue>(*it, result);
    }
    return result;
}

ValuePtr mapFunc(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) throw LispError("map requires two arguments");

    auto proc = args[0];
    auto listArg = args[1];
    if (!listArg->isList()) {
        throw LispError("Second argument to map must be a list");
    }

    auto elements = listArg->toVector();
    std::vector<ValuePtr> result;

    for (auto& item : elements) {
        auto mapped = env.apply(proc, {item});
        result.push_back(mapped);
    }

    ValuePtr resultList = std::make_shared<NilValue>();
    for (auto it = result.rbegin(); it != result.rend(); ++it) {
        resultList = std::make_shared<PairValue>(*it, resultList);
    }
    return resultList;
}

ValuePtr filter(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) throw LispError("filter requires two arguments");

    auto proc = args[0];
    auto listArg = args[1];
    if (!listArg->isList()) {
        throw LispError("Second argument to filter must be a list");
    }

    auto elements = listArg->toVector();
    std::vector<ValuePtr> result;

    for (auto& item : elements) {
        auto test = env.apply(proc, {item});
        if (!test->isNil() && (!test->isBoolean() || test->getValue())) {
            result.push_back(item);
        }
    }

    ValuePtr resultList = std::make_shared<NilValue>();
    for (auto it = result.rbegin(); it != result.rend(); ++it) {
        resultList = std::make_shared<PairValue>(*it, resultList);
    }
    return resultList;
}

ValuePtr reduce(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) throw LispError("reduce requires two arguments");

    auto proc = args[0];
    auto listArg = args[1];
    if (!listArg->isList()) {
        throw LispError("Second argument to reduce must be a list");
    }

    auto elements = listArg->toVector();
    if (elements.empty()) {
        throw LispError("reduce requires non-empty list");
    }

    ValuePtr result = elements[0];
    for (size_t i = 1; i < elements.size(); i++) {
        result = env.apply(proc, {result, elements[i]});
    }
    return result;
}

// ========== 算术运算库 ==========
ValuePtr add(const std::vector<ValuePtr>& args, EvalEnv& env) {
    double result = 0.0;
    for (const auto& arg : args) {
        result += asNumber(arg);
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr subtract(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.empty()) throw LispError("- requires at least one argument");

    double result = asNumber(args[0]);
    if (args.size() == 1) return std::make_shared<NumericValue>(-result);

    for (size_t i = 1; i < args.size(); i++) {
        result -= asNumber(args[i]);
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr multiply(const std::vector<ValuePtr>& args, EvalEnv& env) {
    double result = 1.0;
    for (const auto& arg : args) {
        result *= asNumber(arg);
    }
    return std::make_shared<NumericValue>(result);
}

ValuePtr divide(const std::vector<ValuePtr>& args, EvalEnv& env) {
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

ValuePtr absFunc(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("abs requires one argument");
    return std::make_shared<NumericValue>(std::abs(asNumber(args[0])));
}

ValuePtr expt(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) throw LispError("expt requires two arguments");
    double base = asNumber(args[0]);
    double exponent = asNumber(args[1]);
    return std::make_shared<NumericValue>(std::pow(base, exponent));
}

ValuePtr quotient(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) throw LispError("quotient requires two arguments");
    double dividend = asNumber(args[0]);
    double divisor = asNumber(args[1]);
    if (divisor == 0) throw LispError("Division by zero");
    return std::make_shared<NumericValue>(std::trunc(dividend / divisor));
}

ValuePtr modulo(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) throw LispError("modulo requires two arguments");
    double a = asNumber(args[0]);
    double b = asNumber(args[1]);

    // 处理IEEE 754负零的特殊情况
    if (b == 0.0) throw LispError("modulo division by zero");

    // 正确实现模运算
    double result = std::fmod(a, b);
    if (b > 0 && result < 0) {
        result += b;
    } else if (b < 0 && result > 0) {
        result += b;
    }

    // 确保结果符号与除数相同
    if (std::signbit(result) != std::signbit(b)) {
        if (std::signbit(b)) {
            result = -std::abs(result);
        } else {
            result = std::abs(result);
        }
    }

    return std::make_shared<NumericValue>(result);
}

ValuePtr remainder(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) throw LispError("remainder requires two arguments");
    double a = asNumber(args[0]);
    double b = asNumber(args[1]);

    // 计算余数
    double result = std::fmod(a, b);

    // 确保结果符号与被除数相同
    if (result != 0.0 && std::signbit(result) != std::signbit(a)) {
        if (b > 0) {
            result += b;
        } else {
            result -= b;
        }
    }

    return std::make_shared<NumericValue>(result);
}

// ========== 比较库 ==========
ValuePtr eqFunc(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) throw LispError("eq? requires two arguments");
    return std::make_shared<BooleanValue>(args[0].get() == args[1].get());
}

ValuePtr equalFunc(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) throw LispError("equal? requires two arguments");
    return std::make_shared<BooleanValue>(args[0]->toString() ==
                                          args[1]->toString());
}

ValuePtr notFunc(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("not requires one argument");
    if (args[0]->isBoolean()) {
        return std::make_shared<BooleanValue>(!args[0]->getValue());
    }
    return std::make_shared<BooleanValue>(args[0]->isNil());
}

ValuePtr numEqual(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2) throw LispError("= requires at least two arguments");
    for (size_t i = 0; i < args.size() - 1; i++) {
        if (asNumber(args[i]) != asNumber(args[i + 1])) {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return std::make_shared<BooleanValue>(true);
}

ValuePtr lessThan(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2) throw LispError("< requires at least two arguments");
    for (size_t i = 0; i < args.size() - 1; i++) {
        if (asNumber(args[i]) >= asNumber(args[i + 1])) {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return std::make_shared<BooleanValue>(true);
}

ValuePtr greaterThan(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2) throw LispError("> requires at least two arguments");
    for (size_t i = 0; i < args.size() - 1; i++) {
        if (asNumber(args[i]) <= asNumber(args[i + 1])) {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return std::make_shared<BooleanValue>(true);
}

ValuePtr lessOrEqual(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2) throw LispError("<= requires at least two arguments");
    for (size_t i = 0; i < args.size() - 1; i++) {
        if (asNumber(args[i]) > asNumber(args[i + 1])) {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return std::make_shared<BooleanValue>(true);
}

ValuePtr greaterOrEqual(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() < 2) throw LispError(">= requires at least two arguments");
    for (size_t i = 0; i < args.size() - 1; i++) {
        if (asNumber(args[i]) < asNumber(args[i + 1])) {
            return std::make_shared<BooleanValue>(false);
        }
    }
    return std::make_shared<BooleanValue>(true);
}

ValuePtr evenPred(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("even? requires one argument");
    double n = asNumber(args[0]);
    return std::make_shared<BooleanValue>(static_cast<int>(n) % 2 == 0);
}

ValuePtr oddPred(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("odd? requires one argument");
    double n = asNumber(args[0]);
    return std::make_shared<BooleanValue>(static_cast<int>(n) % 2 != 0);
}

ValuePtr zeroPred(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) throw LispError("zero? requires one argument");
    double n = asNumber(args[0]);
    return std::make_shared<BooleanValue>(n == 0.0);
}
