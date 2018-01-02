#include <stdio.h>

int tab[57][7];
int pass[50];
int NW[35] = {1,2,3,9,11,12,14,16,17,18,8,10,13,15,28,29,32,33,34,35,36,30,31,54,48,49,51,53,55,72,71,52,69,68,50};

void pathfinding(int i, int c, int v, int e){ // i = pt à chercher, c = point courant, v = point d'ou on vient, e = indice ou on va écrire dans le tableau
  int t;
  if ((c < 199) && (c >= 0) && (c != i) && (c != v)){ // si on est sur un cercle
    printf("%d %d P ", i, c);
    for (t = 0; t < e; t++)
      printf("%d ", pass[t]);
    printf("\n");
    return;
  }
  // si on est sur un carré
  int l;
  pass[e] = c;
  for (l = 1; l < 7; l++){
    if ((tab[c-200][l] != -1) && (tab[c-200][l] != v))
      pathfinding(i, tab[c-200][l], c, e+1);
  }
}
      
  
  

int main(){
  FILE *f = fopen("debut_liaisons", "r");
  FILE *F = fopen("MAP", "w");
  char buff[50];
  int ind = 0;
  int i,j,k;
  int cpt;
  while (fgets (buff, 50, f) != NULL ){
    sscanf(buff, "%d %d %d %d %d %d %d %d", &(tab[ind][0]), &(tab[ind][1]), &(tab[ind][2]), &(tab[ind][3]), &(tab[ind][4]), &(tab[ind][5]), &(tab[ind][6]));
    ind++;
  }
  for (i = 0; i < 35; i++){ // pour chaque point
    for (j = 0; j < 57; j++){
      for (k = 0; k < 7; k++){
	if (tab[j][k] == NW[i])
	  pathfinding(NW[i], tab[j][0], NW[i], 0);
      }
    }
  }
	  
    
    

  fclose(f);
  fclose(F);
  return 0;
}
