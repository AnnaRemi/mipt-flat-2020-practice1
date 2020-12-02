#include <string>
#include <stack>
#include <vector>
#include <iostream>

using std::cin;
using std::cout;
using std::count;
using std::min;
using std::exception;
using std::vector;
using std::pair;
using std::stack;
using std::string;

typedef unsigned long long ulong;

static const char alphabet [] = {'a', 'b', 'c', '1'};
static const char operations [] = {'.', '*', '+'};
static const char unary_operator [] = {'*'};
static const char epsilon = '1';

const size_t INF = 1000*1000*1000;

class CurrentSituation {
public:
    ulong MinLen = 0;
    vector<ulong> MinLenPrefix;

    CurrentSituation(ulong min_word_length,  ulong degree): MinLen(min_word_length),
                                                            MinLenPrefix(degree + 1, INF) {}
    CurrentSituation() = default;

    CurrentSituation(const CurrentSituation&) = default;
    CurrentSituation& operator=(const CurrentSituation&) = default;
    CurrentSituation operator*(const CurrentSituation & other);
    CurrentSituation operator+(const CurrentSituation & other);
    void StarFunc();
    ulong &operator[](size_t ind);
};

CurrentSituation CurrentSituation::operator+(const CurrentSituation & other) {
    CurrentSituation result = other;

    for (size_t i = 1; i < result.MinLenPrefix.size(); ++i) {
        result.MinLenPrefix[i] =
                std::min(MinLenPrefix[i],
                         result.MinLenPrefix[i]);
    }

    result.MinLen = std::min(result.MinLen, MinLen);
    return result;
}

CurrentSituation CurrentSituation::operator*(const CurrentSituation & other) {
    CurrentSituation result = other;
    result.MinLen += MinLen;

    for (ulong& length : result.MinLenPrefix) {
        if (length != INF)
            length += MinLen;
    }

    for (size_t i = 1; i < MinLenPrefix.size(); ++i) {
        if (MinLenPrefix[i] == i) {
            for (size_t j = 1; i + j < other.MinLenPrefix.size() ; ++j) {
                if (i != INF && other.MinLenPrefix[j] != INF) {
                    result.MinLenPrefix[i + j] =
                            std::min(result.MinLenPrefix[i + j],
                                     i + other.MinLenPrefix[j]);
                }

            }
        }
    }
    return result;
}

void CurrentSituation::StarFunc() {
    MinLen = 0;

    for (size_t i = 1; i < MinLenPrefix.size(); ++i) {
        if (MinLenPrefix[i] == i) {
            for (size_t j = i; j < MinLenPrefix.size(); j += i)
                MinLenPrefix[j] = j;
        }
    }

    for (ulong i = 1; i < MinLenPrefix.size(); ++i) {
        if (MinLenPrefix[i] == i) {
            for (ulong j = 1; j < MinLenPrefix.size(); ++j) {
                size_t shift = (j % i != 0) ? 1 : 0;
                MinLenPrefix[j] = min(MinLenPrefix[j], i * (j / i + shift));
            }
        }
    }
}

ulong& CurrentSituation::operator[](size_t ind) {
    return MinLenPrefix[ind];
}


class SomethingWentWrong : public exception {
public:
    string error;
    explicit SomethingWentWrong(string error) : error(std::move(error)) {}

    const char * what() const noexcept override {
        return error.c_str(); }
};


class Parser {
public:
    string regexp_;

    static CurrentSituation GetOperationResult(stack<CurrentSituation>& st, char operator_symbol) ;
    explicit Parser(std::string_view regexp) : regexp_(regexp) {}
    ulong FindAnswer(char letter_to_find, ulong degree) const;

};

CurrentSituation Parser::GetOperationResult(stack<CurrentSituation>& st, char operator_symbol) {
    CurrentSituation result;
    size_t operator_valence = (count(std::begin(unary_operator), std::end(unary_operator), operator_symbol)) ? 1 : 2;

    if (st.size() < operator_valence)  {
        throw SomethingWentWrong("Invalid regexp");

    }else {
        if (operator_symbol == '+') {
            CurrentSituation rhs = st.top();
            st.pop();
            CurrentSituation lhs = st.top();
            st.pop();

            result = lhs + rhs;

        }else if (operator_symbol == '.') {
            CurrentSituation rhs = st.top();
            st.pop();
            CurrentSituation lhs = st.top();
            st.pop();

            result = lhs * rhs;

        }else if (operator_symbol == '*') {
            result = st.top();
            st.pop();

            result.StarFunc();
        }
    }
    return result;
}

ulong Parser::FindAnswer(char letter_to_find, ulong degree) const {
    stack<CurrentSituation> regexp_processing;

    for (char symbol : regexp_) {

        if (count(std::begin(alphabet), std::end(alphabet), symbol) != 0) { //if the symbol is a letter
            regexp_processing.emplace(((symbol == epsilon) ? 0 : 1), degree);

            if (symbol == letter_to_find)
                regexp_processing.top()[1] = 1;

        } else if (count(std::begin(operations), std::end(operations), symbol) != 0) { //if the symbol is an operator
            regexp_processing.push(GetOperationResult(regexp_processing, symbol));
        } else
            throw SomethingWentWrong("Symbol is not in the alphabet");
    }

    if (regexp_processing.size() > 1)
        throw SomethingWentWrong("Invalid regexp");

    return regexp_processing.top()[degree];
}

int main() {
    string regexp;
    char letter_to_find;
    ulong degree;

    cin >> regexp >> letter_to_find >> degree;

    Parser regexp_machine(regexp);
    try {
        size_t result =
                regexp_machine.FindAnswer(letter_to_find, degree);

        if (result == INF)
            cout << "INF";
        else
            cout << result;

    } catch (exception& err) {
        cout << err.what();
    }
}
