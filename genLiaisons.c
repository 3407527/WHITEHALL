#include <stdio.h>

int main(){
  FILE *f = fopen("carres", "r");
  char buff[50];
  printf("int liaisons[534][7] = {{0,-1,-1,-1,-1,-1,-1},\n");
  int i,j,k,l,m,n,o;
  while (fgets (buff, 50, f) != NULL ){
      sscanf(buff, "%d %d %d %d %d %d %d", &i, &j, &k, &l, &m, &n, &o);
      printf("{%d,%d,%d,%d,%d,%d,%d},\n", i, j, k, l, m, n, o);
  }
  printf("};\n");
  fclose(f);
  return 0;
}
