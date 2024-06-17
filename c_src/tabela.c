#include "tabela.h"

struct _tabela
{
    FILE *arquivo;
    cabecalho_t *cabecalho;
};

// retorna o ponteiro para o próximo campo do csv
// de acordo com o token offset atual
// chamar esta função modifica o token offset
// e também modifica a linha, substituindo as vírgulas por \0
char *_proximo_campo(char *linha, int *token_offset);

// imprime um campo de registro que é um long,
// exibindo "SEM DADO" se o campo for nulo
void _imprimir_campo_long(long valor);

// imprime um campo de registro que é uma string,
// exibindo "SEM DADO" se o campo for nulo
void _imprimir_campo_string(char *string, int tamanho);

// tenta ler um campo long a partir do token fornecido
// retorna -1 (valor nulo) se o token for inválido
long _ler_campo_csv_long(char *token);

// tenta ler um campo string a partir do token fornecido
// retorna -1 (valor nulo) se o token for inválido
char *_ler_campo_csv_string(char *token, int *tamanho);

// insere um registro na lista de registros logicamente removidos,
// marcando o registro como removido e atualizando o número
// de registros utilizáveis e removidos no cabeçalho
void _inserir_na_lista_de_removidos(tabela_t *tabela, registro_t *registro, long byte_offset);

// remove um registro na lista de registros logicamente removidos,
// retornando seu byte offset e seu tamanho
// e atualizando o número de registros utilizáveis e removidos no cabeçalho
long _remover_da_lista_de_removidos(tabela_t *tabela, int tamanho, int *out_tamanho);

// usada para garantir que o cursor do arquivo
// está na posição correta
// o cursor pode ficar na posição errada
// ao encontrar registros logicamente removidos
// que não consome todo o espaço que ocupam
// e também registros que usam espaço reaproveitado
// que também não leem todo o espaço que ocupam
void _garantir_offset(FILE *arquivo, long offset);

tabela_t *tabela_instanciar(FILE *arquivo)
{
    tabela_t *tabela = malloc(sizeof(tabela_t));
    tabela->arquivo = arquivo;
    tabela->cabecalho = cabecalho_ler(arquivo);
    return tabela;
}

void tabela_ler_csv(tabela_t *tabela, FILE *csv)
{

    // ignora linhas vazias no começo do arquivo
    while (getc(csv) != '\n')
        ;

    // armazena a linha sendo lida do csv
    char linha[1024];

    cabecalho_t *cabecalho = tabela_get_cabecalho(tabela);
    cabecalho_set_status(cabecalho, CABECALHO_STATUS_INCONSISTENTE);

    fseek(tabela->arquivo, 0, SEEK_SET);
    cabecalho_escrever(tabela->arquivo, cabecalho);

    int quantidade_registros = 0;

    // le as linhas do csv e coloca seus valores no arquivo de banco de dados
    long offstet_registro = ftell(tabela->arquivo);

    fseek(tabela->arquivo, TAMANHO_CABECALHO, SEEK_SET);

    // provavelmente quebra se a linha for longa demais
    // mas isso é um problema para nossas próximas iterações
    while (fgets(linha, 1024, csv) != NULL)
    {

        long id;
        int idade;
        char *nome_jogador;
        int tam_nome_jogador;
        char *nacionalidade;
        int tam_nacionalidade;
        char *nome_clube;
        int tam_nome_clube;

        // lê os campos do csv separando a linha pelas vírgulas
        int token_offset = 0;
        char *token = _proximo_campo(linha, &token_offset);
        id = _ler_campo_csv_long(token);

        if (id == -1)
            break;

        token = _proximo_campo(linha, &token_offset);
        idade = _ler_campo_csv_long(token);

        token = _proximo_campo(linha, &token_offset);
        nome_jogador = _ler_campo_csv_string(token, &tam_nome_jogador);

        token = _proximo_campo(linha, &token_offset);
        nacionalidade = _ler_campo_csv_string(token, &tam_nacionalidade);

        token = _proximo_campo(linha, &token_offset);
        nome_clube = _ler_campo_csv_string(token, &tam_nome_clube);

        registro_t *registro = registro_instanciar(id, idade, nome_jogador, tam_nome_jogador, nacionalidade, tam_nacionalidade, nome_clube, tam_nome_clube);
        registro_escrever(tabela->arquivo, registro);

        registro_liberar(&registro);

        // evita que o conteudo antigo do buffer
        // seja lido na proxima iteração caso a próxima
        // linha do arquivo esteja vazia
        linha[0] = '\0';
        offstet_registro = ftell(tabela->arquivo);
        quantidade_registros++;
    }

    cabecalho_set_topo(cabecalho, -1);
    cabecalho_set_prox_byte_offset(cabecalho, offstet_registro);
    cabecalho_set_nro_reg_rem(cabecalho, 0);
    cabecalho_set_nro_reg_arq(cabecalho, quantidade_registros);
    cabecalho_set_status(cabecalho, CABECALHO_STATUS_BOM);

    fseek(tabela->arquivo, 0, SEEK_SET);
    cabecalho_escrever(tabela->arquivo, cabecalho);
}

void tabela_liberar(tabela_t **tabela)
{
    tabela_t *t = *tabela;
    cabecalho_liberar(&t->cabecalho);
    free(*tabela);
    *tabela = NULL;
}

void select_todos(tabela_t *tabela)
{
    // uma busca que seleciona todos os registros
    // é igual a uma busca com um filtro
    // que deixa qualquer registro passar
    filtro_t *filtro = filtro_instanciar();
    select_where(tabela, filtro);
    filtro_liberar(&filtro);
}

void select_where(tabela_t *tabela, filtro_t *filtro)
{
    FILE *arquivo = tabela->arquivo;

    long offset = TAMANHO_CABECALHO;

    // o ponteiro do arquivo pode estar na
    // última posição caso tenhamos feito uma outra consulta anteriormente
    // isso previne que ela afete esta consulta
    fseek(arquivo, offset, SEEK_SET);
    bool encontrado = false;
    while (true)
    {
        registro_t *registro = registro_ler(arquivo);
        if (feof(arquivo))
        {
            registro_liberar(&registro);
            break;
        }

        // ignora registros logicamente removidos
        // ou que não satisfazem o filtro
        if (!registro_get_removido(registro) && filtro_se_aplica(filtro, registro))
        {
            encontrado = true;

            int tam_nome;
            char *nome = registro_get_nome_jogador(registro, &tam_nome);
            printf("Nome do Jogador: ");
            _imprimir_campo_string(nome, tam_nome);

            int tam_nacionalidade;
            char *nacionalidade = registro_get_nacionalidade(registro, &tam_nacionalidade);
            printf("\nNacionalidade do Jogador: ");
            _imprimir_campo_string(nacionalidade, tam_nacionalidade);

            int tam_nome_clube;
            char *nome_clube = registro_get_nome_clube(registro, &tam_nome_clube);
            printf("\nClube do Jogador: ");
            _imprimir_campo_string(nome_clube, tam_nome_clube);

            printf("\n\n");

            // só existe um registro por id
            if (filtro_tem_id(filtro))
                break;
        }

        offset += registro_get_tamanho_registro(registro);

        _garantir_offset(arquivo, offset);
        registro_liberar(&registro);
    }

    if (!encontrado)
    {
        printf("Registro inexistente.\n\n");
    }
}

void delete_where(tabela_t *tabela, indice_t *indice, filtro_t *filtro)
{
    registro_t *registro = NULL;
    long byte_offset = 0;

    if (filtro_tem_id(filtro))
    {
        // se o filtro é por id, podemos usar o índice
        registro_indice_t *ri = indice_buscar_id(indice, filtro_get_id(filtro));

        if (ri == NULL)
            return;

        // recupera o registro
        byte_offset = registro_indice_get_byte_offset(ri);
        fseek(tabela->arquivo, byte_offset, SEEK_SET);
        registro = registro_ler(tabela->arquivo);

        // o filtro pode exigir outras características
        // além do id
        if (filtro_se_aplica(filtro, registro))
        {
            indice_remover_registro(indice, registro_get_id(registro));
            _inserir_na_lista_de_removidos(tabela, registro, byte_offset);
        }

        registro_liberar(&registro);
        return;
    }

    // caso o contrário, fazemos uma busca linear
    FILE *arquivo = tabela->arquivo;

    byte_offset = TAMANHO_CABECALHO;
    fseek(arquivo, byte_offset, SEEK_SET);

    while (true)
    {
        registro = registro_ler(arquivo);
        if (feof(arquivo))
        {
            registro_liberar(&registro);
            break;
        }

        if (!registro_get_removido(registro) && filtro_se_aplica(filtro, registro))
        {
            indice_remover_registro(indice, registro_get_id(registro));
            _inserir_na_lista_de_removidos(tabela, registro, byte_offset);
        }

        byte_offset += registro_get_tamanho_registro(registro);
        _garantir_offset(arquivo, byte_offset);

        registro_liberar(&registro);
    }
}

void insert_into(tabela_t *tabela, indice_t *indice, registro_t *registro)
{
    // impede a inserção de registros com ids
    // duplicados
    if (indice_buscar_id(indice, registro_get_id(registro)) != NULL)
    {
        return;
    }

    cabecalho_t *cabecalho = tabela_get_cabecalho(tabela);
    int tamanho;

    // o byte offset do registro a ser reaproveitado
    long byte_offset = _remover_da_lista_de_removidos(tabela, registro_get_tamanho_registro(registro), &tamanho);
    // o byte offset do fim do registro
    // usado para marcar o espaço não utilizado como lixo
    long fim;

    if (byte_offset == -1)
    {
        byte_offset = cabecalho_get_prox_byte_offset(cabecalho);
        tamanho = registro_get_tamanho_registro(registro);
        cabecalho_set_prox_byte_offset(cabecalho, byte_offset + tamanho);

        // atualiza o cabecalho, incrementando o número de registros
        // disponíveis
        int nro_reg_arq = cabecalho_get_nro_reg_arq(cabecalho);
        cabecalho_set_nro_reg_arq(cabecalho, nro_reg_arq + 1);

        fseek(tabela->arquivo, 0, SEEK_SET);
        cabecalho_escrever(tabela->arquivo, cabecalho);
    }
    else
    {
        // o espaço disponível é maior ou igual
        // ao tamanho que o registro precisa
        registro_set_tamanho_registro(registro, tamanho);
    }

    fim = byte_offset + tamanho;

    fseek(tabela->arquivo, byte_offset, SEEK_SET);
    registro_escrever(tabela->arquivo, registro);

    // marca o espaço não utilizado como lixo
    while (ftell(tabela->arquivo) < fim)
    {
        fwrite("$", sizeof(char), 1, tabela->arquivo);
    }

    registro_indice_t *ri = registro_indice_instanciar(registro_get_id(registro), byte_offset);
    indice_inserir_registro(indice, ri);
}

indice_t *tabela_gerar_indice(tabela_t *tabela)
{
    cabecalho_t *cabecalho = tabela_get_cabecalho(tabela);
    indice_t *indice = indice_instanciar(cabecalho_indice_instanciar(), NULL, 0, 0);
    int capacidade = cabecalho_get_nro_reg_arq(cabecalho);
    registro_indice_t **registros = malloc(capacidade * sizeof(registro_indice_t *));

    long offset = TAMANHO_CABECALHO;

    int i = 0;

    fseek(tabela->arquivo, TAMANHO_CABECALHO, SEEK_SET);
    while (true)
    {
        registro_t *registro = registro_ler(tabela->arquivo);

        // não há mais registros para ler
        if (feof(tabela->arquivo))
        {
            registro_liberar(&registro);
            break;
        }

        if (!registro_get_removido(registro))
        {

            // garante que o vetor de registros tem espaço
            // disponível
            if (i >= capacidade)
            {
                capacidade *= 2;
                registros = realloc(registros, capacidade * sizeof(registro_indice_t *));
            }

            registros[i] = registro_indice_instanciar(registro_get_id(registro), offset);
            i++;
        }

        offset += registro_get_tamanho_registro(registro);

        _garantir_offset(tabela->arquivo, offset);

        registro_liberar(&registro);
    }

    indice_set_registros(indice, registros, i, capacidade);
    return indice;
}

cabecalho_t *tabela_get_cabecalho(tabela_t *tabela)
{
    return tabela->cabecalho;
}

void _imprimir_campo_string(char *string, int tamanho)
{
    if (string == NULL)
    {
        printf("SEM DADO");
    }
    else
    {
        printf("%.*s", tamanho, string);
    }
}

void _imprimir_campo_long(long valor)
{
    if (valor == -1)
    {
        printf("SEM DADO");
    }
    else
    {
        printf("%ld", valor);
    }
}

long _ler_campo_csv_long(char *token)
{
    // isso significa que o campo não está presente no csv
    if (token == NULL)
        return -1;

    long valor;

    // se não for possível ler um long, o campo é inválido
    if (sscanf(token, "%ld", &valor) != 1)
        return -1;

    return valor;
}

char *_ler_campo_csv_string(char *token, int *tamanho)
{
    if (token == NULL)
    {
        *tamanho = 0;
        return NULL;
    }

    int len = strlen(token);

    // tratando uma string vazia como NULL
    if (len == 0)
    {
        *tamanho = 0;
        return NULL;
    }

    char *string = malloc(sizeof(char) * len);

    memcpy(string, token, len);

    *tamanho = len;
    return string;
}

// parecido com strtok, só que esta função não ignora
// tokens vazios
char *_proximo_campo(char *linha, int *token_offset)
{
    int pos = *token_offset;
    char *inicio = linha + pos;

    // encontra a proxima vírgula ou o fim da linha
    while (linha[pos] != '\0' && linha[pos] != ',' && linha[pos] != '\n' && linha[pos] != '\r')
    {
        pos++;
    }

    // se achamos uma vírgula, há pelo menos mais um caractere na string
    // caso o contrário, não é mais possível avançar
    *token_offset = linha[pos] == ',' ? pos + 1 : pos;
    linha[pos] = '\0';

    return inicio;
}

void _inserir_na_lista_de_removidos(tabela_t *tabela, registro_t *registro, long byte_offset)
{
    cabecalho_t *cabecalho = tabela_get_cabecalho(tabela);

    long prox_offset = cabecalho_get_topo(cabecalho);

    registro_t *atual = NULL;
    registro_t *anterior = NULL;
    long offset_anterior;

    if (prox_offset != -1)
    {
        fseek(tabela->arquivo, prox_offset, SEEK_SET);
        atual = registro_ler(tabela->arquivo);
    }

    while (atual != NULL && registro_get_tamanho_registro(atual) <= registro_get_tamanho_registro(registro))
    {
        registro_liberar(&anterior);
        anterior = atual;
        offset_anterior = prox_offset;

        prox_offset = registro_get_prox(anterior);
        atual = NULL;
        if (prox_offset != -1)
        {
            fseek(tabela->arquivo, prox_offset, SEEK_SET);
            atual = registro_ler(tabela->arquivo);
        }
    }

    if (anterior == NULL)
    {
        cabecalho_set_topo(cabecalho, byte_offset);
    }
    else
    {
        registro_set_prox(anterior, byte_offset);
        fseek(tabela->arquivo, offset_anterior, SEEK_SET);
        registro_escrever(tabela->arquivo, anterior);
    }

    registro_set_removido(registro, true);
    registro_set_prox(registro, prox_offset);
    fseek(tabela->arquivo, byte_offset, SEEK_SET);
    registro_escrever(tabela->arquivo, registro);

    int nro_reg_rem = cabecalho_get_nro_reg_rem(cabecalho);
    cabecalho_set_nro_reg_rem(cabecalho, nro_reg_rem + 1);

    int nro_reg_arq = cabecalho_get_nro_reg_arq(cabecalho);
    cabecalho_set_nro_reg_arq(cabecalho, nro_reg_arq - 1);

    fseek(tabela->arquivo, 0, SEEK_SET);
    cabecalho_escrever(tabela->arquivo, cabecalho);

    registro_liberar(&anterior);
    registro_liberar(&atual);
}

long _remover_da_lista_de_removidos(tabela_t *tabela, int tamanho, int *out_tamanho)
{
    cabecalho_t *cabecalho = tabela_get_cabecalho(tabela);

    long offset_atual = cabecalho_get_topo(cabecalho);
    long offset_anterior;

    registro_t *atual = NULL;
    registro_t *anterior = NULL;

    if (offset_atual != -1)
    {
        fseek(tabela->arquivo, offset_atual, SEEK_SET);
        atual = registro_ler(tabela->arquivo);
    }

    while (atual != NULL && registro_get_tamanho_registro(atual) < tamanho)
    {
        registro_liberar(&anterior);
        anterior = atual;
        offset_anterior = offset_atual;

        offset_atual = registro_get_prox(anterior);
        atual = NULL;
        if (offset_atual != -1)
        {
            fseek(tabela->arquivo, offset_atual, SEEK_SET);
            atual = registro_ler(tabela->arquivo);
        }
    }

    if (atual == NULL)
    {
        registro_liberar(&anterior);
        *out_tamanho = -1;
        return -1;
    }

    long prox_offset = registro_get_prox(atual);
    *out_tamanho = registro_get_tamanho_registro(atual);

    if (anterior == NULL)
    {
        cabecalho_set_topo(cabecalho, prox_offset);
    }
    else
    {
        registro_set_prox(anterior, prox_offset);
        fseek(tabela->arquivo, offset_anterior, SEEK_SET);
        registro_escrever(tabela->arquivo, anterior);
    }

    int nro_reg_rem = cabecalho_get_nro_reg_rem(cabecalho);
    cabecalho_set_nro_reg_rem(cabecalho, nro_reg_rem - 1);

    int nro_reg_arq = cabecalho_get_nro_reg_arq(cabecalho);
    cabecalho_set_nro_reg_arq(cabecalho, nro_reg_arq + 1);

    fseek(tabela->arquivo, 0, SEEK_SET);
    cabecalho_escrever(tabela->arquivo, cabecalho);

    registro_liberar(&anterior);
    registro_liberar(&atual);

    return offset_atual;
}

void _garantir_offset(FILE *arquivo, long offset)
{
    // só executa o fseek se necessário
    if (ftell(arquivo) < offset)
    {
        fseek(arquivo, offset, SEEK_SET);
    }
}
