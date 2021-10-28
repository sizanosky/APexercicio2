/*
* Estrutura de Dados
* Atividade Pr�tica - Exerc�cio 2
* Aluno: Marcos Fabricio Sizan�sky RU-3395570
* Professor: Vinicius Pozzobon Borin
* Data cria��o: 13/10/2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <locale>

// Constants.
#define TAMANHO_HASH 10
#define NOME_ARQUIVO "cadastro.csv"

FILE* arq;

// Estrutura heterog�nea de dados que armazena os dados do aluno.
struct HashAlunos {
	int ru;
	char nome[51];
	char email[51];

	// O ponteiro vai conter o endere�o do pr�ximo elemento.
	HashAlunos* prox;
};

struct Cadastro {
	int Tamanho;  // Dimens�o do Cadastro.
	struct HashAlunos** ListaAlunos;  // Lista encadeada com registros de alunos.
};

// Fun��es e procedures.
void LimpaBuffer();
int menu();
void CarregaDados(struct Cadastro* CadastroHash);
int FuncaoHashing(int num);  // Calcula em que posi��o do vetor(ListaAlunos) o registro ser� armazenado.
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
	e aloca espa�o na mem�ria.*/
	Cadastro* CadastroHash = (struct Cadastro*)
		malloc(sizeof(struct Cadastro));
	// Define o tamanho da Hash que recebera as listas.
	CadastroHash->Tamanho = TAMANHO_HASH;
	// Aloca espa�o na mem�ria para as listas encadeadas.
	CadastroHash->ListaAlunos = (struct HashAlunos**)
		malloc(TAMANHO_HASH * sizeof(struct HashAlunos*));

	for (int i = 0; i < TAMANHO_HASH; i++) {
		// Inicia todas as posi��es da Hash.
		CadastroHash->ListaAlunos[i] = NULL; 
	}
	// Preenche a estrutura com os dados de caastro.csv
	CarregaDados(CadastroHash);

	while (true) {

		opcao = menu();

		switch (opcao) {
		case 1: // Inserir.
			printf("\n======== Inserir registro ========\n");
			InserirHash(CadastroHash); // Insere os dados em HashAlunos.
			break;

		case 2: // Remover
			printf("\n======== Remover registro ========\n");
			RemoverHash(CadastroHash); // Remove o registro em HashAlunos.
			break;

		case 3: // Buscar aluno pelo RU.
			printf("\n======== Buscar aluno ========\n");
			BuscarHash(CadastroHash); // Busca o registro em HashAlunos.
			break;

		case 4: // Mostra todos os alunos do cadastro.
			MostrarHash(CadastroHash);
			break;

		case 0: // Encerra a aplica��o.
			exit(0);

		default:
			printf("Op��o invalida! digite novamente.\n");
			break;
		}
	}
	return 0;
}

// Fun��es e procedures.
void LimpaBuffer() {
	/* Fun��o usada para limpar o buffer do teclado.*/
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}
}

int menu() {
	/* Fun��o usada para o menu da aplica��o.*/
	int opcao;
	printf("______________________________________\n");
	printf("\n  MENU:\n");
	printf("\n  1 - Inserir novo registro.");
	printf("\n  2 - Remover registro.");
	printf("\n  3 - Buscar aluno pelo n�mero do RU.");
	printf("\n  4 - Mostrar todos os registros.");
	printf("\n\n  0 - SAIR.\n");

	printf("\n  Digite uma op��o: ");
	scanf_s("%d", &opcao);
	LimpaBuffer();
	printf("______________________________________\n");

	return opcao;
}

void CarregaDados(struct Cadastro* CadastroHash) {
	/* Fun��o que inicializa a estrutura de dados preenchendo os campos
	com dados contidos no arquivo "cadastro.csv" */

	int pos;
	errno_t err;

	HashAlunos* NovoElemento;

	// Abre o arquivo "cadastro.csv".
	err = fopen_s(&arq, NOME_ARQUIVO, "r");

	if (err == 0) {

		for (int i = 0; i < TAMANHO_HASH; i++) {
			NovoElemento = (struct HashAlunos*)
				malloc(sizeof(struct HashAlunos));

			// L� os dados do arquivo e armazena na struct HashAlunos.
			fscanf_s(arq, "%d;%50[a-z A-Z];%s;", 
				&NovoElemento->ru,
				NovoElemento->nome, _countof(NovoElemento->nome),
				NovoElemento->email, _countof(NovoElemento->email));

			// Recebe a posi��o calculada pela fun��o "FuncaoHashing".
			pos = FuncaoHashing(NovoElemento->ru);

			NovoElemento->prox = NULL;
			NovoElemento->prox = CadastroHash->ListaAlunos[pos];
			// Recebe o elemento na posi��o calculada pela "FuncaoHashing".
			CadastroHash->ListaAlunos[pos] = NovoElemento;
		}
		fclose(arq);
	}
	else {
		printf("Erro ao abrir o arquivo!\n");
	}
}

int FuncaoHashing(int num) {
	/* Fun��o que calcula em que posi��o do vetor o registro ser� armazenado */

	return (num % TAMANHO_HASH); // % metodo que retorna o resto da divis�o.
}

void InserirHash(struct Cadastro* CadastroHash) {
	/* Fun��o que faz a inser��o dos dados na estrutura e em uma
	posi��o da Hash que � calculada pela "FuncaoHashing". */

	int pos;

	// Cria um ponteiro para struct e aloca espa�o na mem�ria para o elemento.
	HashAlunos* NovoElemento;
	NovoElemento = (struct HashAlunos*)malloc(sizeof(struct HashAlunos));

	printf("Digite o n�mero do RU para inser��o: ");
	scanf_s("%10d", &NovoElemento->ru);
	LimpaBuffer();

	printf("Digite o nome do aluno: ");
	scanf_s("%50[a-z A-Z]", NovoElemento->nome, 51);
	LimpaBuffer();

	printf("Digite o e-mail do aluno: ");
	scanf_s("%50s", NovoElemento->email, 51);
	LimpaBuffer();

	// Recebe a posi��o calculada pela fun��o "FuncaoHashing".
	pos = FuncaoHashing(NovoElemento->ru); 
	// Ponteiro para prox recebe NULL.
	NovoElemento->prox = NULL; 
	// Aponta para o Head da lista.
	NovoElemento->prox = CadastroHash->ListaAlunos[pos]; 
	// Transforma o Head em um novo elemento.
	CadastroHash->ListaAlunos[pos] = NovoElemento; 
}

void RemoverHash(struct Cadastro* CadastroHash) {
	/* Fun��o que faz a remo��o do registro na estrutura 
	usando como cordenada a posi��o da Hash que � calculada 
	pela "FuncaoHashing". */

	int num, pos;

	printf("Digite o numero para ser removido: ");
	scanf_s("%10d", &num);
	LimpaBuffer();

	// Aplica a fun��o para determinar em qual lista o registro foi alocado.
	pos = FuncaoHashing(num);

	// O bloco abaixo define um ponteiro e aloca espa�o na mem�ria.
	HashAlunos* ElementoVarredura; 
	ElementoVarredura = (struct HashAlunos*)malloc(sizeof(struct HashAlunos));
	ElementoVarredura = CadastroHash->ListaAlunos[pos];

	HashAlunos* Auxiliar; // 
	Auxiliar = (struct HashAlunos*)malloc(sizeof(struct HashAlunos));

	// Se o elemento da lista � igual ao digitado exclui os dados referentes. 
	if (CadastroHash->ListaAlunos[pos]->ru == num)
	{
		CadastroHash->ListaAlunos[pos] = CadastroHash->ListaAlunos[pos]->prox;
		printf("\nRemovendo RU %d...\n", num);
		free(ElementoVarredura);
	}
	// Sen�o passa para o proximo elemento e mantem o anterior. 
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
	/* Fun��o que efetua a busca de um aluno pelo numero do seu RU usando
	o calculo da "FuncaoHashing" para determinar a sua posi��o no registro */

	int num, pos;
	printf("Digite o RU a ser buscado:  ");
	scanf_s("%d", &num);
	LimpaBuffer();

	// Aplica a fun��o para determinar em qual lista o registro foi alocado.
	pos = FuncaoHashing(num); 

	HashAlunos* ElementoVarredura; // Cria um ponteiro do tipo HashAlunos.
	ElementoVarredura = (struct HashAlunos*)malloc(sizeof(struct HashAlunos));
	
	// Recebe a lista na posi��o calculada pela FuncaoHashing.
	ElementoVarredura = CadastroHash->ListaAlunos[pos];

	if (ElementoVarredura == NULL) { // Verifica se a lista esta vazia.
		printf("Registro n�o encontrado! \n");
	}

	while (ElementoVarredura != NULL) {

		// Se o elemento da lista � igual ao digitado imprime as informa��es.
		if (ElementoVarredura->ru == num) {
			printf("\nResultado da busca: \n\n");
			printf("RU:  %d \n", ElementoVarredura->ru);
			printf("NOME: %s \n", ElementoVarredura->nome);
			printf("EMAIL: %s \n", ElementoVarredura->email);

			// Aponta para o proximo elemento.
			ElementoVarredura = ElementoVarredura->prox; 
		}
		// Se o elemento da lista for NULO imprime mensagem de erro.
		else if (ElementoVarredura->prox == NULL) {
			printf("Registro n�o encontrado! \n");
			break;
		}
		// Sen�o aponta para o proximo elemento.
		else
			ElementoVarredura = ElementoVarredura->prox;
	}
}

void MostrarHash(struct Cadastro* CadastroHash) {
	/* Fun��o que faz a impress�o de todos os registros 
	da lista encadeada. */
	int v;

	for (v = 0; v < CadastroHash->Tamanho; v++) {
		struct HashAlunos* ElementoVarredura = CadastroHash->ListaAlunos[v];
		printf("\nFolha de Registros n� %d: \n", v + 1);

		while (ElementoVarredura != NULL) {
			printf("| RU: %d ", ElementoVarredura->ru);
			printf("| NOME: %s ", ElementoVarredura->nome);
			printf("| EMAIL: %s |\n", ElementoVarredura->email);
			ElementoVarredura = ElementoVarredura->prox; 
		}
	}
}