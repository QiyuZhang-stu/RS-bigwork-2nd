#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include <memory>
#include <string>
#include <unordered_map>

#include "value.h"

class EvalEnv {
public:
    EvalEnv();
    ValuePtr eval(ValuePtr expr);

private:
    std::unordered_map<std::string, ValuePtr> symbolTable_;
};

#endif  // EVAL_ENV_H#pragma once
