#include <stdio.h>
#define TAILLE 448

int nb[534];

int main (){
  FILE *f = fopen("LIAISONS", "r");
  char buff[50];
  int tab[TAILLE][2];
  int a, b;
  int i = 0;
  int j,k;
  while (fgets (buff, 50, f) != NULL ){
    sscanf(buff, "%d %d", &a, &b);
    tab[i][0] = a;
    tab[i][1] = b;
    i++;
  }
  for (i = 1; i <= 533; i++){
    printf("%d ", i);
    for (j = 0; j < TAILLE; j++){
      if (tab[j][0] == i){
	printf("%d ", tab[j][1]);
	nb[i]++;
      }
      else if (tab[j][1] == i){
	printf("%d ", tab[j][0]);
	nb[i]++;
      }
    }
    for (k = nb[i]; k < 6; k++)
      printf("-1 ");
    printf("\n");    
  }
 
  return 0;
}
    
