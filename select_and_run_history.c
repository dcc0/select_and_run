/* This program is an example of selector with choice confirmation, written in C (ANSI).
 * Программа для выбора запускаемой программы с подтверждением из history linux. Реализация: C (ANSI или C89).
 * By Ivan Gavryushin (dcc0@mail.ru, ivangavr777@gmail.com, mol0t@list.ru). MSK. Butovo. 2025*/
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    /* Пример реализации выбора с перерисовкой окна и выходом.
     * Сохранение положения элементов окна осуществляется через вызов clear -
     * очищение окна*/
    // Функция подтверждения ввода. Возвращает 0 при успехе.
    int confirm_choice() {
      char input_confirm;
      int  i =  0;
      /*Используем двойной scanf, чтобы подтвердить нажатие*/
      printf("\nНажмите Enter\n");
      scanf("%c", & input_confirm);
      scanf("%c", & input_confirm);
      if (input_confirm == '\n') {
        return 0;
    }
        return 1;
    }
    // Функция вывода селектора.  Возвращает название программы
    char * print_select(int select, char *p) {
      //============READING FILE============//
      // Читаем файл с программами
      FILE * file;
      char buffer;
      char str[200];
      int i = 0;
      /*Количество вариантов*/
      int z = 0;
      int j = 0;
      // Количество показываемых файлов. Массив
      char arr1[500][200];
      // Файл с программами. Ищем файлы .sh в количестве 6 шт. в текущей директории
      // (демо версия:)). system("ls *.sh | head -n 6 > data.txt"); В data.txt
      // следует писать названия программ с абсолютными путями, если используем execl
      // Если system, то так: ls, pwd и т.д.
      file = fopen(".bash_history", "r");
      while ((buffer = getc(file)) != EOF) {
        str[i] = buffer;
        if (str[i] == '\n') {
          // Пишем строку  посимвольно в отдельный массив
          for (j = 0; str[j] != '\n'; j++) {
            arr1[z][j] = str[j];
          }
          /*Запишем 0 в конец строки*/
          arr1[z][j] = '\0';
          z++;
          //Сбросим i , чтобы очистить str[0]
          //Так как ниже инкремент, то сбросим в -1, т.е. ниже i уже = 0
          //Иначе в str[0] всегда будет в начале первый символ
          i = -1;
        }
        i++;
      }
      fclose(file);
      // Завершаем чтение файла
      //===========END OF FILE READING=============//
      /*Выделение цветом*/
      char colored[14] = "\033[43m";
      /*Сброс цвета*/
      char uncolored[14] = "\033[0m";
      //====ВЫВОДИМ СЕЛЕКТОР===
      /*Очистим экран. Обновим. Первый раз выводим*/
      system("clear");
      if (select == 0) {
        /*Количество вариантов выбора в переменной z*/
        for (j = 0; j != z; j++) {
          printf("%d.[%s]\n", j+1, arr1[j]);
        }
      }
    //====ЕСЛИ СДЕЛАН ВЫБОР===
      int choice = select - 1;
      // Выводим результат выбора
      if (select > 0) {
        for (j = 0; j != z; j++) {
          // Выделяем цветом выбор
          if (select == j && select !=7) {
            // Запомним выбор. Нужно, чтобы напечатать в правильно
            // последовательности так как селектор с 1 по 6, массив с 0 по 5
            choice = select - 1;
            printf("->[%s%s%s]\n", colored, arr1[select - 1], uncolored);
            /* Возвращаем указатель на область памяти, где эта строка. Заодно
             * выделили память под нее (strlen - если не известен размер)*/
            //p = (char * ) malloc(strlen(arr1[select - 1]) + 1);
            //Но мы знаем размер 20. char * - приведение к типу "указатель на строку"
            p = (char * ) malloc(200);
            strcpy(p, arr1[select - 1]);
            break;
          }
          if (j != choice) printf("%d.[%s]\n", j+1, arr1[j]);
        }
      }

      if (select ==500) {
	printf("->[%s%s%s]\n", colored, arr1[select - 1], uncolored);
	  p = (char * ) malloc(400);
     strcpy(p, arr1[select -1]);
}
          //Раскрасим выход
              if (select == 501) {
            choice = select - 1;
            printf("->[%s Выход %s]\n", colored,  uncolored);
         } else {
           printf("Выход\n");
        }
        printf("Для выхода: 501 \n");
        printf("Для сброса нажмите: 0  \n");
        printf("Для справки  добавьте ключ -h: select_and_run_history.comb -h\n");
        printf("О программе: select_and_run_history.comb -a\n");
      /*Вернем название программы, которую надо выполнить*/
      if (p != "")
      return p;
      return "1";
    }
    int main(int argc, char * argv[]) {
    if (argc == 1) {
    FILE * file;
    //Проверим файл при старте
        file  = fopen(".bash_history", "r");
        if (file == NULL) {
        printf("Нет файла  .bash_history. Для справки  добавьте ключ -h: select_and_run_history.comb -h\n");
        return 1;
        }
        else {
        fclose(file);
        }
    }
    //Файл справки
    if (argc > 1) {
    if ( strncmp("-h", argv[1], 2) == 0) {
    printf("В директории с программой должен быть файл .bash_history. В нем 500 последних команд. 501  опция - выход\n");
    return 0;
    }
    //Печатаем информацию о программе в виде заставки
    if ( strncmp("-a", argv[1], 2) == 0) {
    system("clear");
    sleep(1);
    printf("Running Mol0T History Commander. Ver. 1.0. Written in C(89). By Ivan Gavryushin (ivangavr777@gmail.com, mol0t@list.ru, dcc0@yandex.ru). MSK. Butovo. 2025\n");
    sleep(5);
    printf("Для справки  добавьте ключ -h: select_and_run_history.comb -h\n");
    sleep(5);
    printf("Спасибо!\n");
    sleep(3);
    }
    }
      char * run_program;
      int select = 0;
      int input;
      //Указатель на строку с программой, которая будет выделена и выполнена
      char * p;
      //Счётчик для неправильных попыток
      int i = 3;
      /*Сбросим цвет при старте*/
      /*Первый вызов селектора*/
      print_select(select, p);
      /*Сканируем ввод*/
      while ( 1) {
        scanf("%d", &input);
       //Сброс
        if (input  == 0 ) {
		select = 0;
		}

        //Проверим введённые данные. Попробуем привести к целому, если символ; так проверим ввод
        if (input != 's'  &&	 input - '\0' == 0 ) {
        //Вызываем подтверждение ввода. Таким образом вернёмся к scanf, сбросим input
        //На следующей итерации сканируем заново
        //Возвращаем 0 и переходим к ожиданию нового ввода. Прыгаем к scanf
        confirm_choice();
        printf("Неправильный ввод %d\n", i);
        if (i == 1)
        return 1;
        i--;
        }


        if (input == 501) {
          select=input;
          print_select(select, p);
          printf("Выход\n");
          break;
        }
        if (input > 501)
        break;
        /*Выбор тут*/
        if (input > 0 && input < 501) {
          select = input;
          run_program = print_select(select, p);
          printf("Выполним программу номер:   %d \n", select);
                  printf("Выполним программу  %s \n", run_program);
          /*Подтвердим и выполним*/
          if (confirm_choice() == 0) {
             system(run_program);
            //execl(run_program, 0, NULL);
            //Освободим память, выделенную для p
            //free(p);
          } else {
            select = 0;
            print_select(select, p);
            //Обнулим и input. Если этого не сделать, то при вводе любого символа и Enter выполится предудущий выбор
            input = 0;
            printf("Выбор сброшен\n");
          }
        }

      }
      return 0;
    }
