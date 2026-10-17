// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <math.h>

#define DEFAULT_PORT 54847 //8080
#define DEFAULT_DOMAIN "localhost"
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	int sock = 0;
	struct sockaddr_in serv_addr = {0};
	char recv_buffer[BUFFER_SIZE] = {0};
    char send_buffer[BUFFER_SIZE] = {0};
	char cadena[BUFFER_SIZE]      = "";
    double input = 0.0;
    int network_byte_order_a = 0;
    int network_byte_order_b = 0;    
    int host_byte_order_a = 0;
    int host_byte_order_b = 0;    

    char domini[BUFFER_SIZE] = {0};
	int port = 0;
	char *endptr = NULL;

	if (argc == 1) {
		// Cap argument -> valors per defecte
		//domini = DEFAULT_DOMAIN;
        strcpy(domini, DEFAULT_DOMAIN);
		port = DEFAULT_PORT;
		printf("Cap argument indicat. Utilitzant valors per defecte: %s %d\n", domini, port);
	} 
	else if (argc == 3) {
		//domini = argv[1];
        strcpy(domini, argv[1]);
        
        printf("Domini: %s\n", domini);
        
        if (strcmp(domini, "localhost") == 0)
            strcpy(domini, "127.0.0.1");

		errno = 0;
		port = strtol(argv[2], &endptr, 10);

		// Comprovacions de validesa
		if (errno != 0 || *endptr != '\0' || port < 1 || port > 65535) {
			fprintf(stderr, "Error: el port ha de ser un enter entre 1 i 65535.\n");
			exit(EXIT_FAILURE);
		}
	} 
	else {
		fprintf(stderr, "Ús: %s [nom_de_domini port]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Crear el socket
	// Afegiu comentari explicant els arguments, i de quines altres opcions hi ha
	// per al segon d'ells (ara SOCK_STREAM)
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\nError en crear el socket\n");
		return -1;
	}

	// Afegiu comentari sobre què són aquests 3 paràmetres,
	// per què s'utilitzen els valors que hi ha,
	// i per què s'utilitza htons()

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	//inet_pton(AF_INET, domini, &serv_addr.sin_addr);	// Convertir adreça IPv4 a binari
    inet_pton(AF_INET, domini, &serv_addr.sin_addr);	// Convertir adreça IPv4 a binari
   

	// Connectar amb el servidor
	// Afegiu comentari explicant els arguments
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnexió fallida.\n");
		return -1;
	}

	printf("Connectat al servidor. Podeu començar a enviar missatges.\n");

	int option = 0;

    
	while (1) {
		memset(recv_buffer, 0, sizeof(recv_buffer)); // Netegem el recv_buffer
        memset(send_buffer, 0, sizeof(send_buffer));

		// Menú principal
		// Cal que implementeu un petit servei remot amb almenys 4 funcionalitats noves
		// Definiu vosaltres mateixos les dades a enviar (demanar en el cleint, i analitzar al servidor) i rebre
		// Si per donar més sentit al servei cal alguna opció més, la podeu afegir
		// Deixeu l'opció 1 com a Enviar missatge i l'última per Sortir

		printf("Menú principal:\n");
		printf("1. Enviar missatge\n");
		printf("2. Convertir de Celsius a Fahrenheit\n");
		printf("3. \"Padding\" de \"struct\"\n");
		printf("4. Conversió de un número entre \"Little Endian\" and \"Big Endian\"\n");
		printf("5. Enters en un buffer\n");
		printf("6. Sortir\n");
		printf("Opció: ");

		scanf("%d", &option);
		while (getchar() != '\n');  // buida el recv_buffer fins al salt de línia

		switch (option)
		{
		case 1:
		case 6:
			if (option==1){
				printf("Introdueix el missatge a enviar ('EXIT' per tancar el servidor i sortir): ");
				fgets(cadena, BUFFER_SIZE, stdin);
				cadena[strcspn(cadena, "\n")] = '\0';  // Eliminar \n final
			} else {
				strcpy(cadena, "EXIT");
			}

			// Enviar missatge al servidor
			// Afegiu control d'errors
			// Afegiu comentari explicant els arguments

			send(sock, cadena, strlen(cadena), 0);

			// Llegir resposta del servidor
			// Afegiu un control d'errors al read()
			// Afegiu comentari explicant què fa i per què s'utilitza memset
			// Afegiu comentari explicant els arguments de la crida a read()

			memset(recv_buffer, 0, BUFFER_SIZE);
			read(sock, recv_buffer, BUFFER_SIZE);
			//printf("Resposta del servidor: %s\n", recv_buffer);
            printf("%s\n", recv_buffer);

			if (strcmp(cadena, "EXIT") == 0) {
				close(sock);
				return 0;
			}
			break;

        // 2. Convertir de Celsius a Fahrenheit
		case 2:
			// Implementar Opció 2
            printf("Please enter a temperature (with or without decimals) in Celsius to convert to Fahrenheit: ");
            scanf("%lf", &input);
            while (getchar() != '\n');
            //printf("%f\n", input);
            sprintf(send_buffer, "2%f", input);
            send(sock, send_buffer, strlen(send_buffer), 0);
            
			//read(sock, recv_buffer, BUFFER_SIZE);
            recv(sock, recv_buffer, BUFFER_SIZE, 0);
			printf("%s\n", recv_buffer);            
			break;
            
        // 3. El "padding" de un "struct"
		case 3:
			// Implementar Opció 3
            /*
            struct padding 
            {
                int    a; // 4 bytes
                short  b; // 2 bytes
                int    c; // 4 bytes
                double d; // 8 bytes
                char   e[1022]; // 1022 bytes
            };
            */            
            sprintf(send_buffer, "3", input);
            send(sock, send_buffer, strlen(send_buffer), 0);
            
            recv(sock, recv_buffer, BUFFER_SIZE, 0);
			printf("%s\n", recv_buffer);               
            
    		break;

        // 4. Conversió de un número entre "Little Endian" and "Big Endian"
		case 4:
			// Implementar Opció 4
            printf("Please enter a natural number between 1 and 100000: ");
            scanf("%ld", &input);
            while (getchar() != '\n');
            sprintf(send_buffer, "4%ld", input);
            send(sock, send_buffer, strlen(send_buffer), 0);
            
            recv(sock, recv_buffer, BUFFER_SIZE, 0);
			printf("%s\n", recv_buffer);              
			break;
        
        // 5. Enters en un buffer
		case 5:
			// Implementar Opció 5
            printf("Please enter a number between -%.0f and %.0f: ",
                    pow(2.0, 31.0), pow(2.0, 31.0) - 1);
            scanf("%d", &host_byte_order_a);
            while (getchar() != '\n');            
            int network_byte_order_a = ntohl(host_byte_order_a);
            printf("0x%x\n", network_byte_order_a);
            //sprintf(send_buffer, "%d", a);
            
            send_buffer[0] = '5';
            memcpy(send_buffer+1, &network_byte_order_a, sizeof(network_byte_order_a));
            send(sock, send_buffer, sizeof(network_byte_order_a) + 1, 0);

            memset(recv_buffer, 0, BUFFER_SIZE);
            recv(sock, recv_buffer, BUFFER_SIZE, 0);
            printf("%s\n", recv_buffer);
            break;

		default:
			printf("Opció invàlida\n");
			break;
		}
        printf("Press any key to continue...\n");
        getchar();
	}

	close(sock);
	return 0;
}
