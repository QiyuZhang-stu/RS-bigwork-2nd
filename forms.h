#ifndef FORMS_H
#define FORMS_H

#include <functional>

#include "eval_env.h"
#include "value.h"



// 特殊形式类型定义
using SpecialFormType =
    std::function<ValuePtr(const std::vector<ValuePtr>&, EvalEnv&)>;

// 特殊形式映射表
extern const std::unordered_map<std::string, SpecialFormType> SPECIAL_FORMS;

// 特殊形式实现函数
ValuePtr quoteForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr ifForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr andForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr orForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr lambdaForm(const std::vector<ValuePtr>& args, EvalEnv& env);
ValuePtr defineForm(const std::vector<ValuePtr>& args, EvalEnv& env);


#endif  // FORMS_H
