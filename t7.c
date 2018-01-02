#include <SDL.h>        
#include <SDL_image.h>        
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

volatile int synchro;
char gbuffer[256];
char gServerIpAddress[256];
int gServerPort;
char gClientIpAddress[256];
int gClientPort;
char gName[256];
int gId;
struct _poi
{
  int x;
  int y;
} poi[200], poic[300];
int indice;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void initPOI()
{
  poi[0].x=-1; poi[0].y=-1;
  poi[1].x=95; poi[1].y=71;
  poi[2].x=282; poi[2].y=54;
  poi[3].x=383; poi[3].y=62;
  poi[4].x=535; poi[4].y=59;
  poi[5].x=611; poi[5].y=59;
  poi[6].x=718; poi[6].y=60;
  poi[7].x=880; poi[7].y=87;
  poi[8].x=54; poi[8].y=125;
  poi[9].x=120; poi[9].y=89;
  poi[10].x=136; poi[10].y=136;
  poi[11].x=162; poi[11].y=100;
  poi[12].x=194; poi[12].y=98;
  poi[13].x=229; poi[13].y=156;
  poi[14].x=259; poi[14].y=99;
  poi[15].x=289; poi[15].y=162;
  poi[16].x=303; poi[16].y=117;
  poi[17].x=344; poi[17].y=117;
  poi[18].x=379; poi[18].y=124;
  poi[19].x=444; poi[19].y=140;
  poi[20].x=541; poi[20].y=92;
  poi[21].x=555; poi[21].y=155;
  poi[22].x=600; poi[22].y=108;
  poi[23].x=658; poi[23].y=95;
  poi[24].x=715; poi[24].y=150;
  poi[25].x=792; poi[25].y=101;
  poi[26].x=899; poi[26].y=167;
  poi[27].x=960; poi[27].y=112;
  poi[28].x=76; poi[28].y=201;
  poi[29].x=127; poi[29].y=177;
  poi[30].x=133; poi[30].y=229;
  poi[31].x=180; poi[31].y=237;
  poi[32].x=217; poi[32].y=202;
  poi[33].x=264; poi[33].y=213;
  poi[34].x=313; poi[34].y=218;
  poi[35].x=365; poi[35].y=218;
  poi[36].x=396; poi[36].y=250;
  poi[37].x=427; poi[37].y=188;
  poi[38].x=476; poi[38].y=174;
  poi[39].x=479; poi[39].y=235;
  poi[40].x=522; poi[40].y=222;
  poi[41].x=561; poi[41].y=200;
  poi[42].x=620; poi[42].y=195;
  poi[43].x=679; poi[43].y=251;
  poi[44].x=694; poi[44].y=196;
  poi[45].x=789; poi[45].y=174;
  poi[46].x=794; poi[46].y=238;
  poi[47].x=950; poi[47].y=202;
  poi[48].x=84; poi[48].y=260;
  poi[49].x=106; poi[49].y=289;
  poi[50].x=108; poi[50].y=326;
  poi[51].x=181; poi[51].y=311;
  poi[52].x=247; poi[52].y=341;
  poi[53].x=264; poi[53].y=303;
  poi[54].x=286; poi[54].y=273;
  poi[55].x=316; poi[55].y=336;
  poi[56].x=426; poi[56].y=273;
  poi[57].x=479; poi[57].y=313;
  poi[58].x=549; poi[58].y=333;
  poi[59].x=584; poi[59].y=273;
  poi[60].x=671; poi[60].y=321;
  poi[61].x=729; poi[61].y=325;
  poi[62].x=740; poi[62].y=274;
  poi[63].x=801; poi[63].y=317;
  poi[64].x=880; poi[64].y=322;
  poi[65].x=903; poi[65].y=279;
  poi[66].x=933; poi[66].y=339;
  poi[67].x=949; poi[67].y=287;
  poi[68].x=95; poi[68].y=384;
  poi[69].x=151; poi[69].y=404;
  poi[70].x=209; poi[70].y=422;
  poi[71].x=277; poi[71].y=394;
  poi[72].x=358; poi[72].y=356;
  poi[73].x=435; poi[73].y=353;
  poi[74].x=483; poi[74].y=389;
  poi[75].x=533; poi[75].y=404;
  poi[76].x=604; poi[76].y=370;
  poi[77].x=622; poi[77].y=410;
  poi[78].x=693; poi[78].y=405;
  poi[79].x=770; poi[79].y=397;
  poi[80].x=826; poi[80].y=401;
  poi[81].x=67; poi[81].y=453;
  poi[82].x=157; poi[82].y=466;
  poi[83].x=245; poi[83].y=487;
  poi[84].x=262; poi[84].y=439;
  poi[85].x=291; poi[85].y=492;
  poi[86].x=358; poi[86].y=491;
  poi[87].x=362; poi[87].y=447;
  poi[88].x=400; poi[88].y=466;
  poi[89].x=471; poi[89].y=494;
  poi[90].x=515; poi[90].y=455;
  poi[91].x=549; poi[91].y=507;
  poi[92].x=555; poi[92].y=460;
  poi[93].x=641; poi[93].y=473;
  poi[94].x=718; poi[94].y=464;
  poi[95].x=778; poi[95].y=451;
  poi[96].x=887; poi[96].y=509;
  poi[97].x=897; poi[97].y=429;
  poi[98].x=967; poi[98].y=468;
  poi[99].x=114; poi[99].y=525;
  poi[100].x=183; poi[100].y=565;
  poi[101].x=188; poi[101].y=519;
  poi[102].x=257; poi[102].y=589;
  poi[103].x=279; poi[103].y=538;
  poi[104].x=339; poi[104].y=593;
  poi[105].x=397; poi[105].y=538;
  poi[106].x=476; poi[106].y=558;
  poi[107].x=526; poi[107].y=569;
  poi[108].x=629; poi[108].y=557;
  poi[109].x=631; poi[109].y=521;
  poi[110].x=707; poi[110].y=594;
  poi[111].x=740; poi[111].y=514;
  poi[112].x=770; poi[112].y=580;
  poi[113].x=849; poi[113].y=548;
  poi[114].x=857; poi[114].y=598;
  poi[115].x=901; poi[115].y=585;
  poi[116].x=968; poi[116].y=558;
  poi[117].x=91; poi[117].y=660;
  poi[118].x=110; poi[118].y=603;
  poi[119].x=179; poi[119].y=647;
  poi[120].x=259; poi[120].y=678;
  poi[121].x=270; poi[121].y=641;
  poi[122].x=363; poi[122].y=653;
  poi[123].x=381; poi[123].y=610;
  poi[124].x=424; poi[124].y=647;
  poi[125].x=461; poi[125].y=679;
  poi[126].x=509; poi[126].y=668;
  poi[127].x=516; poi[127].y=611;
  poi[128].x=593; poi[128].y=611;
  poi[129].x=621; poi[129].y=647;
  poi[130].x=655; poi[130].y=614;
  poi[131].x=798; poi[131].y=649;
  poi[132].x=879; poi[132].y=636;
  poi[133].x=920; poi[133].y=627;
  poi[134].x=136; poi[134].y=724;
  poi[135].x=195; poi[135].y=751;
  poi[136].x=239; poi[136].y=768;
  poi[137].x=291; poi[137].y=727;
  poi[138].x=329; poi[138].y=741;
  poi[139].x=362; poi[139].y=704;
  poi[140].x=429; poi[140].y=716;
  poi[141].x=519; poi[141].y=738;
  poi[142].x=636; poi[142].y=726;
  poi[143].x=682; poi[143].y=688;
  poi[144].x=779; poi[144].y=748;
  poi[145].x=814; poi[145].y=719;
  poi[146].x=841; poi[146].y=759;
  poi[147].x=853; poi[147].y=699;
  poi[148].x=883; poi[148].y=739;
  poi[149].x=900; poi[149].y=685;
  poi[150].x=917; poi[150].y=721;
  poi[151].x=952; poi[151].y=703;
  poi[152].x=60; poi[152].y=836;
  poi[153].x=101; poi[153].y=784;
  poi[154].x=99; poi[154].y=845;
  poi[155].x=138; poi[155].y=791;
  poi[156].x=175; poi[156].y=796;
  poi[157].x=192; poi[157].y=827;
  poi[158].x=230; poi[158].y=816;
  poi[159].x=260; poi[159].y=853;
  poi[160].x=372; poi[160].y=794;
  poi[161].x=377; poi[161].y=847;
  poi[162].x=465; poi[162].y=775;
  poi[163].x=466; poi[163].y=848;
  poi[164].x=529; poi[164].y=818;
  poi[165].x=623; poi[165].y=824;
  poi[166].x=669; poi[166].y=780;
  poi[167].x=771; poi[167].y=842;
  poi[168].x=810; poi[168].y=802;
  poi[169].x=852; poi[169].y=831;
  poi[170].x=885; poi[170].y=813;
  poi[171].x=905; poi[171].y=850;
  poi[172].x=943; poi[172].y=778;
  poi[173].x=66; poi[173].y=940;
  poi[174].x=101; poi[174].y=894;
  poi[175].x=197; poi[175].y=909;
  poi[176].x=276; poi[176].y=928;
  poi[177].x=334; poi[177].y=884;
  poi[178].x=404; poi[178].y=882;
  poi[179].x=462; poi[179].y=923;
  poi[180].x=527; poi[180].y=891;
  poi[181].x=572; poi[181].y=876;
  poi[182].x=573; poi[182].y=930;
  poi[183].x=622; poi[183].y=904;
  poi[184].x=666; poi[184].y=874;
  poi[185].x=705; poi[185].y=932;
  poi[186].x=797; poi[186].y=890;
  poi[187].x=834; poi[187].y=897;
  poi[188].x=873; poi[188].y=927;
  poi[189].x=133; poi[189].y=953;

  poic[0].x=148; poic[0].y=75;
  poic[1].x=182; poic[1].y=66;
  poic[2].x=252; poic[2].y=75;
  poic[3].x=307; poic[3].y=40;
  poic[4].x=293; poic[4].y=82;
  poic[5].x=333; poic[5].y=90;
  poic[6].x=404; poic[6].y=103;
  poic[7].x=481; poic[7].y=80;
  poic[8].x=588; poic[8].y=42;
  poic[9].x=631; poic[9].y=79;
  poic[10].x=680; poic[10].y=107;
  poic[11].x=759; poic[11].y=82;
  poic[12].x=818; poic[12].y=58;
  poic[13].x=83; poic[13].y=105;
  poic[14].x=177; poic[14].y=130;
  poic[15].x=213; poic[15].y=133;
  poic[16].x=273; poic[16].y=133;
  poic[17].x=353; poic[17].y=144;
  poic[18].x=416; poic[18].y=141;
  poic[19].x=472; poic[19].y=146;
  poic[20].x=513; poic[20].y=115;
  poic[21].x=579; poic[21].y=133;
  poic[22].x=615; poic[22].y=143;
  poic[23].x=727; poic[23].y=131;
  poic[24].x=821; poic[24].y=120;
  poic[25].x=888; poic[25].y=125;
  poic[26].x=65; poic[26].y=168;
  poic[27].x=199; poic[27].y=170;
  poic[28].x=243; poic[28].y=184;
  poic[29].x=297; poic[29].y=186;
  poic[30].x=365; poic[30].y=193;
  poic[31].x=534; poic[31].y=177;
  poic[32].x=640; poic[32].y=178;
  poic[33].x=681; poic[33].y=177;
  poic[34].x=739; poic[34].y=188;
  poic[35].x=928; poic[35].y=150;
  poic[36].x=157; poic[36].y=207;
  poic[37].x=111; poic[37].y=247;
  poic[38].x=140; poic[38].y=288;
  poic[39].x=238; poic[39].y=236;
  poic[40].x=205; poic[40].y=269;
  poic[41].x=277; poic[41].y=248;
  poic[42].x=340; poic[42].y=233;
  poic[43].x=358; poic[43].y=267;
  poic[44].x=427; poic[44].y=232;
  poic[45].x=474; poic[45].y=209;
  poic[46].x=564; poic[46].y=237;
  poic[47].x=647; poic[47].y=234;
  poic[48].x=707; poic[48].y=217;
  poic[49].x=820; poic[49].y=219;

}

int findPOI(int mx,int my)
{
  int i;

  for (i=1;i<190;i++)
    {
      //printf("poi[%d].x=%d ; poi[%d].y=%d ;\n",i,poi[i].x, i,poi[i].y);
      if ((mx>=(poi[i].x-8)) && (mx<=(poi[i].x+8))
	  && (my>=(poi[i].y-8)) && (my<=(poi[i].y+8)))
	return i;
    }
  return -1;
}

void sendMessageToServer(char *ipAddress, int portno, char *mess)
{
  int sockfd, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char sendbuffer[256];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  server = gethostbyname(ipAddress);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
      printf("ERROR connecting\n");
      exit(1);
    }

  sprintf(sendbuffer,"%s\n",mess);
  n = write(sockfd,sendbuffer,strlen(sendbuffer));

  close(sockfd);
}

void *server_function(void *arg)
{
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd<0)
    {
      printf("sockfd error\n");
      exit(1);
    }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = gClientPort;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
      printf("bind error\n");
      exit(1);
    }
     
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  while (1)
    {
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
      if (newsockfd < 0)
	{
	  printf("accept error\n");
	  exit(1);
	}

      bzero(gbuffer,256);
      n = read(newsockfd,gbuffer,255);
      if (n < 0)
	{
	  printf("read error\n");
	  exit(1);
	}

      printf("%s",gbuffer);

      pthread_mutex_lock( &mutex );
      synchro=1;
      pthread_mutex_unlock( &mutex );

      while (synchro);

      close(newsockfd);
    }
  //usleep(100000);
}

int main(int argc, char ** argv)
{
  int quit = 0;
  SDL_Event event;
  pthread_t thread1;
  int mx,my;

  char com;
  int px,py;
  char mess[256];
 
  /*
    if (argc<6)
    {
    printf("<app> <Main server ip address> <Main server port> <Client ip address> <Client port> <player name>\n");
    exit(1);
    }

    strcpy(gServerIpAddress,argv[1]);
    gServerPort=atoi(argv[2]);
    strcpy(gClientIpAddress,argv[3]);
    gClientPort=atoi(argv[4]);
    strcpy(gName,argv[5]);
  */


  SDL_Init(SDL_INIT_VIDEO);
 
  SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
					 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024+128, 1024, 0);
  
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
  //SDL_Surface * image = SDL_LoadBMP("whitehall_1024x1024.bmp");
  SDL_Surface * image = IMG_Load("whitehall_1024x1024.png");
  SDL_Surface * pawn = IMG_Load("pawn.png");
  SDL_SetColorKey( pawn, SDL_TRUE, SDL_MapRGB( pawn->format, 0, 0, 0 ) );
  //SDL_Surface * pawn = SDL_LoadBMP("pawn.bmp");
  SDL_Surface * rond = IMG_Load("rond.png");
  SDL_SetColorKey( rond, SDL_TRUE, SDL_MapRGB( rond->format, 0, 0, 0 ) );
  //SDL_Surface * rond = SDL_LoadBMP("rond.bmp");
  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_Texture * texture_pawn = SDL_CreateTextureFromSurface(renderer, pawn);
  SDL_Texture * texture_rond = SDL_CreateTextureFromSurface(renderer, rond);
  SDL_SetTextureAlphaMod(texture_rond,128);
 
  initPOI();

  synchro=0;

  pthread_create( &thread1, NULL, server_function, NULL);

  while (!quit)
    {
      //SDL_WaitEvent(&event);
 
      if (SDL_PollEvent(&event))
	{
	  //printf("un event\n");
	  switch (event.type)
	    {
	    case SDL_QUIT:
	      quit = 1;
	      pthread_cancel(thread1);
	      break;
	    case  SDL_MOUSEBUTTONDOWN:
	      SDL_GetMouseState( &mx, &my );
	      printf("mx=%d my=%d\n",mx,my);
	      if (mx>=1024)
		{
		  sprintf(mess,"C %s %d %s",gClientIpAddress, gClientPort, gName);
		  printf("mess vers server=%s\n",mess);
		  sendMessageToServer(gServerIpAddress, gServerPort, mess);
		}
	      else
		{
		  indice=findPOI(mx,my);
		  sprintf(mess,"K %d",indice);
		  sendMessageToServer(gServerIpAddress, gServerPort, mess);
		}
	      
	      break;
	    case SDL_MOUSEMOTION:
	      SDL_GetMouseState( &mx, &my );
	      indice=findPOI(mx,my);
	      break;
	    }
	}
      
      if (synchro==1)
	{
	  puts("consomme");
	  pthread_mutex_lock( &mutex );
	  
	  switch (gbuffer[0])
	    {
	    case 'C':
	      sscanf(gbuffer,"%c %d %d",&com, &px, &py);
	      printf("COM=%c (%d,%d)\n",com,px,py);
	      break;
	    case 'I':
	      sscanf(gbuffer,"%c %d",&com, &gId);
	      printf("COM=%c id=%d\n",com,gId);
	      break;
	    case 'T':
	      sscanf(gbuffer,"%c %s",&com, mess);
	      printf("COM=%c mess=%s\n",com,mess);
	      break;
	    default:
	      break;
	    }
	  synchro=0;
	  pthread_mutex_unlock( &mutex );
	}
      
      SDL_Rect dstrect = { 0, 0, 1024, 1024 };
      //SDL_Rect srcrect_pawn = { 0, 0, 87, 131 };
      SDL_Rect dstrect_pawn = { 100, 100, 87, 131 };
      SDL_Rect dstrect_rond = { 300, 300, 200, 200 };
      
      SDL_RenderCopy(renderer, texture, NULL, &dstrect);

      //SDL_RenderCopy(renderer, texture_pawn, NULL, &dstrect_pawn);
      if (indice != -1)
	{
	  SDL_Rect dstrect_rond = { poi[indice].x-16, poi[indice].y-16, 32, 32 };
	  SDL_RenderCopy(renderer, texture_rond, NULL, &dstrect_rond);
	}
      SDL_RenderPresent(renderer);
    }
  
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(image);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  
  SDL_Quit();
 
  return 0;
}
