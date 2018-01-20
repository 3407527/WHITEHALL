#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "liaisons.h"


struct _client
{
  char ipAddress[40];
  int port;
  char name[40];
} tcpClients[4];
int nbClients;
int fsmServer;
int nbPts;
int ptKill[4];
int carresJaunes[6] = {251, 252, 406, 341, 340, 408}; // A MODIFIER UNE FOIS LES CLICS FAITS
int nbTour;
int cibles_restantes = 4;
int feuille_route_jack[16];
int indice;
int ind_j;
int ind_v;
int ind_b;
int ind_k;

int quartiers[190] = {5,1,1,1,5,2,2,2,1,1,1,1,1,1,1,1,1,1,1,5,5,5,5,2,2,2,2,2,1,1,1,1,1,1,1,1,1,5,5,5,5,5,2,2,2,2,2,2,1,1,1,1,1,1,1,1,5,5,5,5,2,2,2,2,2,2,2,2,1,1,5,1,1,5,5,5,5,2,2,2,2,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,3,5,3,3,3,3,3,5,5,5,5,5,4,4,4,4,4,3,3,3,3,3,3,5,5,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,3,3,5,5,5,5,4,4,4,4,4,4,3}; // la case d'indice i : 1 => NO, 2 => NE, 3 => SO, 4 => SE, 5 => case noire
char cibles_quartier[4]; // permettra de vérifier que les quatres cibles sont de quartiers différents

void printClients()
{
  int i;

  for (i=0;i<nbClients;i++)
    printf("%d: %s %5.5d %s\n",i,tcpClients[i].ipAddress, 
	   tcpClients[i].port,
	   tcpClients[i].name);
}

int findClientByName(char *name)
{
  int i;

  for (i=0;i<nbClients;i++)
    if (strcmp(tcpClients[i].name,name)==0)
      return i;
  return -1;
}

void error(const char *msg)
{
  perror(msg);
  exit(1);
}

void sendMessageToClient(char *clientip,int clientport,char *mess)
{
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char buffer[256];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  server = gethostbyname(clientip);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
  serv_addr.sin_port = htons(clientport);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
      printf("ERROR connecting\n");
      exit(1);
    }

  sprintf(buffer,"%s\n",mess);
  n = write(sockfd,buffer,strlen(buffer));

  close(sockfd);
}

void broadcastMessage(char *mess)
{
  int i;

  for (i=0;i<nbClients;i++)
    sendMessageToClient(tcpClients[i].ipAddress, 
			tcpClients[i].port,
			mess);
}


void broadcastPolice(char *mess)
{
  int i;

  for (i=1;i<nbClients;i++)
    sendMessageToClient(tcpClients[i].ipAddress, 
			tcpClients[i].port,
			mess);
}

int est_present(int tab[], int e, int taille){
  int i;
  for (i = 0; i < taille; i++){
    if (tab[i] == e)
      return 1;
  }
  return 0;
}

int mouvementAutorise_Jack(int indice){
  int i,j,b;
  int test = 0;
  if (indice == ind_k)
    return 0;
  for (i = 0; i < 1576; i++){
    if (map[i][0] > ind_k)
      return 0;
    if ((map[i][0] == ind_k) && (map[i][1] == indice)){
      b = 1;
      test = 1;
      for (j = 2; j < 5; j++){
	if ((map[i][j] == ind_j) || (map[i][j] == ind_v) || (map[i][j] == ind_b))
	  b = 0;
      }
      if (b == 1)
	return 1;
    }
  }
  if (test == 0) // le tableau a été parcouru sans succès, ne se passe que si ind_k vaut 189
    return 0;
  else // l'indice est valide mais des policiers bloquent le chemin
    return -1;
}


int pass[533];

int mouvementAutorise_Police_rec_j(int actuel, int destination, int mov){
  int i, next;
  if ((actuel == destination) && (actuel != ind_v) && (actuel != ind_b)) // si on est arrivé et pas d'autre policier sur la case
    return 1;
  else if (mov >= 1){ // sinon, si on peut encore se déplacer
    pass[actuel] = 1;
    for (i = 1; i < 7; i++){ // on regarde chaque liaison partant de l'indice actuel
      next = liaisons[actuel][i];
      if ((next > 0) && (pass[next] != 1)){ // si existante et qu'on est pas passé par là
	if (next <= 189){ // si c'est un rond, on ne consomme pas de mouvement
	  if (mouvementAutorise_Police_rec_j(next, destination, mov)){ // si la destination est atteinte
	    pass[actuel] = 0;
	    return 1;
	  }
	}
	else {
	  if (mouvementAutorise_Police_rec_j(next, destination, mov - 1)){
	    pass[actuel] = 0;
	    return 1;
	  }
	}
      }
    }
    pass[actuel] = 0;
  }
  return 0;
}

int mouvementAutorise_Police_rec_v(int actuel, int destination, int mov){
  int i, next;
  if ((actuel == destination) && (actuel != ind_j) && (actuel != ind_b))
    return 1;
  else if (mov >= 1){
    pass[actuel] = 1;
    for (i = 1; i < 7; i++){
      next = liaisons[actuel][i];
      if ((next > 0) && (pass[next] != 1)){
	if (next <= 189){
	  if (mouvementAutorise_Police_rec_v(next, destination, mov)){
	    pass[actuel] = 0;
	    return 1;
	  }
	}
	else {
	  if (mouvementAutorise_Police_rec_v(next, destination, mov - 1)){
	    pass[actuel] = 0;
	    return 1;
	  }
	}
      }
    }
    pass[actuel] = 0;
  }
  return 0;
}

int mouvementAutorise_Police_rec_b(int actuel, int destination, int mov){
  int i, next;
  if ((actuel == destination) && (actuel != ind_v) && (actuel != ind_j))
    return 1;
  else if (mov >= 1){
    pass[actuel] = 1;
    for (i = 1; i < 7; i++){
      next = liaisons[actuel][i];
      if ((next > 0) && (pass[next] != 1)){
	if (next <= 189){
	  if (mouvementAutorise_Police_rec_b(next, destination, mov)){
	    pass[actuel] = 0;
	    return 1;
	  }
	}
	else {
	  if (mouvementAutorise_Police_rec_b(next, destination, mov - 1)){
	    pass[actuel] = 0;
	    return 1;
	  }
	}
      }
    }
    pass[actuel] = 0;
  }
  return 0;
}
		  
int mouvementAutorise_Police(int destination, int joueur){
  if (destination <= 200)
    return 0;
  if (joueur == 1)
    return mouvementAutorise_Police_rec_j(ind_j,destination, 2);
  if (joueur == 2)
    return mouvementAutorise_Police_rec_v(ind_v,destination, 2);
  else
    return mouvementAutorise_Police_rec_b(ind_b,destination, 2);
}


int traceJack(int indice){
  int i;
  for (i = 0; i < nbTour + 1; i++)
    if (feuille_route_jack[i] == indice)
      return indice;
  return 0;
}

void fin_de_tour(){
  int i;
  char reply[256];
  if (est_present(ptKill, ind_k, 4)){ // si meurtre
    sprintf(reply, "M %d", ind_k);
    broadcastMessage(reply);
    cibles_restantes--;
    if(cibles_restantes == 0){ // si dernière cible
      broadcastMessage("T Jack a gagné! Fin de la partie!");
      fsmServer=19;
    }
    else { // sinon on commence un nouveau round
      nbTour = 1;
      broadcastMessage("N 1");
      feuille_route_jack[0] = ind_k;
      for (i = 1; i < 17; i++)
	feuille_route_jack[i] = 0;
      broadcastMessage("T Jack se déplace.");
      fsmServer= 6;
    }
  }
  else{ // si pas de meurtre
    if (nbTour == 15){ // si dernier tour
      broadcastMessage("T Jack n'a pas éliminé une cible à temps! Les policiers ont gagné!");
      fsmServer = 19;
    }
    else { // sinon on continue normalement
      nbTour++;
      sprintf(reply, "N %d", nbTour);
      broadcastMessage(reply);
      broadcastMessage("T Jack se déplace.");
      fsmServer = 6;
    }
  }
}

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
  char reply[256];

  char com;
  char clientIpAddress[256], clientName[256];
  int clientPort;
  int id;
  char tmp;
  char joueur;

  
  // tableaux pour représenter les ronds voisins de chaque policer, et un tableau contenant les tests de la présence de preuve a l indice correspondant, et leur longueur
  int *vj = (int *) malloc (sizeof(int));
  int *vjt = (int *) malloc (sizeof(int));
  int *vv = (int *) malloc (sizeof(int));
  int *vvt = (int *) malloc (sizeof(int));
  int *vb = (int *) malloc (sizeof(int));
  int *vbt = (int *) malloc (sizeof(int));
  int lj;
  int lv;
  int lb;
  int nbtj;
  int nbtv;
  int nbtb;

  int *tmp_alloc;
  
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  int i; //boucles
  int trace = 0;

  nbClients=0;
  fsmServer=0;

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
  
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
	   sizeof(serv_addr)) < 0) 
    error("ERROR on binding");
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  while (1)
    {    
      newsockfd = accept(sockfd, 
			 (struct sockaddr *) &cli_addr, 
			 &clilen);
      if (newsockfd < 0) 
	error("ERROR on accept");

      bzero(buffer,256);
      n = read(newsockfd,buffer,255);
      if (n < 0) 
	error("ERROR reading from socket");

      printf("Received packet from %s:%d\nData: [%s]\n\n",
	     inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), buffer);

      printf("buffer[0]=%c\n",buffer[0]);

      printf("ETAT : %d\n", fsmServer);

      switch (buffer[0])
	{
	case 'C':
	  sscanf(buffer,"%c %s %d %s",
		 &com, clientIpAddress, &clientPort, clientName);
	  printf("COM=%c ipAddress=%s port=%d name=%s\n",com, clientIpAddress, clientPort, clientName);
			
	  // fsmServer==0 alors j'attends les connexions de tous les joueurs
	  if (fsmServer==0)
	    {
	      strcpy(tcpClients[nbClients].ipAddress,clientIpAddress);
	      tcpClients[nbClients].port=clientPort;
	      strcpy(tcpClients[nbClients].name,clientName);
	      nbClients++;

	      printClients();

	      id=findClientByName(clientName);
	      printf("id=%d\n",id);
	      sprintf(reply,"I %d",id);
	      sendMessageToClient(tcpClients[id].ipAddress,
				  tcpClients[id].port,
				  reply);	

	      if (nbClients==4){
		broadcastMessage("N O");
		broadcastMessage("T Jack choisit quatre cibles");
		fsmServer=1;
	      }
	    }
	  break;
	case 'K':
	  //fsmServer = 1 : Jack choisit les 4 points 
	  if(fsmServer == 1)
	    {
	      sscanf(buffer, "%c %d", &tmp, &indice);
	      printf("%d %d\n", quartiers[indice], cibles_quartier[quartiers[indice] - 1]);
	      if ((indice <= 189) && (quartiers[indice] != 5) && (cibles_quartier[quartiers[indice] - 1] == 0)){
		  ptKill[nbPts++] = indice;
		  sprintf(reply, "X %d", indice);
		  sendMessageToClient(tcpClients[0].ipAddress, tcpClients[0].port, reply);
		  cibles_quartier[quartiers[indice] - 1] = 1;
		  printf("%d %d %d %d\n", ptKill[0], ptKill[1], ptKill[2], ptKill[3]);
		}
	      else
		sendMessageToClient(tcpClients[0].ipAddress, tcpClients[0].port,"E Veuillez séléctionner une clible valide.");
	      //sendMessageToClient(tcpClients[0].ipAddress, tcpClients[0].port,"T messageprisencompte\n");
	      if(nbPts == 4) {
		broadcastMessage("T Le policier Jaune choisit son point de départ");
		fsmServer = 2;
	      }
	    }

	  //fsmServer = 5 : Placement de Jack 
	  if(fsmServer == 5)
	    {
	      sscanf(buffer, "%c %d", &tmp, &indice);
	      if ((indice <= 190) && ((indice == ptKill[0]) || (indice == ptKill[1]) || (indice == ptKill[2]) || (indice == ptKill[3]))){
		ind_k = indice;
		feuille_route_jack[0] = ind_k;
		//sendMessageToClient(tcpClients[0].ipAddress, tcpClients[0].port,"T Pointdedépartvalide");
		sprintf(reply,"M %d",ind_k);
		broadcastMessage(reply);
		sprintf(reply,"K %d",ind_k);
		sendMessageToClient(tcpClients[0].ipAddress, tcpClients[0].port, reply);
		cibles_restantes = 3;
		broadcastMessage("N 1");
		nbTour++;
		broadcastMessage("T Jack se déplace.");
		fsmServer = 6;
	      }
	      else
		sendMessageToClient(tcpClients[0].ipAddress, tcpClients[0].port,"E Veuillez séléctionner une des cibles.");
	    }
	  //fsmServer = 6 : Déplacement de Jack : Escape in the Night
	  if(fsmServer == 6)
	    {
	      sscanf(buffer, "%c %d", &tmp, &indice);
	      if (mouvementAutorise_Jack(indice) > 0){
		ind_k = indice;
		feuille_route_jack[nbTour] = ind_k;
		sprintf(reply,"K %d",ind_k);
		sendMessageToClient(tcpClients[0].ipAddress, tcpClients[0].port, reply);
		broadcastMessage("T Le policier Jaune se déplace.");
		fsmServer = 7;
	      }
	      else
		sendMessageToClient(tcpClients[0].ipAddress, tcpClients[0].port,"E Veuillez séléctionner une case valide.");
	    }
	  break;
	case 'J':
	  //fsmServer = 2 : Placement de policier jaune
	  if(fsmServer == 2)
	    {
	      sscanf(buffer, "%c %d", &tmp, &indice);
	      if (est_present(carresJaunes, indice, 6)){
		ind_j = indice;
		sprintf(reply,"J %d",ind_j);
		broadcastMessage(reply);
		broadcastMessage("T Le policier Vert choisit son point de départ");
		//sendMessageToClient(tcpClients[1].ipAddress, tcpClients[1].port,"T messageprisencompte\n");
		fsmServer = 3;
	      }
	      else
		sendMessageToClient(tcpClients[1].ipAddress, tcpClients[1].port,"E Non valide : Veuillez séléctionner l'un des carrés jaunes.");
	    }
	  //fsmServer = 7 : Deplacement du policier Jaune : Hunting the Monster
	  if (fsmServer == 7){
	    sscanf(buffer, "%c %d", &tmp, &indice);
	    if (mouvementAutorise_Police(indice, 1)){
	      ind_j = indice;
	      sprintf(reply,"J %d",ind_j);
	      broadcastMessage(reply);
	      lj = 0;
	      nbtj = 0;
	      for (i = 1; i <= 7; i++) // on regarde combien de cercles sont voisins
		if ((liaisons[ind_j][i] <= 189) && (liaisons[ind_j][i] >= 1))
		  lj++;
	      tmp_alloc = realloc(vj, lj);
	      vj = tmp_alloc;
	      tmp_alloc = realloc(vjt,lj);
	      vjt = tmp_alloc;
	      lj = 0;
	      for (i = 1; i <= 7; i++) // on crée les tableaux associés
		if ((liaisons[ind_j][i] <= 189) && (liaisons[ind_j][i] >= 1)){ 
		  vj[lj] = liaisons[ind_j][i];
		  vjt[lj] = 0;
		  lj++;
		}	      
	      broadcastMessage("T Le policier Vert se déplace.");
	      fsmServer = 8;
	    }
	    else
	      sendMessageToClient(tcpClients[1].ipAddress, tcpClients[1].port,"E Veuillez séléctionner une case valide.");
	  }
	  //fsmServer = 11 : Jaune recherche des indices
	  if (fsmServer == 11){
	    sscanf(buffer, "%c %d", &tmp, &indice);
	    if((indice <= 189) && (est_present(liaisons[ind_j], indice, 7))){
	      trace = traceJack(indice);
	      if (trace){
		sprintf(reply, "S %d", trace);
		broadcastMessage(reply);
		broadcastMessage("Le policier Vert choisit une action à effectuer");
		fsmServer = 13;
	      }
	      // A COMPLETER : Si pas de trace alors...
	      else {
		for (i = 0; i < lj; i++) // on cherche la position de indice dans le tableau des voisins
		  if (vj[i] == indice){
		    break;
		  }
		if (vjt[i] == 1) // Si cet indice a déja été testé, on envoie une erreur
		  sendMessageToClient(tcpClients[1].ipAddress, tcpClients[1].port,"E Cette case a déjà été vérifiée.");
		else{ // sinon, on marque qu'il a été testé et on vérifie si tous les voisins ont été verifiés sans succès pour passer au tour suivant
		  vjt[i] = 1;
		  nbtj++;
		  if (nbtj == lj){
		    broadcastMessage("Le policier Vert choisit une action à effectuer");
		    fsmServer = 13;
		  }
		}
	      }		
	    }
	    else
	      sendMessageToClient(tcpClients[1].ipAddress, tcpClients[1].port,"E Veuillez séléctionner une case valide.");
	  }
	  //fsmServer = 12 : Jaune effectue une arrestation
	  if (fsmServer == 12){
	    sscanf(buffer, "%c %d", &tmp, &indice);
	    if((indice <= 189) && (est_present(liaisons[ind_j], indice, 7))){
	      if (indice != ind_k){
		sprintf(reply, "T Jack n'est pas en %d.", indice);
		broadcastMessage(reply);
		broadcastMessage("T Le policier Vert choisit une action à effectuer.");
		fsmServer = 13;
		  }
	      else{
		sprintf(reply, "T Jack a été arrêté par le policier Jaune en %d! Fin de la partie !", indice);
		broadcastMessage(reply);
		fsmServer = 19;
	      }
	    }
	    else
	      sendMessageToClient(tcpClients[1].ipAddress, tcpClients[1].port,"E Veuillez séléctionner une case valide.");
	  } 
	  break;
	case 'V':
	  //fsmServer = 3 : Placement de policier vert
	  if(fsmServer == 3)
	    {
	      sscanf(buffer, "%c %d", &tmp, &indice);
	      if ((indice != ind_j) && (est_present(carresJaunes, indice, 6))){
		ind_v = indice;
		sprintf(reply,"V %d",ind_v);
		broadcastMessage(reply);
		broadcastMessage("T Le policier Bleu choisit son point de départ");	      
		fsmServer = 4;
	      }
	      else
		sendMessageToClient(tcpClients[2].ipAddress, tcpClients[2].port,"E Non valide : Veuillez séléctionner l'un des carrés jaunes.");
	    }
	  //fsmServer = 8 : Deplacement du policier Vert : Hunting the Monster
	  if (fsmServer == 8){
	    sscanf(buffer, "%c %d", &tmp, &indice);
	    if (mouvementAutorise_Police(indice, 2)){
	      ind_v = indice;
	      sprintf(reply,"V %d",ind_v);
	      broadcastMessage(reply);
	      lv = 0;
	      nbtv = 0;
	      for (i = 1; i <= 7; i++) // on regarde combien de cercles sont voisins
		if ((liaisons[ind_v][i] <= 189) && (liaisons[ind_v][i] >= 1))
		  lv++;
	      tmp_alloc = realloc(vv, lv);
	      vv = tmp_alloc;
	      tmp_alloc = realloc(vvt,lv);
	      vvt = tmp_alloc;
	      lv = 0;
	      for (i = 1; i <= 7; i++) // on crée les tableaux associés
		if ((liaisons[ind_v][i] <= 189) && (liaisons[ind_v][i] >= 1)){ 
		  vv[lv] = liaisons[ind_v][i];
		  vvt[lv] = 0;
		  lv++;
		}
	      broadcastMessage("T Le policier Bleu se déplace.");
	      fsmServer = 9;
	    }
	    else
	      sendMessageToClient(tcpClients[2].ipAddress, tcpClients[2].port,"E Veuillez séléctionner une case valide.");
	  }
	  //fsmServer = 14 : Vert recherche des indices
	  if (fsmServer == 14){
	    sscanf(buffer, "%c %d", &tmp, &indice);
	    if((indice <= 189) && (est_present(liaisons[ind_v], indice, 7))){
	      trace = traceJack(indice);
	      if (trace){
		sprintf(reply, "S %d", trace);
		broadcastMessage(reply);
		broadcastMessage("Le policier Bleu choisit une action à effectuer");
		fsmServer = 16;
	      }
	      // A COMPLETER : Si pas de trace alors...
	      else {
		for (i = 0; i < lv; i++) // on cherche la position de indice dans le tableau des voisins
		  if (vv[i] == indice){
		    break;
		  }
		if (vvt[i] == 1) // Si cet indice a déja été testé, on envoie une erreur
		  sendMessageToClient(tcpClients[2].ipAddress, tcpClients[2].port,"E Cette case a déjà été vérifiée.");
		else{ // sinon, on marque qu'il a été testé et on vérifie si tous les voisins ont été verifiés sans succès pour passer au tour suivant
		  vvt[i] = 1;
		  nbtv++;
		  if (nbtv == lv){
		    broadcastMessage("Le policier Bleu choisit une action à effectuer");
		    fsmServer = 16;
		  }
		}
	      }		
	    }
	    else
	      sendMessageToClient(tcpClients[1].ipAddress, tcpClients[1].port,"E Veuillez séléctionner une case valide.");
	  }
	  //fsmServer = 15 : Vert effectue une arrestation
	  if (fsmServer == 15){
	    sscanf(buffer, "%c %d", &tmp, &indice);
	    if((indice <= 189) && (est_present(liaisons[ind_v], indice, 7))){
	      if (indice != ind_k){
		sprintf(reply, "T Jack n'est pas en %d.", indice);
		broadcastMessage(reply);
		broadcastMessage("T Le policier Bleu choisit une action à effectuer.");
		fsmServer = 16;
		  }
	      else{
		sprintf(reply, "T Jack a été arrêté par le policier Vert en %d! Fin de la partie !", indice);
		broadcastMessage(reply);
		fsmServer = 19;
	      }
	    }
	    else
	      sendMessageToClient(tcpClients[2].ipAddress, tcpClients[2].port,"E Veuillez séléctionner une case valide.");
	  } 
	  break;
	case 'B':
	  //fsmServer = 4 : Placement de policier bleu
	  if(fsmServer == 4)
	    {
	      sscanf(buffer, "%c %d", &tmp, &indice);
	      if ((indice != ind_j) && (indice != ind_v) && (est_present(carresJaunes, indice, 6))){
		ind_b = indice;
		sprintf(reply,"B %d",ind_b);
		broadcastMessage(reply);
		broadcastMessage("T Jack choisit son point de départ");	      
		fsmServer = 5;
	      }
	      else
		sendMessageToClient(tcpClients[3].ipAddress, tcpClients[3].port,"E Non valide : Veuillez séléctionner l'un des carrés jaunes.");
	    }
	  //fsmServer = 9 : Deplacement du policier Bleu : Hunting the Monster
	  if (fsmServer == 9){
	    sscanf(buffer, "%c %d", &tmp, &indice);
	    if (mouvementAutorise_Police(indice, 3)){
	      ind_b = indice;
	      sprintf(reply,"B %d",ind_b);
	      broadcastMessage(reply);
	      lb = 0;
	      nbtb = 0;
	      for (i = 1; i <= 7; i++) // on regarde combien de cercles sont voisins
		if ((liaisons[ind_b][i] <= 189) && (liaisons[ind_b][i] >= 1))
		  lb++;
	      tmp_alloc = realloc(vb, lb);
	      vb = tmp_alloc;
	      tmp_alloc = realloc(vbt,lb);
	      vbt = tmp_alloc;
	      lb = 0;
	      for (i = 1; i <= 7; i++) // on crée les tableaux associés
		if ((liaisons[ind_b][i] <= 189) && (liaisons[ind_b][i] >= 1)){
		  vb[lb] = liaisons[ind_b][i];
		  vbt[lb] = 0;
		  lb++;
		}
	      broadcastMessage("T Le policier Jaune choisit une action à effectuer.");
	      fsmServer = 10;
	    }
	    else
	      sendMessageToClient(tcpClients[3].ipAddress, tcpClients[3].port,"E Veuillez séléctionner une case valide.");
	  }
	  //fsmServer = 17 : Bleu recherche des indices
	  if (fsmServer == 17){
	    sscanf(buffer, "%c %d", &tmp, &indice);
	    if((indice <= 189) && (est_present(liaisons[ind_b], indice, 7))){
	      trace = traceJack(indice);
	      if (trace){
		sprintf(reply, "S %d", trace);
		broadcastMessage(reply);
		fin_de_tour();
	      }
	      // A COMPLETER : Si pas de trace alors...
	      else {
		for (i = 0; i < lb; i++) // on cherche la position de indice dans le tableau des voisins
		  if (vb[i] == indice){
		    break;
		  }
		if (vbt[i] == 1) // Si cet indice a déja été testé, on envoie une erreur
		  sendMessageToClient(tcpClients[3].ipAddress, tcpClients[3].port,"E Cette case a déjà été vérifiée.");
		else{ // sinon, on marque qu'il a été testé et on vérifie si tous les voisins ont été verifiés sans succès pour passer au tour suivant
		  vbt[i] = 1;
		  nbtb++;
		  if (nbtb == lb){
		    fin_de_tour();
		  }
		}
	      }		
	    }
	    else
	      sendMessageToClient(tcpClients[1].ipAddress, tcpClients[1].port,"E Veuillez séléctionner une case valide.");
	  }
	  //fsmServer = 18 : Bleu effectue une arrestation
	  if (fsmServer == 18){
	    sscanf(buffer, "%c %d", &tmp, &indice);
	    if((indice <= 189) && (est_present(liaisons[ind_b], indice, 7))){ // si l'indice est valide
	      if (indice != ind_k){ // si pas d'arrestation
		sprintf(reply, "T Jack n'est pas en %d.", indice);
		broadcastMessage(reply);
		if (est_present(ptKill, ind_k, 4)){ // si meurtre
		  sprintf(reply, "M %d", ind_k);
		  broadcastMessage(reply);
		  cibles_restantes--;
		  if(cibles_restantes == 0){ // si dernière cible
		    broadcastMessage("T Jack a gagné! Fin de la partie!");
		    fsmServer=19;
		  }
		  else { // sinon on commence un nouveau round
		    nbTour = 1;
		    broadcastMessage("N 1");
		    feuille_route_jack[0] = ind_k;
		    for (i = 1; i < 17; i++)
		      feuille_route_jack[i] = 0;
		    broadcastMessage("T Jack se déplace.");
		    fsmServer= 6;
		  }
		}
		else{ // si pas de meurtre
		  if (nbTour == 15){ // si dernier tour
		    broadcastMessage("T Jack n'a pas éliminé une cible à temps! Les policiers ont gagné!");
		    fsmServer = 19;
		  }
		  else { // sinon on continue normalement
		    nbTour++;
		    sprintf(reply, "N %d", nbTour);
		    broadcastMessage(reply);
		    broadcastMessage("T Jack se déplace.");
		    fsmServer = 6;
		  }
		}	    
	      }
	      else{ // sinon Jack est arrêté
		sprintf(reply, "T Jack a été arrêté par le policier Bleu en %d! Fin de la partie !", indice);
		broadcastMessage(reply);
		fsmServer = 19;
	      }
	    }
	    else // sinon la case n'est pas valide
	      sendMessageToClient(tcpClients[3].ipAddress, tcpClients[3].port,"E Veuillez séléctionner une case valide.");
	  }
	  break;
	case 'L':
	  sscanf(buffer, "%c %c", &tmp, &joueur);
	  // fsmServer = 10 : Le policier jaune choisit une action : Clues and Suspicion
	  if ((fsmServer == 10) && (joueur = 'J')){
	    broadcastMessage("T Le policier Jaune cherche des indices.");
	    fsmServer = 11;
	  }
	  // fsmServer = 13 : Le policier vert choisit une action : Clues and Suspicion
	  if ((fsmServer == 13) && (joueur = 'V')){
	    broadcastMessage("T Le policier Vert cherche des indices.");
	    fsmServer = 14;
	  }
	  // fsmServer = 16 : Le policier bleu choisit une action : Clues and Suspicion
	  if ((fsmServer == 16) && (joueur = 'B')){
	    broadcastMessage("T Le policier Bleu cherche des indices.");
	    fsmServer = 17;
	  }
	  break;
	case 'A':
	  sscanf(buffer, "%c %c", &tmp, &joueur);
	  // fsmServer = 10 : Le policier jaune choisit une action : Clues and Suspicion
	  if ((fsmServer == 10) && (joueur = 'J')){
	    broadcastMessage("T Le policier Jaune effectue une arrestation.");
	    fsmServer = 12;
	  }
	  // fsmServer = 13 : Le policier vert choisit une action : Clues and Suspicion
	  if ((fsmServer == 13) && (joueur = 'V')){
	    broadcastMessage("T Le policier Vert effectue une arrestation.");
	    fsmServer = 15;
	  }
	  // fsmServer = 16 : Le policier bleu choisit une action : Clues and Suspicion
	  if ((fsmServer == 16) && (joueur = 'B')){
	    broadcastMessage("T Le policier Bleu effectue une arrestation.");
	    fsmServer = 18;
	  }
	  break;
	default:
	  break;
	}
      printf("ETAT SUIVANT : %d\n", fsmServer);
    }

      

    /*   //fsmServer = 6 : Phase de jeu: Jack joue en premier, puis les policiers et chaque policier questionne le serveur pour savoir si Jack est passé par là, et peut lancer une accusation  */
    /*   if(fsmServer == 6) */
    /* 	{ */
	  
    /* 	  nbTour++; */
    /* 	  //nombre de tour max atteint: Jack a perdu, fin de partie */
    /* 	  if(nbTour == 15) */
    /* 	    { */
    /* 	      exit(0); */
    /* 	    } */
    /* 	} */

    /*   //strcpy(ipaddress,inet_ntoa(cli_addr.sin_addr)); */
    /*   //sendMessageToClient(ipaddress,"T messageprisencompte\n");	 */

    /*   close(newsockfd); */
    /* } */
  close(sockfd);
  free(vj);
  free(vv);
  free(vb);
  free(vjt);
  free(vvt);
  free(vbt);
  return 0; 
}
