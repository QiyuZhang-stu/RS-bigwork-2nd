#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include <memory>
#include <string>
#include <unordered_map>

#include "value.h"
#include "builtins.h"

class EvalEnv {
public:
    EvalEnv();
    ValuePtr eval(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);

private:
    std::vector<ValuePtr> evalList(ValuePtr expr);
    void initializeBuiltins();
    std::unordered_map<std::string, ValuePtr> symbolTable_;
};

#endif  // EVAL_ENV_H#pragma once
