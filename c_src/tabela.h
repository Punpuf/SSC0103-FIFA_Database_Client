#ifndef TABELA_H
#define TABELA_H

#include <string.h>
#include "cabecalho.h"
#include "registro.h"
#include "filtro.h"
#include "indice.h"

typedef struct _tabela tabela_t;

// Cria um novo objeto que representa a tabela utilizando os dados fornecidos no arquivo de entrada
// Esta operação não altera o arquivo, mas o lê para carregar o cabeçalho
tabela_t *tabela_instanciar(FILE *arquivo);
// Destroi o objeto que representa a tabela e libera a memória alocada
// Esta operação não lê nem escreve no arquivo
void tabela_liberar(tabela_t **tabela);

// Lê os dados do arquivo csv e os coloca no arquivo de banco de dados,
// substituindo os dados antigos
void tabela_ler_csv(tabela_t *tabela, FILE *csv);

// Exibe na tela os dados de todos os registros da tabela
void select_todos(tabela_t *tabela);
// Exibe na tela os dados dos registros que satisfazem o filtro
void select_where(tabela_t *tabela, filtro_t *filtro);

// Apaga todos os registros que satisfazem o filtro
// e atualiza o índice.
//
// Os registros são marcados como logicamente removidos
// e inseridos na lista de registros removidos
void delete_where(tabela_t *tabela, indice_t *indice, filtro_t *filtro);

// Insere o novo registro na tabela e atualiza o índice, tentando reutilizar o espaço
// utilizado por registros logicamente removidos.
//
// Se não houver espaço disponível, o registro é inserido no final do arquivo
void insert_into(tabela_t *tabela, indice_t *indice, registro_t *registro);

// Cria um novo índice na memória com base nos dados da tabela
// essa operação não modifica nem cria um arquivo de índice.
indice_t *tabela_gerar_indice(tabela_t *tabela);

// Retorna o cabeçalho da tabela
cabecalho_t *tabela_get_cabecalho(tabela_t *tabela);

#endif