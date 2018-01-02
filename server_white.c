#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

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
int nbTour;
int indice;
int ind_j;
int ind_v;
int ind_b;
int ind_k;
int l;

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
  

  int quartiers[190] = {5,1,1,1,5,2,2,2,1,1,1,1,1,1,1,1,1,1,1,5,5,5,5,2,2,2,2,2,1,1,1,1,1,1,1,1,1,5,5,5,5,5,2,2,2,2,2,2,1,1,1,1,1,1,1,1,5,5,5,5,2,2,2,2,2,2,2,2,1,1,5,1,1,5,5,5,5,2,2,2,2,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,3,5,3,5,3,3,3,5,5,5,5,5,4,4,4,4,4,3,3,3,3,3,3,5,5,4,4,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3,3,3,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,3,3,5,5,5,5,4,4,4,4,4,4,3}; // la case d'indice i : 1 => NO, 2 => NE, 3 => SO, 4 => SE, 5 => case noire
  char cibles_quartier[4]; // permettra de vérifier que les quatres cibles sont de quartiers différents
  
  struct sockaddr_in serv_addr, cli_addr;
  int n;


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

	      broadcastMessage("T messagesent");
	      if (nbClients==4)
		fsmServer=1;
	    }
	  break;
	case 'K':
	  //fsmServer = 1 : Jack choisit les 4 points 
	  if(fsmServer == 1)
	    {
	      sscanf(buffer, "%c %d", &tmp, &indice);
	      if ((quartiers[indice] != 5) && (cibles_quartier[quartiers[indice] - 1] == 0){
		  ptKill[nbPts++] = indice;
		  cibles_quartier[quartiers[indice]] = 1;
		}
	      id=findClientByName(clientName);
	      sendMessageToClient(tcpClients[id].ipAddress, tcpClients[id].port,"T messageprisencompte\n");
	      if(nbPts == 4) 
		fsmServer = 2;
	    }

	  //fsmServer = 5 : Placement de Jack 
	  if(fsmServer == 5)
	    {
	      sscanf(buffer, "%c %d", &tmp, &indice);
	      ind_k = indice;
	      id=findClientByName(clientName);
	      sendMessageToClient(tcpClients[id].ipAddress, tcpClients[id].port,"T messageprisencompte\n");
	      if(nbPts == 4) 
		fsmServer = 6;
	    }
	  break;
	case 'J':
	  //fsmServer = 2 : Placement de policier jaune
	  if(fsmServer == 2)
	    {
	      sscanf(buffer, "%c %d", &tmp, &indice);
	      ind_j = indice;
	      id=findClientByName(clientName);
	      sendMessageToClient(tcpClients[id].ipAddress, tcpClients[id].port,"T messageprisencompte\n");
	      fsmServer = 3;
	    }
	  break;
	case 'V':
	  //fsmServer = 3 : Placement de policier vert
	  if(fsmServer == 3)
	    {
	      sscanf(buffer, "%c %d", &tmp, &indice);
	      ind_v = indice;
	      id=findClientByName(clientName);
	      sendMessageToClient(tcpClients[id].ipAddress, tcpClients[id].port,"T messageprisencompte\n");
	      fsmServer = 4;
	    }
	  break;
	case 'B':
	  //fsmServer = 4 : Placement de policier bleu
	  if(fsmServer == 4)
	    {
	      sscanf(buffer, "%c %d", &tmp, &indice);
	      ind_b = indice;
 	      id=findClientByName(clientName);
	      sendMessageToClient(tcpClients[id].ipAddress, tcpClients[id].port,"T messageprisencompte\n");
	      fsmServer = 5;
	    }
	  break;
	default:
	  break;
	}

      

      //fsmServer = 6 : Phase de jeu: Jack joue en premier, puis les policiers et chaque policier questionne le serveur pour savoir si Jack est passé par là, et peut lancer une accusation 
      if(fsmServer == 6)
	{
	  
	  nbTour++;
	  //nombre de tour max atteint: Jack a perdu, fin de partie
	  if(nbTour == 15)
	    {
	      exit(0);
	    }
	}

      //strcpy(ipaddress,inet_ntoa(cli_addr.sin_addr));
      //sendMessageToClient(ipaddress,"T messageprisencompte\n");	

      close(newsockfd);
    }
  close(sockfd);
  return 0; 
}
