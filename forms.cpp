#include "forms.h"

#include "error.h"

// 特殊形式映射表定义
const std::unordered_map<std::string, SpecialFormType> SPECIAL_FORMS = {
    {"quote", quoteForm}, 
    {"if", ifForm},         
    {"and", andForm},
    {"or", orForm},       
    {"lambda", lambdaForm}, 
    {"define", defineForm}
};



// ================= 特殊形式实现 =================

ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 1) {
        throw LispError("quote requires exactly one argument");
    }
    // 直接返回被引用的表达式（不求值）
    return args[0];
}

ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    // 检查参数数量
    if (args.size() < 2 || args.size() > 3) {
        throw LispError("if requires 2 or 3 arguments");
    }

    // 求值条件表达式
    auto condition = env.eval(args[0]);

    // 只有在布尔值false时视为假
    if (auto boolVal = dynamic_cast<BooleanValue*>(condition.get())) {
        if (boolVal->getValue() == false) {
            return args.size() > 2 ? env.eval(args[2])
                                   : std::make_shared<NilValue>();
        }
    }
    // 其他所有值（包括空表）都视为真
    return env.eval(args[1]);
}

ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    // 空and表达式返回true
    if (args.empty()) {
        return std::make_shared<BooleanValue>(true);
    }

    // 依次求值每个参数
    for (size_t i = 0; i < args.size() - 1; i++) {
        auto value = env.eval(args[i]);

        // 遇到false立即返回false
        if (auto boolVal = dynamic_cast<BooleanValue*>(value.get())) {
            if (boolVal->getValue() == false) {
                return std::make_shared<BooleanValue>(false);
            }
        }
        // 所有其他值都视为真
    }

    // 返回最后一个表达式的值
    return env.eval(args.back());
}

ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    // 空or表达式返回false
    if (args.empty()) {
        return std::make_shared<BooleanValue>(false);
    }

    // 依次求值每个参数
    for (auto& arg : args) {
        auto value = env.eval(arg);

        // 如果值为真则返回
        if (auto boolVal = dynamic_cast<BooleanValue*>(value.get())) {
            if (boolVal->getValue() == true) {
                return value;
            }
        } else {
            // 非false值都视为真
            return value;
        }
    }

    // 所有值都是false
    return std::make_shared<BooleanValue>(false);
}

ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    // 检查参数数量
    if (args.size() < 2) {
        throw LispError("lambda requires at least 2 arguments");
    }

    // 解析参数列表
    auto paramsValue = args[0];
    if (!paramsValue->isList()) {
        throw LispError("Lambda parameter list must be a list");
    }

    std::vector<std::string> params;
    auto paramList = paramsValue->toVector();
    for (auto& param : paramList) {
        if (auto name = param->asSymbol()) {
            params.push_back(*name);
        } else {
            throw LispError("Lambda parameter must be a symbol");
        }
    }

    // 从参数列表之后的所有表达式
    std::vector<ValuePtr> body;
    for (size_t i = 1; i < args.size(); i++) {
        body.push_back(args[i]);
    }

     auto envPtr = env.getSharedPtr();  // 使用新增的 getSharedPtr 方法
    auto childEnv = envPtr->createChild();
    return std::make_shared<LambdaValue>(params, body, childEnv);
}

ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env) {
    if (args.size() != 2) {
        throw LispError("define requires exactly 2 arguments");
    }

    // 函数定义：(define (f x) ...)
    if (auto pair = dynamic_cast<PairValue*>(args[0].get())) {
        auto list = args[0]->toVector();
        if (list.empty()) {
            throw LispError("Invalid define form");
        }

        // 获取函数名
        auto funcName = list[0]->asSymbol();
        if (!funcName) {
            throw LispError("Expected function name");
        }

        // 构造参数列表
        std::vector<std::string> params;
        if (list.size() > 1) {
            for (size_t i = 1; i < list.size(); i++) {
                if (auto paramName = list[i]->asSymbol()) {
                    params.push_back(*paramName);
                } else {
                    throw LispError("Function parameter must be a symbol");
                }
            }
        }

        // 构建参数列表 (符号列表)
        ValuePtr paramList = std::make_shared<NilValue>();
        for (auto it = params.rbegin(); it != params.rend(); ++it) {
            auto symbol = std::make_shared<SymbolValue>(*it);
            paramList = std::make_shared<PairValue>(symbol, paramList);
        }

        // 构建 lambda 表达式参数
        auto lambdaArgs = std::vector<ValuePtr>();
        lambdaArgs.push_back(paramList);
        lambdaArgs.push_back(args[1]);

        // 创建 lambda 值
        auto lambda = lambdaForm(lambdaArgs, env);

        // 绑定函数名
        env.defineBinding(*funcName, lambda);
        return std::make_shared<NilValue>();
    }

    // 变量定义：(define x 42)
    if (auto name = args[0]->asSymbol()) {
        auto value = env.eval(args[1]);
        env.defineBinding(*name, value);
        return std::make_shared<NilValue>();
    }

    throw LispError("Invalid define form");
}
