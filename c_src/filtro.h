#ifndef FILTRO_H
#define FILTRO_H
#include <string.h>
#include "registro.h"

// filtros são utilizados para selecionar registros
// de acordo com critérios específicos
typedef struct _filtro filtro_t;

// cria um filtro vazio, que seleciona qualquer registro
filtro_t *filtro_instanciar();

// libera a memória alocada para o filtro
void filtro_liberar(filtro_t **filtro);

// verifica se o filtro possui o campo id
bool filtro_tem_id(filtro_t *filtro);
int filtro_get_id(filtro_t *filtro);
void filtro_set_id(filtro_t *filtro, int id);

// verifica se o filtro possui o campo idade
bool filtro_tem_idade(filtro_t *filtro);
int filtro_get_idade(filtro_t *filtro);
void filtro_set_idade(filtro_t *filtro, int idade);

// verifica se o filtro possui o campo idade
bool filtro_tem_nome_jogador(filtro_t *filtro);
char *filtro_get_nome_jogador(filtro_t *filtro, int *out_tamanho);

// lê a string fornecida e guarda uma cópia dela como nome do jogador no filtro
void filtro_set_nome_jogador(filtro_t *filtro, char *nome, int tamanho);

// verifica se o filtro possui o campo nacionalidade
bool filtro_tem_nacionalidade(filtro_t *filtro);
char *filtro_get_nacionalidade(filtro_t *filtro, int *out_tamanho);

// lê a string fornecida e guarda uma cópia dela como nacionalidade no filtro
void filtro_set_nacionalidade(filtro_t *filtro, char *nacionalidade, int tamanho);

// verifica se o filtro possui o campo nomeClube
bool filtro_tem_nome_clube(filtro_t *filtro);
char *filtro_get_nome_clube(filtro_t *filtro, int *out_tamanho);
// lê a string fornecida e guarda uma cópia dela como nome do clube no filtro
void filtro_set_nome_clube(filtro_t *filtro, char *nome_clube, int tamanho);

// retorna true se um registro satisfaz as condições do filtro
bool filtro_se_aplica(filtro_t *filtro, registro_t *registro);
#endif