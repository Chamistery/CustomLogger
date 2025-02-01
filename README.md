Тестовое задание для стажера на направление «Разработчик C++» в **ИнфоТеКС**.<br/>
Выполнил **Камлев Виталий**.<br/>
Для запуска на *Linux* необходимо  склонировать GitHub репозиторий в папку и, заранее перейдя в нее в терминале, прописать следующие команды:<br/>
```make```<br/>
```make run``` или же ```./build/logger_app <файл> <уровень>```<br/>
где *<файл>* --- файл, куда будут записываться сообщения; *<уровень>* --- начальный уровень логирования ([DEBUG/INFO/ERROR])<br/>
В случае вызова ```make run``` запись будет происходить в файл *log.txt*, уровень логирования же будет выставлен *INFO*.<br/>
Для очистки каталога от временных файлов (*<файл>* не является временным) необходимо прописать следующее<br/>
```make clean```
