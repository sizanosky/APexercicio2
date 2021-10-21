/*
* Estrutura de Dados
* Atividade Prática - Exercício 2
* Aluno: Marcos Fabricio Sizanósky RU-3395570
* Professor: Vinicius Pozzobon Borin
* Data criação: 13/10/2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale>

// Constants.
#define TAMANHO_HASH 10
#define NOME_ARQUIVO "cadastro.csv"

FILE* arq;

// Estrutura heterogênea de dados que armazena os dados do aluno.
struct HashAlunos {
	int ru;
	char nome_aluno[51];
	char email[51];

	// O ponteiro vai conter o endereço do próximo elemento.
	HashAlunos* prox;
};

struct Cadastro {
	int Tamanho;  // Dimensão do Cadastro.
	struct HashAlunos** ListaAlunos;  // Lista encadeada com registros de alunos.
};

// Funções e procedures.
void LimpaBuffer();
int menu();
void CarregaDados(struct Cadastro* CadastroHash);
int FuncaoHashing(int num);  // Calcula em que posição do vetor(ListaAlunos) o registro será armazenado.
void InserirHash(struct Cadastro* CadastroHash);  // Insere um novo registro.
void RemoverHash(struct Cadastro* CadastroHash);  // Remove um registro.
void BuscarHash(struct Cadastro* CadastroHash);  // Busca um aluno pelo numero do RU.
void MostrarHash(struct Cadastro* CadastroHash);  // Mostra todos os registros.

int main() {
	setlocale(LC_ALL, "Portuguese");
	printf("Hello, World! \n\n");
	printf("======== Cadastro Alunos ========\n");

	int opcao;

	/* Define uma variavel do tipo "Cadastro" que contem as listas encadeadas
	e aloca espaço na memória.*/
	Cadastro* CadastroHash = (struct Cadastro*)
		malloc(sizeof(struct Cadastro));
	// Define o tamanho da Hash que recebera as listas.
	CadastroHash->Tamanho = TAMANHO_HASH;
	// Aloca espaço na memória para as listas encadeadas.
	CadastroHash->ListaAlunos = (struct HashAlunos**)
		malloc(TAMANHO_HASH * sizeof(struct HashAlunos*));

	for (int i = 0; i < TAMANHO_HASH; i++) {
		// Inicia todas as posições da Hash.
		CadastroHash->ListaAlunos[i] = NULL; 
	}
	// Preenche a estrutura com os dados de caastro.csv
	CarregaDados(CadastroHash);

	while (true) {

		opcao = menu();

		switch (opcao) {
		case 1: // Inserir.
			printf("\n======== Inserir registro ========\n");
			InserirHash(CadastroHash);  // Insere os dados em uma posicao calculada pela função "FuncaoHashing".
			break;

		case 2: // Remover
			printf("\n======== Remover registro ========\n");
			RemoverHash(CadastroHash);  // Remove os dados em uma posicao.
			break;

		case 3: // Buscar aluno pelo RU.
			printf("\n======== Buscar aluno ========\n");
			BuscarHash(CadastroHash);
			break;

		case 4: // Mostra todos os alunos do cadastro.
			MostrarHash(CadastroHash);
			break;

		case 0: // Encerra a aplicação.
			exit(0);

		default:
			printf("Opção invalida! digite novamente.\n");
			break;
		}
	}
	return 0;
}

// Funções e procedures.
void LimpaBuffer() {
	/* Função usada para limpar o buffer do teclado.*/
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}
}

int menu() {
	/* Função usada para o menu da aplicação.*/
	int opcao;
	printf("______________________________________\n");
	printf("\nMENU:\n");
	printf("\n1 - Inserir novo registro.");
	printf("\n2 - Remover registro.");
	printf("\n3 - Buscar aluno pelo número do RU.");
	printf("\n4 - Mostrar todos os registros.");
	printf("\n\n0 - SAIR.\n");

	printf("\nDigite uma opção: ");
	scanf_s("%d", &opcao);
	LimpaBuffer();
	printf("______________________________________\n");

	return opcao;
}

void CarregaDados(struct Cadastro* CadastroHash) {
	/* Função que inicializa a estrutura de dados preenchendo os campos
	com dados contidos no arquivo "cadastro.csv" */

	int pos;
	errno_t err;

	HashAlunos* NovoElemento;

	// Abre o arquivo "cadastro.csv".
	err = fopen_s(&arq, NOME_ARQUIVO, "r");

	if (err == 0) {

		for (int i = 0; i < TAMANHO_HASH; i++) {
			NovoElemento = (struct HashAlunos*)malloc(sizeof(struct HashAlunos));

			fscanf_s(arq, "%d;", &NovoElemento->ru);
			fscanf_s(arq, "%50[a-z A-Z];",
				NovoElemento->nome_aluno, _countof(NovoElemento->nome_aluno));
			fscanf_s(arq, "%50s",
				NovoElemento->email, _countof(NovoElemento->email));
			pos = FuncaoHashing(NovoElemento->ru);

			NovoElemento->prox = NULL;
			NovoElemento->prox = CadastroHash->ListaAlunos[pos];
			CadastroHash->ListaAlunos[pos] = NovoElemento;
		}
		fclose(arq);
	}
	else {
		printf("Erro ao abrir o arquivo!\n");
	}
}

int FuncaoHashing(int num) {
	/* Função que calcula em que posição do vetor o registro será armazenado */

	return (num % TAMANHO_HASH); // % metodo que retorna o resto da divisão.
}

void InserirHash(struct Cadastro* CadastroHash) {
	/* Função que faz a inserção dos dados na estrutura e em uma
	posição da Hash que é calculada pela "FuncaoHashing". */

	int pos;

	// Cria um ponteiro para struct e aloca espaço na memória para o elemento.
	HashAlunos* NovoElemento;
	NovoElemento = (struct HashAlunos*)malloc(sizeof(struct HashAlunos));

	printf("Digite o número do RU para inserção: ");
	scanf_s("%10d", &NovoElemento->ru);
	LimpaBuffer();

	printf("Digite o nome do aluno: ");
	scanf_s("%50[a-z A-Z]", NovoElemento->nome_aluno, 51);
	LimpaBuffer();

	printf("Digite o e-mail do aluno: ");
	scanf_s("%50s", NovoElemento->email, 51);
	LimpaBuffer();

	pos = FuncaoHashing(NovoElemento->ru); // Recebe a posição calculada pela função "FuncaoHashing".
	NovoElemento->prox = NULL; // Ponteiro para prox recebe NULL.
	NovoElemento->prox = CadastroHash->ListaAlunos[pos]; // Aponta para o Head da lista.
	CadastroHash->ListaAlunos[pos] = NovoElemento; // Transforma o Head em um novo elemento.
}

void RemoverHash(struct Cadastro* CadastroHash) {
	/* Função que faz a remoção do registro na estrutura 
	usando como cordenada a posição da Hash que é calculada 
	pela "FuncaoHashing". */

	int num, pos;

	printf("Digite o numero para ser removido: ");
	scanf_s("%10d", &num);
	LimpaBuffer();
	pos = FuncaoHashing(num);
	printf("\nRemovendo RU %d...\n", num);

	// O bloco abaixo aloca um espaço na memória.
	HashAlunos* ElementoVarredura; 
	ElementoVarredura = (struct HashAlunos*)malloc(sizeof(struct HashAlunos));
	ElementoVarredura = CadastroHash->ListaAlunos[pos];

	HashAlunos* Auxiliar;
	Auxiliar = (struct HashAlunos*)malloc(sizeof(struct HashAlunos));

	if (CadastroHash->ListaAlunos[pos]->ru == num)
	{
		CadastroHash->ListaAlunos[pos] = CadastroHash->ListaAlunos[pos]->prox;
		free(ElementoVarredura);
	}
	else
	{
		while (ElementoVarredura->ru != num)
		{
			Auxiliar = ElementoVarredura; //Mantem o anterior
			ElementoVarredura = ElementoVarredura->prox;
		}
		Auxiliar->prox = ElementoVarredura->prox;
		free(ElementoVarredura);
	}
}

void BuscarHash(struct Cadastro* CadastroHash) {
	/* Função que efetua a busca de um aluno pelo numero do seu RU usando
	o calculo da "FuncaoHashing" para determinar a sua posição no registro */

	int num, pos;
	printf("Digite o RU a ser buscado:  ");
	scanf_s("%d", &num);
	LimpaBuffer();

	pos = FuncaoHashing(num);

	HashAlunos* ElementoVarredura;
	ElementoVarredura = (struct HashAlunos*)malloc(sizeof(struct HashAlunos));
	ElementoVarredura = CadastroHash->ListaAlunos[pos];

	if (ElementoVarredura == NULL) {
		printf("Registro não encontrado! \n");
	}

	while (ElementoVarredura != NULL) {

		//if (ElementoVarredura == NULL) { printf("Registro não encontrado! \n"); ElementoVarredura = NULL; };
		if (ElementoVarredura->ru == num) {
			printf("\nResultado da busca: \n\n");
			printf("RU:  %d \n", ElementoVarredura->ru);
			printf("NOME: %s \n", ElementoVarredura->nome_aluno);
			printf("EMAIL: %s \n", ElementoVarredura->email);

			ElementoVarredura = ElementoVarredura->prox;
		}
		else if (ElementoVarredura->prox == NULL) {
			printf("Registro não encontrado! \n");
			break;
		}
		else
			ElementoVarredura = ElementoVarredura->prox;

	}
}

void MostrarHash(struct Cadastro* CadastroHash) {
	/* Função que faz a impressão de todos os registros 
	da lista encadeada. */
	int v;

	for (v = 0; v < CadastroHash->Tamanho; v++) {
		struct HashAlunos* ElementoVarredura = CadastroHash->ListaAlunos[v];
		printf("\nFolha de Registros nº %d: \n", v + 1);

		while (ElementoVarredura != NULL) {
			printf("| RU: %d ", ElementoVarredura->ru);
			printf("| NOME: %s ", ElementoVarredura->nome_aluno);
			printf("| EMAIL: %s |\n", ElementoVarredura->email);
			ElementoVarredura = ElementoVarredura->prox; 
		}
		printf("\n");
	}
}