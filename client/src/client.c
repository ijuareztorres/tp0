#include "client.h"
#include "utils.h"
#include <commons/config.h>
#include <readline/readline.h>
#include <string.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger; //log_create("tp0.log", "CLIENTE", true, LOG_LEVEL_INFO);
	t_config* config; //config_create("cliente.config")

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	valor = config_get_string_value(config, "CLAVE");
	// Loggeamos el valor de config
	log_info(logger, "Leí la clave: %s", valor);
	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);
	//log_destroy(logger);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}
t_log* iniciar_logger(void)
{
	t_log* logger;
	if((logger = log_create("tp0.log", "TP0", 1, LOG_LEVEL_INFO)) ==NULL) {
		printf("No pudo crear el logger\n");
		exit(1);
	}
	return logger;
}

t_config* iniciar_config(void)
{
	t_config* config;
	if((config= config_create("cliente.config")) == NULL) {
		printf("No pudo leer la config\n");
		exit(2);
	}
	return config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (leido!=NULL && strcmp (leido, "") !=0) {
		log_info(logger,"Leí de consola: %s", leido);
		free(leido);
		leido = readline("> ");
	}
 
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;
	paquete=crear_paquete();

	leido = readline("> ");
	while (leido != NULL && strcmp(leido, "") !=0 ){
		agregar_a_paquete(paquete,leido,strlen(leido)+1);
		free(leido);
		leido = readline("> ");
	}
	free(leido);
	// Leemos y esta vez agregamos las lineas al paquete

	enviar_paquete(paquete, conexion);
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	if(logger!=NULL) {
		log_destroy(logger);
	}

	if(config!=NULL) {
		config_destroy(config);
	}
}
