#include <stdio_ext.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

/*
 * Servidor UDP
 */

int main(int argc, char **argv)
{
        FILE *arq;
	struct timeval tempo_entrada,tempo_saida;
	long int delta; //Armazena o tempo de utilizacao da sala

	int sockint,s, namelen, client_address_size;
	struct sockaddr_in client, server;
	char buf[10];

	if(argc != 2)
	{
		printf("Use: %s porta.\n",argv[0]);
		exit(1);
	}

   	/*
    	* Cria um socket UDP (dgram). 
    	*/
  	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   	{
      		perror("socket()");
       		exit(1);
   	}

   	/*
    	* Define a qual endereço IP e porta o servidor estará ligado.
    	* Porta = 0 -> faz com que seja utilizada uma porta qualquer livre.
    	* IP = INADDDR_ANY -> faz com que o servidor se ligue em todos
    	* os endereços IP
    	*/
	
   	server.sin_family      = AF_INET;   /* Tipo do endereço             */	
   	server.sin_port        = htons((atoi(argv[1])));   /*define a porta entrada pelo usuario*/
	server.sin_addr.s_addr = INADDR_ANY;/* Endereço IP do servidor      */

   	/*
    	* Liga o servidor à porta definida anteriormente.
    	*/
   	if (bind(s, (struct sockaddr *)&server, sizeof(server)) < 0)
   	{
      		perror("bind()");
       		exit(1);
   	}

   	/* Consulta qual porta foi utilizada. */
   	namelen = sizeof(server);
  	if (getsockname(s, (struct sockaddr *) &server, &namelen) < 0)
   	{
       		perror("getsockname()");
		exit(1);
  	}
	
   	/* Imprime qual porta foi utilizada. */
 	printf("Porta utilizada eh %d\n", ntohs(server.sin_port));

	for(;;)
	{
		client_address_size = sizeof(client);
		
		
		if(recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &client,
			 &client_address_size) <0)
		{
			perror("recvfrom()");
			exit(1);
		}

		int numero = atoi(buf);
		
		switch(numero)
		{
			case 1: 
				if(gettimeofday(&tempo_entrada,NULL)==-1)
				{
					printf("Impossivel conseguir o tempo atural");
				}
				break;			
		
	
			case 2:
				if(recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *) &client,
				 &client_address_size) <0)
				{
					perror("recvfrom()");
					exit(1);
				}
				int numero_pessoas=atoi(buf);
			
				printf("numero maximo de pessoas na sala %d\n",numero_pessoas);

				if(gettimeofday(&tempo_saida,NULL)==-1)
				{
					printf("Impossivel conseguir o tempo atual");
				}
				printf("Tempo entrada = %ld\n",(long int)tempo_entrada.tv_sec);
		                printf("Tempo saida = %ld\n",(long int)tempo_saida.tv_sec);
				delta= tempo_saida.tv_sec - tempo_entrada.tv_sec;
				//delta+= tempo_saida.tv_usec - tempo_entrada.tv_usec;

				printf("A sala foi utilizada por %.2ld segundos\n\n",delta);
			
		                arq=fopen("relatorio.txt","a");
		                if(arq==NULL)
		                {
		                    printf("Erro ao abrir o arquivo\n");
		                    exit(1);
		                }

				time_t  t= time(NULL);
				struct tm *tm = localtime(&t);
				char s[64];
				strftime(s,sizeof(s),"%c",tm);
			

		                fprintf(arq,"%s\nQuantidade de pessoas na sala = %d\nTempo de utilizacao da sala = %.2ld segundos\n\n",s,numero_pessoas,delta);
		                fclose(arq); 
				break;
		}
	}
  	/*
    	* Fecha o socket.
    	*/
   	close(s);
}
