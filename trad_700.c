#include <stdio.h>

/* Pour déterminer quelle case se situe dans quel quartier. Génère le code */

int NW[] = {1,2,3,8,9,10,11,12,13,14,15,16,17,18,28,29,30,31,32,33,34,35,36,48,49,50,51,52,53,54,55,68,69,71,72};
int NE[] = {5,6,7,23,24,25,26,27,42,43,44,45,46,47,60,61,62,63,64,65,66,67,77,78,79,80};
int SW[] = {117,119,120,121,122,123,134,135,136,137,138,139,152,153,154,155,156,157,158,159,160,161,173,174,175,176,177,178,189};
int SE[] = {129,130,131,132,133,142,143,144,145,146,147,148,149,150,151,165,166,167,168,169,170,171,172,183,184,185,186,187,188};
int quartiers[190];

int main(){
  int i = 0;
  for (i = 0; i < 190; i++)
    quartiers[i] = 5;
  i = 0;
  while (NW[i] < 72){
    quartiers[NW[i]] = 1;
    i++;
  }
  quartiers[72] = 1;
  i = 0;
  while (NE[i] < 80){
    quartiers[NE[i]] = 2;
    i++;
  }
  quartiers[80] = 2;
  i = 0;
  while (SW[i] < 189){
    quartiers[SW[i]] = 3;
    i++;
  }
  quartiers[189] = 3;
  i = 0;
  while (SE[i] < 188){
    quartiers[SE[i]] = 4;
    i++;
  }
  quartiers[188] = 4;
  i = 0;
  printf("int quartiers[190] = {");
  for (i = 0; i < 190; i++)
    printf("%d,", quartiers[i]);
  printf("};\n");
  return 0;
}
