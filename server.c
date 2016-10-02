
#include <stdio.h>//printf
#include <stdlib.h> /* pour exit */
#include <sys/types.h> //accept, connect types de données élémentaires (u_char, u_long, ...)
#include <sys/socket.h> //déclarations concernant les sockets
#include <arpa/inet.h> //pour htons et inet_aton
#include <string.h>  /* pour memset*/
#include <netinet/in.h> // déclarations concernant le protocole IP  struct sockaddr_in 
#include <unistd.h>// pour close
#include <errno.h> // pour detecter les erreurs
#include <fcntl.h> // contient les methodes du file descriptor
#define PORTS 6261 // variable definie
#define BUFSIZE 1024 // variable definie
char *lire_client(int sock); // declaration d'entete de la methode lire_client
int main()
{

char name[100]; // declaration d'une array de type char
int socketEcoute, socketService; //socketEcoute--> server socket
struct sockaddr_in adresseClient, adresseServeur; // declaration deux variables de type sockaddr_in pour 


int nfds; // variable pour la taille total du file descriptor
fd_set rfds, afds; // variables de type file descriptor 
int fd,c; //variable utilise comme indice de loop 
char *message; // pour lire le message du client

socklen_t longueurAdresseClient; // variable qui represente la taille de l'adresse client

socketEcoute = socket(AF_INET, SOCK_STREAM, 0);/* Crée un socket de communication , de la famille AF_INET, sock_stream qui designe TCP
,0 indique que l’on utilisera le protocole par défaut associé à SOCK_STREAM soit TCP */

if(socketEcoute < 0) // Teste la valeur renvoyée par l’appel système socket() 
{
perror("error socket");// Affiche le message d’erreur
exit(-1);// On sort en indiquant un code erreur
}
printf("Socket créée avec succès ! (%d)\n", socketEcoute); // affichage du socket cree
 
longueurAdresseClient = sizeof(adresseClient); // On prépare l’adresse d’attachement locale

memset(&adresseServeur, 0x00, longueurAdresseClient); // Initialise à 0 la structure adresseServeur de taille longueurAdresseClient
  /* Initialisation du socket serveur */
adresseServeur.sin_family    = AF_INET; // de la famille AF_INET
adresseServeur.sin_addr.s_addr = INADDR_ANY; // d'adresse n'importe quel adresse
adresseServeur.sin_port = htons(PORTS); // port 6261
// On demande l’attachement local de la socket
if((bind(socketEcoute, (struct sockaddr *)&adresseServeur,longueurAdresseClient)) < 0)
{
perror("error bind: adresse utilisee");
exit(-2);
}
printf("Socket attachée avec succès au port: %d \n", PORTS);
// On fixe la taille de la file d’attente à 5 (pour les demandes de connexion non encore traitées)
if(listen(socketEcoute, 5) < 0){
perror("error listen:  mise en ecoute impossible");
exit(-3);
}


/* Init des listes de sockets */
  nfds = getdtablesize();  // le maximum size d'un file descriptor est affecte a nfds
  FD_ZERO(&afds); // initialiser afds a zero
  FD_SET(socketEcoute, & afds); // affectation du socket  socketEcoute au file descriptor afds





/* Gestion des clients */
while(1){

/* Copie des listes de sockets */
    memcpy(&rfds, &afds, sizeof(rfds)); // copie du file descriptor afds au rfds de taille rfds

/* Surveillance des descripteurs en lecture */
    if( select(nfds, & rfds, 0, 0, 0) == -1 ) {
      /* Signaux non bloquants */
      if( errno == EINTR ) continue;
      
      fprintf(stderr,"select: %s", strerror(errno));
      exit(10);
    }



if( FD_ISSET(socketEcoute, &rfds) ) {  // test si socketEcoute fait partie du rfds

socketService = accept(socketEcoute, (struct sockaddr*)&adresseClient, &longueurAdresseClient); // confirmation du socketEcoute sur adresseClient de taille longueurAdresseClient
if(socketService < 0){
perror("error accept:  connexion impossible");
exit(-4);
}
printf("Connexion d'un client depuis %s \n", inet_ntoa(adresseClient.sin_addr));
fflush(stdout);

/* Ajout du client dans les sockets a surveiller */
      FD_SET(socketService, &afds);

/* Lecture non bloquante */
      fcntl(socketService, F_SETFL, O_NONBLOCK | fcntl(socketService, F_GETFL, 0));
      
    }
/* Tester si les sockets clientes ont bougees */
    for( fd=0; fd<nfds; fd++ )
      if( fd != socketEcoute && FD_ISSET(fd, &rfds) )
	/* Traiter le client */
	if( (message = lire_client(fd)) == NULL ) { // dans le cas ou il n'y a pas un message
	  close(fd);
	  FD_CLR(fd, &afds);
	  fprintf(stderr, "-- perte d'un client ! --\n");
	}else
	  /* Envoyer le message a tous les clients */
	  for( c=0; c<nfds; c++)
	    if( c!=socketEcoute && FD_ISSET(c, &afds) )
	      write(c, message, strlen(message)+1);
  }
}

char *lire_client(int sock) // definir la methode lire_client
{
  char *msg = (char *)malloc(BUFSIZE * sizeof(char)); // allocation du bufsize dans la memoire
  int nb_lu;

   nb_lu = read(sock, msg, BUFSIZE); // lire le msg du socket sock de taille BUFSIZE

  if( nb_lu > 0 ) {
    printf("%s", msg);// affichage du message

    return msg;
  } else
    return NULL;
}

