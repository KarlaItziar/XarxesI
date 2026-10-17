// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define DEFAULT_PORT 54847 //8080
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	int sock, new_socket;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char recv_buffer[BUFFER_SIZE] = {0};
    char send_buffer[BUFFER_SIZE] = {0};
    char buffer[BUFFER_SIZE]      = {0};
    int  network_byte_order_a = 0;
    int  network_byte_order_b = 0;    
    int  host_byte_order_a    = 0;
    int  host_byte_order_b    = 0;   

	int port;
	char *endptr;
    int char_read = 0;

	if (argc == 1) {
		// Cap argument -> port per defecte
		port = DEFAULT_PORT;
		printf("Cap port indicat. Utilitzant valor per defecte: %d\n", port);
	}
	else if (argc == 2) {
		errno = 0;
		port = strtol(argv[1], &endptr, 10);

		// Comprovacions
		if (errno != 0 || *endptr != '\0' || port < 1 || port > 65535) {
			fprintf(stderr, "Error: el port ha de ser un enter entre 1 i 65535.\n");
			exit(EXIT_FAILURE);
		}
	}
	else {
		fprintf(stderr, "Ús: %s [port]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Crear el socket
	// Afegiu comentari explicant els arguments
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("Error en crear el socket");
		exit(EXIT_FAILURE);
	}

	// Afegiu comentari explicant què són aquests 3 paràmetres,
	// per què s'utilitzen htons() i htonl(),
	// i per què s'utilitzen els valors que hi ha

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);	// Per a INADDR_ANY no faria falta htonl(), però li posem per coherència
	address.sin_port = htons(port);

    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	// Enllaçar el socket al port especificat
	// Afegiu comentari explicant els arguments
	if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("Error en fer el bind");
		close(sock);
		exit(EXIT_FAILURE);
	}

	// Escoltar connexions entrants
	// Afegiu comentari explicant els arguments
	
	if (listen(sock, 3) < 0) {
		perror("Error en escoltar");
		close(sock);
		exit(EXIT_FAILURE);
	}

	printf("Servidor en funcionament, esperant connexions...\n");

	while (1) {
		// Acceptar connexions de clients
		// Afegiu comentari explicant els arguments i per què hi ha i cal new_socket si ja tenim sock
		
		if ((new_socket = accept(sock, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
			perror("Error en acceptar la connexió");
			close(sock);
			exit(EXIT_FAILURE);
		}

		while (1) {
			memset(recv_buffer, 0, BUFFER_SIZE);
            memset(send_buffer, 0, BUFFER_SIZE);
			
			// Llegir missatge del client
			if ((char_read = read(new_socket, recv_buffer, BUFFER_SIZE)) <= 0) {
				printf("Client desconnectat (PID: %d)\n", getpid());
				break;
			}

			// Aquí haureu d'implementar l'anàlisi de la cadena rebuda per saber l'operació,
			// i si n'hi ha els arguments, executar-la i tornar el(s) resultat(s)

			// A continuació hi ha el codi corresponent a l'opció d'Enviar missatge, amb el retorn d'una cadena fixa,
			// i la de Sortida de la connexió/bucle si és el cas.
			// Modifiqueu la cadena de retorn per tal que sigui la que diu l'enunciat.

			printf("Missatge rebut del client (PID: %d): %s\n", getpid(), recv_buffer);
            
            // 2. Convertir de Celsius a Fahrenheit
            if (recv_buffer[0] == '2') {
                double celsius = atof(recv_buffer + 1);
                //printf("%f\n", celsius);
                double fahrenheit = celsius * 9.0 / 5.0 + 32.0;
                //printf("double: %f\n", fahrenheit);
                sprintf(send_buffer, "===> %0.2f° Celsius is equal to %0.2f° Fahrenheit.\n", celsius, fahrenheit);
                send(new_socket, send_buffer, strlen(send_buffer), 0);
            }
            // 3. El "padding" de un "struct" 
            else if (recv_buffer[0] == '3') {
                char msg[4][512] = {0};
                struct padding {
                    int    a;       // 4 bytes
                    short  b;       // 2 bytes
                    int    c;       // 4 bytes
                    double d;       // 8 bytes
                    char   e[1022]; // 1022 bytes
                };
                
                sprintf(msg[0], "\nstruct padding {\n    int    a;       // 4 bytes\n    short  b;       // 2 bytes\n    int    c;       // 4 bytes\n    double d;       // 8 bytes\n    char   e[1022]; // 1022 bytes\n};\nstruct padding var;\n\n");
                
                struct padding var;
                
                sprintf(msg[1], "La suma de los tamaños de todos los elementos de la estructura es sizeof(var.a) + sizeof(var.b) + sizeof(var.c) + sizeof(var.d) + sizeof(var.e) = %d.\n\n",
                    sizeof(var.a) + sizeof(var.b) + sizeof(var.c) + sizeof(var.d) + sizeof(var.e));
                sprintf(msg[2], "PERO el tamaño de la estructura es sizeof(var) = %d.\n\n", sizeof(var));

                sprintf(msg[3], "Es así porque hay bytes que están vacíos entre algunos de los elementos de la estructura. Por tanto,\n"
                    "es posible que la alineación de estos elementos sea diferente entre el servidor y el cliente.\n"
                    "Para evitar este problema de la alineación, sería mejor no utilizar la estructura\n"
                    "para enviar los datos en el buffer que se pasa a las funciones send() y recv() como argumento.\n");
                    
                for (int i = 0; i < 4; i++)
                    strcat(send_buffer, msg[i]);

                // send_buffer[0] = '3';
                // memcpy(send_buffer+1, &network_byte_order_a, sizeof(network_byte_order_a));
                send(new_socket, send_buffer, strlen(send_buffer) + 1, 0);                  
            }
            // 4. Conversió de un número entre "Little Endian" and "Big Endian"
            else if (recv_buffer[0] == '4') { 
                char msg[6][256] = {0};
                unsigned char *ptr = NULL; // declare a pointer to point to a char or byte in memory

                host_byte_order_a = atoi(recv_buffer + 1);
                
                printf("%d\n", host_byte_order_a);
               
                sprintf(msg[0], "\nThe number you just entered is %d (in decimal)\n", host_byte_order_a);
                sprintf(msg[1], "This number in hexadecimal is 0x%08x\n", host_byte_order_a);

                ptr = (unsigned char *)&host_byte_order_a;
                sprintf(msg[2], "The order of this number is in HOST BYTE ORDER / LITTLE ENDIAN:\nFirst Byte  = 0x%02x\nSecond Byte = 0x%02x\nThird Byte  = 0x%02x\nFourth Byte = 0x%02x\n",
                    *ptr, *(ptr+1), *(ptr+2), *(ptr+3));
                
                sprintf(msg[3], "\nNow converting the number from little endian to big endian using htonl().\n");
                
                network_byte_order_a = htonl(host_byte_order_a);

                sprintf(msg[4], "After conversion the number (in hexdecimal) is 0x%08x\n", network_byte_order_a);
                
                ptr = (unsigned char *)&network_byte_order_a;
                sprintf(msg[5], "The order of the number in NETWORK BYTE ORDER / BIG ENDIAN is:\nFirst Byte  = 0x%02x\nSecond Byte = 0x%02x\nThird Byte  = 0x%02x\nFourth Byte = 0x%02x\n",
                    *ptr, *(ptr+1), *(ptr+2), *(ptr+3));
        
                memset(send_buffer, 0 , BUFFER_SIZE);
                for (int i = 0; i < 6; i++)
                    strcat(send_buffer, msg[i]);
                
                send(new_socket, send_buffer, strlen(send_buffer) + 1, 0);
            }
            // 5. Enteros en un buffer
            else if (recv_buffer[0] == '5') {
                for (int i = 0; i < char_read; i++) {
                    printf("%d: 0x%02x\n", i, recv_buffer[i]);
                }
                network_byte_order_a = 0;
                host_byte_order_a    = 0;
                
                memcpy(&network_byte_order_a, &recv_buffer[1], sizeof(int));
                host_byte_order_a = ntohl(network_byte_order_a);

                printf("%d\n", host_byte_order_a);
                
                memset(send_buffer, 0, BUFFER_SIZE);
                sprintf(send_buffer,
                "===> This message is from the server. The number you just entered is: %d\n", host_byte_order_a);
            
                // send_buffer[0] = '3';
                // memcpy(send_buffer+1, &network_byte_order_a, sizeof(network_byte_order_a));
                send(new_socket, send_buffer, strlen(send_buffer) + 1, 0);
            }
			// Comprovar si el client vol tancar la connexió
			else if (strcmp(recv_buffer, "EXIT") == 0) {
				printf("Tancant connexió amb el client (PID: %d)...\n", getpid());
				send(new_socket, "Connexió tancada.\n", strlen("Connexió tancada\n"), 0);
				break;	// Sortir del bucle
			} else {
                sprintf(send_buffer, "===> Missatge rebut: %s\n", recv_buffer);
				//send(new_socket, "Missatge rebut\n", strlen("Missatge rebut\n"), 0);
                send(new_socket, send_buffer, strlen(send_buffer), 0);
			}
		}

		close(new_socket); // Tancar la connexió amb el client
	}

	close(sock);
	return 0;
}
