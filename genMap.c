#include <stdio.h>

int pass[533];
int tab[534][7];
int res[10000][5];
int cpt;
int chemin[3];

void PF(int start, int actuel, int indtab){
  int i;
  int next;
  if ((actuel >= 1) && (actuel <= 189)){
      res[cpt][0] = start;
      res[cpt][1] = actuel;
      for (i = 0; i < indtab; i++)
	res[cpt][2+i] = chemin[i];
      for (i = 0; i < 3 - indtab; i++)
	res[cpt][2+indtab+i] = -1;
      cpt++;
      return;
    }
  pass[actuel] = 1;
  chemin[indtab] = actuel;
  for (i = 1; i < 7; i++){
    next = tab[actuel][i];
    if ((next != -1) && (pass[next] == 0))
      PF(start, next, indtab+1);
  }
  pass[actuel] = 0;
}
  
  

int main(){
  FILE *f = fopen("carres", "r");
  char buff[50];
  int i = 1;
  int j;
  while (fgets (buff, 50, f) != NULL ){
    sscanf(buff, "%d %d %d %d %d %d %d", &tab[i][0],  &tab[i][1],  &tab[i][2],  &tab[i][3],  &tab[i][4],  &tab[i][5],  &tab[i][6]);
    i++;
  }
  for (i = 1; i <=189; i++){
    pass[i] = 1;
    for (j = 1; j < 7; j++)
      if (tab[i][j] != -1)
	PF(i, tab[i][j], 0);
    pass[i] = 0;
  }
  printf("int map[%d][5] = {", cpt);
  for (i = 0; i < cpt; i++)
    printf("{%d, %d, %d, %d, %d},\n", res[i][0], res[i][1], res[i][2], res[i][3], res[i][4]);
  printf("};\n");
  return 0;
}
  
  
