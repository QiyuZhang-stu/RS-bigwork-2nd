// builtins.h
#ifndef BUILTINS_H
#define BUILTINS_H

#include "value.h"

// 数值操作
ValuePtr add(const std::vector<ValuePtr>& args);
ValuePtr subtract(const std::vector<ValuePtr>& args);
ValuePtr multiply(const std::vector<ValuePtr>& args);
ValuePtr divide(const std::vector<ValuePtr>& args);

// 输出操作
ValuePtr print(const std::vector<ValuePtr>& args);
ValuePtr display(const std::vector<ValuePtr>& args);
ValuePtr newline(const std::vector<ValuePtr>& args);

// 类型检查
ValuePtr isNumber(const std::vector<ValuePtr>& args);
ValuePtr isBoolean(const std::vector<ValuePtr>& args);
ValuePtr isString(const std::vector<ValuePtr>& args);
ValuePtr isSymbol(const std::vector<ValuePtr>& args);
ValuePtr isList(const std::vector<ValuePtr>& args);
ValuePtr isNull(const std::vector<ValuePtr>& args);
ValuePtr isPair(const std::vector<ValuePtr>& args);
ValuePtr isProcedure(const std::vector<ValuePtr>& args);

// 列表操作
ValuePtr car(const std::vector<ValuePtr>& args);
ValuePtr cdr(const std::vector<ValuePtr>& args);
ValuePtr cons(const std::vector<ValuePtr>& args);
ValuePtr length(const std::vector<ValuePtr>& args);
ValuePtr list(const std::vector<ValuePtr>& args);

// 系统操作
ValuePtr exitFunc(const std::vector<ValuePtr>& args);

#endif  // BUILTINS_H
