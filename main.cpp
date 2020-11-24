#include<iostream>
#include<algorithm>
#include<stack>

using std::stack;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::max;

/* this class is a tree-like structure including different conditions where each node complies
* some subword of language(reg_ex)
*exit_cycle - if we have substring form x* and we actually have x^k as a substring
*epsilon - is empty way
*/

class Parsing_Tree {
private:
    bool exit_cycle;
    bool epsilon;
    int PrefixLen;
    int SuffixLen;
    int SymbolWayLen;
    int SubstringLen;
public:
    explicit Parsing_Tree(int a) {
        exit_cycle = epsilon = PrefixLen = SymbolWayLen = SuffixLen = SubstringLen = 0;
    }

    void reload(int a) {
        exit_cycle = epsilon = PrefixLen = SuffixLen = SymbolWayLen = SubstringLen = a;
    }

/*
*In "parse" function we iterate the input expression.
*If the symbol met in line is letter, we increment all field structures,
*as well as prefix, suffix, and subword consisting of letters.
*If we met '1' - we increment the flag with an empty word and put a structure with information about the letter in the stack
*We also have ancillary functions which helps us to parse operators
*/


    static void parse(const string& reg_ex, char x, unsigned int k, stack<Parsing_Tree> S) {
        int counter = 0;
        for (char i : reg_ex) {

            if (!(i == '.' || i == '*' || i == '+'
                  || i == 'a' || i == 'b' || i == 'c')) {
                cout <<  "ERROR_WRONG_SYMBOLS";
                return;
            }

            if (isalpha(i) != 0) {
                Parsing_Tree Buffer(1);

                if (i == x) {
                    Buffer.reload(1);
                    Buffer.epsilon = false;
                    Buffer.exit_cycle = false;
                }

                S.push(Buffer);
            } else if (i == '+' || i == '*' || i == '.') {
                    if (i == '+') {
                        Parsing_Tree subsidiary_object(1);

                        if (S.size() < 2){
                            cout << "ERROR_INVALID_EXPRESSION";
                            return;
                        }

                        Parsing_Tree second_element(1);

                        second_element = S.top();
                        S.pop();

                        Parsing_Tree first_element(1);

                        first_element = S.top();
                        S.pop();

                        subsidiary_object.SubstringLen =
                                max(first_element.SubstringLen, second_element.SubstringLen);

                        subsidiary_object.SuffixLen =
                                max(first_element.SuffixLen, second_element.SuffixLen);

                        subsidiary_object.PrefixLen =
                                max(first_element.PrefixLen, second_element.PrefixLen);

                        subsidiary_object.SymbolWayLen =
                                max(first_element.SymbolWayLen, second_element.SymbolWayLen);

                        subsidiary_object.exit_cycle =
                                max(first_element.exit_cycle, second_element.exit_cycle);

                        subsidiary_object.epsilon =
                                max(first_element.epsilon, second_element.epsilon);

                        S.push(subsidiary_object);
                    }

                    if (i == '*') {
                        Parsing_Tree subsidiary_object(1);

                        if (S.empty()){
                            cout << "ERROR_INVALID_EXPRESSION";
                            return;

                        } else {
                            Parsing_Tree top_element(1);

                            top_element = S.top();
                            S.pop();

                            subsidiary_object.exit_cycle = top_element.exit_cycle;

                            subsidiary_object.PrefixLen = top_element.PrefixLen;

                            subsidiary_object.epsilon = true;

                            subsidiary_object.SuffixLen = top_element.SuffixLen;

                            subsidiary_object.SymbolWayLen = top_element.SymbolWayLen;

                            subsidiary_object.SubstringLen =
                                    max(top_element.SymbolWayLen,
                                        max(top_element.SubstringLen, subsidiary_object.PrefixLen +
                                                                          subsidiary_object.SuffixLen));

                            if (top_element.SymbolWayLen)
                                subsidiary_object.exit_cycle = true;

                            ++counter;
                            S.push(subsidiary_object);
                        }
                    }
                    if (i == '.') {
                        if (S.size() < 2){
                            cout << "ERROR_INVALID_EXPRESSION";
                            return;
                        }

                        Parsing_Tree second_operand(1);

                        second_operand = S.top();
                        S.pop();

                        Parsing_Tree first_operand(1);

                        first_operand = S.top();
                        S.pop();

                        Parsing_Tree subsidiary_object(1);

                        subsidiary_object.exit_cycle =
                                first_operand.exit_cycle || second_operand.exit_cycle;

                        subsidiary_object.SymbolWayLen =
                                (second_operand.SymbolWayLen * first_operand.SymbolWayLen == 0)
                                ? 0 : (second_operand.SymbolWayLen + first_operand.SymbolWayLen);

                        subsidiary_object.PrefixLen =
                                std::max(first_operand.PrefixLen, (first_operand.SymbolWayLen == 0)
                                                                      ? 0 : (first_operand.SymbolWayLen + second_operand.PrefixLen));

                        subsidiary_object.SuffixLen =
                                std::max(second_operand.SuffixLen, (second_operand.SymbolWayLen == 0)
                                                                       ? 0 : (first_operand.SuffixLen + second_operand.SymbolWayLen));

                        if (first_operand.epsilon) {
                            subsidiary_object.PrefixLen =
                                    std::max(subsidiary_object.PrefixLen, second_operand.PrefixLen);

                            subsidiary_object.SymbolWayLen =
                                    std::max(subsidiary_object.SymbolWayLen, second_operand.SymbolWayLen);
                        }
                        if (second_operand.epsilon) {
                            subsidiary_object.SuffixLen =
                                    std::max(subsidiary_object.SuffixLen, first_operand.SuffixLen);

                            subsidiary_object.SymbolWayLen =
                                    std::max(subsidiary_object.SymbolWayLen, first_operand.SymbolWayLen);
                        }

                        subsidiary_object.epsilon =
                                second_operand.SymbolWayLen * first_operand.SymbolWayLen != 0;

                        subsidiary_object.SubstringLen =
                                max(max(subsidiary_object.PrefixLen, subsidiary_object.SuffixLen),
                                    max(max(first_operand.SubstringLen, second_operand.SubstringLen),
                                        first_operand.SuffixLen + second_operand.PrefixLen));

                        ++counter;
                        S.push(subsidiary_object);
                    }

                }

        }
        if (S.empty()) {
            cout << "ERROR_INVALID_EXPRESSION";
            return;
        } else {
            Parsing_Tree subsidiary_object(1);

            subsidiary_object = S.top();
            if (subsidiary_object.exit_cycle){
                cout << k;
                return;
            } if (subsidiary_object.SubstringLen >= k) {
                cout << subsidiary_object.SubstringLen << '\n';
                return;
            } else {
                cout << "INF";
                return;
            }
        }
    }
};

int main() {
    stack<Parsing_Tree> S;
    string reg_ex;
    unsigned int k;
    char x;
    cin >> reg_ex >> x >> k;
    Parsing_Tree::parse(reg_ex, x, k, S);
    return 0;
}

