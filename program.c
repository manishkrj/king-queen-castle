#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
   int row_idx;
   int col_idx;
} Pair;

typedef struct {
   int m, n;
   int **horizontal_walls;
   int **vertical_walls;
   Pair **parents;
} Maze;

typedef int **Path;

Maze initializeMaze(int m, int n)
{
   Maze M;
   int i, j;

   M.m = m; M.n = n;

   M.horizontal_walls = (int **)malloc((m - 1) * sizeof(int *));
   for (i = 0; i < m - 1; ++i) {
      M.horizontal_walls[i] = (int *)malloc(n * sizeof(int));
      for (j = 0; j < n; ++j) M.horizontal_walls[i][j] = 1;
   }

   M.vertical_walls = (int **)malloc(m * sizeof(int *));
   for (i = 0; i < m; ++i) {
      M.vertical_walls[i] = (int *)malloc((n - 1) * sizeof(int));
      for (j = 0; j < n - 1; ++j) M.vertical_walls[i][j] = 1;
   }

   M.parents = (Pair **)malloc(m * sizeof(Pair *));
   for (i = 0; i < m; ++i) {
      M.parents[i] = (Pair *)malloc(n * sizeof(Pair));
      for (j = 0; j < n; ++j) M.parents[i][j] = (Pair){-1, -1};
   }

   return M;
}

char printCell(int t)
{
   if (t == 1) return 'x';
   if (t == 2) return 'K';
   if (t == 3) return 'Q';
   return ' ';
}

void printMaze(Maze M, int **path)
{
   int i, j;

   printf("    +");
   for (j = 0; j < M.n; ++j) printf("---+");
   printf("\n");
   for (i = 0; i < M.m; ++i) {
      printf("    |");
      if (path == NULL) {
         for (j = 0; j < M.n - 1; ++j) printf("   %c", M.vertical_walls[i][j] ? '|' : ' ');
         printf("   |\n");
      } else {
         for (j = 0; j < M.n - 1; ++j)
            printf(" %c %c", printCell(path[i][j]), M.vertical_walls[i][j] ? '|' : ' ');
         printf(" %c |\n", printCell(path[i][M.n - 1]));
      }
      if (i != M.m - 1) {
         printf("    +");
         for (j = 0; j < M.n; ++j) printf("%s+", M.horizontal_walls[i][j] ? "---" : "   ");
         printf("\n");
      }
   }
   printf("    +");
   for (j = 0; j < M.n; ++j) printf("---+");
   printf("\n");
}

void depthFirstSearch(Maze M, int i, int j, int **visited)
{
   int direction, k, m, n, u, v;

   visited[i][j] = 1;

   m = M.m; n = M.n;
   direction = rand() % 4;
   for (k = 0; k < 4; ++k) {
      switch (direction) {
         case 0: u = i; v = j - 1; break;
         case 1: u = i - 1; v = j; break;
         case 2: u = i; v = j + 1; break;
         case 3: u = i + 1; v = j; break;
      }
      if ((u >= 0) && (u < m) && (v >= 0) && (v < n) && (visited[u][v] == 0)) {
         M.parents[u][v] = (Pair){i, j};
         switch (direction) {
            case 0: M.vertical_walls[u][v] = 0; break;
            case 1: M.horizontal_walls[u][v] = 0; break;
            case 2: M.vertical_walls[i][j] = 0; break;
            case 3: M.horizontal_walls[i][j] = 0; break;
         }
         depthFirstSearch(M, u, v, visited);
      }
      ++direction;
      direction %= 4;
   }
}

void generateMaze(Maze M)
{
   int m, n, i, j;
   int **visited;

   m = M.m; n = M.n;
   visited = (int **)malloc(m * sizeof(int *));
   for (i = 0; i < m; ++i) {
      visited[i] = (int *)malloc(n * sizeof(int));
      for (j = 0; j < n; ++j) visited[i][j] = 0;
   }

   i = rand() % m; j = rand() % n;
   depthFirstSearch(M, i, j, visited);
   for (i = 0; i < m; ++i) free(visited[i]);
   free(visited);
}

Path generatePath(Maze M, int u, int v, int x, int y)
{
   int m, n, i, j;
   Path p;
   Pair t;

   m = M.m; n = M.n;
   p = (int **)malloc(m * sizeof(int *));
   for (i = 0; i < m; ++i) {
      p[i] = (int *)malloc(n * sizeof(int));
      for (j = 0; j < n; ++j) p[i][j] = 0;
   }

   t = (Pair){u, v};
   while (t.row_idx >= 0) {
      p[t.row_idx][t.col_idx] = 1;
      t = M.parents[t.row_idx][t.col_idx];
   }

   t = (Pair){x, y};
   while (p[t.row_idx][t.col_idx] == 0) {
      p[t.row_idx][t.col_idx] = 1;
      t = M.parents[t.row_idx][t.col_idx];
   }

   t = M.parents[t.row_idx][t.col_idx];
   while (t.row_idx >= 0) {
      p[t.row_idx][t.col_idx] = 0;
      t = M.parents[t.row_idx][t.col_idx];
   }

   return p;
}

int main(int argc, char *argv[])
{
   int m, n, u, v, x, y;
   m=n=u=v=x=y=-1;
   Maze M;
   Path p;

   srand((unsigned int)time(NULL));

   if (argc >= 7) { m = atoi(argv[1]); n = atoi(argv[2]); u = atoi(argv[3]); v = atoi(argv[4]); x = atoi(argv[5]); y = atoi(argv[6]); }
   if (argc >= 5) { m = atoi(argv[1]); n = atoi(argv[2]); u = atoi(argv[3]); v = atoi(argv[4]); }
   if (argc >= 3) { m = atoi(argv[1]); n = atoi(argv[2]); }
   else
   {
      printf("Enter rows: ");scanf("%d", &m);
      printf("Enter columns: ");scanf("%d", &n);
   }
   printf("m = %d\nn = %d\n", m, n);

   M = initializeMaze(m, n);
   printf("\n+++ Initializing maze\n");
   printMaze(M, NULL);

   generateMaze(M);
   printf("\n+++ Generating random maze\n");
   printMaze(M, NULL);

   do
   {
      if(u==-1)printf("Enter King's position (x,y): ");scanf("%d%d",&u,&v);
      if(x==-1)printf("Enter Queen's position (x,y): ");scanf("%d%d",&x,&y);
      if(u==x&&v==y)
      {
         printf("They are in same castle.\n");
         return 0;
      }
   }
   while(u<0||u>=m||v<0||v>=n||x<0||x>=m||y<0||y>=n);
   p = generatePath(M, u, v, x, y);
   p[u][v] = 2; p[x][y] = 3;
   printf("\n+++ Path from King = (%d,%d) to Queen = (%d,%d)\n", u, v, x, y);
   printMaze(M, p);
   printf("Reached!!");

   for (u = 0; u < m; ++u) {
      if (u != m - 1) free(M.horizontal_walls[u]);
      free(M.vertical_walls[u]);
      free(M.parents[u]);
      free(p[u]);
   }
   free(M.horizontal_walls); free(M.vertical_walls); free(M.parents); free(p);

   exit(0);
}
