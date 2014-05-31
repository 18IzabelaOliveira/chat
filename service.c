/*IZABELA*/
//gcc -o service service.c $(pkg-config --cflags --libs gtk+-2.0) -lpthread



#include <gtk/gtk.h>
#include <string.h>	
/*parte udp*/
#include <unistd.h>
#include <sys/time.h> /* select() */
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>

#define MAX_MSG 100
#define MAXCONECT 2
#define REMOTE_CLIENT_PORT 2000
#define LOCAL_SERVER_PORT 1500


int ds, n, cliLen, msgerro, escutador, rc;
struct sockaddr_in cliAddr, servAddr;
char msg[MAX_MSG], mensagem[10];


void* enviar(void *arg);
void* receber(void *arg);
/* final parte udp*/

GtkWidget *campoTexto, *labelMensagem, *janela; 

void botaoEnviarClicado(GtkWidget *w, gpointer p) {
	char aux1[1000];
	char aux2[1000];
	strcpy(aux1,gtk_entry_get_text(GTK_ENTRY(campoTexto)));

  rc = sendto(ds, aux1, strlen(aux1), 0, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
        if(rc<0) {
            printf("Dados não foram enviados \n");
            close(ds);
        }

	char titulo[100];
	strcpy(aux2,gtk_label_get_text(GTK_LABEL(labelMensagem)));
	strcpy(titulo, "\nServidor: ");
	strcat(titulo, aux1);

	strcat(aux2,titulo); 
	gtk_label_set_text(GTK_LABEL(labelMensagem), aux2);
}


void inicializaInterface() {
    GError    *error = NULL;
    

	gtk_init( NULL, NULL);

  	janela = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  	gtk_window_set_title(GTK_WINDOW(janela), "Chat Servidor");
	gtk_window_set_default_size(GTK_WINDOW(janela), 700, 400);

 	GtkWidget *layout = gtk_fixed_new();
  
  	campoTexto = gtk_entry_new();
  	gtk_widget_set_size_request(campoTexto, 670, 30);

	GtkWidget *botaoEnviar = gtk_button_new_with_label("Enviar");
  	gtk_widget_set_size_request(botaoEnviar, 90, 30);
 	g_signal_connect(G_OBJECT(botaoEnviar), "clicked", G_CALLBACK(botaoEnviarClicado), NULL);

  	labelMensagem=gtk_label_new(" ");
	gtk_widget_set_size_request(labelMensagem, 670, 300);
  	
gtk_fixed_put(GTK_FIXED(layout), campoTexto, 20, 350);
  	gtk_fixed_put(GTK_FIXED(layout), botaoEnviar, 700, 350);
  	gtk_fixed_put(GTK_FIXED(layout), labelMensagem, 20, 10);

        gtk_container_add(GTK_CONTAINER(janela), layout);

 	gtk_widget_show_all(janela);
	
  	g_signal_connect_swapped(G_OBJECT(janela), "destroy",G_CALLBACK(gtk_main_quit), NULL);
   
    	gtk_main();
}

int main(int argc,char **argv) {
    cliAddr.sin_family = AF_INET;
    cliAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    cliAddr.sin_port = htons(REMOTE_CLIENT_PORT);


   ds=socket(AF_INET, SOCK_DGRAM, 0);

    if(ds<0)
    {
        printf("Socket não pode ser aberto!\n");
        //exit(1);
    }

    //Vincular Porta Local do Servidor
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(LOCAL_SERVER_PORT);


    rc = bind (ds, (struct sockaddr *) &servAddr,sizeof(servAddr));

    if(rc<0)
    {
        printf("Nao pode ser vinculada a porta de numero: %d \n",LOCAL_SERVER_PORT);

    }

    escutador = listen(ds, MAXCONECT);


    pthread_t thread1;



    pthread_create(&thread1, NULL, &receber, NULL);





	inicializaInterface();
	return( 0 );
}


void* receber(void *arg)
{

	char aux2[1000];

	

    while(1)
    {

        //Iniciando o buffer
        memset(msg,0x0,MAX_MSG);


        //Recebendo a mensagem
        cliLen = sizeof(cliAddr);

        n = recvfrom(ds, msg, MAX_MSG, 0, (struct sockaddr *) &cliAddr, &cliLen);

        if(n<0)
        {
            printf("\nNao foi possivel receber os dados! \n");
            continue;
        }

        //Printar a mensagem recebida
//        printf("Mensagem Recebida: %s \n" ,);

	char titulo[100];
	strcpy(aux2,gtk_label_get_text(GTK_LABEL(labelMensagem)));
	strcpy(titulo, "\nCliente: ");
	strcat(titulo, msg);

	strcat(aux2,titulo); 
	gtk_label_set_text(GTK_LABEL(labelMensagem), aux2);
    }

}


