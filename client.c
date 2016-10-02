#include <stdio.h> /* for printf() and fprintf() */
#include <stdlib.h> /* pour exit */  /* for atoi() */
#include <sys/types.h> //accept, connect types de données élémentaires (u_char, u_long, ...)/* for Socket data types */
#include <sys/socket.h> //déclarations concernant les sockets /* for socket(), connect(), send(), and recv() */ 
#include <arpa/inet.h> //pour htons et inet_aton /* for sockaddr_in and inet_addr() */ 
#include <string.h>  /* pour memset*/ /* for memset() strtok */
#include <netinet/in.h> // déclarations concernant le protocole IP  struct sockaddr_in /* for IP Socket data types */
#include <unistd.h>// pour close /* for close() */
#include <errno.h> // pour detecter des erreurs
#define PORT 6260 // variable port
#define PORTS 6261 // variable port
#define MAXLEN 1024 // variable maxlen
#define BYE "/bye" 
#define T_BYE strlen(BYE)

 

	int main(int argc, char**argv){

	  int socketClient;  // variable de socketClient
	  char *msg; // variable de message
	  struct sockaddr_in adresseServeur, adresseClient; // variables de type sockaddr_in
	  socklen_t longueurAdresseServeur, longueurAdresseClient; // longeur des adresse serveur et client
	  
	  char fullname[100],name[100];// tableau des utilisateurs connectes
	  char * namecond; 
	  
	  int ndfs; // maximum taille du file descriptor
	  fd_set afds, rfds; // declaration de deux file descriptor
	  char *buf = (char *)malloc(MAXLEN*sizeof(char)); // allocation de memoire de taille maxlength the char
	  



	  // connecter au serveur avec hello alias
	  printf("Pour connecter au serveur ecrit : hello alias  \n");
	  namecond = fgets(name, 99, stdin); //fgets(tab, sizeof tab, stdin); on lit maximum 99 caractère sur stdin (entrée standard) 
	   

	  if(strncmp(namecond,"hello",5)==0){ // comparaison du string "hello" avec "hello alias"
	 
	    char * fin;
	    char * debut;
	    debut = strchr(namecond,' ');// me donne le premier espace et tous ce qui est apres car char *strchr(const char *s, int c) Retourner un pointeur sur la première occurence d'espace

	    if(debut != NULL && debut !=" "){

	      debut++;  // debut: est le début de la chaine si je l'enleve alors le debut de la chaine et la fin de la chaine sont egaux alors si je fais printf tableau name est vide ex: hello cendrella sami     chahine debut: cendrella sami chahine 

	      fin = strchr(debut,' '); // fin est juste derrière la fin de la chaine : debut ex: sami chahine

		if(fin != NULL){

		  strcpy(fullname, debut);// cendrella sami chahine dans le tableau fullname

		  fullname[fin-debut]= 0;// ex: cendrella sami chahine - sami chahine = cendrella
		  
	    

		    if(strcmp(name,fullname)==0){// dans le cas ou le nom existe 
		      printf("changer l'alias %s car il existe deja \n",fullname);
		      exit(-4);

		     }else{ // le nom n'existe pas  

			strcpy(name,fullname); // ex: cendrella dans le tableau name
			printf("l'alias %s est connecte \n",name);//est ajouter dans name[%d]= %s                
		
			}

	
	    
		 
		}else{ 

			if(strcmp(name,debut) !=94 && strcmp(name,debut) != 0 ){//  si ce n'est pas un espace 94 (est considere comme un caractere) ou le nom n'existe pas               
			
				strcpy(name, debut); // ex: cendrella est dans le tableau name	
				printf("l'alias %s est connecte! \n",name);//est ajouter dans name[%d]= %s		
			
			}else{//  nom existe avant ou c'est un espace
				printf("changer l'alias %s car il esiste deja ou car c'est un espace ce qui est invalide \n ",debut);
				exit(-1);
			     }
			 
		     }
	      
	    }else{//si debut est null et debut est un espace
		  printf("la chaine est vide!! \n");
		  exit(-2);   
		 }
		    
	  }else{ //si il n'a pas entrer hello
		printf("SVP, pour connecter au serveur ecrit: hello alias\n" );
		exit(-3);
	       }

	  //creer un socket de communication
	  socketClient = socket(AF_INET , SOCK_STREAM , 0);/* creation du socket de famille AF_INET, de type TCP et 0 indique que l’on utilisera le protocole par défaut associé à SOCK_STREAM soit TCP */
		// Teste la valeur renvoyée par l’appel système socket()
	    	if (socketClient <0)/**/
	    	{
	      	    perror("Error create socket");
		    exit(1);	
		}
	  printf("Socket créée avec succès ! (%d)\n", socketClient); // Affichage du socket
	longueurAdresseClient = sizeof(adresseClient);
	 memset(&adresseClient, 0x00, longueurAdresseClient); // initialisation de adresseClient a zero de taille longueurAdresseClient
	  // Renseigne la structure sockaddr_in avec les informations du serveur 
	    adresseClient.sin_family = AF_INET; // de la famille AF_INET
	    adresseClient.sin_addr.s_addr = INADDR_ANY; // de n'importe quel adresse
	    adresseClient.sin_port = htons( PORT ); // de port 6260

	  // Obtient la longueur en octets de la structure sockaddr_in
	  longueurAdresseServeur = sizeof(adresseServeur);
	  memset(&adresseServeur, 0x00, longueurAdresseServeur);// initialisation de adresseServeur a zero de taille longueurAdresseServeur
	  // Renseigne la structure sockaddr_in avec les informations du serveur 
	    adresseServeur.sin_family = AF_INET; // de la famille AF_INET
	    adresseServeur.sin_addr.s_addr = INADDR_ANY; // de n'importe quel adresse
	    adresseServeur.sin_port = htons( PORTS ); // de port 6261

	 
	  if((connect(socketClient, (struct sockaddr*)&adresseServeur,longueurAdresseServeur)) == -1) // connecter le socketCLient a l'adresseServeur de taille longueurAdresseServeur
	  {
	     
	     perror("connect");// Affiche le message d’erreur
	   //close(socketClient);// On ferme la ressource avant de quitter
	     exit(2);// On sort en indiquant un code erreur
	  }


ndfs = getdtablesize(); // maximum size the file descriptor
  FD_ZERO(&afds); // initialiser afds a zero
  FD_SET(0, &afds); // initialiser afds a socket numero 0
  FD_SET(socketClient, &afds); // initialiser afds a socketCLient
while( 1 ) {
    /* Nettoyage du buffer */
    bzero(buf, MAXLEN);
    
    /* Copie des listes de sockets */
    memcpy(&rfds, &afds, sizeof(rfds)); // copie de afds au rfds de taille rfds
    
    /* Ecoute des sockets */
    if( select(ndfs, & rfds, NULL, NULL, NULL) == -1 ){
      /* Signaux non bloquants */
      if( errno == EINTR ) continue;
      
      fprintf(stderr, "select: %s", strerror(errno));
      exit(1);
    }
    if( FD_ISSET(0, &rfds) ) {
      /* Ecriture du client sur l'entree standard */
      buf = fgets(buf, MAXLEN, stdin);
      if( !strncmp(buf, BYE, T_BYE) ){
	sprintf(buf, "Deconnection de %s \n",name);
	write( socketClient, buf, strlen(buf)+1 );
	close(socketClient);
	return 0;
      } else {
	  char tmp[MAXLEN];

	  /* Envoi de la ligne dans la socket, avec le nom*/
	  strcpy(tmp, name);
	  strcat(tmp, ": ");
	  strcat(tmp, buf);
	  
	  
	  if( write( socketClient, tmp, strlen(tmp)+1 ) == 0 ) { // envoie du message tmp en utilisant socketCLient de taille tmp + 1
	    fprintf(stderr, "%s: socket write error \n", argv[0]);
	    exit(1);
	  }
	}
    }
    if( FD_ISSET(socketClient, &rfds) ) { // test si socketClient est affecte dans rfds
      /* Réception de données depuis la socket */
      if( read(socketClient, buf, MAXLEN) == -1 ){ // lecture du socketClient dans buf de taille MAXLEN
	fprintf(stderr, "read: lecture impossible sur la socket \n");
	exit(1);
      }   
     char *token;
     char *bufcpy = (char *)malloc(MAXLEN*sizeof(char)); // allocation de memoire de taille maxlen*char
     strcpy(bufcpy,buf); // copie du buf au bufcpy
     token = strtok(bufcpy, ":"); // diviser le bufcpy a travers le symbol ":"	
     if(strcmp(token,name)!=0) // pour ne pas ecrire chez le client lui-meme
     printf("%s",buf);// affichage du message
    }
  }
}

