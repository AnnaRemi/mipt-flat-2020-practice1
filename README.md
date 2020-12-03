# mipt-flat-2020-practice1
Practices 3d semester

Даны α, буква x и натуральное число k. Вывести длину кратчайшего слова из языка L, содержащего префикс x^k.

Создаются классы CurrentSituation & Parser

В стеке лежат объекты типа CurrentSituation

В CurrentSituation:
  
MinLen - длина минимального слова в текущем состоянии
  
MinLenPrefix - массив длины k, где в MinLenPrefix[i] хранится длина минимального слова, которое начинается на x^i; если такового не нашлось, то INF

Update CurrentSituation due to:
1. если буква, кладем в стек и MinLenPrefix[1] = 1
2. если пустой символ, то MinLenPrefix[0] = 0
3. если '+', то выбираем минимум из полей двух операндов
4. если '.', то если для первого операнда MinLenPrefix[i] == i, то идем по MinLenPrefix второго операнда и пытаемся сконкатенировать слова и увеличить длину результирующего слова
5. если '*', то получаем все кратные слова из тех, которые полностью состоят из букв, которые хотим найти. 
