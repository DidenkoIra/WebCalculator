
#ifndef SRC_SMARTCALCQT_SMARTCALCV2_SMARTCALCMODEL_H_
#define SRC_SMARTCALCQT_SMARTCALCV2_SMARTCALCMODEL_H_
#include <iostream>
#include <list>
#include <vector>

namespace s21 {

class CalcModel {
enum TokenType {x_,
                value_,
                operator_,
                function_,
                left_bracket_,
                right_bracket_,
                unary_};

class Token {
 private:
    std::string Data_;
    TokenType Type_;
    int Priority_;
    double Value_;

 public:
    Token(const std::string& Data, TokenType Type): Data_(Data),  Type_(Type), Priority_(0), Value_(0.) {}
    Token(const std::string& Data, TokenType Type, int Priority): Data_(Data),
                                                           Type_(Type),
                                                           Priority_(Priority),
                                                           Value_(0.) {}
    Token(char symbol, TokenType Type): Data_{symbol, 1}, Type_(Type), Priority_(0), Value_(0.) {}
    Token(char symbol, TokenType Type, int Priority): Data_{symbol, 1},
                                                      Type_(Type),
                                                      Priority_(Priority),
                                                      Value_(0.) {}
    explicit Token(double Value): Data_(""), Type_(value_), Priority_(0), Value_(Value) {}
    std::string GetTokenData() const {
        return Data_;
    }
    TokenType GetTokenType() const {
        return Type_;
    }
    int GetPriority() const {
        return Priority_;
    }
    double GetValue() const {
        return Value_;
    }
    void SetValueFromData() {
        char *pos;
        Value_ = std::strtod(Data_.c_str(), &pos);
    }
    void SetValueFromX(double x) {
        Value_ = x;
    }
    ~Token() {}
};

 private:
    std::string Expression_;
    std::list<Token> ExpressionList_;
    std::list<Token> PolishList_;
    std::vector<double> GraphDataX_;
    std::vector<double> GraphDataY_;
    double XBegin_;
    double XEnd_;
    double YBegin_;
    double YEnd_;

    double ValueX_;
    bool Paremeterized_;
    std::string::iterator GetToken(std::string::iterator pos, bool *error);
    bool is_operator(char symbol);
    bool is_left_bracket(char symbol);
    bool is_right_bracket(char symbol);
    bool is_x(char symbol);
    bool is_digit(char symbol);
    bool is_valid_symbol(char symbol);
    std::string::iterator EmplaceOperator(std::string::iterator pos);
    std::string::iterator EmplaceNumber(std::string::iterator pos, bool *error);
    std::string::iterator EmplaceFunction(std::string::iterator pos, bool *error);
    double CalcBinaryOperation(const Token& Oper, const Token& RightToken, const Token& LeftToken);
    double CalcFunction(const Token& Func, const Token& CurrentToken);
    double ChangeSign(const Token& Oper, const Token& CurrentToken);

 public:
    CalcModel():Expression_(""),
                XBegin_(0), XEnd_(0),
                YBegin_(0), YEnd_(0),
                ValueX_(0), Paremeterized_(false) {}
    ~CalcModel();
    void ClearExpression();
    bool GetExpressionList();
    void PrintExpressionList();  //  service
    bool GetPolishList();
    void PrintPolishList();  //  service
    double CalcResult(double x, bool *success);
    bool CalcX(std::string ExpressionX);
    std::string CalcResult(const std::string& Expression, const std::string& ExpressionX);
    std::string CalcGraphData(const std::string&, double XBegin, double XEnd, double YBegin, double YEnd);
    std::pair<std::vector<double>, std::vector<double>> GetGraphData();
    double GetXBegin() const {return XBegin_;}
    double GetYBegin() const {return YBegin_;}
    double GetXEnd() const {return XEnd_;}
    double GetYEnd() const {return YEnd_;}
    std::pair<double, double> GetYRange();
};
};    //  namespace s21

#endif  //  SRC_SMARTCALCQT_SMARTCALCV2_SMARTCALCMODEL_H_
