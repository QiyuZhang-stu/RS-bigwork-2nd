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

bool BooleanValue::getValue() const {
    return value_;
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

bool NumericValue::isProcedure() const {
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

bool StringValue::isProcedure() const {
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

bool BuiltinProcValue::isProcedure() const {
    return true;
}


LambdaValue::LambdaValue(std::vector<std::string> params,
                         std::vector<ValuePtr> body,
                         std::shared_ptr<EvalEnv> env)
    : params(std::move(params)), body(std::move(body)), closureEnv(env) {}

bool LambdaValue::isSelfEvaluating() const {
    return false;
}
bool LambdaValue::isNil() const {
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
    auto env = std::make_shared<EvalEnv>(closureEnv);

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
