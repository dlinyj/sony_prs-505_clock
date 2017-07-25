Часы на базе ридера PRS-505

![Alt text](test.png?raw=true "Title")

Картинки для отображения генрируются командами

`for i in ``seq 0 9``; do convert  -font Gideon’s-Army- -pointsize 150 -size 100x600 -gravity center -rotate 90 -depth 8  caption:"$i" $i.pgm;done
convert  -font Gideon’s-Army- -pointsize 150 -size 100x600 -gravity center -depth 8 -rotate 90 caption:':' dots.pgm`

где Gideon’s-Army- - это шрифт картинки. Первая строка генерирует 
картинки цифр (от нуля до 9), вторая - одну картинку символа двоеточие.

После компиляции запуск осуществляется скриптом start.sh . В качестве
gараметров передаётся файл-устройство /dev/sd* с диском книжки.
По умолчанию запись идёт в test.pgm.

TODO: Перенести код на книжку, добавив запись прямо во фреймбуффер.

Автор Долин Сергей ака dlinyj dlinyj@gmail.com
