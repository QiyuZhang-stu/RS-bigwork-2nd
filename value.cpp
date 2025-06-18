#include "value.h"
#include<cmath>
#include <iomanip>
#include <sstream>





// BooleanValue
bool BooleanValue::isSelfEvaluating() const {
    return true;
}

// NumericValue
bool NumericValue::isSelfEvaluating() const {
    return true;
}

// StringValue
bool StringValue::isSelfEvaluating() const {
    return true;
}

// NilValue
bool NilValue::isNil() const {
    return true;
}

// SymbolValue
std::optional<std::string> SymbolValue::asSymbol() const {
    return name_;
}

// PairValue
std::vector<std::shared_ptr<Value>> PairValue::toVector() const {
    std::vector<std::shared_ptr<Value>> result;
    auto current = std::make_shared<PairValue>(*this);
    while (current && !current->isNil()) {
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

// BooleanValue: #t 或 #f

std::string BooleanValue::toString() const {
    return value_ ? "#t" : "#f";
}

// NumericValue: 整数去小数部分，浮点数保留有效数字
std::string NumericValue::toString() const {
    double intPart;
    if (std::modf(value_, &intPart) == 0.0) {
        return std::to_string(static_cast<long>(intPart));
    }

    // 对浮点数使用精确格式化
    std::ostringstream oss;
    oss << std::setprecision(15) << value_;
    return oss.str();
}

// StringValue: 双引号包裹，转义特殊字符
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

// NilValue: 空表表示
std::string NilValue::toString() const {
    return "()";
}

// SymbolValue: 直接返回名称
std::string SymbolValue::toString() const {
    return name_;
}

// PairValue: 递归格式化列表表示
std::string PairValue::toString() const {
    std::ostringstream oss;

    // 处理 car 部分
    oss << '(' << car_->toString();

    // 处理 cdr 部分
    const Value* current = cdr_.get();
    while (true) {
        // 空表结束
        if (dynamic_cast<const NilValue*>(current)) {
            oss << ')';
            break;
        }
        // 继续处理列表
        else if (auto pair = dynamic_cast<const PairValue*>(current)) {
            oss << ' ' << pair->car_->toString();
            current = pair->cdr_.get();
        }
        // 点列表结束
        else {
            oss << " . " << current->toString() << ')';
            break;
        }
    }
    return oss.str();
}
