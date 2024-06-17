#ifndef REGISTRO_H
#define REGISTRO_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "cabecalho.h"

// registros são as entidades que são armazenadas no arquivo
// cada registro possui um tamanho variável
// diferentemente do cabeçalho, registros têm seus carregados na memória
// para serem manipulados
typedef struct _registro registro_t;

// cria objeto de registro com os dados fornecidos
// não modifica o arquivo de banco de dados por si só
registro_t *registro_instanciar(int id, int idade, char *nome_jogador, int tam_nome, char *nacionalidade, int tam_nacionalidade, char *nome_clube, int tam_nome_clube);

// destroi o objeto de registro e libera a memória alocada
// não modifica o arquivo de banco de dados por si só
void registro_liberar(registro_t **registro);

// carrega um registro do arquivo de banco de dados
// na posição atual do cursor do arquivo
registro_t *registro_ler(FILE *arquivo);

// salva um registro no arquivo de banco de dados
// na posição atual do cursor do arquivo
void registro_escrever(FILE *arquivo, registro_t *registro);

// retorna true se o registro foi logicamente removido, false caso contrário
bool registro_get_removido(registro_t *registro);
void registro_set_removido(registro_t *registro, bool removido);

// o tamanho em bytes do registro
int registro_get_tamanho_registro(registro_t *registro);
void registro_set_tamanho_registro(registro_t *registro, int tamanho);

// o byte offset do próximo registro logicamente removido na lista
long registro_get_prox(registro_t *registro);
void registro_set_prox(registro_t *registro, long prox);

// o id do registro, ele é único para cada registro
int registro_get_id(registro_t *registro);
void registro_set_id(registro_t *registro, int id);

// a idade do jogador
int registro_get_idade(registro_t *registro);
void registro_set_idade(registro_t *registro, int idade);

// o nome do jogador
char *registro_get_nome_jogador(registro_t *registro, int *out_tamanho);
void registro_set_nome_jogador(registro_t *registro, char *nome, int tamanho);

// a nacionalidade do jogador
char *registro_get_nacionalidade(registro_t *registro, int *out_tamanho);
void registro_set_nacionalidade(registro_t *registro, char *nacionalidade, int tamanho);

// o nome do clube do jogador
char *registro_get_nome_clube(registro_t *registro, int *out_tamanho);
void registro_set_nome_clube(registro_t *registro, char *nome_clube, int tamanho);

#endif