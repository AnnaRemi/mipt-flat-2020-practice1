# mipt-flat-2020-practice1
Practices 3d semester
Преобразовываем регулярку в обратной польской записи в обычный вид со скобочками;
Начиная с самой вложенной скобки, представляем выражение в виде X$Y, где X - длина префикса, Y - осташегося слова
Если операция . - сливаем в одно слово
Если операция * - то два вариана (либо искомый символ*, тогда мы победили; либо нет и тогда смотрим два случая)
Если операция + - просто распараллеливаем 

Выбираем минимальный вариант
