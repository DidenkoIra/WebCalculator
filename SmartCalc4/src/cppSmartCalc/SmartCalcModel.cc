
#include "SmartCalcModel.h"
#include <iostream>
#include <list>
#include <cmath>
#include <stack>

namespace s21 {

CalcModel::~CalcModel() {
    ClearExpression();
}

bool CalcModel::GetExpressionList() {
    bool error = false;
    auto pos = Expression_.begin();
    while (!error && pos != Expression_.end()) {
        pos = GetToken(pos, &error);
    }
    return !error;
}

void CalcModel::PrintExpressionList() {
    std::cout << "size:" << ExpressionList_.size() << std::endl;
    auto iter = ExpressionList_.begin();
    while (iter != ExpressionList_.end()) {
        Token tok = *iter;
        std::cout << tok.GetTokenData() << " ";
        iter++;
    }
    std::cout << std::endl;
}

void CalcModel::PrintPolishList() {
    std::cout << "size:" << PolishList_.size() << std::endl;
    auto iter = PolishList_.begin();
    while (iter != PolishList_.end()) {
        Token tok = *iter;
        std::cout << tok.GetTokenData() << " ";
        iter++;
    }
    std::cout << std::endl;
    iter = PolishList_.begin();
    while (iter != PolishList_.end()) {
        Token tok = *iter;
        std::cout << tok.GetPriority() << " ";
        iter++;
    }
    std::cout << std::endl;
    iter = PolishList_.begin();
    while (iter != PolishList_.end()) {
        Token tok = *iter;
        std::cout << tok.GetTokenType() << " ";
        iter++;
    }
    std::cout << std::endl;
}

std::string::iterator CalcModel::GetToken(std::string::iterator pos, bool *error) {
    if (*pos == ' ') {
        ++pos;
    } else if (is_operator(*pos)) {
        pos = EmplaceOperator(pos);
    } else if (is_left_bracket(*pos)) {
        ExpressionList_.emplace_back(*pos, left_bracket_);
        ++pos;
    } else if (is_right_bracket(*pos)) {
        ExpressionList_.emplace_back(*pos, right_bracket_);
        ++pos;
    } else if (is_x(*pos)) {
        ExpressionList_.emplace_back(*pos, x_);
        Paremeterized_ = true;
        ++pos;
    } else if (is_digit(*pos) || *pos  == '.' || *pos == ',') {
        pos = EmplaceNumber(pos, error);
    } else if (is_valid_symbol(*pos)) {
        pos = EmplaceFunction(pos, error);
    } else {
        *error = true;
    }
    return pos;
}

bool CalcModel::is_operator(char symbol) {
    std::string operators = "*+-/^";
    return operators.find(symbol) != operators.npos;
}

bool CalcModel::is_left_bracket(char symbol) {
    return symbol == '(';
}

bool CalcModel::is_right_bracket(char symbol) {
    return symbol == ')';
}

bool CalcModel::is_x(char symbol) {
    return symbol == 'x';
}

bool CalcModel::is_digit(char symbol) {
    std::string valid_symbols = "0123456789";
    return valid_symbols.find(symbol) != valid_symbols.npos;
}

bool CalcModel::is_valid_symbol(char symbol) {
    std::string valid_symbols = "asctlm";
    return valid_symbols.find(symbol) != valid_symbols.npos;
}

std::string::iterator CalcModel::EmplaceNumber(std::string::iterator pos, bool *error) {
    std::string Number = "";
    char prev = ' ';
    bool end = false;
    bool dot = false;
    bool is_correct = true;
    bool exponent = false;
    while (is_correct && !end && pos != Expression_.end()) {
        char symbol = *pos;
        if (is_digit(symbol)) {
            Number.append(1, symbol);
            ++pos;
            prev = ' ';
        } else if (symbol  == '.' || symbol == ',') {
            if (dot || exponent) {
                is_correct = false;
            } else {
                Number.append(1, '.');;
                dot = true;
                ++pos;
                prev = ' ';
            }
        } else if (symbol == 'e' || symbol == 'E') {
            if (exponent) {
                is_correct = false;
            } else {
                Number.append(1, symbol);
                exponent = true;
                ++pos;
                prev = 'e';
            }
        } else if ((symbol == '-' || symbol == '+') && prev == 'e') {
            Number.append(1, symbol);
            ++pos;
            prev = ' ';
        } else {
            end = true;
        }
    }
    *error = !is_correct;
    ExpressionList_.emplace_back(Number, value_);
    return pos;
}

std::string::iterator CalcModel::EmplaceFunction(std::string::iterator pos, bool *error) {
    std::string Function = Expression_.substr(pos-Expression_.begin(), 4);
    if (Function == "asin" || Function == "acos" || Function == "atan" || Function == "sqrt") {
        ExpressionList_.emplace_back(Function, function_);
        pos += 4;
    } else {
        Function = Expression_.substr(pos-Expression_.begin(), 3);
        if (Function == "sin" || Function == "cos" || Function == "tan" || Function == "log") {
            ExpressionList_.emplace_back(Function, function_);
            pos += 3;
        } else if (Function == "mod") {
            ExpressionList_.emplace_back(Function, operator_, 2);
            pos += 3;
        } else if (Expression_.substr(pos-Expression_.begin(), 2) == "ln") {
            ExpressionList_.emplace_back("ln", function_);
            pos += 2;
        } else {
            *error = true;
        }
    }
    return pos;
}

std::string::iterator CalcModel::EmplaceOperator(std::string::iterator pos) {
    int priority = 1;
    TokenType Type = operator_;
    if (*pos == '*' || *pos == '/') {
        priority = 2;
    } else if (*pos == '^') {
        priority = 3;
    } else {
        if (ExpressionList_.size() == 0 || ExpressionList_.back().GetTokenType() == left_bracket_
                                        || ExpressionList_.back().GetTokenType() == operator_) Type = unary_;
    }
    ExpressionList_.emplace_back(*pos, Type, priority);
    ++pos;
    return pos;
}

bool CalcModel::GetPolishList() {
    std::stack<Token> Operators;
    bool error = false;
    for (auto iter = ExpressionList_.begin(); !error && iter != ExpressionList_.end(); iter++) {
        auto CurrentToken = *iter;
        auto CurrentTokenType = CurrentToken.GetTokenType();
        if (CurrentTokenType == value_ || CurrentTokenType == x_) {
            PolishList_.push_back(CurrentToken);
        } else if (CurrentTokenType == left_bracket_
                || CurrentTokenType == function_
                || CurrentTokenType == unary_) {
            Operators.push(CurrentToken);
        } else if (CurrentTokenType == right_bracket_) {
            while (!Operators.empty() && Operators.top().GetTokenType() != left_bracket_) {
                PolishList_.push_back(Operators.top());
                Operators.pop();
            }
            if (Operators.empty()) {
                error = true;
            } else {
                Operators.pop();
            }
        } else if (CurrentTokenType == operator_) {
            while (!Operators.empty() && (Operators.top().GetTokenType() == function_
                                      ||  Operators.top().GetTokenType() == unary_
                                      ||  Operators.top().GetPriority() >= CurrentToken.GetPriority())) {
                PolishList_.push_back(Operators.top());
                Operators.pop();
            }
            Operators.push(CurrentToken);
        }
    }
    while (!Operators.empty()) {
        PolishList_.push_back(Operators.top());
        Operators.pop();
    }
    return !error;
}

double CalcModel::CalcResult(double x, bool *success) {
    double result = 0;
    bool no_error = true;
    std::list<Token> CalcList(PolishList_);
    std::list<Token> Output;
    while (no_error && !CalcList.empty()) {
        auto CurrentToken = CalcList.front();
        CalcList.pop_front();
        if (CurrentToken.GetTokenType() == x_) {
            CurrentToken.SetValueFromX(x);
            Output.push_front(CurrentToken);
        } else if (CurrentToken.GetTokenType() == value_) {
            CurrentToken.SetValueFromData();
            Output.push_front(CurrentToken);
        } else if (CurrentToken.GetTokenType() == unary_) {
            if (Output.empty()) {
                no_error = false;
            } else {
                auto NextToken = Output.front();
                Output.pop_front();
                Output.emplace_front(ChangeSign(CurrentToken, NextToken));
            }
        } else if (CurrentToken.GetTokenType() != function_) {
            if (Output.size() < 2) {
                no_error = false;
            } else {
                auto LeftToken = Output.front();
                Output.pop_front();
                auto RightToken = Output.front();
                Output.pop_front();
                Output.emplace_front(CalcBinaryOperation(CurrentToken, LeftToken, RightToken));
            }
        } else {
            if (Output.empty()) {
                no_error = false;
            } else {
                auto NextToken = Output.front();
                Output.pop_front();
                Output.emplace_front(CalcFunction(CurrentToken, NextToken));
            }
        }
    }
    if (Output.size() != 1) no_error = false;
    *success = no_error;
    result = Output.front().GetValue();
    return result;
}


double CalcModel::CalcBinaryOperation(const Token& Oper, const Token& RightToken, const Token& LeftToken) {
    double result = 0;
    auto oper = Oper.GetTokenData();
    if (oper.substr(0, 1) == "+") {
        result = LeftToken.GetValue() + RightToken.GetValue();
    } else if (oper.substr(0, 1) == "-") {
        result = LeftToken.GetValue()- RightToken.GetValue();
    } else if (oper.substr(0, 1) == "*") {
        result = LeftToken.GetValue() * RightToken.GetValue();
    } else if (oper.substr(0, 1) == "/") {
        result =  LeftToken.GetValue() / RightToken.GetValue();
    } else if (oper.substr(0, 3) == "mod") {
        result = fmod(LeftToken.GetValue(), RightToken.GetValue());
    } else if (oper.substr(0, 1) == "^") {
        result = pow(LeftToken.GetValue(), RightToken.GetValue());
    }
    return result;
}

double CalcModel::ChangeSign(const Token& Func, const Token& CurrentToken) {
    double result = CurrentToken.GetValue();
    if (Func.GetTokenData().substr(0, 1) == "-") result = -result;
    return result;
}

double CalcModel::CalcFunction(const Token& Func, const Token& CurrentToken) {
    double result = 0;
    auto func = Func.GetTokenData();
    if (func.substr(0, 3) == "sin") {
        result = sin(CurrentToken.GetValue());
    } else if (func.substr(0, 4) == "asin") {
        result = asin(CurrentToken.GetValue());
    } else if (func.substr(0, 3) == "cos") {
        result = cos(CurrentToken.GetValue());
    } else if (func.substr(0, 4) == "acos") {
        result = acos(CurrentToken.GetValue());
    } else if (func.substr(0, 3) == "tan") {
        result = tan(CurrentToken.GetValue());
    } else if (func.substr(0, 4) == "atan") {
        result = atan(CurrentToken.GetValue());
    } else if (func.substr(0, 4) == "sqrt") {
        result = sqrt(CurrentToken.GetValue());
    } else if (func.substr(0, 2) == "ln") {
        result = log(CurrentToken.GetValue());
    } else if (func.substr(0, 3) == "log") {
        result = log10(CurrentToken.GetValue());
    }
    return result;
}

void CalcModel::ClearExpression() {
    ExpressionList_.clear();
    PolishList_.clear();
    GraphDataX_.clear();
    GraphDataY_.clear();
    Expression_ = "";
    Paremeterized_ = false;
}

bool CalcModel::CalcX(std::string ExpressionX) {
    bool is_correct = true;
    ValueX_ = 0;
    if (!ExpressionX.empty()) {
        ClearExpression();
        Expression_ = ExpressionX;
        if (GetExpressionList() && !Paremeterized_ && GetPolishList()) {
            ValueX_ = CalcResult(0, &is_correct);
        } else {
            is_correct = false;
        }
    }
    return is_correct;
}

std::string CalcModel::CalcResult(const std::string& Expression, const std::string& ExpressionX) {
    std::string str_result = "Error!";
    if (CalcX(ExpressionX)) {
        ClearExpression();
        Expression_ = Expression;
        if (GetExpressionList() && GetPolishList()) {
            bool is_correct = true;
            double result = CalcResult(ValueX_, &is_correct);
            if (is_correct) str_result = std::to_string(result);
        }
    }
    return str_result;
}

std::string CalcModel::CalcGraphData(const std::string& Expression, double XBegin, double XEnd,
                                                             double YBegin, double YEnd) {
    std::string str_result = "Error!";
    ClearExpression();
    Expression_ = Expression;
    XBegin_ = XBegin;
    XEnd_ = XEnd;
    YBegin_ = YBegin;
    YEnd_ = YEnd;
    bool is_correct = true;
    if (GetExpressionList() && GetPolishList()) {
        CalcResult(0, &is_correct);
       if (is_correct) {
           str_result = Expression_;
           double step = 0.1;
            for (double x = XBegin_; x <= XEnd_; x += step) {
                double result = CalcResult(x, &is_correct);
                if (!std::isnan(result) && !std::isinf(result)) {
                    GraphDataX_.emplace_back(x);
                    GraphDataY_.emplace_back(result);
                }
            }
       }
    }
    if (YBegin_ == YEnd_) {
        YBegin_ -= 1.;
        YEnd_ +=1.;
     }

    return str_result;
}

std::pair<std::vector<double>, std::vector<double>> CalcModel::GetGraphData() {
    return std::pair{GraphDataX_, GraphDataY_};
}

};  //  namespace s21
