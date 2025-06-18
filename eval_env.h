#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include <memory>
#include <string>
#include <unordered_map>

#include "value.h"
#include "builtins.h"

class EvalEnv : public std::enable_shared_from_this<EvalEnv> {
public:
    EvalEnv();
    explicit EvalEnv(const std::shared_ptr<EvalEnv>& parent);
    ValuePtr eval(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    std::shared_ptr<EvalEnv> createSharedFromThis();
    void defineBinding(const std::string& name, ValuePtr value);
    void initializeBuiltins();
    ValuePtr lookup(const std::string& name);
    std::vector<ValuePtr> evalList(ValuePtr expr);

private:
    
    std::unordered_map<std::string, ValuePtr> symbolTable_;
    std::shared_ptr<EvalEnv> parent_;
};

#endif  // EVAL_ENV_H#pragma once
