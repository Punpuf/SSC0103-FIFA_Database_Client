#include "cabecalho.h"
#include "registro.h"
#include "tabela.h"
#include "indice.h"
#include "funcoes_fornecidas.h"

// Membros do grupo:
// Jean Patrick Ngandu Mamani - 14712678
// Didrick Chancel EGNINA NDOMBI - 14822368

#define MENSAGEM_ERRO_ARQUIVO "Falha no processamento do arquivo."

filtro_t *ler_filtro();
registro_t *ler_registro();

bool tabela_valida(tabela_t *tabela);

int main()
{
    int funcionalidade;
    char nome_csv[100];
    char nome_binario[100];

    scanf("%d", &funcionalidade);

    if (funcionalidade == 1)
    {

        scanf("%s", nome_csv);
        scanf("%s", nome_binario);

        FILE *csv = fopen(nome_csv, "r");
        FILE *binario = fopen(nome_binario, "wb");

        if (csv == NULL || binario == NULL)
        {
            printf(MENSAGEM_ERRO_ARQUIVO);
            return 0;
        }

        tabela_t *tabela = tabela_instanciar(binario);
        tabela_ler_csv(tabela, csv);

        tabela_liberar(&tabela);
        fclose(binario);
        fclose(csv);

        binarioNaTela(nome_binario);
    }
    else if (funcionalidade == 2)
    {
        scanf("%s", nome_binario);
        FILE *binario = fopen(nome_binario, "rb");
        if (binario == NULL)
        {
            printf(MENSAGEM_ERRO_ARQUIVO);
            return 0;
        }

        tabela_t *tabela = tabela_instanciar(binario);

        if (!tabela_valida(tabela))
        {
            printf(MENSAGEM_ERRO_ARQUIVO);
            tabela_liberar(&tabela);
            fclose(binario);
            return 0;
        }

        select_todos(tabela);

        tabela_liberar(&tabela);
        fclose(binario);
    }
    else if (funcionalidade == 3)
    {
        int n;
        scanf("%s %d", nome_binario, &n);

        FILE *binario = fopen(nome_binario, "rb");
        if (binario == NULL)
        {
            printf(MENSAGEM_ERRO_ARQUIVO);
            return 0;
        }

        tabela_t *tabela = tabela_instanciar(binario);

        if (!tabela_valida(tabela))
        {
            printf(MENSAGEM_ERRO_ARQUIVO);
            tabela_liberar(&tabela);
            fclose(binario);
            return 0;
        }

        // guarda os filtros usados por cada busca
        filtro_t **filtros = malloc(n * sizeof(filtro_t *));

        for (int i = 0; i < n; i++)
        {
            filtros[i] = ler_filtro();
        }

        for (int i = 0; i < n; i++)
        {
            printf("Busca %d\n\n", i + 1);
            select_where(tabela, filtros[i]);
            filtro_liberar(&filtros[i]);
        }

        tabela_liberar(&tabela);
        fclose(binario);
        free(filtros);
    }
    else if (funcionalidade == 4)
    {
        char nome_indice[100];

        scanf("%s %s", nome_binario, nome_indice);

        FILE *binario = fopen(nome_binario, "rb");
        FILE *aquivo_indice = fopen(nome_indice, "wb");

        if (binario == NULL)
        {
            printf(MENSAGEM_ERRO_ARQUIVO);
            fclose(aquivo_indice);
            return 0;
        }

        tabela_t *tabela = tabela_instanciar(binario);

        if (!tabela_valida(tabela))
        {
            printf(MENSAGEM_ERRO_ARQUIVO);
            tabela_liberar(&tabela);
            fclose(binario);
            fclose(aquivo_indice);
            return 0;
        }

        indice_t *indice = tabela_gerar_indice(tabela);
        indice_escrever(indice, aquivo_indice);

        indice_liberar(&indice);
        tabela_liberar(&tabela);

        fclose(binario);
        fclose(aquivo_indice);

        binarioNaTela(nome_indice);
    }
    else if (funcionalidade == 5)
    {
        int n;
        char nome_indice[100];

        scanf("%s %s %d", nome_binario, nome_indice, &n);

        FILE *binario = fopen(nome_binario, "rb+");
        FILE *arquivo_indice = fopen(nome_indice, "rb+");

        if (arquivo_indice == NULL)
        {
            arquivo_indice = fopen(nome_indice, "wb+");
        }

        if (binario == NULL)
        {
            printf(MENSAGEM_ERRO_ARQUIVO);
            fclose(arquivo_indice);
            return 0;
        }

        tabela_t *tabela = tabela_instanciar(binario);

        if (!tabela_valida(tabela))
        {
            printf(MENSAGEM_ERRO_ARQUIVO);
            tabela_liberar(&tabela);
            fclose(binario);
            fclose(arquivo_indice);
            return 0;
        }

        indice_t *indice = indice_ler(arquivo_indice);
        if (indice == NULL)
        {
            indice = tabela_gerar_indice(tabela);
        }

        // guarda os filtros usados por cada busca
        filtro_t **filtros = malloc(n * sizeof(filtro_t *));

        for (int i = 0; i < n; i++)
        {
            filtros[i] = ler_filtro();
        }

        for (int i = 0; i < n; i++)
        {
            delete_where(tabela, indice, filtros[i]);
            filtro_liberar(&filtros[i]);
        }

        indice_escrever(indice, arquivo_indice);

        indice_liberar(&indice);
        tabela_liberar(&tabela);

        free(filtros);
        fclose(binario);
        fclose(arquivo_indice);

        binarioNaTela(nome_binario);
        binarioNaTela(nome_indice);
    }
    else if (funcionalidade == 6)
    {
        int n;
        char nome_indice[100];
        scanf("%s %s %d", nome_binario, nome_indice, &n);

        FILE *binario = fopen(nome_binario, "rb+");
        FILE *arquivo_indice = fopen(nome_indice, "rb+");

        if (arquivo_indice == NULL)
        {
            arquivo_indice = fopen(nome_indice, "wb+");
        }

        if (binario == NULL)
        {
            printf(MENSAGEM_ERRO_ARQUIVO);
            fclose(arquivo_indice);
            return 0;
        }

        tabela_t *tabela = tabela_instanciar(binario);

        if (!tabela_valida(tabela))
        {
            printf(MENSAGEM_ERRO_ARQUIVO);
            tabela_liberar(&tabela);
            fclose(binario);
            fclose(arquivo_indice);
            return 0;
        }

        indice_t *indice = indice_ler(arquivo_indice);
        if (indice == NULL)
        {
            indice = tabela_gerar_indice(tabela);
        }

        registro_t **registros = malloc(n * sizeof(registro_t *));

        for (int i = 0; i < n; i++)
        {
            registros[i] = ler_registro();
        }

        for (int i = 0; i < n; i++)
        {
            registro_t *registro = registros[i];
            insert_into(tabela, indice, registro);
            registro_liberar(&registro);
        }

        indice_escrever(indice, arquivo_indice);

        indice_liberar(&indice);
        tabela_liberar(&tabela);

        free(registros);
        fclose(binario);
        fclose(arquivo_indice);

        binarioNaTela(nome_binario);
        binarioNaTela(nome_indice);
    }
}

bool tabela_valida(tabela_t *tabela)
{
    cabecalho_t *cabecalho = tabela_get_cabecalho(tabela);

    bool valido = cabecalho_get_status(cabecalho) == CABECALHO_STATUS_BOM;

    return valido;
}

// lê um filtro fornecido pelo usuário
filtro_t *ler_filtro()
{
    filtro_t *filtro = filtro_instanciar();

    // quantidade de campos a serem lidos
    int m;

    // guarda o conteúdo de campos inteiros
    int valor;
    // buffer para o nome do campo
    char campo[64];
    // buffer para o conteúdo de campos de texto
    char texto[64];

    scanf("%d", &m);
    for (int j = 0; j < m; j++)
    {
        scanf("%s", campo);
        if (strcmp(campo, "id") == 0)
        {
            scanf("%d", &valor);
            filtro_set_id(filtro, valor);
        }
        else if (strcmp(campo, "idade") == 0)
        {
            scanf("%d", &valor);
            filtro_set_idade(filtro, valor);
        }
        else if (strcmp(campo, "nomeJogador") == 0)
        {
            scan_quote_string(texto);
            char *nome = strdup(texto);
            filtro_set_nome_jogador(filtro, nome, strlen(nome));
        }
        else if (strcmp(campo, "nacionalidade") == 0)
        {
            scan_quote_string(texto);
            char *nacionalidade = strdup(texto);
            filtro_set_nacionalidade(filtro, nacionalidade, strlen(nacionalidade));
        }
        else if (strcmp(campo, "nomeClube") == 0)
        {
            scan_quote_string(texto);
            char *nome = strdup(texto);
            filtro_set_nome_clube(filtro, nome, strlen(nome));
        }
    }

    return filtro;
}

registro_t *ler_registro()
{
    char campo[100];

    int id = -1;
    int idade = -1;
    char *nome_jogador = NULL;
    char *nacionalidade = NULL;
    char *nome_clube = NULL;

    scan_quote_string(campo);
    sscanf(campo, "%d", &id);

    scan_quote_string(campo);
    sscanf(campo, "%d", &idade);

    scan_quote_string(campo);
    nome_jogador = strdup(campo);

    scan_quote_string(campo);
    nacionalidade = strdup(campo);

    scan_quote_string(campo);
    nome_clube = strdup(campo);

    return registro_instanciar(id, idade, nome_jogador, strlen(nome_jogador), nacionalidade, strlen(nacionalidade), nome_clube, strlen(nome_clube));
}