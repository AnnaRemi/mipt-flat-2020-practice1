lang      = {'a', 'b', 'c', 'e', '.', '+', '*'}
literals  = {'a', 'b', 'c', 'e'}
binary_op = {'.', '+'}
unar_op   = {'*'}

answers = []

def analysis_concat(substr):
    divide = substr.split('.')
    first_elem = divide[0]
    second_elem = divide[1]
    first_word  =  first_elem.split('$')
    second_word = second_elem.split('$')

    if int(first_word[1]) == 0:
        cnt_1 = int(first_word[0]) + int(second_word[0])
        cnt_2 = int(first_word[1]) + int(second_word[1])
    if int(first_word[1]) != 0:
        cnt_1 = int(first_word[0])
        cnt_2 = int(first_word[1]) + int(second_word[1]) + int(second_word[0])

    new_elem = str(cnt_1) + '$' + str(cnt_2)
    return new_elem

def analysis(str, k, cycle, len_cycle):
    #print(str)
    global inf_flag


    def analysis_star(substr):
        nonlocal cycle
        nonlocal len_cycle
        divide = substr.split('*')
        elem = divide[0]
        word = elem.split('$')

        if int(word[0]) > 0 and int(word[1]) == 0:
            cycle = True
            len_cycle = int(word[0])
            new_elem = "0$0"
            return new_elem

       # print(new_elem + "%")
        return - 1

    try:

        while 1:


            right_brace = str.index(')')
            left_brace = str[:right_brace + 1].rindex('(')
            substr = str[left_brace + 1:right_brace]
            # print(substr)
            new_elem = ''
            if substr.find('+') != -1:
                divide = substr.split('+')
                analysis(str[:left_brace] + divide[0] + str[right_brace + 1:], k, cycle, len_cycle)
                analysis(str[:left_brace] + divide[1] + str[right_brace + 1:], k, cycle, len_cycle)
                return
            if substr.find('.') != -1:
                new_elem = analysis_concat(substr)
            if substr.find('*') != -1:
                new_elem = analysis_star(substr)
                if new_elem == -1:
                    divide = substr.split('*')
                    analysis(str[:left_brace] + divide[0] + str[right_brace + 1:], k, cycle, len_cycle)
                    analysis(str[:left_brace] + "0$0" + str[right_brace + 1:], k, cycle, len_cycle)
                    return

            str = str[:left_brace] + new_elem + str[right_brace + 1:]

    except ValueError:

        #print (str)
        answer = str.split('$')
        prefix = int(answer[0])
        suffix = int(answer[1])
        if prefix < k and cycle == True:
            while prefix < k:
                prefix += len_cycle

        if prefix >= k :
            answers.append(prefix + suffix)

        return

def solver(str, letter, k):
    stack = []
    str = str.replace('1', 'e')

    if len(letter) > 1:
        return "ERROR"

    for i in str:
        if i not in lang:
            return "ERROR"
        if i in literals:
            stack.append(i)
        if i in binary_op:
            if len(stack) > 1:
                second = stack.pop()
                first = stack.pop()
                stack.append('(' + first + i + second + ')')
            else:
                return "ERROR"
        if i in unar_op:
            if len(stack) > 0:
                sym = stack.pop()
                stack.append('(' + sym + i + ')')
            else:
                return "ERROR"
    if len(stack) > 1:
        return "ERROR"

    #print(stack[0])

    for i in literals:
        if i == letter:
            stack[0] = stack[0].replace(i, '1$0')

        if i == 'e':
            stack[0] = stack[0].replace(i, '0$0')

        else:
            stack[0] = stack[0].replace(i, "0$1")

    #print(stack[0])

    analysis(stack[0], k, False, 0)

    if len(answers) == 0:
        return "INF"
    else:
        return min(answers)

    return

def main():
    str = input()
    str = ''.join(str.split(' '))
    letter = input()
    k = int(input())
    print(solver(str, letter, k))

if __name__ == "__main__":
    main()
