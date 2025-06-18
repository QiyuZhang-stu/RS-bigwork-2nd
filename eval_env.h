#ifndef EVAL_ENV_H
#define EVAL_ENV_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "value.h"

class EvalEnv : public std::enable_shared_from_this<EvalEnv> {
public:
    // 工厂方法 - 安全创建环境实例
    static std::shared_ptr<EvalEnv> createGlobal();
    std::shared_ptr<EvalEnv> createChild();

    // 环境操作
    ValuePtr eval(ValuePtr expr);
    ValuePtr apply(ValuePtr proc, std::vector<ValuePtr> args);
    void defineBinding(const std::string& name, ValuePtr value);
    ValuePtr lookup(const std::string& name);

    // 添加获取共享指针的方法
    std::shared_ptr<EvalEnv> getSharedPtr() {
        return shared_from_this();
    }

    // 禁用拷贝和移动
    EvalEnv(const EvalEnv&) = delete;
    EvalEnv& operator=(const EvalEnv&) = delete;
    EvalEnv(EvalEnv&&) = delete;
    EvalEnv& operator=(EvalEnv&&) = delete;

private:
    // 私有构造函数 - 确保所有环境都通过工厂方法创建
    EvalEnv();                                                 // 根环境构造函数
    explicit EvalEnv(const std::shared_ptr<EvalEnv>& parent);  // 子环境构造函数

    // 辅助方法
    void initializeBuiltins();
    std::vector<ValuePtr> evalList(ValuePtr expr);

    // 环境数据
    std::unordered_map<std::string, ValuePtr> symbolTable_;
    std::shared_ptr<EvalEnv> parent_;
};

#endif  // EVAL_ENV_H


