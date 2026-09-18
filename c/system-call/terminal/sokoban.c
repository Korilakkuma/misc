#include <curses.h>
#include <stdio.h>

static void sokoban_init(char *filename);
static void sokoban_end(void);
static void sokoban_main_loop(void);

static void load_map(char *filename);
static void step(int dy, int dx);
static void run(int dy, int dx);

static int y;
static int x;

static int number_of_holes;

enum OBJECTS {
  WALL  = '#',
  ROCK  = '\'',
  PIT   = '^',
  SELF  = '@',
  SPACE = ' '
};

enum KEYBOARDS {
  KB_UP      = 'k',
  KB_DOWN    = 'j',
  KB_RIGHT   = 'l',
  KB_LEFT    = 'h',
  KB_UP2     = 'K',
  KB_DOWN2   = 'J',
  KB_RIGHT2  = 'L',
  KB_LEFT2   = 'H',
  KB_QUIT    = 'q',
  KB_REFRESH = 'r'
};

int main(void) {
  sokoban_init("map0.txt");
  sokoban_main_loop();
  sokoban_end();

  return 0;
}

static void sokoban_init(char *filename) {
  initscr();
  noecho();
  cbreak();

  load_map(filename);

  move(y, x);
}

static void sokoban_end(void) {
  if (number_of_holes == 0) {
    move(1, 1);
    printw("You won !!");
    move(2, 1);
    printw("Type any key to end: ");
    getch();
  }

  clear();
  refresh();
  endwin();
}

static void sokoban_main_loop(void) {
  refresh();

  int ch;

  while ((number_of_holes != 0) && ((ch = getch()) != KB_QUIT)) {
    switch (ch) {
      case KB_LEFT: {
        step(0, -1);
        break;
      }

      case KB_LEFT2: {
        run(0, -1);
        break;
      }

      case KB_DOWN: {
        step(+1, 0);
        break;
      }

      case KB_DOWN2: {
        run(+1, 0);
        break;
      }

      case KB_UP: {
        step(-1, 0);
        break;
      }

      case KB_UP2: {
        run(-1, 0);
        break;
      }

      case KB_RIGHT: {
        step(0, +1);
        break;
      }

      case KB_RIGHT2: {
        run(0, +1);
        break;
      }

      case KB_REFRESH: {
        refresh();
        break;
      }

      default: {
        break;
      }
    }

    move(y, x);
    refresh();
  }
}

static void load_map(char *filename) {
  char buf[1024];

  FILE *fp = fopen(filename, "r");

  fgets(buf, sizeof(buf), fp);

  sscanf(buf, "y = %d, x = %d, num = %d", &y, &x, &number_of_holes);

  for (int n = 0; n < LINES; n++) {
    if (fgets(buf, sizeof(buf), fp) == NULL) {
      break;
    }

    move(n, 0);
    printw("%s", buf);
  }

  fclose(fp);
}

static void step(int dy, int dx) {
  chtype c;

  int new_y = y + dy;
  int new_x = x + dx;

  move(new_y, new_x);

  c = inch() & A_CHARTEXT;

  if ((c == WALL) || (c == PIT)) {
    return;
  }

  if (c == ROCK) {
    move((new_y + dy), (new_x + dx));

    c = inch() & A_CHARTEXT;

    if (!((c == SPACE) || (c == PIT))) {
      return;
    }

    if (c == SPACE) {
      addch(ROCK);
    } else if (c == PIT) {
      addch(SPACE);

      --number_of_holes;

      move((LINES - 1), 0);
      printw("%d items left", number_of_holes);
    }
  }

  move(y, x);

  addch(SPACE);

  y += dy;
  x += dx;

  move(y, x);

  addch(SELF);
}

static void run(int dy, int dx) {
  while (1) {
    int prev_y = y;
    int prev_x = x;

    step(dy, dx);

    if ((y == prev_y) && (x == prev_x)) {
      break;
    }
  }
}
