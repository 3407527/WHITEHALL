#include <SDL.h>        
#include <SDL_image.h>
#include <SDL2/SDL_ttf.h>
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
char joueur = 'U';
char couleur;
int ind_k = -1;
int ind_j = -1;
int ind_v = -1;
int ind_b = -1; 
struct _poi
{
  int x;
  int y;
} poi[600];
int indice;


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int nbvictimes;
int victimes[4];
int nbindices;
int indices[45];
int nbcibles;
int cibles[4];


void initPOI()
{
poi[1].x = 77 ; poi[1].y = 59;
poi[2].x = 234 ; poi[2].y = 45;
poi[3].x = 318 ; poi[3].y = 52;
poi[4].x = 445 ; poi[4].y = 51;
poi[5].x = 508 ; poi[5].y = 49;
poi[6].x = 596 ; poi[6].y = 50;
poi[7].x = 731 ; poi[7].y = 69;
poi[8].x = 45 ; poi[8].y = 104;
poi[9].x = 99 ; poi[9].y = 73;
poi[10].x = 111 ; poi[10].y = 113;
poi[11].x = 135 ; poi[11].y = 84;
poi[12].x = 163 ; poi[12].y = 81;
poi[13].x = 189 ; poi[13].y = 130;
poi[14].x = 215 ; poi[14].y = 82;
poi[15].x = 238 ; poi[15].y = 133;
poi[16].x = 253 ; poi[16].y = 96;
poi[17].x = 284 ; poi[17].y = 96;
poi[18].x = 316 ; poi[18].y = 103;
poi[19].x = 370 ; poi[19].y = 116;
poi[20].x = 450 ; poi[20].y = 77;
poi[21].x = 462 ; poi[21].y = 130;
poi[22].x = 497 ; poi[22].y = 92;
poi[23].x = 545 ; poi[23].y = 78;
poi[24].x = 593 ; poi[24].y = 125;
poi[25].x = 658 ; poi[25].y = 83;
poi[26].x = 746 ; poi[26].y = 137;
poi[27].x = 797 ; poi[27].y = 93;
poi[28].x = 63 ; poi[28].y = 167;
poi[29].x = 106 ; poi[29].y = 147;
poi[30].x = 112 ; poi[30].y = 188;
poi[31].x = 149 ; poi[31].y = 195;
poi[32].x = 180 ; poi[32].y = 168;
poi[33].x = 218 ; poi[33].y = 178;
poi[34].x = 262 ; poi[34].y = 180;
poi[35].x = 301 ; poi[35].y = 180;
poi[36].x = 329 ; poi[36].y = 207;
poi[37].x = 354 ; poi[37].y = 156;
poi[38].x = 395 ; poi[38].y = 145;
poi[39].x = 395 ; poi[39].y = 197;
poi[40].x = 431 ; poi[40].y = 184;
poi[41].x = 466 ; poi[41].y = 167;
poi[42].x = 516 ; poi[42].y = 161;
poi[43].x = 563 ; poi[43].y = 208;
poi[44].x = 576 ; poi[44].y = 161;
poi[45].x = 656 ; poi[45].y = 145;
poi[46].x = 659 ; poi[46].y = 196;
poi[47].x = 789 ; poi[47].y = 168;
poi[48].x = 69 ; poi[48].y = 215;
poi[49].x = 89 ; poi[49].y = 239;
poi[50].x = 91 ; poi[50].y = 271;
poi[51].x = 150 ; poi[51].y = 257;
poi[52].x = 206 ; poi[52].y = 284;
poi[53].x = 218 ; poi[53].y = 251;
poi[54].x = 238 ; poi[54].y = 225;
poi[55].x = 262 ; poi[55].y = 278;
poi[56].x = 353 ; poi[56].y = 226;
poi[57].x = 398 ; poi[57].y = 260;
poi[58].x = 456 ; poi[58].y = 276;
poi[59].x = 484 ; poi[59].y = 226;
poi[60].x = 557 ; poi[60].y = 266;
poi[61].x = 606 ; poi[61].y = 268;
poi[62].x = 613 ; poi[62].y = 227;
poi[63].x = 665 ; poi[63].y = 263;
poi[64].x = 730 ; poi[64].y = 267;
poi[65].x = 750 ; poi[65].y = 232;
poi[66].x = 775 ; poi[66].y = 281;
poi[67].x = 787 ; poi[67].y = 237;
poi[68].x = 78 ; poi[68].y = 319;
poi[69].x = 125 ; poi[69].y = 336;
poi[70].x = 174 ; poi[70].y = 350;
poi[71].x = 230 ; poi[71].y = 326;
poi[72].x = 297 ; poi[72].y = 295;
poi[73].x = 361 ; poi[73].y = 292;
poi[74].x = 400 ; poi[74].y = 324;
poi[75].x = 440 ; poi[75].y = 335;
poi[76].x = 502 ; poi[76].y = 305;
poi[77].x = 517 ; poi[77].y = 341;
poi[78].x = 575 ; poi[78].y = 335;
poi[79].x = 639 ; poi[79].y = 329;
poi[80].x = 686 ; poi[80].y = 332;
poi[81].x = 55 ; poi[81].y = 376;
poi[82].x = 130 ; poi[82].y = 386;
poi[83].x = 203 ; poi[83].y = 405;
poi[84].x = 216 ; poi[84].y = 367;
poi[85].x = 239 ; poi[85].y = 410;
poi[86].x = 296 ; poi[86].y = 409;
poi[87].x = 299 ; poi[87].y = 370;
poi[88].x = 334 ; poi[88].y = 387;
poi[89].x = 392 ; poi[89].y = 409;
poi[90].x = 427 ; poi[90].y = 376;
poi[91].x = 456 ; poi[91].y = 419;
poi[92].x = 461 ; poi[92].y = 381;
poi[93].x = 533 ; poi[93].y = 392;
poi[94].x = 596 ; poi[94].y = 383;
poi[95].x = 646 ; poi[95].y = 374;
poi[96].x = 736 ; poi[96].y = 422;
poi[97].x = 745 ; poi[97].y = 355;
poi[98].x = 802 ; poi[98].y = 389;
poi[99].x = 92 ; poi[99].y = 437;
poi[100].x = 151 ; poi[100].y = 469;
poi[101].x = 156 ; poi[101].y = 431;
poi[102].x = 212 ; poi[102].y = 490;
poi[103].x = 230 ; poi[103].y = 446;
poi[104].x = 282 ; poi[104].y = 492;
poi[105].x = 331 ; poi[105].y = 445;
poi[106].x = 397 ; poi[106].y = 464;
poi[107].x = 435 ; poi[107].y = 471;
poi[108].x = 522 ; poi[108].y = 463;
poi[109].x = 524 ; poi[109].y = 433;
poi[110].x = 587 ; poi[110].y = 493;
poi[111].x = 615 ; poi[111].y = 426;
poi[112].x = 639 ; poi[112].y = 482;
poi[113].x = 704 ; poi[113].y = 454;
poi[114].x = 713 ; poi[114].y = 495;
poi[115].x = 749 ; poi[115].y = 486;
poi[116].x = 806 ; poi[116].y = 463;
poi[117].x = 75 ; poi[117].y = 548;
poi[118].x = 93 ; poi[118].y = 501;
poi[119].x = 147 ; poi[119].y = 537;
poi[120].x = 214 ; poi[120].y = 561;
poi[121].x = 225 ; poi[121].y = 533;
poi[122].x = 302 ; poi[122].y = 541;
poi[123].x = 315 ; poi[123].y = 508;
poi[124].x = 351 ; poi[124].y = 538;
poi[125].x = 380 ; poi[125].y = 563;
poi[126].x = 421 ; poi[126].y = 554;
poi[127].x = 426 ; poi[127].y = 506;
poi[128].x = 491 ; poi[128].y = 507;
poi[129].x = 513 ; poi[129].y = 537;
poi[130].x = 545 ; poi[130].y = 508;
poi[131].x = 663 ; poi[131].y = 539;
poi[132].x = 728 ; poi[132].y = 528;
poi[133].x = 763 ; poi[133].y = 520;
poi[134].x = 112 ; poi[134].y = 602;
poi[135].x = 163 ; poi[135].y = 624;
poi[136].x = 198 ; poi[136].y = 636;
poi[137].x = 242 ; poi[137].y = 603;
poi[138].x = 274 ; poi[138].y = 613;
poi[139].x = 301 ; poi[139].y = 583;
poi[140].x = 357 ; poi[140].y = 592;
poi[141].x = 431 ; poi[141].y = 613;
poi[142].x = 527 ; poi[142].y = 602;
poi[143].x = 566 ; poi[143].y = 571;
poi[144].x = 644 ; poi[144].y = 622;
poi[145].x = 677 ; poi[145].y = 596;
poi[146].x = 700 ; poi[146].y = 631;
poi[147].x = 710 ; poi[147].y = 581;
poi[148].x = 736 ; poi[148].y = 614;
poi[149].x = 748 ; poi[149].y = 568;
poi[150].x = 760 ; poi[150].y = 600;
poi[151].x = 791 ; poi[151].y = 584;
poi[152].x = 48 ; poi[152].y = 695;
poi[153].x = 82 ; poi[153].y = 651;
poi[154].x = 82 ; poi[154].y = 699;
poi[155].x = 114 ; poi[155].y = 655;
poi[156].x = 146 ; poi[156].y = 660;
poi[157].x = 158 ; poi[157].y = 686;
poi[158].x = 190 ; poi[158].y = 677;
poi[159].x = 215 ; poi[159].y = 707;
poi[160].x = 307 ; poi[160].y = 660;
poi[161].x = 314 ; poi[161].y = 705;
poi[162].x = 385 ; poi[162].y = 643;
poi[163].x = 386 ; poi[163].y = 703;
poi[164].x = 439 ; poi[164].y = 679;
poi[165].x = 519 ; poi[165].y = 684;
poi[166].x = 554 ; poi[166].y = 647;
poi[167].x = 640 ; poi[167].y = 699;
poi[168].x = 672 ; poi[168].y = 664;
poi[169].x = 708 ; poi[169].y = 689;
poi[170].x = 736 ; poi[170].y = 675;
poi[171].x = 751 ; poi[171].y = 705;
poi[172].x = 784 ; poi[172].y = 644;
poi[173].x = 56 ; poi[173].y = 779;
poi[174].x = 85 ; poi[174].y = 742;
poi[175].x = 164 ; poi[175].y = 754;
poi[176].x = 232 ; poi[176].y = 769;
poi[177].x = 280 ; poi[177].y = 733;
poi[178].x = 335 ; poi[178].y = 730;
poi[179].x = 383 ; poi[179].y = 765;
poi[180].x = 437 ; poi[180].y = 740;
poi[181].x = 475 ; poi[181].y = 727;
poi[182].x = 475 ; poi[182].y = 771;
poi[183].x = 514 ; poi[183].y = 750;
poi[184].x = 551 ; poi[184].y = 724;
poi[185].x = 585 ; poi[185].y = 772;
poi[186].x = 660 ; poi[186].y = 736;
poi[187].x = 691 ; poi[187].y = 744;
poi[188].x = 724 ; poi[188].y = 768;
poi[189].x = 110 ; poi[189].y = 790;
poi[201].x = 70 ; poi[201].y = 86;
poi[202].x = 123 ; poi[202].y = 59;
poi[203].x = 152 ; poi[203].y = 54;
poi[204].x = 208 ; poi[204].y = 64;
poi[205].x = 244 ; poi[205].y = 69;
poi[206].x = 255 ; poi[206].y = 31;
poi[207].x = 276 ; poi[207].y = 75;
poi[208].x = 336 ; poi[208].y = 86;
poi[209].x = 399 ; poi[209].y = 64;
poi[210].x = 344 ; poi[210].y = 115;
poi[211].x = 395 ; poi[211].y = 123;
poi[212].x = 148 ; poi[212].y = 107;
poi[213].x = 175 ; poi[213].y = 111;
poi[214].x = 227 ; poi[214].y = 111;
poi[215].x = 247 ; poi[215].y = 157;
poi[216].x = 294 ; poi[216].y = 119;
poi[217].x = 303 ; poi[217].y = 156;
poi[218].x = 200 ; poi[218].y = 154;
poi[219].x = 165 ; poi[219].y = 139;
poi[220].x = 130 ; poi[220].y = 169;
poi[221].x = 85 ; poi[221].y = 115;
poi[222].x = 53 ; poi[222].y = 137;
poi[223].x = 95 ; poi[223].y = 205;
poi[224].x = 170 ; poi[224].y = 220;
poi[225].x = 117 ; poi[225].y = 238;
poi[226].x = 186 ; poi[226].y = 254;
poi[227].x = 60 ; poi[227].y = 239;
poi[228].x = 57 ; poi[228].y = 271;
poi[229].x = 51 ; poi[229].y = 330;
poi[230].x = 107 ; poi[230].y = 302;
poi[231].x = 82 ; poi[231].y = 356;
poi[232].x = 105 ; poi[232].y = 405;
poi[233].x = 144 ; poi[233].y = 409;
poi[234].x = 191 ; poi[234].y = 382;
poi[235].x = 155 ; poi[235].y = 316;
poi[236].x = 136 ; poi[236].y = 286;
poi[237].x = 216 ; poi[237].y = 307;
poi[238].x = 240 ; poi[238].y = 353;
poi[239].x = 264 ; poi[239].y = 392;
poi[240].x = 216 ; poi[240].y = 425;
poi[241].x = 289 ; poi[241].y = 337;
poi[242].x = 273 ; poi[242].y = 308;
poi[243].x = 245 ; poi[243].y = 246;
poi[244].x = 232 ; poi[244].y = 205;
poi[245].x = 300 ; poi[245].y = 220;
poi[246].x = 282 ; poi[246].y = 193;
poi[247].x = 197 ; poi[247].y = 196;
poi[248].x = 354 ; poi[248].y = 194;
poi[249].x = 351 ; poi[249].y = 265;
poi[250].x = 320 ; poi[250].y = 293;
poi[251].x = 327 ; poi[251].y = 355;
poi[252].x = 336 ; poi[252].y = 415;
poi[253].x = 413 ; poi[253].y = 389;
poi[254].x = 399 ; poi[254].y = 348;
poi[255].x = 399 ; poi[255].y = 301;
poi[256].x = 394 ; poi[256].y = 221;
poi[257].x = 396 ; poi[257].y = 173;
poi[301].x = 490 ; poi[301].y = 32;
poi[302].x = 423 ; poi[302].y = 93;
poi[303].x = 479 ; poi[303].y = 106;
poi[304].x = 524 ; poi[304].y = 65;
poi[305].x = 565 ; poi[305].y = 146;
poi[306].x = 513 ; poi[306].y = 122;
poi[307].x = 563 ; poi[307].y = 85;
poi[308].x = 606 ; poi[308].y = 105;
poi[309].x = 629 ; poi[309].y = 64;
poi[310].x = 677 ; poi[310].y = 48;
poi[311].x = 681 ; poi[311].y = 97;
poi[312].x = 736 ; poi[312].y = 102;
poi[313].x = 772 ; poi[313].y = 123;
poi[314].x = 804 ; poi[314].y = 209;
poi[315].x = 757 ; poi[315].y = 252;
poi[316].x = 680 ; poi[316].y = 179;
poi[317].x = 693 ; poi[317].y = 264;
poi[318].x = 705 ; poi[318].y = 279;
poi[319].x = 666 ; poi[319].y = 301;
poi[320].x = 630 ; poi[320].y = 294;
poi[321].x = 585 ; poi[321].y = 241;
poi[322].x = 639 ; poi[322].y = 207;
poi[323].x = 588 ; poi[323].y = 183;
poi[324].x = 615 ; poi[324].y = 154;
poi[325].x = 534 ; poi[325].y = 194;
poi[326].x = 534 ; poi[326].y = 144;
poi[327].x = 445 ; poi[327].y = 145;
poi[328].x = 465 ; poi[328].y = 193;
poi[329].x = 437 ; poi[329].y = 203;
poi[330].x = 498 ; poi[330].y = 255;
poi[331].x = 528 ; poi[331].y = 289;
poi[332].x = 556 ; poi[332].y = 305;
poi[333].x = 535 ; poi[333].y = 368;
poi[334].x = 442 ; poi[334].y = 361;
poi[335].x = 472 ; poi[335].y = 332;
poi[336].x = 440 ; poi[336].y = 300;
poi[337].x = 583 ; poi[337].y = 407;
poi[338].x = 620 ; poi[338].y = 350;
poi[339].x = 783 ; poi[339].y = 424;
poi[340].x = 485 ; poi[340].y = 403;
poi[341].x = 498 ; poi[341].y = 357;
poi[401].x = 63 ; poi[401].y = 461;
poi[402].x = 106 ; poi[402].y = 460;
poi[403].x = 122 ; poi[403].y = 481;
poi[404].x = 165 ; poi[404].y = 452;
poi[405].x = 245 ; poi[405].y = 467;
poi[406].x = 306 ; poi[406].y = 474;
poi[407].x = 392 ; poi[407].y = 434;
poi[408].x = 400 ; poi[408].y = 489;
poi[409].x = 402 ; poi[409].y = 513;
poi[410].x = 375 ; poi[410].y = 534;
poi[411].x = 422 ; poi[411].y = 578;
poi[412].x = 384 ; poi[412].y = 590;
poi[413].x = 334 ; poi[413].y = 760;
poi[414].x = 339 ; poi[414].y = 698;
poi[415].x = 336 ; poi[415].y = 641;
poi[416].x = 327 ; poi[416].y = 599;
poi[417].x = 324 ; poi[417].y = 569;
poi[418].x = 325 ; poi[418].y = 541;
poi[419].x = 253 ; poi[419].y = 510;
poi[420].x = 256 ; poi[420].y = 536;
poi[421].x = 185 ; poi[421].y = 510;
poi[422].x = 167 ; poi[422].y = 573;
poi[423].x = 90 ; poi[423].y = 572;
poi[424].x = 63 ; poi[424].y = 519;
poi[425].x = 115 ; poi[425].y = 628;
poi[426].x = 167 ; poi[426].y = 590;
poi[427].x = 216 ; poi[427].y = 617;
poi[428].x = 269 ; poi[428].y = 593;
poi[429].x = 311 ; poi[429].y = 632;
poi[430].x = 300 ; poi[430].y = 684;
poi[431].x = 258 ; poi[431].y = 704;
poi[432].x = 172 ; poi[432].y = 711;
poi[433].x = 127 ; poi[433].y = 717;
poi[434].x = 115 ; poi[434].y = 684;
poi[435].x = 52 ; poi[435].y = 667;
poi[436].x = 43 ; poi[436].y = 719;
poi[437].x = 45 ; poi[437].y = 760;
poi[438].x = 52 ; poi[438].y = 802;
poi[439].x = 113 ; poi[439].y = 770;
poi[440].x = 168 ; poi[440].y = 780;
poi[441].x = 226 ; poi[441].y = 747;
poi[442].x = 291 ; poi[442].y = 757;
poi[443].x = 183 ; poi[443].y = 650;
poi[501].x = 463 ; poi[501].y = 440;
poi[502].x = 447 ; poi[502].y = 496;
poi[503].x = 438 ; poi[503].y = 650;
poi[504].x = 435 ; poi[504].y = 708;
poi[505].x = 433 ; poi[505].y = 769;
poi[506].x = 515 ; poi[506].y = 728;
poi[507].x = 512 ; poi[507].y = 771;
poi[508].x = 649 ; poi[508].y = 774;
poi[509].x = 685 ; poi[509].y = 773;
poi[510].x = 696 ; poi[510].y = 712;
poi[511].x = 740 ; poi[511].y = 725;
poi[512].x = 666 ; poi[512].y = 701;
poi[513].x = 718 ; poi[513].y = 664;
poi[514].x = 762 ; poi[514].y = 683;
poi[515].x = 729 ; poi[515].y = 634;
poi[516].x = 780 ; poi[516].y = 605;
poi[517].x = 741 ; poi[517].y = 592;
poi[518].x = 673 ; poi[518].y = 625;
poi[519].x = 677 ; poi[519].y = 572;
poi[520].x = 688 ; poi[520].y = 546;
poi[521].x = 799 ; poi[521].y = 562;
poi[522].x = 701 ; poi[522].y = 517;
poi[523].x = 754 ; poi[523].y = 545;
poi[524].x = 773 ; poi[524].y = 500;
poi[525].x = 722 ; poi[525].y = 474;
poi[526].x = 754 ; poi[526].y = 443;
poi[527].x = 565 ; poi[527].y = 443;
poi[528].x = 522 ; poi[528].y = 639;
poi[529].x = 532 ; poi[529].y = 566;
poi[530].x = 534 ; poi[530].y = 545;
poi[531].x = 491 ; poi[531].y = 534;
poi[532].x = 492 ; poi[532].y = 477;
poi[533].x = 552 ; poi[533].y = 475;
//poi[534].x = 565 ; poi[534].y = 443;
}	

int findPOI(int mx,int my)
{
  int i;

  for (i=1;i<600;i++)
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
  int k;
  int z =7;
  char com;
  int px,py;
  char mess[256];
  int tour = 0;
  int i;
  
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
  
  
  SDL_Init(SDL_INIT_VIDEO);

  //TTF_Init();
  //TTF_Font *police = NULL;
  //SDL_Surface *texte = NULL; 
  // SDL_Color couleurNoire = {0,0,0};
  //SDL_Rect positionText;
  //positionText.x = 50;
  //positionText.y = 50;
  //police = TTF_OpenFont("~/WHITEHALL/KIN668.TTF",8);

  SDL_Window * window = SDL_CreateWindow("SDL2 Displaying Image",
					 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 850+128, 850, 0);
  
  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
  //SDL_Surface * image = SDL_LoadBMP("whitehall_1024x1024.bmp");
  SDL_Surface * image = IMG_Load("whitehall_850x850.png");
  SDL_Surface * pawnJ = IMG_Load("pawnJ.png");
  SDL_SetColorKey( pawnJ, SDL_TRUE, SDL_MapRGB( pawnJ->format, 0, 0, 0 ) );
  SDL_Surface * pawnK = IMG_Load("pawnK.png");
  SDL_SetColorKey( pawnK, SDL_TRUE, SDL_MapRGB( pawnK->format, 0, 0, 0 ) );
  SDL_Surface * pawnV = IMG_Load("pawnV.png");
  SDL_SetColorKey( pawnV, SDL_TRUE, SDL_MapRGB( pawnV->format, 0, 0, 0 ) );
  SDL_Surface * pawnB = IMG_Load("pawnB.png");
  SDL_SetColorKey( pawnB, SDL_TRUE, SDL_MapRGB( pawnB->format, 0, 0, 0 ) );
  SDL_Surface * pawnT = IMG_Load("pawnK.png");
  SDL_SetColorKey( pawnT, SDL_TRUE, SDL_MapRGB( pawnT->format, 0, 0, 0 ) );
  //SDL_Surface * pawn = SDL_LoadBMP("pawn.bmp");
  SDL_Surface * rond = IMG_Load("rond.png");
  SDL_SetColorKey( rond, SDL_TRUE, SDL_MapRGB( rond->format, 0, 0, 0 ) );
  SDL_Surface * rondj = IMG_Load("rond_j.png");
  SDL_SetColorKey( rondj, SDL_TRUE, SDL_MapRGB( rondj->format, 0, 0, 0 ) );
  SDL_Surface * cross = IMG_Load("cross.png");
  SDL_SetColorKey( cross, SDL_TRUE, SDL_MapRGB( cross->format, 0, 0, 0 ) );
  //SDL_Surface * rond = SDL_LoadBMP("rond.bmp");
  SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
  SDL_Texture * texture_pawnJ = SDL_CreateTextureFromSurface(renderer, pawnJ);
  SDL_Texture * texture_pawnK = SDL_CreateTextureFromSurface(renderer, pawnK);
  SDL_Texture * texture_pawnV = SDL_CreateTextureFromSurface(renderer, pawnV);
  SDL_Texture * texture_pawnB = SDL_CreateTextureFromSurface(renderer, pawnB);
  SDL_Texture * texture_pawnT = SDL_CreateTextureFromSurface(renderer, pawnT);
  SDL_Texture * texture_rond = SDL_CreateTextureFromSurface(renderer, rond);
  SDL_Texture * texture_rondj = SDL_CreateTextureFromSurface(renderer, rondj);
  SDL_Texture * texture_cross = SDL_CreateTextureFromSurface(renderer, cross);

  SDL_SetTextureAlphaMod(texture_rond,128);
  SDL_SetTextureAlphaMod(texture_rondj,128);
  SDL_SetTextureAlphaMod(texture_cross,200);
  SDL_SetTextureAlphaMod(texture_pawnJ,200);
  SDL_SetTextureAlphaMod(texture_pawnK,200);
  SDL_SetTextureAlphaMod(texture_pawnV,200);
  SDL_SetTextureAlphaMod(texture_pawnB,200);
  SDL_SetTextureAlphaMod(texture_pawnT,200);
  //texte = TTF_RenderText_Blended(police,"oui",couleurNoire);
  //SDL_BlitSurface(texte,NULL,image,&positionText);

  SDL_Rect dstrect_rond_M;
  SDL_Rect dstrect_rond_S;
  SDL_Rect dstrect_cross;
  SDL_Rect dstrect_pawnJ;
  SDL_Rect dstrect_pawnV;
  SDL_Rect dstrect_pawnB;
  SDL_Rect dstrect_pawnK;
  SDL_Rect dstrect_pawnT;

  SDL_Rect dstrect = { 0, 0, 850, 850 };
  SDL_RenderCopy(renderer, texture, NULL, &dstrect);

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
	    case SDL_KEYDOWN:{
	      SDL_Keycode keyPressed = event.key.keysym.sym;
	      switch(keyPressed)
	    	{
		case SDLK_l: // à supprimer une fois le bouton fait
		  sprintf(mess, "L %c", joueur);
		  sendMessageToServer(gServerIpAddress, gServerPort, mess);
		  break;
	    	case SDLK_a: // pareil que l
	    	  //fputs("A ", f);
		  sprintf(mess, "A %c", joueur);
		  sendMessageToServer(gServerIpAddress, gServerPort, mess);
	    	  break;
	    	default:
	    	  break;
	    	}
	    }
	      break;
	    case  SDL_MOUSEBUTTONDOWN:
	      SDL_GetMouseState( &mx, &my );
	      //printf("mx=%d my=%d\n",mx,my);
	      if (mx>=850)
		{
		  sprintf(mess,"C %s %d %s",gClientIpAddress, gClientPort, gName);
		  printf("mess vers server=%s\n",mess);
		  sendMessageToServer(gServerIpAddress, gServerPort, mess);
		}
	      else
		{
		  indice=findPOI(mx,my);
		  if (indice != -1){
		    sprintf(mess, "%c %d", joueur, indice);
		    printf("mess vers server=%s\n",mess);
		    sendMessageToServer(gServerIpAddress, gServerPort, mess);
		    }		  
		  
		}
	      
	      break;
	    case  SDL_MOUSEMOTION:
	      SDL_GetMouseState( &mx, &my );
	      //indice=findPOI(mx,my);
	      break;
	    }
	}
      
      if (synchro==1)
	{
	  //puts("consomme");
	  pthread_mutex_lock( &mutex );
	  
	  switch (gbuffer[0])
	    {
	    case 'C':
	      sscanf(gbuffer,"%c %d %d",&com, &px, &py);
	      //printf("COM=%c (%d,%d)\n",com,px,py);
	      break;
	    case 'K':
	      sscanf(gbuffer, "%c %d", &com, &ind_k);
	      break;
	    case 'J':
	      sscanf(gbuffer, "%c %d", &com, &ind_j);
	      break;
	    case 'V':
	      sscanf(gbuffer, "%c %d", &com, &ind_v);
	      break;
	    case 'B':
	      sscanf(gbuffer, "%c %d", &com, &ind_b);
	      break;
	    case 'N':
	      sscanf(gbuffer, "%c %d", &com, &tour);
	      break;
	    case 'M':
	      sscanf(gbuffer, "%c %d", &com, &victimes[nbvictimes]);
	      nbvictimes++;
	      for (i = 0; i < nbindices; i++)
		indices[i] = 0;
	      nbindices = 0;
	      break;
	    case 'S':
	      sscanf(gbuffer, "%c %d", &com, &indices[nbindices]);
	      nbindices++;
	      break;
	    case 'X':
	      sscanf(gbuffer, "%c %d", &com, &cibles[nbcibles]);
	      nbcibles++;
	      break;
	    case 'I':
	      sscanf(gbuffer,"%c %d",&com, &gId);
	      // printf("COM=%c id=%d\n",com,gId);
	      switch (gId)
		{
		case 0 :
		  joueur = 'K';
		  break;
		case 1 :
		  joueur = 'J';
		  break;
		case 2 :
		  joueur = 'V';
		  break;
		case 3 :
		  joueur = 'B';
		  break;
		}
	      break;
	    case 'T':
	      sscanf(gbuffer,"%c %s",&com, mess);
	      //printf("COM=%c mess=%s\n",com,mess);
	      break;
	    case 'E':
	      break;
	    default:
	      break;
	    }
	  synchro=0;
	  pthread_mutex_unlock( &mutex );
	}
      // On réinitialise l'affichage, puis on ajoute au fur et à mesure
      SDL_Rect dstrect = { 0, 0, 850, 850 };
      SDL_RenderCopy(renderer, texture, NULL, &dstrect);

      // Jack
      dstrect_pawnK.x = poi[ind_k].x-16;
      dstrect_pawnK.y = poi[ind_k].y-16;
      dstrect_pawnK.w = 20;
      dstrect_pawnK.h = 32;
      SDL_RenderCopy(renderer, texture_pawnK, NULL, &dstrect_pawnK);
      
      // Jaune
      dstrect_pawnJ.x = poi[ind_j].x-16;
      dstrect_pawnJ.y = poi[ind_j].y-16;
      dstrect_pawnJ.w = 20;
      dstrect_pawnJ.h = 32;
      SDL_RenderCopy(renderer, texture_pawnJ, NULL, &dstrect_pawnJ);
      
      // Vert
      dstrect_pawnV.x = poi[ind_v].x-16;
      dstrect_pawnV.y = poi[ind_v].y-16;
      dstrect_pawnV.w = 20;
      dstrect_pawnV.h = 32;
      SDL_RenderCopy(renderer, texture_pawnV, NULL, &dstrect_pawnV);

      //Bleu
      dstrect_pawnB.x = poi[ind_b].x-16;
      dstrect_pawnB.y = poi[ind_b].y-16;
      dstrect_pawnB.w = 20;
      dstrect_pawnB.h = 32;
      SDL_RenderCopy(renderer, texture_pawnB, NULL, &dstrect_pawnB);

      // Pion tour
      dstrect_pawnT.x = 120 + 40 * tour - 16;
      dstrect_pawnT.y = 823-16;
      dstrect_pawnT.w = 20;
      dstrect_pawnT.h = 32;
      SDL_RenderCopy(renderer, texture_pawnT, NULL, &dstrect_pawnT);

      //Discovery Locations (assassinats)
      for (i = 0; i < nbvictimes; i++){
      	dstrect_rond_M.x = poi[victimes[i]].x-16;
      	dstrect_rond_M.y = poi[victimes[i]].y-16;
      	dstrect_rond_M.w = 32;
      	dstrect_rond_M.h = 32;
      	SDL_RenderCopy(renderer, texture_rond, NULL, &dstrect_rond_M);
      }

      //Indices
      for (i = 0; i < nbindices; i++){
      	dstrect_rond_S.x = poi[indices[i]].x-16;
      	dstrect_rond_S.y = poi[indices[i]].y-16;
      	dstrect_rond_S.w = 32;
      	dstrect_rond_S.h = 32;
      	SDL_RenderCopy(renderer, texture_rondj, NULL, &dstrect_rond_S);
      }

      //Cibles
      for (i = 0; i < nbcibles; i++){
	
	dstrect_cross.x = poi[cibles[i]].x-16;
      	dstrect_cross.y = poi[cibles[i]].y-16;
      	dstrect_cross.w = 32;
      	dstrect_cross.h = 32;
      	SDL_RenderCopy(renderer, texture_cross, NULL, &dstrect_cross);
      }
	
	
      
      //SDL_Rect srcrect_pawn = { 0, 0, 87, 131 };
      //SDL_Rect dstrect_pawn = { 100, 100, 87, 131 };
      //SDL_Rect dstrect_rond = { 300, 300, 200, 200 };
      

      
      //SDL_RenderCopy(renderer, texture_pawn, NULL, &dstrect_pawn);
      /* if (indice != -1)
	{
	    SDL_Rect dstrect_rond = { poi[indice].x-16, poi[indice].y-16, 32, 32 };
	  SDL_RenderCopy(renderer, texture_rond, NULL, &dstrect_rond);
	}
      */
     
      SDL_RenderPresent(renderer);
    }
  
  SDL_DestroyTexture(texture);
  SDL_FreeSurface(image);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  TTF_Quit();
  SDL_Quit();
  
  return 0;
}
