#pragma once

#include <cmath>
#include <functional>
#include <memory>
#include <new>
#include <optional>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include "error.h"

class Value;
using ValuePtr = std::shared_ptr<Value>;
class EvalEnv;
class LambdaValue;

class Value {
public:
    virtual ~Value() = default;
    virtual std::string toString() const = 0;
    virtual bool isSelfEvaluating() const = 0;
    virtual bool isNil() const = 0;
    virtual std::optional<std::string> asSymbol() const = 0;
    virtual std::vector<ValuePtr> toVector() const = 0;
    virtual double asNumber() const = 0;
    virtual bool isNumber() const = 0;
    virtual bool isList() const = 0;
    virtual bool isPair() const = 0;
    virtual bool isString() const = 0;
    virtual bool isProcedure() const = 0;
    virtual const std::string& getString() const = 0;

    // 新增方法
    virtual bool isBoolean() const = 0;
    virtual bool getValue() const;  // 获取布尔值
    virtual bool isSymbol() const = 0;
    virtual ValuePtr getCar() const {
        throw LispError("Cannot get car of non-pair value");
    }

    virtual ValuePtr getCdr() const {
        throw LispError("Cannot get cdr of non-pair value");
    }
    operator std::vector<ValuePtr>() const;
};

class BooleanValue : public Value {
public:
    explicit BooleanValue(bool value);
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
    std::optional<std::string> asSymbol() const override;
    std::vector<ValuePtr> toVector() const override;
    double asNumber() const override;
    bool isNumber() const override;
    bool isList() const override;
    bool isPair() const override;
    bool isString() const override;
    bool isProcedure() const override;
    const std::string& getString() const override;

    // 新增方法实现
    bool isBoolean() const override;
    bool getValue() const override;
    bool isSymbol() const override;

private:
    bool value_;
};

class NumericValue : public Value {
public:
    explicit NumericValue(double value);
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
    std::optional<std::string> asSymbol() const override;
    std::vector<ValuePtr> toVector() const override;
    double asNumber() const override;
    bool isNumber() const override;
    bool isList() const override;
    bool isPair() const override;
    bool isString() const override;
    bool isProcedure() const override;
    const std::string& getString() const override;

    // 新增方法实现
    bool isBoolean() const override;
    bool getValue() const override;
    bool isSymbol() const override;
    double getNumberValue() const;

private:
    double value_;
};

class StringValue : public Value {
public:
    explicit StringValue(std::string value);
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
    std::optional<std::string> asSymbol() const override;
    std::vector<ValuePtr> toVector() const override;
    double asNumber() const override;
    bool isNumber() const override;
    bool isList() const override;
    bool isPair() const override;
    bool isString() const override;
    bool isProcedure() const override;
    const std::string& getString() const override;

    // 新增方法实现
    bool isBoolean() const override;
    bool getValue() const override;
    bool isSymbol() const override;
    const std::string& getStringValue() const;

private:
    std::string value_;
};

class NilValue : public Value {
public:
    NilValue();
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
    std::optional<std::string> asSymbol() const override;
    std::vector<ValuePtr> toVector() const override;
    double asNumber() const override;
    bool isNumber() const override;
    bool isList() const override;
    bool isPair() const override;
    bool isString() const override;
    bool isProcedure() const override;
    const std::string& getString() const override;

    // 新增方法实现
    bool isBoolean() const override;
    bool getValue() const override;
    bool isSymbol() const override;
};

class SymbolValue : public Value {
public:
    explicit SymbolValue(std::string name);
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
    std::optional<std::string> asSymbol() const override;
    std::vector<ValuePtr> toVector() const override;
    double asNumber() const override;
    bool isNumber() const override;
    bool isList() const override;
    bool isPair() const override;
    bool isString() const override;
    bool isProcedure() const override;
    const std::string& getString() const override;

    // 新增方法实现
    bool isBoolean() const override;
    bool getValue() const override;
    bool isSymbol() const override;

private:
    std::string name_;
};

class PairValue : public Value, public std::enable_shared_from_this<PairValue> {
public:
    PairValue(ValuePtr car, ValuePtr cdr);
    PairValue(const std::vector<ValuePtr>& car, ValuePtr cdr);
    explicit PairValue(ValuePtr car);
    PairValue();

    std::shared_ptr<PairValue> shared_from_this() {
        return std::enable_shared_from_this<PairValue>::shared_from_this();
    }
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
    std::optional<std::string> asSymbol() const override;
    std::vector<ValuePtr> toVector() const override;
    double asNumber() const override;
    bool isNumber() const override;

    bool isList() const override;
    bool isPair() const override;
    bool isString() const override;
    bool isProcedure() const override;
    const std::string& getString() const override;

    // 新增方法实现
    bool isBoolean() const override;
    bool getValue() const override;
    bool isSymbol() const override;

    ValuePtr getCar() const override;
    ValuePtr getCdr() const;

private:
    ValuePtr car_;
    ValuePtr cdr_;
};

class BuiltinProcValue : public Value {
public:
    using BuiltinFunc = ValuePtr(const std::vector<ValuePtr>&, EvalEnv&);

    explicit BuiltinProcValue(BuiltinFunc* func,
                              std::string name = "#<procedure>");
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
    std::optional<std::string> asSymbol() const override;
    std::vector<ValuePtr> toVector() const override;
    double asNumber() const override;
    bool isNumber() const override;
    bool isList() const override;
    bool isPair() const override;
    bool isString() const override;
    bool isProcedure() const override;
    const std::string& getString() const override;

    // 新增方法实现
    bool isBoolean() const override;
    bool getValue() const override;
    bool isSymbol() const override;

    BuiltinFunc* getFunc() const;
    void setName(const std::string& name);

private:
    BuiltinFunc* func_;
    std::string name_;
};

class LambdaValue : public Value {
public:
    LambdaValue(std::vector<std::string> params, std::vector<ValuePtr> body,
                std::shared_ptr<EvalEnv> env);

    std::string toString() const override;

    bool isProcedure() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
    std::optional<std::string> asSymbol() const override;
    std::vector<ValuePtr> toVector() const override;
    double asNumber() const override;
    bool isNumber() const override;
    bool isList() const override;
    bool isPair() const override;
    bool isString() const override;
    const std::string& getString() const override;

    // 新增方法实现
    bool isBoolean() const override;
    bool getValue() const override;
    bool isSymbol() const override;
    // 应用函数参数
    ValuePtr apply(const std::vector<ValuePtr>& args, EvalEnv& callerEnv);

private:
    std::vector<std::string> params;
    std::vector<ValuePtr> body;
    std::shared_ptr<EvalEnv> closureEnv;  // 闭包环境
};
