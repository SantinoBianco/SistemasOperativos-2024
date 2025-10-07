#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>  // Necesario para waitpid

#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "builtin.h"

#include "obfuscated.h"

static void show_prompt(void) {
    printf("mybash> ");
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    pipeline pipe;
    Parser input;
    bool quit = false;

    input = parser_new(stdin);
    while (!quit) {
        ping_pong_loop(NULL);
        show_prompt();
        pipe = parse_pipeline(input);

        /* Hay que salir luego de ejecutar? */
        quit = parser_at_eof(input);
        
        if (pipe != NULL) {
            // Obtener el primer comando del pipeline como string
            char *command = pipeline_to_string(pipe);  // Asumimos que esta función convierte el pipeline a una cadena

            // Si el comando es "exit", salir del bucle
            if (strcmp(command, "exit") == 0) {
                quit = true;
            } else if (strcmp(command, "ls") == 0) {
                // Si el comando es "ls" sin argumentos, ejecutar `ls`
                pid_t pid = fork();
                if (pid == 0) {
                    // Proceso hijo: ejecutar `ls`
                    execlp("ls", "ls", (char *)NULL);
                    perror("execlp");
                    exit(EXIT_FAILURE);
                } else if (pid > 0) {
                    // Proceso padre: esperar a que termine el hijo
                    int status;
                    waitpid(pid, &status, 0);
                } else {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
            } else {
                // Aquí iría la lógica para ejecutar otros comandos o built-ins
                execute_pipeline(pipe);
            }
            pipeline_destroy(pipe);  // Liberar la memoria usada por el pipeline
        }
    }
    parser_destroy(input); input = NULL;
    return EXIT_SUCCESS;
}
