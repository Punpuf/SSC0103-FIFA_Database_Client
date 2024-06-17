#ifndef REGISTRO_INDICE_H
#define REGISTRO_INDICE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define TAMANHO_REGISTRO_INDICE (sizeof(int) + sizeof(long))

// registros de índice são as entidades que são armazenadas no arquivo de índice
// possuem um tamanho fixo igual a TAMANHO_REGISTRO_INDICE
// registros de índice têm seus carregados na memória
// para serem manipulados
typedef struct _registro_indice registro_indice_t;

// cria uma nova instância de registro de índice com os dados fornecidos
registro_indice_t *registro_indice_instanciar(int id, long byte_offset);

// libera a memória alocada para o registro de índice
void registro_indice_liberar(registro_indice_t **registro);

// lê o registro de índice do arquivo na posição atual do cursor do arquivo
registro_indice_t *registro_indice_ler(FILE *arquivo);

// salva o registro de índice no arquivo na posição atual do cursor do arquivo
void registro_indice_escrever(FILE *arquivo, registro_indice_t *registro);

// o id do registro referenciado por este registro de índice
int registro_indice_get_id(registro_indice_t *registro);
void registro_indice_set_id(registro_indice_t *registro, int id);

// o byte offset do registro referenciado por este registro de índice
long registro_indice_get_byte_offset(registro_indice_t *registro);
void registro_indice_set_byte_offset(registro_indice_t *registro, long byte_offset);

#endif