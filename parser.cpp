#include "parser.h"

#include "error.h"

/// <summary>
/// /////
/// </summary>
/// <param name="tokens"></param>
Parser::Parser(std::deque<TokenPtr> tokens) : tokens_(std::move(tokens)) {}

ValuePtr Parser::parse() {
    if (tokens_.empty()) {
        throw SyntaxError("Unexpected end of input");
    }

    auto token = popToken();
    switch (token->getType()) {
        case TokenType::BOOLEAN_LITERAL: {
            bool value = static_cast<BooleanLiteralToken&>(*token).getValue();
            return std::make_shared<BooleanValue>(value);
        }
        case TokenType::NUMERIC_LITERAL: {
            double value = static_cast<NumericLiteralToken&>(*token).getValue();
            return std::make_shared<NumericValue>(value);
        }
        case TokenType::STRING_LITERAL: {
            std::string value =
                static_cast<StringLiteralToken&>(*token).getValue();
            return std::make_shared<StringValue>(value);
        }
        case TokenType::IDENTIFIER: {
            if (auto* symToken = dynamic_cast<IdentifierToken*>(token.get())) {
                
                return std::make_shared<SymbolValue>(symToken->getName());
                
            } else {
                throw SyntaxError("Expected identifier token");
                
            }
        }
        case TokenType::QUOTE:
        case TokenType::QUASIQUOTE:  // 修改：BACKQUOTE -> QUASIQUOTE
        case TokenType::UNQUOTE: {   // 修改：COMMA -> UNQUOTE
            const char* symbolName = "";
            switch (token->getType()) {
                case TokenType::QUOTE: symbolName = "quote"; break;
                case TokenType::QUASIQUOTE: symbolName = "quasiquote"; break;
                case TokenType::UNQUOTE: symbolName = "unquote"; break;
                default: break;
            }

            auto quoteSym = std::make_shared<SymbolValue>(symbolName);
            auto quotedValue = parse();
            return buildList({quoteSym, quotedValue});
        }
        case TokenType::LEFT_PAREN: {
            return parseTails();
        }
        default: throw SyntaxError("Unexpected token: " + token->toString());
    }
}

ValuePtr Parser::parseTails() {
    if (lookahead(TokenType::RIGHT_PAREN)) {
        popToken();
        return std::make_shared<NilValue>();
    }

    ValuePtr car = parse();

    if (lookahead(TokenType::DOT)) {
        popToken();
        ValuePtr cdr = parse();

        if (!lookahead(TokenType::RIGHT_PAREN)) {
            throw SyntaxError("Expected ')' after dot expression");
        }
        popToken();

        return std::make_shared<PairValue>(car, cdr);
    }

    ValuePtr cdr = parseTails();
    return std::make_shared<PairValue>(car, cdr);
}

ValuePtr Parser::buildList(const std::vector<ValuePtr>& values) {
    ValuePtr result = std::make_shared<NilValue>();
    for (auto it = values.rbegin(); it != values.rend(); ++it) {
        result = std::make_shared<PairValue>(*it, result);
    }
    return result;
}

TokenPtr Parser::popToken() {
    if (tokens_.empty()) {
        throw SyntaxError("Unexpected end of input");
    }
    TokenPtr token = std::move(tokens_.front());
    tokens_.pop_front();
    return token;
}

TokenType Parser::nextTokenType() const {
    if (tokens_.empty()) {
        throw SyntaxError("Unexpected end of input");
    }
    return tokens_.front()->getType();
}

bool Parser::lookahead(TokenType type) const {
    return !tokens_.empty() && tokens_.front()->getType() == type;
}
