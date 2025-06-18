#pragma once


#include <memory>
#include <string>
#include <cmath>
#include <typeinfo>

class Value; 

using ValuePtr = std::shared_ptr<Value>; 

class Value {
public:
    virtual ~Value() = default;
    virtual std::string toString() const = 0;
};

class BooleanValue : public Value {
private:
    bool value_;

public:
    explicit BooleanValue(bool value) : value_(value) {}
    std::string toString() const override;
};

class NumericValue : public Value {
private:
    double value_;

public:
    explicit NumericValue(double value) : value_(value) {}
    std::string toString() const override;
};

class StringValue : public Value {
private:
    std::string value_;

public:
    explicit StringValue(std::string value) : value_(std::move(value)) {}
    std::string toString() const override;
};

class NilValue : public Value {
public:
    NilValue() = default;
    std::string toString() const override;
};

class SymbolValue : public Value {
private:
    std::string name_;

public:
    explicit SymbolValue(std::string name) : name_(std::move(name)) {}
    std::string toString() const override;
};

class PairValue : public Value {
private:
    std::shared_ptr<Value> car_;
    std::shared_ptr<Value> cdr_;

public:
    PairValue(std::shared_ptr<Value> car, std::shared_ptr<Value> cdr)
        : car_(std::move(car)), cdr_(std::move(cdr)) {}
    std::string toString() const override;
};
