#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "../token/token.hpp"

class Parser {
    public:
        Parser(std::vector<Token>& tokens);

    private:
        std::vector<Token>& tokens;
};



#endif //PARSER_H