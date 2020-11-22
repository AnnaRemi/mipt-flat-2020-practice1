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
    int bestPrefixLen;
    int bestSuffixLen;
    int bestXwayLength;
    int bestSubstringLen;
public:
    explicit Parsing_Tree(int a) {
        exit_cycle = epsilon = bestPrefixLen = bestXwayLength = bestSuffixLen = bestSubstringLen = 0;
    }

    void reload(int a) {
        exit_cycle = epsilon = bestPrefixLen = bestSuffixLen = bestXwayLength = bestSubstringLen = a;
    }

/*
*In "parse" function we iterate the input expression.
*If the meeting is in line letter, we incriminating all field structure,
*so how did the prefix, suffix, and subword consisting of letters.
*If we met '1' - we incriminate the flag with an empty word and put a structure with information about the letter in the stack
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

                        subsidiary_object.bestSubstringLen =
                                max(first_element.bestSubstringLen, second_element.bestSubstringLen);

                        subsidiary_object.bestSuffixLen =
                                max(first_element.bestSuffixLen, second_element.bestSuffixLen);

                        subsidiary_object.bestPrefixLen =
                                max(first_element.bestPrefixLen, second_element.bestPrefixLen);

                        subsidiary_object.bestXwayLength =
                                max(first_element.bestXwayLength, second_element.bestXwayLength);

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

                            subsidiary_object.bestPrefixLen = top_element.bestPrefixLen;

                            subsidiary_object.epsilon = true;

                            subsidiary_object.bestSuffixLen = top_element.bestSuffixLen;

                            subsidiary_object.bestXwayLength = top_element.bestXwayLength;

                            subsidiary_object.bestSubstringLen =
                                    max(top_element.bestXwayLength,
                                        max(top_element.bestSubstringLen, subsidiary_object.bestPrefixLen +
                                                                          subsidiary_object.bestSuffixLen));

                            if (top_element.bestXwayLength)
                                subsidiary_object.exit_cycle = true;

                            ++counter;
//                            cout << counter;
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

                        subsidiary_object.bestXwayLength =
                                (0 == second_operand.bestXwayLength * first_operand.bestXwayLength)
                                ? 0 : (second_operand.bestXwayLength + first_operand.bestXwayLength);

                        subsidiary_object.bestPrefixLen =
                                std::max(first_operand.bestPrefixLen, (first_operand.bestXwayLength == 0)
                                                                      ? 0 : (first_operand.bestXwayLength + second_operand.bestPrefixLen));

                        subsidiary_object.bestSuffixLen =
                                std::max(second_operand.bestSuffixLen, (second_operand.bestXwayLength == 0)
                                                                       ? 0 : (first_operand.bestSuffixLen + second_operand.bestXwayLength));

                        if (first_operand.epsilon) {
                            subsidiary_object.bestPrefixLen =
                                    std::max(subsidiary_object.bestPrefixLen, second_operand.bestPrefixLen);

                            subsidiary_object.bestXwayLength =
                                    std::max(subsidiary_object.bestXwayLength, second_operand.bestXwayLength);
                        }
                        if (second_operand.epsilon) {
                            subsidiary_object.bestSuffixLen =
                                    std::max(subsidiary_object.bestSuffixLen, first_operand.bestSuffixLen);

                            subsidiary_object.bestXwayLength =
                                    std::max(subsidiary_object.bestXwayLength, first_operand.bestXwayLength);
                        }

                        subsidiary_object.epsilon =
                                second_operand.bestXwayLength * first_operand.bestXwayLength != 0;

                        subsidiary_object.bestSubstringLen =
                                max(max(subsidiary_object.bestPrefixLen, subsidiary_object.bestSuffixLen),
                                    max(max(first_operand.bestSubstringLen, second_operand.bestSubstringLen),
                                        first_operand.bestSuffixLen + second_operand.bestPrefixLen));

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
            } if (subsidiary_object.bestSubstringLen >= k) {
                cout << subsidiary_object.bestSubstringLen;
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

