#pragma once


#include <memory>
#include <string>
#include <cmath>
#include <typeinfo>
#include <optional>
#include <vector>
#include "error.h"

class Value; 

using ValuePtr = std::shared_ptr<Value>; 

class Value {
public:
    virtual ~Value() = default;
    virtual std::string toString() const = 0;
    virtual bool isSelfEvaluating() const {
        return false;
    }
    virtual bool isNil() const {
        return false;
    }
    virtual std::optional<std::string> asSymbol() const {
        return std::nullopt;
    }
    virtual std::vector<std::shared_ptr<Value>> toVector() const {
        throw std::runtime_error("Value cannot be converted to vector");
    }
};

class BooleanValue : public Value {
private:
    bool value_;

public:
    explicit BooleanValue(bool value) : value_(value) {}
    bool isSelfEvaluating() const;
    std::string toString() const override;
};

class NumericValue : public Value {
private:
    double value_;

public:
    explicit NumericValue(double value) : value_(value) {}
    bool isSelfEvaluating() const;
    std::string toString() const override;
};

class StringValue : public Value {
private:
    std::string value_;

public:
    explicit StringValue(std::string value) : value_(std::move(value)) {}
    bool isSelfEvaluating() const;
    std::string toString() const override;
};

class NilValue : public Value {
public:
    NilValue() = default;
    bool isNil() const;
    std::string toString() const override;
};

class SymbolValue : public Value {
private:
    std::string name_;

public:
    explicit SymbolValue(std::string name) : name_(std::move(name)) {}
    std::optional<std::string> asSymbol() const;
    std::string toString() const override;
};

class PairValue : public Value {
private:
    std::shared_ptr<Value> car_;
    std::shared_ptr<Value> cdr_;

public:
    PairValue(std::shared_ptr<Value> car, std::shared_ptr<Value> cdr)
        : car_(std::move(car)), cdr_(std::move(cdr)) {}
    std::vector<std::shared_ptr<Value>> toVector() const;
    std::string toString() const override;
};
