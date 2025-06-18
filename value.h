#pragma once

#include <cmath>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <typeinfo>
#include <vector>

#include "error.h"

class Value;
using ValuePtr = std::shared_ptr<Value>;

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
    virtual const std::string& getString() const = 0;
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
    const std::string& getString() const override;

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
    const std::string& getString() const override;
    double getValue() const;

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
    const std::string& getString() const override;
    const std::string& getValue() const;

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
    const std::string& getString() const override;
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
    const std::string& getString() const override;

private:
    std::string name_;
};

class PairValue : public Value {
public:
    PairValue(ValuePtr car, ValuePtr cdr);
    std::string toString() const override;
    bool isSelfEvaluating() const override;
    bool isNil() const override;
    std::optional<std::string> asSymbol() const override;
    std::vector<ValuePtr> toVector() const override;
    double asNumber() const override;
    bool isNumber() const override;
    bool isList() const override;
    bool isPair() const override;
    const std::string& getString() const override;
    ValuePtr getCar() const;
    ValuePtr getCdr() const;

private:
    ValuePtr car_;
    ValuePtr cdr_;
};

class BuiltinProcValue : public Value {
public:
    using BuiltinFunc = ValuePtr(const std::vector<ValuePtr>&);

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
    const std::string& getString() const override;
    BuiltinFunc* getFunc() const;
    void setName(const std::string& name);

private:
    BuiltinFunc* func_;
    std::string name_;
};
