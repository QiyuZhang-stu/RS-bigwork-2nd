#include "value.h"

#include <cmath>
#include <iomanip>
#include <sstream>

// ===== Value基类默认实现 =====
bool Value::isNil() const {
    return false;
}

// ===== BooleanValue实现 =====
BooleanValue::BooleanValue(bool value) : value_(value) {}

std::string BooleanValue::toString() const {
    return value_ ? "#t" : "#f";
}

bool BooleanValue::isSelfEvaluating() const {
    return true;
}

bool BooleanValue::isNil() const {
    return false;
}

std::optional<std::string> BooleanValue::asSymbol() const {
    return std::nullopt;
}

std::vector<ValuePtr> BooleanValue::toVector() const {
    throw std::runtime_error("Boolean cannot be converted to vector");
}

double BooleanValue::asNumber() const {
    throw LispError("Boolean is not a number");
}

bool BooleanValue::isNumber() const {
    return false;
}

bool BooleanValue::isList() const {
    return false;
}

bool BooleanValue::isPair() const {
    return false;
}

const std::string& BooleanValue::getString() const {
    throw LispError("Boolean is not a string");
}

// ===== NumericValue实现 =====
NumericValue::NumericValue(double value) : value_(value) {}

std::string NumericValue::toString() const {
    double intPart;
    if (std::modf(value_, &intPart) == 0.0) {
        return std::to_string(static_cast<long>(intPart));
    }

    std::ostringstream oss;
    oss << std::setprecision(15) << value_;
    return oss.str();
}

bool NumericValue::isSelfEvaluating() const {
    return true;
}

bool NumericValue::isNil() const {
    return false;
}

std::optional<std::string> NumericValue::asSymbol() const {
    return std::nullopt;
}

std::vector<ValuePtr> NumericValue::toVector() const {
    throw std::runtime_error("Number cannot be converted to vector");
}

double NumericValue::asNumber() const {
    return value_;
}

bool NumericValue::isNumber() const {
    return true;
}

bool NumericValue::isList() const {
    return false;
}

bool NumericValue::isPair() const {
    return false;
}

const std::string& NumericValue::getString() const {
    throw LispError("Number is not a string");
}

double NumericValue::getValue() const {
    return value_;
}

// ===== StringValue实现 =====
StringValue::StringValue(std::string value) : value_(std::move(value)) {}

std::string StringValue::toString() const {
    std::ostringstream oss;
    oss << '"';
    for (char c : value_) {
        switch (c) {
            case '"': oss << "\\\""; break;
            case '\\': oss << "\\\\"; break;
            default: oss << c;
        }
    }
    oss << '"';
    return oss.str();
}

bool StringValue::isSelfEvaluating() const {
    return true;
}

bool StringValue::isNil() const {
    return false;
}

std::optional<std::string> StringValue::asSymbol() const {
    return std::nullopt;
}

std::vector<ValuePtr> StringValue::toVector() const {
    throw std::runtime_error("String cannot be converted to vector");
}

double StringValue::asNumber() const {
    throw LispError("String is not a number");
}

bool StringValue::isNumber() const {
    return false;
}

bool StringValue::isList() const {
    return false;
}

bool StringValue::isPair() const {
    return false;
}

const std::string& StringValue::getString() const {
    return value_;
}

const std::string& StringValue::getValue() const {
    return value_;
}

// ===== NilValue实现 =====
NilValue::NilValue() = default;

std::string NilValue::toString() const {
    return "()";
}

bool NilValue::isSelfEvaluating() const {
    return false;
}

bool NilValue::isNil() const {
    return true;
}

std::optional<std::string> NilValue::asSymbol() const {
    return std::nullopt;
}

std::vector<ValuePtr> NilValue::toVector() const {
    return {};
}

double NilValue::asNumber() const {
    throw LispError("Nil is not a number");
}

bool NilValue::isNumber() const {
    return false;
}

bool NilValue::isList() const {
    return true;
}

bool NilValue::isPair() const {
    return false;
}

const std::string& NilValue::getString() const {
    throw LispError("Nil is not a string");
}

// ===== SymbolValue实现 =====
SymbolValue::SymbolValue(std::string name) : name_(std::move(name)) {}

std::string SymbolValue::toString() const {
    return name_;
}

bool SymbolValue::isSelfEvaluating() const {
    return false;
}

bool SymbolValue::isNil() const {
    return false;
}

std::optional<std::string> SymbolValue::asSymbol() const {
    return name_;
}

std::vector<ValuePtr> SymbolValue::toVector() const {
    throw std::runtime_error("Symbol cannot be converted to vector");
}

double SymbolValue::asNumber() const {
    throw LispError("Symbol is not a number");
}

bool SymbolValue::isNumber() const {
    return false;
}

bool SymbolValue::isList() const {
    return false;
}

bool SymbolValue::isPair() const {
    return false;
}

const std::string& SymbolValue::getString() const {
    throw LispError("Symbol is not a string");
}

// ===== PairValue实现 =====
PairValue::PairValue(ValuePtr car, ValuePtr cdr)
    : car_(std::move(car)), cdr_(std::move(cdr)) {}

std::string PairValue::toString() const {
    std::ostringstream oss;
    oss << '(' << car_->toString();

    ValuePtr current = cdr_;
    while (true) {
        if (current->isNil()) {
            oss << ')';
            break;
        } else if (auto pair = std::dynamic_pointer_cast<PairValue>(current)) {
            oss << ' ' << pair->getCar()->toString();
            current = pair->getCdr();
        } else {
            oss << " . " << current->toString() << ')';
            break;
        }
    }
    return oss.str();
}

bool PairValue::isSelfEvaluating() const {
    return false;
}

bool PairValue::isNil() const {
    return false;
}

std::optional<std::string> PairValue::asSymbol() const {
    return std::nullopt;
}

std::vector<ValuePtr> PairValue::toVector() const {
    std::vector<ValuePtr> result;
    auto current = std::make_shared<PairValue>(*this);
    while (!current->isNil()) {
        result.push_back(current->car_);
        if (auto pair = std::dynamic_pointer_cast<PairValue>(current->cdr_)) {
            current = pair;
        } else {
            if (!current->cdr_->isNil()) {
                throw std::runtime_error("Improper list");
            }
            break;
        }
    }
    return result;
}

double PairValue::asNumber() const {
    throw LispError("Pair is not a number");
}

bool PairValue::isNumber() const {
    return false;
}

bool PairValue::isList() const {
    return true;
}

bool PairValue::isPair() const {
    return true;
}

const std::string& PairValue::getString() const {
    throw LispError("Pair is not a string");
}

ValuePtr PairValue::getCar() const {
    return car_;
}

ValuePtr PairValue::getCdr() const {
    return cdr_;
}

// ===== BuiltinProcValue实现 =====
BuiltinProcValue::BuiltinProcValue(BuiltinFunc* func, std::string name)
    : func_(func), name_(std::move(name)) {}

std::string BuiltinProcValue::toString() const {
    return "#<procedure>";
}

bool BuiltinProcValue::isSelfEvaluating() const {
    return true;
}

bool BuiltinProcValue::isNil() const {
    return false;
}

std::optional<std::string> BuiltinProcValue::asSymbol() const {
    return std::nullopt;
}

std::vector<ValuePtr> BuiltinProcValue::toVector() const {
    throw std::runtime_error("Procedure cannot be converted to vector");
}

double BuiltinProcValue::asNumber() const {
    throw LispError("Procedure is not a number");
}

bool BuiltinProcValue::isNumber() const {
    return false;
}

bool BuiltinProcValue::isList() const {
    return false;
}

bool BuiltinProcValue::isPair() const {
    return false;
}

const std::string& BuiltinProcValue::getString() const {
    throw LispError("Procedure is not a string");
}

BuiltinProcValue::BuiltinFunc* BuiltinProcValue::getFunc() const {
    return func_;
}

void BuiltinProcValue::setName(const std::string& name) {
    name_ = name;
}
