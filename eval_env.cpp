#include "eval_env.h"

#include <optional>

#include "builtins.h"
#include "error.h"
#include "forms.h"

std::shared_ptr<EvalEnv> EvalEnv::createGlobal() {
    auto env = std::shared_ptr<EvalEnv>(new EvalEnv());
    env->initializeBuiltins();
    return env;
}

std::shared_ptr<EvalEnv> EvalEnv::createChild() {
    // 使用 new 而不是 make_shared
    return std::shared_ptr<EvalEnv>(new EvalEnv(shared_from_this()));
}

EvalEnv::EvalEnv() : parent_(nullptr) {
    // 内置函数在createGlobal()中初始化
}

EvalEnv::EvalEnv(const std::shared_ptr<EvalEnv>& parent) : parent_(parent) {
    // 子环境初始化逻辑
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

    symbolTable_[">"] = std::make_shared<BuiltinProcValue>(greaterThan, ">");
    symbolTable_["="] = std::make_shared<BuiltinProcValue>(&numEqual, "=");
    symbolTable_["<"] = std::make_shared<BuiltinProcValue>(&lessThan, "<");
    symbolTable_["<="] = std::make_shared<BuiltinProcValue>(&lessOrEqual, "<=");
    symbolTable_[">="] =
        std::make_shared<BuiltinProcValue>(&greaterOrEqual, ">=");
    symbolTable_["apply"] = std::make_shared<BuiltinProcValue>(&applyFunc, "apply");
    symbolTable_["displayln"] =
        std::make_shared<BuiltinProcValue>(&displayln, "displayln");
    symbolTable_["atom?"] =
        std::make_shared<BuiltinProcValue>(&isAtom, "atom?");
    symbolTable_["integer?"] =
        std::make_shared<BuiltinProcValue>(&isInteger, "integer?");
    symbolTable_["map"] = std::make_shared<BuiltinProcValue>(&mapFunc, "map");
    symbolTable_["filter"] =
        std::make_shared<BuiltinProcValue>(&filter, "filter");
    symbolTable_["eq?"] = std::make_shared<BuiltinProcValue>(&eqFunc, "eq?");
    symbolTable_["equal?"] =
        std::make_shared<BuiltinProcValue>(&equalFunc, "equal?");
    symbolTable_["not"] = std::make_shared<BuiltinProcValue>(&notFunc, "not");
    symbolTable_["even?"] =
        std::make_shared<BuiltinProcValue>(&evenPred, "even?");
    symbolTable_["odd?"] = std::make_shared<BuiltinProcValue>(&oddPred, "odd?");
    symbolTable_["zero?"] =
        std::make_shared<BuiltinProcValue>(&zeroPred, "zero?");
    
    symbolTable_["abs"] =
        std::make_shared<BuiltinProcValue>(&absFunc, "abs");  // 绝对值
    symbolTable_["expt"] =
        std::make_shared<BuiltinProcValue>(&expt, "expt");  // 指数
    symbolTable_["quotient"] =
        std::make_shared<BuiltinProcValue>(&quotient, "quotient");  // 整数除法
    symbolTable_["modulo"] =
        std::make_shared<BuiltinProcValue>(&modulo, "modulo");  // 模运算
    symbolTable_["remainder"] =
        std::make_shared<BuiltinProcValue>(&remainderFunc, "remainder");  // 余数
    symbolTable_["exit"] = std::make_shared<BuiltinProcValue>(exitFunc, "exit");
    symbolTable_["append"] =
        std::make_shared<BuiltinProcValue>(&append, "append");
    symbolTable_["reduce"] =
        std::make_shared<BuiltinProcValue>(&reduce, "reduce");
    symbolTable_["error"] = std::make_shared<BuiltinProcValue>(&error, "error");
    symbolTable_["memq"] =
        std::make_shared<BuiltinProcValue>(&memqFunc, "memq");
    symbolTable_["eval"] = std::make_shared<BuiltinProcValue>(&evalFunc, "eval");
}

ValuePtr EvalEnv::lookup(const std::string& name) {
    // 在当前环境查找
    auto it = symbolTable_.find(name);
    if (it != symbolTable_.end()) {
        return it->second;
    }

    // 如果在父环境中查找
    if (parent_) {
        return parent_->lookup(name);
    }

    throw LispError("Variable " + name + " not defined.");
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
        return this->lookup(*name);
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
        if (auto firstSym = list[0]->asSymbol()) {
            auto it = SPECIAL_FORMS.find(*firstSym);
            if (it != SPECIAL_FORMS.end()) {
                // 移除特殊形式符号，处理剩余参数
                std::vector<ValuePtr> formArgs(list.begin() + 1, list.end());
                return it->second(formArgs, *this);
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
        return builtin->getFunc()(args, *this);
    }
    if (auto lambda = dynamic_cast<LambdaValue*>(proc.get())) {
        return lambda->apply(args, *this);
    }

    throw LispError("Unsupported procedure type: " + proc->toString());
}

void EvalEnv::defineBinding(const std::string& name, ValuePtr value) {
    symbolTable_[name] = value;
}


