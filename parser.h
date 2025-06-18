#ifndef PARSER_H
#define PARSER_H

#include <deque>
#include <memory>
#include <stdexcept>
#include <vector>

#include "token.h"
#include "value.h"

class Parser {
public:
    Parser(std::deque<TokenPtr> tokens);
    ValuePtr parse();

private:
    ValuePtr parseTails();
    TokenPtr popToken();
    TokenType nextTokenType() const;
    bool lookahead(TokenType type) const;
    ValuePtr buildList(const std::vector<ValuePtr>& values);

    std::deque<TokenPtr> tokens_;
};

#endif  // PARSER_H
