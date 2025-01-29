//////////ПОИСК И ЗАПУСК КОМАНД ИЗ .BASH_HISTORY//////
//////////Принцип: селектор, выбор через цифры. Enter: подтверждение///////
///////////////////Рассчитан на 500 строк .bash_history//////
/////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>

#include <stdlib.h>

#include <termios.h>

#include <string.h>

#include <unistd.h>
///////////////////////////////////////////////////////////////////////////////
/*Перевод в режим non-canonical, getch работает без Enter. Этот фрагмент кода взят в Интернете*/
//This function found on stackoverflow
/////////SWITCH TO NON-CANONICAL//////
//////ФУНКЦИЯ/////
/////////SOURCE: stackoverflow.com SLASH questions/63751531/non-canonical-terminal-mode-buffer-stdout-in-c-program//////////
////////////////////////////////////////////////////////////////////////////////////
static struct termios stored_settings;

void set_keypress(void) {
  struct termios new_settings;

  tcgetattr(0, & stored_settings);

  new_settings = stored_settings;

  /* Disable canonical mode, and set buffer size to 1 byte */
  new_settings.c_lflag &= (~ICANON);
  new_settings.c_cc[VTIME] = 0;
  new_settings.c_cc[VMIN] = 1;

  tcsetattr(0, TCSANOW, & new_settings);
  return;
}

void reset_keypress(void) {
  tcsetattr(0, TCSANOW, & stored_settings);
  return;
}

                      //////////////////////////////Вызов Селектора/////////////////
                 ///////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////ФУНКЦИЯ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Функция вывода селектора.  Возвращает название программы
char * print_select(int select, char * p, char  arr1[500][200], int z)  {
  int j = 0;
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
      printf("%d.[%s]\n", j + 1, arr1[j]);
    }
  }

  //====ЕСЛИ СДЕЛАН ВЫБОР===
  int choice = select - 1;
  // Выводим результат выбора
  if (select > 0) {
    for (j = 0; j < z; j++) {
      // Выделяем цветом выбор
      if (select == j && select != 501) {
        // Запомним выбор. Нужно, чтобы напечатать в правильно
        // последовательности так как селектор с 1 по 6, массив с 0 по 5
        choice = select - 1;
        printf("->[%s%s%s]\n", colored, arr1[select - 1], uncolored);
        /* Возвращаем указатель на область памяти, где эта строка. Заодно
         * выделили память под нее (strlen - если не известен размер)*/
        //p = (char * ) malloc(strlen(arr1[select - 1]) + 1);
        //Но мы знаем размер 20. char * - приведение к типу "указатель на строку"
        p = (char * ) malloc(400);
        strcpy(p, arr1[select - 1]);
        break;
      }
      if (j != choice) printf("%d.[%s]\n", j + 1, arr1[j]);
    }

//Последнюю опцию добавим вне цикла
if (select ==500) {
	printf("->[%s%s%s]\n", colored, arr1[select - 1], uncolored);
	  p = (char * ) malloc(400);
     strcpy(p, arr1[select -1]);
}

  }

  printf("Для выхода: `  или 501 \n");
  printf("Для сброса нажмите  букву: s  \n");
  printf("Если трехзначное число, то нажатие s обнулит буфер!\n");
  printf("Перезагрузить файл .bash_history в буфер:  b  \n");
  printf("Быстрое редактирование команды(есть ограничения):  e. Отредактировать команду в vim: v \n");
  printf("Аргумент 1 при старте отключит буферизацию (файл .bash_history  будет читаться повторно)\n");

  /*Вернем название программы, которую надо выполнить*/
  if (p != "")
    return p;
  return "1";
}




              //////////////////////////////BUFFERING ////////////////////////////////
        //////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////ФУНКЦИЯ/////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


char * buffering (char *  argv_bash_history, char arr1[500][200], int z_size ) {


///////////////////////////////////////////////////////////////////////////////////////////
  //==================READING FILE============//
  /////////////////////////////////////////////////////////////////////////////////////////
  // Читаем файл с программами 1 раз. Потом будем хранить в буфере arr1
  FILE * file;
  char buffer1;
  char str[200];
  int i = 0;
  /*Количество вариантов*/
  int z = 0;
  int j = 0;

  // Количество показываемых файлов. Массив. Буфер для .bash_history
  file = fopen(argv_bash_history, "r");

while ((buffer1 = getc(file)) != EOF) {
	if(z==500)
	break;
    str[i] = buffer1;

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
  //Вернем указатель на массив
  return * arr1;

}

char * edit_string(char * arr1) {

	//Стрелки
	int  KEY_D = 68;
	int  KEY_C = 67;
	int  KEY_Backspace = 127;
	int  KEY_space = 32;
	int  KEY_backqote = 96;
	int KEY_ENTER=10;
	int  KEY_ext = 27;

	char * str=arr1;


     int z = 0; //Длина строки
	//char str[20] = "Edit thic string!";
	set_keypress(); //Переключим в non-canonical
	 system("clear");
	 printf("Перемещение к символу стрелки, стереть Backspace. Enter - выполнить\n");
	 printf("Редактирование команды в vim: v \n");
	 printf("Быстрое редактирование: e (ограничения на редактирование команды)\n");
	 printf("Для выхода нажмите Esc\n");
	 printf("%s", str); //Печатаем строку первый раз

	char value = getchar(); //Ввод
	int i = 0;
	int y = 0;
	int input = value - '\0';
	int input2 = value - '\0';
	char  runp[200];


//Ищем длину строки
for (i=0; str[i] != '\0'; i++)
z++;
y = z;

  while (1) {
input = value - '\0';
//Двигаемся влево
	if (input == KEY_D ) {
	  system("clear");
	  y--;
	 for (i=0; i <= z; i++) {
		 if (i == y)
		  printf("[");
		 printf("%c", str[i]);
		 if (i == y)
		  printf("]");
		}
	  input= KEY_backqote;
	}

	//Двигаемся вправо
if (input == KEY_C ) {
	  system("clear");
	  y++;
	 for (i=0; i <= z; i++) {
		 if (i == y)
		  printf("[");
		 printf("%c", str[i]);
		 if (i == y)
		  printf("]");
		}
		input= KEY_backqote;
	}

//Удаляем символы. BackSpace
	  if (input ==KEY_Backspace && input != KEY_backqote) {
	  system("clear");
	  y--;
	  input= KEY_backqote;
	 for (i=0; i <= z; i++) {
		 str[y]=value;
		 if (i == y)
		  printf("[");
		 printf("%c", str[i]);
		 if (i == y)
		  printf("]");
		}
		 printf("%c", str[i]);
	}

		  if (input ==KEY_space && y ==  z) {
			  z++;
		  }


//Если достигли предела декремента
	if (y < 0)
	y=0;

//Если достигли предела инкремента
	if (y > z)
	y=z;

//Редактируем символ
	  if (input > 96 && input < 127 || input > 31 && input  < 67 ||  input > 68 && input  <  89 || input > 91 && input  < 96) {
	  system("clear");
	 for (i=0; i <= z; i++) {
		 str[y]=value;
		 if (i == y)
		  printf("[");
		 printf("%c", str[i]);
		 if (i == y)
		  printf("]");
		}
		 printf("%c", str[i]);
	input= KEY_backqote;
	 y++;
	}

//Записываем новое значение в файл
	if (input == KEY_ENTER ) {
		 for (i=0; str[i] != '\0'; i++)
		return(str);
	}


//Ожидание символа
	value = getchar();
	}

return str;
}



                      ////////////////////////////////MAIN///////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                     ///////////////////////////////////////////////////////////////////////////////
                  ////////////////////////////////////ФУНКЦИЯ//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char * argv[]) {
  FILE * file;
  if (argc <= 2) {
   file = fopen(".bash_history", "r") ;
//Проверим файл .bash_history
  char  str_error[200]="Не найден файл .bash_history. Программу следует запускать из директории пользователя\n";
  char  str_error1[200]="Если запускаете программу не из директории пользователя, следует задать путь к .bash_history, так: \n";
  char  str_error2[200]="select-history-comb.comb 1 <путь к .bash_history>: \n";
  if  (file==NULL) {
	  printf("%s", str_error);
	  printf("%s", str_error1);
	   printf("%s", str_error2);
	  return 1;
  } else {
    fclose(file);
}
argv[2]=".bash_history";
}

if (argc > 2) {
	 file = fopen(argv[2], "r") ;
	   if  (file==NULL) {
	  printf("Неправильно задан путь к .bash_history. Следует задавать абсолютный путь так: /home/user/.bash_history\n");
	  return 1;
	   } else {
    fclose(file);
}

}
	  ////////////////////////////////KEYS//////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////УПРАВЛЕНИЕ/////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////////////
	char  const KEY_A='A'; //Стрелка вверх
	char const KEY_B='B'; //Стрелка вниз
	char  const KEY_C='C'; //Стрелка вправо - переход на 30 позиций вверх
	char const KEY_D='D'; //Стрелка влево  - переход на 30 позиций вниз
	char const KEY_s='s';  //Сброс
	char const KEY_b='b';  //Перезагрузить буфер. Перезагрузить файл .bash_history
	char const KEY_EXIT='`'; //Выход (`)
	char const KEY_SPACE=' '; //Пробел
	char const KEY_ENTER='\n'; //Enter
	char const KEY_e = 'e'; //Быстрое редактирование
	char const KEY_v = 'v'; //Вызвать vim  для редактироваия
///////////////////////////////////////////////////////////////////////////////////

//Массив хранит .bash_history
  char arr1[500][200];
//Функция буферизации
  buffering(argv[2],arr1, 500);
//Размер массива - буфера
  int z = 500;

///////////////////////////////////////////////////////////////////////////////


  char * run_program;
  //для названия команды
  char * p;
  //Цвет
  char colored[14] = "\033[43m";
  /*Сброс цвета*/
  char uncolored[14] = "\033[0m";
  int select = 0;
  //Вызываем селектор 1 раз, пустой
  print_select(select, p, arr1, z);
  //С помощью terminos переключим вывод терминала в non-canonical mode.
  //Это позволит не нажимать Enter при переходе от опции к опции, но для выполнения нажать надо
  set_keypress();
  //Для чтения   с терминала
  char value = getchar();
  //Для перемещения по разрядам
  char buffer[3] = {0};
  //Счётчик буфера
  int i = 0;
  int y = 0;


//////////////////////////////////////////////////////////////////////////////////
////////////////////////СЕЛЕКТОР В ЦИКЛЕ////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

  while (1) {
//////////////////////////////////////////////////////////////////////////////////
///////////////////УПРАВЛЕНИЕ СТРЕЛКАМИ///////////////////
//////////////////////////////////////////////////////////////////////////////////

//Пролистывание может вызвать Stack Smashing

	if (value  == KEY_b) {
	value = KEY_SPACE;
		//Перезагрузим буфер
	  buffering(argv[2],arr1, 500);
	run_program = print_select(select, p, arr1, 500);
	select=0;
	}


	 if (value  == KEY_D) {
		 y=y-30;
		 value = KEY_SPACE;
		select=select+y;
		//Читаем заново файл. Если нужна  однократная буферизация, то можно отключить/закомментировать
		 if (argc > 1)
		  buffering(argv[2],arr1, 500);
		run_program = print_select(select, p, arr1, 500);
		select=0;
		}

	 if (value  == KEY_C) {
		 y=y+30;
		 value = KEY_SPACE;
		select=select+y;
		//Читаем заново файл. Если нужна  однократная буферизация, то можно отключить/закомментировать
		  if (argc > 1)
	  buffering(argv[2],arr1, 500);
		run_program = print_select(select, p, arr1, 500);
		select=0;
		}

//Листаем вниз и добавляем опции
//Пролистывание может вызвать Stack Smashing
	 if (value == KEY_B) {
		 y++;
		 value = KEY_SPACE;
		select=select+y;
		//Читаем заново файл. Если нужна  однократная буферизация, то можно отключить/закомментировать
		  if (argc > 1)
  buffering(argv[2],arr1, 500);
		run_program = print_select(select, p, arr1, 500);
		select=0;
		}

//Листаем вверх и добавляем опции
//Пролистывание может вызвать Stack Smashing
	if (value == KEY_A) {
		y--;
		 value = KEY_SPACE;
		select=select+y;
		//Читаем заново файл. Если нужна  однократная буферизация, то можно отключить/закомментировать
		 if (argc > 1)
	  buffering(argv[2],arr1, 500);
		run_program = print_select(select, p, arr1, 500);
		select=0;
		}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

    if (value != KEY_SPACE && value != KEY_s) {
      buffer[i] = value;
      value = KEY_SPACE;
      //Приведем к int
      select = strtol(buffer, NULL, 10);
      //Вызываем селектор с выбором
      //Читаем заново файл. Если нужна  однократная буферизация, то можно отключить/закомментировать
       if (argc > 1)
      buffering(argv[2],arr1, 500);
      run_program = print_select(select, p, arr1, 500);
      i++;
      //Освободим память
      //free(run_program);
    }


	///////////////////////////////////////////////////////////////////////////////
    //Проверим буфер. Если буфер заполнен, то обнулим его. Напечатаем то что в нем было в начале
    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////
    //Проверим буфер. Если буфер заполнен, то обнулим его. Напечатаем то что в нем было в начале
    //Можно описать циклом, но оставим всё явно для наглядности
    //Тут конъюнкция (логическое "и", логическое сложение, в курсе булевой алгебры или логики:))
    if (value !=  KEY_A && value !=  KEY_B && y == 0)  {
		if (buffer[0] > 0  && buffer[1]  == 0) {
      printf("%c", buffer[0]);

    }

       if (buffer[0] > 0 && buffer[1] > 0 && buffer[2]  == 0) {
      printf("%c %c", buffer[0], buffer[1]);

    }

	     if (buffer[0] > 0 && buffer[1] > 0 && buffer[2] > 0) {
      printf("%c %c %c", buffer[0], buffer[1], buffer[2]);

    }
    value = KEY_SPACE;
}
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

    //Добавляем новое число в буфер
    if (buffer[i] > 0) {
		//Чтобы не получить stack smashing
		if (i >= 3) {
			i = 3;
		}
      buffer[i] = value;
    }

    //Ожидаем ввод
    value = getchar();
    //Выполним программу
    //Проверим, что нажат Enter (\n) и то, что не выходим
    if (value == KEY_ENTER && select !=501) {
		system(run_program);
      //Чтобы остановить обновление
      //и показать результат программы
    value = KEY_SPACE;
    }
    //Обнулим проход (для наглядности все опишем явно)
    if (value == KEY_s && select!=501) {
      //Обнулим буфер
      buffer[0] = 0;
      buffer[1] = 0;
      buffer[2] = 0;
      //Обнулим счётчик буфера
      i = 0;
      select = 0;
      y = 0;
        if (argc > 1)
      buffering(argv[2],arr1, 500);
      print_select(select, p, arr1, z);
    }
//Редактирование команды в vim
	if (value == KEY_v) {
      char  ppp[50];
      char str123[6] = "vi +";
      char str1234[15] = " .bash_history ";
      snprintf(ppp, 50, "%s%d%s", str123, select,  str1234);
     system(ppp);
     sleep(3);
      value = KEY_SPACE;
	}
	//Быстрое редактирование
	 if (value == KEY_e) {
      edit_string(run_program);
      system(edit_string(run_program));
      sleep(3);
       value = KEY_SPACE;
	}
    //Выход (дизъюнкция - нестрогое "или".  Или то, или то. Если первое условие верно, то второе не проверяется)
    if (value == KEY_EXIT || select==501) {
      break;
  }
  }

  printf("\033[43m Выход \033[0m \n");
  return 0;
}
