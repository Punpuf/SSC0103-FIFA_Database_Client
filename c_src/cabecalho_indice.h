#ifndef CABECALHO_INDICE_H
#define CABECALHO_INDICE_H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// olhar o arquivo .c se quiser verificar a paridade do valor
#define TAMANHO_CABECALHO_INDICE sizeof(char)
#define CABECALHO_INDICE_STATUS_BOM '1'
#define CABECALHO_INDICE_STATUS_INCONSISTENTE '0'

// gerencia o cabeçalho do arquivo de índice
// os dados do cabecalho são lidos e armazenados diretamente no arquivo
// através de funções get e set
typedef struct _cabecalho_indice cabecalho_indice_t;

// cria um novo objeto que gerencia o cabeçalho do arquivo de índice
cabecalho_indice_t *cabecalho_indice_instanciar();

// libera a memória alocada para o objeto que gerencia o cabeçalho do arquivo de índice
void cabecalho_indice_liberar(cabecalho_indice_t **cabecalho);

// lê o status do cabecalho do índice diretamente do arquivo
char cabecalho_indice_get_status(cabecalho_indice_t *cabecalho);
// salva o status do cabecalho do índice diretamente no arquivo
void cabecalho_indice_set_status(cabecalho_indice_t *cabecalho, char status);

cabecalho_indice_t *cabecalho_indice_ler(FILE *arquivo);
void cabecalho_indice_escrever(FILE *arquivo, cabecalho_indice_t *cabecalho);

#endif