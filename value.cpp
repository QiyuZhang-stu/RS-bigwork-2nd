#include "value.h"

#include <cmath>
#include <iomanip>
#include <sstream>

#include "eval_env.h"

Value::operator std::vector<ValuePtr>() const {
    if (this->isList()) {
        return this->toVector();
    }
    throw std::runtime_error("Cannot convert non-list value to vector");
}

// 基类默认实现
bool Value::getValue() const {
    throw LispError("Value is not a boolean");
}

bool Value::isSymbol() const {
    return false;
}

// ===== BooleanValue实现 =====
BooleanValue::BooleanValue(bool value) : value_(value) {}

std::string BooleanValue::toString() const {
    return value_ ? "#t" : "#f";
}

std::string BooleanValue::getType() const {
    return "boolean";
}

bool BooleanValue::isSelfEvaluating() const {
    return true;
}

bool BooleanValue::isNil() const {
    return false;
}

bool BooleanValue::isBoolean() const {
    return true;
}

bool BooleanValue::getValue() const {
    return value_;
}

bool BooleanValue::isSymbol() const {
    return false;
}

bool BooleanValue::isTrue() const {
    return false;
}

bool BooleanValue::operator==(const Value& other) const {
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

bool BooleanValue::isString() const {
    return false;
}

bool BooleanValue::isProcedure() const {
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

std::string NumericValue::getType() const {
    return "number";
}

bool NumericValue::isSelfEvaluating() const {
    return true;
}

bool NumericValue::isNil() const {
    return false;
}

bool NumericValue::isBoolean() const {
    return false;
}

bool NumericValue::getValue() const {
    throw LispError("Numeric value is not a boolean");
}

bool NumericValue::isSymbol() const {
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

bool NumericValue::isString() const {
    return false;
}

bool NumericValue::isProcedure() const {
    return false;
}

const std::string& NumericValue::getString() const {
    throw LispError("Number is not a string");
}

double NumericValue::getNumberValue() const {
    return value_;
}

bool NumericValue::isTrue() const {
    return false;
}

bool NumericValue::operator==(const Value& other) const {
    if (auto num = dynamic_cast<const NumericValue*>(&other)) {
        return value_ == num->value_;
    }
    return false;
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

std::string StringValue::getType() const {
    return "string";
}

bool StringValue::isSelfEvaluating() const {
    return true;
}

bool StringValue::isNil() const {
    return false;
}

bool StringValue::isBoolean() const {
    return false;
}

bool StringValue::getValue() const {
    throw LispError("String value is not a boolean");
}

bool StringValue::isSymbol() const {
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

bool StringValue::isString() const {
    return true;
}

bool StringValue::isProcedure() const {
    return false;
}

const std::string& StringValue::getString() const {
    return value_;
}

const std::string& StringValue::getStringValue() const {
    return value_;
}

bool StringValue::isTrue() const {
    return false;
}

bool StringValue::operator==(const Value& other) const {
    if (auto str = dynamic_cast<const StringValue*>(&other)) {
        return value_ == str->value_;
    }
    return false;
}

// ===== NilValue实现 =====
NilValue::NilValue() = default;

std::string NilValue::toString() const {
    return "()";
}

std::string NilValue::getType() const {
    return "nil";
}

bool NilValue::isSelfEvaluating() const {
    return false;
}

bool NilValue::isNil() const {
    return true;
}

bool NilValue::isBoolean() const {
    return false;
}

bool NilValue::getValue() const {
    throw LispError("Nil value is not a boolean");
}

bool NilValue::isSymbol() const {
    return false;
}

bool NilValue::isTrue() const {
    return false;
}

bool NilValue::operator==(const Value& other) const {
    return false;
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

bool NilValue::isString() const {
    return false;
}

bool NilValue::isProcedure() const {
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

std::string SymbolValue::getType() const {
    return "symbol";
}

bool SymbolValue::isSelfEvaluating() const {
    return false;
}

bool SymbolValue::isNil() const {
    return false;
}

bool SymbolValue::isBoolean() const {
    return false;
}

bool SymbolValue::getValue() const {
    throw LispError("Symbol value is not a boolean");
}

bool SymbolValue::isSymbol() const {
    return true;
}

bool SymbolValue::isTrue() const {
    return false;
}

bool SymbolValue::operator==(const Value& other) const {
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

bool SymbolValue::isString() const {
    return false;
}

bool SymbolValue::isProcedure() const {
    return false;
}

const std::string& SymbolValue::getString() const {
    throw LispError("Symbol is not a string");
}

// ===== PairValue实现 =====
PairValue::PairValue(ValuePtr car, ValuePtr cdr)
    : car_(std::move(car)), cdr_(std::move(cdr)) {}

PairValue::PairValue(const std::vector<ValuePtr>& car_list, ValuePtr cdr) {
    ValuePtr current = cdr;

    // 从右向左构建列表
    for (auto it = car_list.rbegin(); it != car_list.rend(); ++it) {
        current = std::make_shared<PairValue>(*it, current);
    }
    car_ = current;
    cdr_ = cdr;
}

PairValue::PairValue(ValuePtr car)
    : car_(std::move(car)), cdr_(std::make_shared<NilValue>()) {}

PairValue::PairValue()
    : car_(std::make_shared<NilValue>()), cdr_(std::make_shared<NilValue>()) {}

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

std::string PairValue::getType() const {
    return "pair";
}

bool PairValue::isSelfEvaluating() const {
    return false;
}

bool PairValue::isNil() const {
    return false;
}

bool PairValue::isBoolean() const {
    return false;
}

bool PairValue::getValue() const {
    throw LispError("Pair value is not a boolean");
}

bool PairValue::isSymbol() const {
    return false;
}

bool PairValue::isTrue() const {
    return false;
}

bool PairValue::operator==(const Value& other) const {
    return false;
}

std::optional<std::string> PairValue::asSymbol() const {
    return std::nullopt;
}

std::vector<ValuePtr> PairValue::toVector() const {
    std::vector<ValuePtr> result;
    const Value* current = this;
    while (const auto* pair = dynamic_cast<const PairValue*>(current)) {
        result.push_back(pair->car_);
        current = pair->cdr_.get();
    }
    if (!current->isNil()) throw LispError("Malformed list.");
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

bool PairValue::isString() const {
    return false;
}

bool PairValue::isProcedure() const {
    return false;
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

std::string BuiltinProcValue::getType() const {
    return "builtin-procedure";
}

bool BuiltinProcValue::isSelfEvaluating() const {
    return true;
}

bool BuiltinProcValue::isNil() const {
    return false;
}

bool BuiltinProcValue::isBoolean() const {
    return false;
}

bool BuiltinProcValue::getValue() const {
    throw LispError("Procedure value is not a boolean");
}

bool BuiltinProcValue::isSymbol() const {
    return false;
}

bool BuiltinProcValue::isTrue() const {
    return false;
}

bool BuiltinProcValue::operator==(const Value& other) const {
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

bool BuiltinProcValue::isString() const {
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

bool BuiltinProcValue::isProcedure() const {
    return true;
}

// ===== LambdaValue实现 =====
LambdaValue::LambdaValue(std::vector<std::string> params,
                         std::vector<ValuePtr> body,
                         std::shared_ptr<EvalEnv> env)
    : params(std::move(params)), body(std::move(body)), closureEnv(env) {}

std::string LambdaValue::getType() const {
    return "lambda-procedure";
}

bool LambdaValue::isSelfEvaluating() const {
    return false;
}

bool LambdaValue::isNil() const {
    return false;
}

bool LambdaValue::isBoolean() const {
    return false;
}

bool LambdaValue::getValue() const {
    throw LispError("Lambda value is not a boolean");
}

bool LambdaValue::isSymbol() const {
    return false;
}

bool LambdaValue::isTrue() const {
    return false;
}

bool LambdaValue::operator==(const Value& other) const {
    return false;
}

std::optional<std::string> LambdaValue::asSymbol() const {
    return std::nullopt;
}

std::vector<ValuePtr> LambdaValue::toVector() const {
    throw std::runtime_error("procedure cannot be converted to vector");
}

double LambdaValue::asNumber() const {
    throw LispError("procedure is not a number");
}

bool LambdaValue::isNumber() const {
    return false;
}

bool LambdaValue::isList() const {
    return false;
}

bool LambdaValue::isPair() const {
    return false;
}

bool LambdaValue::isString() const {
    return false;
}

const std::string& LambdaValue::getString() const {
    throw LispError("procedure is not a string");
}

bool LambdaValue::isProcedure() const {
    return true;
}

std::string LambdaValue::toString() const {
    return "#<procedure>";
}

ValuePtr LambdaValue::apply(const std::vector<ValuePtr>& args,
                            EvalEnv& callerEnv) {
    // 参数数量检查
    if (args.size() != params.size()) {
        throw LispError("Argument count mismatch. Expected " +
                        std::to_string(params.size()) + " but got " +
                        std::to_string(args.size()));
    }

    // 创建新的求值环境（基于闭包环境）
    auto env = closureEnv->createChild();

    // 绑定参数
    for (size_t i = 0; i < params.size(); i++) {
        env->defineBinding(params[i], args[i]);  // 使用成员变量 params
    }

    // 执行函数体
    ValuePtr result = std::make_shared<NilValue>();
    for (auto& expr : body) {  // 使用成员变量 body
        result = env->eval(expr);
    }
    return result;
}
