#ifndef BUILTINS_H
#define BUILTINS_H

#include "eval_env.h"
#include "value.h"

using BuiltinFunc = ValuePtr(const std::vector<ValuePtr>& args, EvalEnv& env);

// 核心库
ValuePtr applyFunc(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr display(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr displayln(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr error(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr evalFunc(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr exitFunc(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr newline(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr print(const std::vector<ValuePtr>& args, EvalEnv& env);

// 类型检查库
ValuePtr isAtom(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr isBoolean(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr isInteger(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr isList(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr isNumber(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr isNull(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr isPair(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr isProcedure(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr isString(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr isSymbol(const std::vector<ValuePtr>& args, EvalEnv& env);

// 列表操作库
ValuePtr append(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr car(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr cdr(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr cons(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr length(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr list(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr mapFunc(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr filter(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr reduce(const std::vector<ValuePtr>& args, EvalEnv& env);

// 算术运算库
ValuePtr add(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr subtract(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr multiply(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr divide(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr absFunc(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr expt(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr quotient(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr modulo(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr remainderFunc(const std::vector<ValuePtr>& args, EvalEnv& env);

// 比较库
ValuePtr eqFunc(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr equalFunc(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr notFunc(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr numEqual(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr lessThan(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr greaterThan(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr lessOrEqual(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr greaterOrEqual(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr evenPred(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr oddPred(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr zeroPred(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr memqFunc(const std::vector<ValuePtr>& args, EvalEnv& env);
#endif  // BUILTINS_H
