#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#define MASK '*'

typedef struct{             /*ESTRUTURA DE DATA*/
    int dia;
    int mes;
    int ano;
    }Data;

typedef struct{             /*ESTRUTURA DE HORA*/
    int hora;
    int min;
    }Hora;

typedef struct{             /*ESTRUTURA DE CLIENTE*/
    char nome[100];
    char sobreNome[100];
    Data dataNasc;
    char cnh[20];
    char cpf[12];
    char cep[10];
    char numRes[10];
    char telefone[15];
    }Cliente;

typedef struct{             /*ESTRUTURA DE LOCA��O*/
    char cpf[12];
    char placa[10];
    Data dataLoc;
    Data dataDev;
    Hora horaLoc;
    Hora horaDev;
    int devolvido;          /*0 - sim | 1 - n�o*/
    float preco;
    int ongParceira;        /*1 - sim | 0 - n�o*/
    }Locacao;
char* devolvido[] = {"Finalizada",   /*0*/        /*CONTROLE DE LOCA��O*/
                   "Em aberto"};     /*1*/

typedef struct{             /*ESTRUTURA DE VE�CULO*/
    char fabricante[50];
    char modelo[50];
    int categoria;
    int cambio;
    int anoFabricacao;
    char placa[10];
    int reserva;
    }Veiculo;

char* reserva[] = {"Dispon�vel",    /*0*/        /*RESERVA DE VE�CULO*/
                   "Alugado"};   /*1*/

char* categoria[] = {"Econ�mico����",    /*0*/       /*CATEGORIAS DE VE�CULO*/
                     "Compacto�����",     /*1*/
                     "Intermedi�rio",    /*2*/
                     "Standard�����",     /*3*/
                     "SUV����������",          /*4*/
                     "Utilit�rio���",   /*5*/
                     "Premium������",      /*6*/
                     "Minivan������",      /*7*/
                     "Especial�����",     /*8*/
                     "Luxo���������",         /*9*/
                     "Blindado�����"};    /*10*/
int tamanhoCategoria = sizeof(categoria)/sizeof(categoria[0]);

char* cambio[] = {"Manual",     /*0*/            /*CATEGORIAS DE CAMBIO*/
                  "Autom�tico"};/*1*/
int tamanhoCambio = sizeof(cambio)/sizeof(cambio[0]);

float preco[] = {60,  /*0*/                       /*PRE�OS DE CATEGORIA DE VE�CULO*/
               70,  /*1*/
               90,  /*2*/
               120, /*3*/
               140, /*4*/
               170, /*5*/
               180, /*6*/
               190, /*7*/
               200, /*8*/
               380, /*9*/
               580};/*10*/

void ParseData(char str[9], Data *Alvo) /*converte retorno char em Data*/
{
	// 0, 1 = mes
	// 2    = /
	// 3, 4 = dia
	// 5    = /
	// 6, 7 = ano
	// 8    = NULO
	char _dia[3] = {
		str[3],
		str[4],
		0
	};
	char _mes[3] = {
		str[0],
		str[1],
		0
	};
	char _ano[3] = {
		str[6],
		str[7],
		0
	};
	Alvo->dia = atoi(_dia);
	Alvo->mes = atoi(_mes);
	Alvo->ano = atoi(_ano);
	return;
}

 /* guarda o numero de dias em cada mes para anos normais e bissextos */
int dias_mes[2][13] = {{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
                       {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};

int bissexto (int ano) { /*processo para calcular ano bissexto*/
	return (ano % 4 == 0) && ((ano % 100 != 0) || (ano % 400 == 0));
}

long dist_dias (Data inicio, Data fim) { /*calculo de dias entre duas datas*/
	long idias, fdias;	/* guarda quantos dias tem da data ate o comeco do ano */
	long def_anos = 0;	/* guarda diferenca entre anos das datas inicio e fim medida em dias */
	register int i;
	int dbissexto;

	idias = inicio.dia;
	dbissexto = bissexto (inicio.ano);
	for (i = inicio.mes - 1; i > 0; --i)
		idias += dias_mes[dbissexto][i];

	fdias = fim.dia;
	dbissexto = bissexto (fim.ano);
	for (i = fim.mes - 1; i > 0; --i)
		fdias += dias_mes[dbissexto][i];

	while (inicio.ano < fim.ano)
		def_anos += 365 + bissexto(inicio.ano++);

	return def_anos - idias + fdias;
}

int calcIdade(int nasDia, int nasMes, int nasAno){ /*CALCULO DA IDADE PELA DATA DE NASCIMENTO*/
    char dateStr[9];
    _strdate( dateStr);
    Data atual;
	ParseData(dateStr, &atual);

    int i, h, a, m, d, ah, mh, dh,ida;

    dh = atual.dia;
    mh = atual.mes;
    ah = atual.ano + 2000; //FUN��O V�LIDA POR 1000 ANOS
    d = nasDia;
    m = nasMes;
    a = nasAno;

    ida = ah-a;

        if(m>mh){
          ida = ida - 1;
        }else if(m==mh){
            if(d>dh){
                ida = ida;
                }else{
                    ida = ida - 1;
                }
        }

        return ida;
}

    /*Ao inicializar o programa essas vari�veis dever�o ser populadas e controlar�o os cadastros*/
    int controle_cadastros = 0;     /*varievel que controla quantos cadastros dever� ter o vetor dinamico 'clientes', deve ser populada antes da cria��o do vetor dinamico*/
    Cliente *clientes;              /*Base de dados dos clientes*/
    int clientes_cadastrados = 0;   /*Contador para controle dos clientes cadastrados*/
    int posicao_cliente = 0;        /*Contador para controle da posi��o do vetor*/
    int controle_linhas = 0;        /*Contador para controle das linhas do arquivo de armazenamento*/

    int controle_cadastros_veic = 0;    /*varievel que controla quantos cadastros dever� ter o vetor dinamico 'veiculos', deve ser populada antes da cria��o do vetor dinamico*/
    Veiculo *veiculos;                  /*Base de dados dos veicuos*/
    int veiculos_cadastrados = 0;       /*Contador para controle dos veiculos cadastrados*/
    int posicao_veiculo = 0;            /*Contador para controle da posi��o do vetor*/
    int controle_linhas_veic = 0;        /*Contador para controle das linhas do arquivo de armazenamento*/

    int controle_cadastros_loc = 0;    /*varievel que controla quantos cadastros dever� ter o vetor dinamico 'locacoes', deve ser populada antes da cria��o do vetor dinamico*/
    Locacao *locacoes;                  /*Base de dados de locacoes*/
    int locacoes_cadastrados = 0;       /*Contador para controle das locacoes*/
    int posicao_locacao = 0;            /*Contador para controle da posi��o do vetor*/
    int controle_linhas_loc = 0;        /*Contador para controle das linhas do arquivo de armazenamento*/

    void lerArquivo(){ /*FUN��O RESPONS�VEL POR LER OS ARQUIVOS DE ARMAZENAMENTO E POPULAR OS VETORES DO PROGRAMA*/

    /*PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 1*/
    char url[]="controleCadastrosClientes.mrb";     //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE CLIENTES CADASTRADOS
	FILE *arq;                                      //VARIAVEL DO TIPO "FILE"
	arq = fopen(url, "r+");                         //Modo leitura
	if(arq == NULL)                                 //VERIFICA SE A VARIAVEL arq RECEBEU CONTE�DO
	    {
	        arq = fopen(url, "w");
	        fprintf(arq, "%d\n", 0);
            fclose(arq);
            //inicializa as vari�veis caso n�o encontre o arquivo de controle
            controle_cadastros = 0;
            clientes = (Cliente *) malloc(controle_cadastros * sizeof(Cliente));
            clientes_cadastrados = controle_cadastros;
            posicao_cliente = controle_cadastros - 1;
        }
	else
	    {
	        while( (fscanf(arq,"%d", &controle_cadastros))!= EOF ) //"EOF" END OF FILE - INDICA QUE N�O EXISTE MAIS NAD A SER LIDO
            {
                //sem c�digo pois esse controle tem apenas uma linha
            }

            //informa a quantidade de cadastros j� existentes e ajusta todos os contadores
            clientes = (Cliente *) malloc(controle_cadastros * sizeof(Cliente));
            clientes_cadastrados = controle_cadastros;
            posicao_cliente = controle_cadastros - 1;
	    }

	fclose(arq);//fecha o arquivo
	/*FIM DO PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 1*/

    /*PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 2*/
	char url2[]="bdClientes.mrb";   //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE CLIENTES CADASTRADOS
	FILE *arq2;                     //VARIAVEL DO TIPO "FILE"
	arq2 = fopen(url2, "r+");       //Modo leitura
	if(arq2 == NULL)                //VERIFICA SE A VARIAVEL arq RECEBEU CONTE�DO
	    {
	        fclose(arq2);
	        arq2 = fopen(url2, "w");
            fclose(arq2);
        }
	else
	    {
	         while((fscanf(arq,"%s %s %d %d %d %s %s %s %s %s", /*CAPTURA A STRING DO ARQUIVO E POPULA O VETOR DE CLIENTES*/
                              clientes[controle_linhas].nome,
                              clientes[controle_linhas].sobreNome,
                              &clientes[controle_linhas].dataNasc.dia,
                              &clientes[controle_linhas].dataNasc.mes,
                              &clientes[controle_linhas].dataNasc.ano,
                              clientes[controle_linhas].cnh,
                              clientes[controle_linhas].cpf,
                              clientes[controle_linhas].cep,
                              clientes[controle_linhas].numRes,
                              clientes[controle_linhas].telefone
                              ))!=EOF) //"EOF" - INDICA QUE N�O EXISTE MAIS NAD A SER LIDO
                    controle_linhas++;
        }

	fclose(arq);//fecha o arquivo
	/*FIM DO PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 2*/
}

    void lerArquivoVeic(){ /*FUN��O RESPONS�VEL POR LER OS ARQUIVOS DE ARMAZENAMENTO.PT2 E POPULAR OS VETORES DO PROGRAMA*/

    /*PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 3*/
    char urlV[]="controleCadastrosVeiculos.mrb";     //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE VEICULOS CADASTRADOS
	FILE *arqV;                                      //VARIAVEL DO TIPO "FILE"
	arqV = fopen(urlV, "r+");                        //Modo leitura
	if(arqV == NULL)                                 //VERIFICA SE A VARIAVEL arqV RECEBEU CONTE�DO
	    {
	        arqV = fopen(urlV, "w");
	        fprintf(arqV, "%d\n", 0);
            fclose(arqV);
            //inicializa as vari�veis caso n�o encontre o arquivo de controle
            controle_cadastros_veic = 0;
            veiculos = (Veiculo *) malloc(controle_cadastros_veic * sizeof(Veiculo));
            veiculos_cadastrados = controle_cadastros_veic;
            posicao_veiculo = controle_cadastros_veic - 1;
        }
	else
	    {
	        while( (fscanf(arqV,"%d", &controle_cadastros_veic))!= EOF ) //"EOF" END OF FILE - INDICA QUE N�O EXISTE MAIS NAD A SER LIDO
            {
                //sem c�digo pois esse controle tem apenas uma linha
            }

            //informa a quantidade de cadastros j� existentes e ajusta todos os contadores
            veiculos = (Veiculo *) malloc(controle_cadastros_veic * sizeof(Veiculo));
            veiculos_cadastrados = controle_cadastros_veic;
            posicao_veiculo = controle_cadastros_veic - 1;
	    }

	fclose(arqV);//fecha o arquivo
	/*FIM DO PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 3*/

    /*PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 4*/
	char urlV2[]="bdVeiculos.mrb";   //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE VEICULOS CADASTRADOS
	FILE *arqV2;                     //VARIAVEL DO TIPO "FILE"
	arqV2 = fopen(urlV2, "r+");      //Modo leitura
	if(arqV2 == NULL)                //VERIFICA SE A VARIAVEL arqV2 RECEBEU CONTE�DO
	    {
	        fclose(arqV2);
	        arqV2 = fopen(urlV2, "w");
            fclose(arqV2);
        }
	else
	    {
	         while((fscanf(arqV2,"%s %s %d %d %d %s %d", /*CAPTURA A STRING DO ARQUIVO E POPULA O VETOR DE VEICULOS*/
                              veiculos[controle_linhas_veic].fabricante,
                              veiculos[controle_linhas_veic].modelo,
                              &veiculos[controle_linhas_veic].categoria,
                              &veiculos[controle_linhas_veic].cambio,
                              &veiculos[controle_linhas_veic].anoFabricacao,
                              veiculos[controle_linhas_veic].placa,
                              &veiculos[controle_linhas_veic].reserva
                              ))!=EOF) //"EOF" - INDICA QUE N�O EXISTE MAIS NAD A SER LIDO
                    controle_linhas_veic++;
        }

	fclose(arqV2);//fecha o arquivo
	/*FIM DO PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 4*/
}

    void lerArquivoLoc(){ /*FUN��O RESPONS�VEL POR LER OS ARQUIVOS DE ARMAZENAMENTO E POPULAR OS VETORES DO PROGRAMA*/

    /*PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 5*/
    char url[]="controleCadastrosLocacao.mrb";     //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE LOCA��O
	FILE *arq;                                      //VARIAVEL DO TIPO "FILE"
	arq = fopen(url, "r+");                         //Modo leitura
	if(arq == NULL)                                 //VERIFICA SE A VARIAVEL arq RECEBEU CONTE�DO
	    {
	        arq = fopen(url, "w");
	        fprintf(arq, "%d\n", 0);
            fclose(arq);
            //inicializa as vari�veis caso n�o encontre o arquivo de controle
            controle_cadastros_loc = 0;
            locacoes = (Locacao *) malloc(controle_cadastros_loc * sizeof(Locacao));
            locacoes_cadastrados = controle_cadastros_loc;
            posicao_locacao = controle_cadastros_loc - 1;
        }
	else
	    {
	        while( (fscanf(arq,"%d", &controle_cadastros_loc))!= EOF ) //"EOF" END OF FILE - INDICA QUE N�O EXISTE MAIS NAD A SER LIDO
            {
                //sem c�digo pois esse controle tem apenas uma linha
            }

            //informa a quantidade de cadastros j� existentes e ajusta todos os contadores
            locacoes = (Locacao *) malloc(controle_cadastros_loc * sizeof(Locacao));
            locacoes_cadastrados = controle_cadastros_loc;
            posicao_locacao = controle_cadastros_loc - 1;
	    }

	fclose(arq);//fecha o arquivo
	/*FIM DO PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 5*/

    /*PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 6*/
	char url2[]="bdLocacoes.mrb";   //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE CLIENTES CADASTRADOS
	FILE *arq2;                     //VARIAVEL DO TIPO "FILE"
	arq2 = fopen(url2, "r+");       //Modo leitura
	if(arq2 == NULL)                //VERIFICA SE A VARIAVEL arq RECEBEU CONTE�DO
	    {
	        fclose(arq2);
	        arq2 = fopen(url2, "w");
            fclose(arq2);
        }
	else
	    {
	         while((fscanf(arq,"%s %s %d %d %d %d %d %d %d %d %d %d %d %f %d", /*CAPTURA A STRING DO ARQUIVO E POPULA O VETOR DE LOCACOES*/
                              locacoes[controle_linhas_loc].cpf,
                              locacoes[controle_linhas_loc].placa,
                              &locacoes[controle_linhas_loc].dataLoc.dia,
                              &locacoes[controle_linhas_loc].dataLoc.mes,
                              &locacoes[controle_linhas_loc].dataLoc.ano,
                              &locacoes[controle_linhas_loc].dataDev.dia,
                              &locacoes[controle_linhas_loc].dataDev.mes,
                              &locacoes[controle_linhas_loc].dataDev.ano,
                              &locacoes[controle_linhas_loc].horaLoc.hora,
                              &locacoes[controle_linhas_loc].horaLoc.min,
                              &locacoes[controle_linhas_loc].horaDev.hora,
                              &locacoes[controle_linhas_loc].horaDev.min,
                              &locacoes[controle_linhas_loc].devolvido,
                              &locacoes[controle_linhas_loc].preco,
                              &locacoes[controle_linhas_loc].ongParceira
                              ))!=EOF) //"EOF" - INDICA QUE N�O EXISTE MAIS NAD A SER LIDO
                    controle_linhas_loc++;
        }

	fclose(arq);//fecha o arquivo
	/*FIM DO PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 6*/
}

    void atualizaCtrl6(){//PROCESSO PARA AUXILIAR NA ATUALIZA��O DOS ARQUIVOS DE CONTROLE

    /*PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 6*/
                            char url2[]="bdLocacoes.mrb"; //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE CLIENTES CADASTRADOS
                            FILE *arq2; //VARIAVEL DO TIPO "FILE"
                            arq2 = fopen(url2, "w");//ABRE PARA ESCRITA SEM APAGAR DADOS ANTERIORES
                            if(arq2 == NULL){
                                    printf("Erro ao abrir o arquivo \"bdLocacoes.mrb\"!\n");
                            }
                            else{
                                    for(int i = 0; i < locacoes_cadastrados; i++){
                          /*CPF   */fprintf(arq2, "%s ", locacoes[i].cpf);
                          /*PLACA */fprintf(arq2, "%s ", locacoes[i].placa);
                          /*DT LOC*/fprintf(arq2, "%d %d %d ", locacoes[i].dataLoc.dia,locacoes[i].dataLoc.mes,locacoes[i].dataLoc.ano);
                          /*DT DEV*/fprintf(arq2, "%d %d %d ", locacoes[i].dataDev.dia,locacoes[i].dataDev.mes,locacoes[i].dataDev.ano);
                          /*HH LOC*/fprintf(arq2, "%d %d ", locacoes[i].horaLoc.hora,locacoes[i].horaLoc.min);
                          /*HH DEV*/fprintf(arq2, "%d %d ", locacoes[i].horaDev.hora,locacoes[i].horaDev.min);
                          /*STATUS*/fprintf(arq2, "%d ", locacoes[i].devolvido);
                          /*PRE�O */fprintf(arq2, "%.2f ", locacoes[i].preco);
                          /*ONG P */fprintf(arq2, "%d\n", locacoes[i].ongParceira);
                                    }
                           }
                            fclose(arq2); //fecha o arquivo
                            /*FIM DO PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 6*/

    }
    void atualizaCtrl4(){ //PROCESSO PARA AUXILIAR NA ATUALIZA��O DOS ARQUIVOS DE CONTROLE

                            /*PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 4*/
                            char url4[]="bdVeiculos.mrb"; //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE VEICULOS CADASTRADOS
                            FILE *arq4; //VARIAVEL DO TIPO "FILE"
                            arq4 = fopen(url4, "w");//ABRE PARA ESCRITA
                            if(arq4 == NULL){
                                    printf("Erro ao abrir o arquivo \"bdVeiculos.mrb\"!\n");
                            }
                            else{
                                    for(int i = 0; i < veiculos_cadastrados; i++){
                                  /*FABRICANTE*/fprintf(arq4, "%s ", veiculos[i].fabricante);
                                  /*MODELO    */fprintf(arq4, "%s ", veiculos[i].modelo);
                                  /*CATEGORIA */fprintf(arq4, "%d ", veiculos[i].categoria);
                                  /*CAMBIO    */fprintf(arq4, "%d ", veiculos[i].cambio);
                                  /*ANO       */fprintf(arq4, "%d ", veiculos[i].anoFabricacao);
                                  /*PLACA     */fprintf(arq4, "%s ", veiculos[i].placa);
                                  /*DISPONIVEL*/fprintf(arq4, "%d\n", veiculos[i].reserva);
                                    }
                           }
                            fclose(arq4); //fecha o arquivo
                            /*FIM DO PROCESSO DE ATUALIZA��O DO ARQUIVO DE CONTROLE 4*/

    }

    void listarPrecos(){ /*LISTA PRE�OS POR CATEGORIA*/
                    printf("\n\t  >    VALOR DA DI�RIA POR CATEGORIA DE VE�CULO:\n");
                    for(int i = 0; i < tamanhoCategoria;i++){
                        printf("\n\t  >    %s - R$ %.2f",categoria[i],preco[i]);
                    }
                    printf("\n");
                    system("PAUSE");

}

int ckCliente(char* cpf)
{
    for(int i = 0; i < clientes_cadastrados; i++){ //percorre todo o vetor de clientes at;e o ultimo cadastro
            if(strcasecmp(cpf,clientes[i].cpf) == 0){//verifica se o CPF � o mesmo. Retorna 0 caso seja
                printf("\n\t              >    Nome: %s", clientes[i].nome);
                printf("\n\t              >    Sobrenome: %s", clientes[i].sobreNome);
                printf("\n\t              >    Data de nascimento: %d/%d/%d", clientes[i].dataNasc.dia,clientes[i].dataNasc.mes,clientes[i].dataNasc.ano);
                printf("\n\t              >    CNH: %s", clientes[i].cnh);
                printf("\n\t              >    CPF: %s", clientes[i].cpf);
                printf("\n\t              >    CEP: %s", clientes[i].cep);
                printf("\n\t              >    N�mero: %s", clientes[i].numRes);
                printf("\n\t              >    Telefone: %s", clientes[i].telefone);
                int copyCli = i;
                return copyCli;
                break; //encerra a busca depois de encontrar
            }
        }
}

float calcMulta(int diasHelper, Hora horaDev, Hora horaPrevista){

    float retorno;
    if(diasHelper>=0){
        diasHelper = diasHelper * 24;
        int horaResult = horaDev.hora - horaPrevista.hora;
        int minResult = horaDev.min - horaPrevista.min;
        retorno = ((horaResult*60) + minResult);
        retorno = retorno/60;
        retorno = retorno + diasHelper;
        if(retorno>0){
            return retorno;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

    void devolverVeiculo(){

            char* posicaoHelper;
            char* cpfHelper;
            printf("\n\n\t       DEVOLVER VE�CULO ALUGADO\n");
            printf("\n\t  >    PLACA DO VE�CULO (XXX9999): ");
            gets(posicaoHelper);
            int copyVeic;
            int copyCli;
            int ckVeiculoCad = 1;
            int ckVeiculoCad2 = 1;

            /*inicio verifica��o de cadastro existente*/
            for(int i = 0; i < veiculos_cadastrados; i++){ //percorre todo o vetor de veiculos ate o ultimo cadastro
                if(strcasecmp(posicaoHelper,veiculos[i].placa) == 0){
                        copyVeic = i;
                        for(int j = 0; j < locacoes_cadastrados; j++){ //percorre todo o vetor de locacoes ate o ultimo cadastro
                            if((strcasecmp(posicaoHelper,locacoes[j].placa) == 0) && (1 == locacoes[j].devolvido)){//verifica se o veiculo est� dispon�vel
                            printf("\n\n\t                   LOCA��O ENCONTRADA:   \n");
                            printf("\n\t              >    CPF do Cliente: %s", locacoes[j].cpf);
                            copyCli = ckCliente(locacoes[j].cpf);
                            printf("\n\t              >    Placa: %s", locacoes[j].placa);
                            printf("\n\t              >    Fabricante: %s", veiculos[copyVeic].fabricante);
                            printf("\n\t              >    Modelo: %s", veiculos[copyVeic].modelo);
                            printf("\n\t              >    Categoria: %s", categoria[veiculos[copyVeic].categoria]);
                            printf("\n\t              >    Data de loca��o: %d/%d/%d", locacoes[j].dataLoc.dia,locacoes[j].dataLoc.mes,locacoes[j].dataLoc.ano);
                            printf("\n\t              >    Devolu��o prevista para: %d/%d/%d", locacoes[j].dataDev.dia,locacoes[j].dataDev.mes,locacoes[j].dataDev.ano);
                            if(locacoes[j].horaDev.min<10){
                            printf("\n\t              >    Hora limite da devolu��o: %d:%d0", locacoes[j].horaDev.hora,locacoes[j].horaDev.min);}else{
                            printf("\n\t              >    Hora limite da devolu��o: %d:%d", locacoes[j].horaDev.hora,locacoes[j].horaDev.min);
                            }
                            printf("\n\t              >    Valor Total Pago: R$ %.2f", locacoes[j].preco);
                            do{
                                printf("\n\t              >    DIGITE 0 PARA DEVOLVER ESTE VE�CULO: ");
                                printf("\n\t              >    DIGITE 1 PARA SAIR: ");
                                scanf("%d", &ckVeiculoCad);
                                fflush(stdin);
                            }while(ckVeiculoCad!=1 && ckVeiculoCad!=0);
                            /*faz devolu��o do ve�culo*/
                            if(ckVeiculoCad == 0){

                                Data devD; //DEVOLU��O REAL
                                Hora devH; //DEVOLU��O REAL
                                int diaHelper; //AUXILIADOR PARA CALCULO DE MULTA
                                float varMulta; //VARIAVEL PARA CALCULO DE MULTA
                                printf("\n\t  >    Insira data da devolu��o (dd mm aaaa): ");
                                scanf("%d %d %d", &devD.dia, &devD.mes, &devD.ano);
                                fflush(stdin);
                                printf("\n\t  >    Insira a hora da devolu��o (hh mm): ");
                                scanf("%d %d", &devH.hora, &devH.min);
                                fflush(stdin);
                                diaHelper = dist_dias(locacoes[j].dataDev, devD);

                                 /*calculo de multa*/
                                    varMulta = (calcMulta(diaHelper, devH, locacoes[j].horaDev))/24;
                                    varMulta = varMulta * preco[veiculos[copyVeic].categoria];
                                /*FIM - calculo de multa*/

                                system("cls");
                                printf("\n\n\t                   DEVOLU��O DE VE�CULO:   \n");
                                printf("\n\t              >    CPF do Cliente: %s", locacoes[j].cpf);
                                copyCli = ckCliente(locacoes[j].cpf);
                                printf("\n\t              >    Placa: %s", locacoes[j].placa);
                                printf("\n\t              >    Fabricante: %s", veiculos[copyVeic].fabricante);
                                printf("\n\t              >    Modelo: %s", veiculos[copyVeic].modelo);
                                printf("\n\t              >    Categoria: %s", categoria[veiculos[copyVeic].categoria]);
                                printf("\n\t              >    Data de loca��o: %d/%d/%d", locacoes[j].dataLoc.dia,locacoes[j].dataLoc.mes,locacoes[j].dataLoc.ano);
                                printf("\n\t              >    Devolu��o prevista para: %d/%d/%d", locacoes[j].dataDev.dia,locacoes[j].dataDev.mes,locacoes[j].dataDev.ano);
                                if(locacoes[j].horaDev.min<10){
                                printf("\n\t              >    Hora limite da devolu��o: %d:%d0", locacoes[j].horaDev.hora,locacoes[j].horaDev.min);}else{
                                printf("\n\t              >    Hora limite da devolu��o: %d:%d", locacoes[j].horaDev.hora,locacoes[j].horaDev.min);
                                }
                                printf("\n\t              >    Valor Total Pago: R$ %.2f", locacoes[j].preco);
                                printf("\n");
                                printf("\n\t              >    Data da Devolu��o: %d/%d/%d", devD.dia,devD.mes,devD.ano);
                                if(devH.min<10){
                                printf("\n\t              >    Hora da devolu��o: %d:%d0", devH.hora,devH.min);}else{
                                printf("\n\t              >    Hora da devolu��o: %d:%d", devH.hora,devH.min);
                                }
                                printf("\n");
                                printf("\n\t              >    Valor A PAGAR de multa por atraso: R$ %.2f\n", varMulta);

                                do{
                                    printf("\n\t              >    DIGITE 0 PARA CONTINUAR: ");
                                    printf("\n\t              >    DIGITE 1 PARA SAIR: ");
                                    scanf("%d", &ckVeiculoCad2);
                                    fflush(stdin);
                                }while(ckVeiculoCad2!=1 && ckVeiculoCad2!=0);

                                if(ckVeiculoCad2==0){

                                    veiculos[copyVeic].reserva = 0;
                                    locacoes[j].devolvido = 0;
                                    atualizaCtrl4(); /*ATUALIZA��O DOS ARQUIVOS DE CONTROLE*/
                                    atualizaCtrl6(); /*ATUALIZA��O DOS ARQUIVOS DE CONTROLE*/

                                    system("cls");
                                    printf("\n\n\t|||     ||||||  /||||\\  //||\\\\  |||   ");     /*NOME DA EMPRESA*/
                                    printf("\n\t|||     |||     ||  |/  ||  ||  |||   ");
                                    printf("\n\t|||     |||||   || __   ||||||  |||   ");
                                    printf("\n\t|||     |||     || \\\\\\  ||  ||  |||   ");
                                    printf("\n\t||||||  ||||||  \\||||/  ||  ||  ||||||");
                                    printf("\n\t _   __       _____        __  __   _ ");
                                    printf("\n\t| \\ |   |\\  |   |     .   /   /  \\ | \\");
                                    printf("\n\t|-/ |-  | \\ |   |    /_\\  |   |--| |-/");
                                    printf("\n\t| \\ |__ |  \\|   |    | |  \\__ |  | | \\\n");
                                    printf("\n\t      DEVOLU��O DE VE�CULO REALIZADA COM SUCESSO!\n");
                                    printf("\n\t      Valor recebido de multa por atraso: R$ %.2f\n\n", varMulta);
                                    system("PAUSE");
                            }

                            /*fim devolu��o do ve�culo*/
                           }//FIM - IF 3
                            break; //encerra a busca depois de encontrar
                        }//FIM -IF 2
                    }//FIM - FOR 2
                }//FIM  IF 1
            }//FIM - FOR 1
    }


    void novaLocacao(){ /*FUN��O RESPONS�VEL POR REALIZAR O CADASTRO DE UMA NOVA LOCA��O*/

    /*atualiza o espa�o de mem�ria para incluir um novo cadastro*/
    locacoes = (Locacao *) realloc(locacoes, (controle_cadastros_loc + 1) * sizeof(Locacao));

    int posicaoHelper = posicao_locacao + 1; /*VARI�VEL AUXILIADORA PARA NOVO CADASTRO*/
    int varVolta = 22117435;
    int ckCliente = 2;
    int ckVeiculoCad = 2;
    int copyVeic = 0;
    int copyCli = 0;

    printf("\n\t       REALIZAR NOVA LOCA��O\n");
    printf("\n\t       INFORMA��ES DO CLIENTE\n");
        printf("\n\t  >    CPF (apenas n�meros): ");
        gets(locacoes[posicaoHelper].cpf);
        /*inicio verifica��o de cadastro existente*/
        for(int i = 0; i < clientes_cadastrados; i++){ //percorre todo o vetor de clientes at;e o ultimo cadastro
            if(strcmp(locacoes[posicaoHelper].cpf,clientes[i].cpf) == 0){//verifica se o CPF � o mesmo. Retorna 0 caso seja
                printf("\n\n\t                   FAVOR CONFIRMAR COM O CLIENTE SE OS DADOS EST�O CORRETOS:   \n");
                printf("\n\t              >    Nome: %s", clientes[i].nome);
                printf("\n\t              >    Sobrenome: %s", clientes[i].sobreNome);
                printf("\n\t              >    Data de nascimento: %d/%d/%d", clientes[i].dataNasc.dia,clientes[i].dataNasc.mes,clientes[i].dataNasc.ano);
                printf("\n\t              >    CNH: %s", clientes[i].cnh);
                printf("\n\t              >    CPF: %s", clientes[i].cpf);
                printf("\n\t              >    CEP: %s", clientes[i].cep);
                printf("\n\t              >    N�mero: %s", clientes[i].numRes);
                printf("\n\t              >    Telefone: %s", clientes[i].telefone);
                copyCli = i;
                do{
                    printf("\n\n\t              >    DIGITE 0 PARA CONTINUAR A LOCA��O: ");
                    printf("\n\t              >    DIGITE 1 PARA SAIR: ");
                    printf("\n\t              >    ");
                    scanf("%d", &ckCliente);
                    fflush(stdin);
                    }while((ckCliente!=0) && (ckCliente!=1));
                break; //encerra a busca depois de encontrar
            }
        }

        if(ckCliente == 0){ //ck cliente cadastrado
            printf("\n\n\t       INFORMA��ES DA LOCA��O\n");
            printf("\n\t  >    PLACA DO VE�CULO (XXX9999): ");
            gets(locacoes[posicaoHelper].placa);
            /*inicio verifica��o de cadastro existente*/
            for(int i = 0; i < veiculos_cadastrados; i++){ //percorre todo o vetor de veiculos ate o ultimo cadastro
                if(strcasecmp(locacoes[posicaoHelper].placa,veiculos[i].placa) == 0){
                        ckVeiculoCad = 0;
                        copyVeic = i;
                        for(int i = 0; i < locacoes_cadastrados; i++){ //percorre todo o vetor de locacoes ate o ultimo cadastro
                            if((strcasecmp(locacoes[posicaoHelper].placa,locacoes[i].placa) == 0) && (1 == locacoes[i].devolvido)){//verifica se o veiculo est� dispon�vel
                            system("cls"); // limpa conteudo anterior
                            printf("\n\n\t                   ESTE VE�CULO N�O EST� DISPON�VEL PARA LOCA��O:   \n");
                            printf("\n\t              >    CPF do Cliente: %s", locacoes[i].cpf);
                            printf("\n\t              >    Placa: %s", locacoes[i].placa);
                            printf("\n\t              >    Data de loca��o: %d/%d/%d", locacoes[i].dataLoc.dia,locacoes[i].dataLoc.mes,locacoes[i].dataLoc.ano);
                            printf("\n\t              >    Devolu��o prevista para: %d/%d/%d", locacoes[i].dataDev.dia,locacoes[i].dataDev.mes,locacoes[i].dataDev.ano);
                            printf("\n\t              >    Valor Total: R$ %.2f", locacoes[i].preco);
                            printf("\n\t              >    DIGITE 0 (ZERO) PARA VOLTAR AO MENU ANTERIOR: ");
                            scanf("%d", &varVolta);
                            fflush(stdin);
                            break; //encerra a busca depois de encontrar
                        }
                    }
                }
            }

            if(ckVeiculoCad == 0){ //ck veiculo cadastrado
                switch(varVolta)
                {
                    case 22117435:{
                        printf("\n\t  >    Fabricante: %s", veiculos[copyVeic].fabricante);
                        printf("\n\t  >    Modelo: %s", veiculos[copyVeic].modelo);
                        printf("\n\t  >    C�mbio: %s", cambio[veiculos[copyVeic].cambio]);
                        printf("\n\t  >    Categoria: %s", categoria[veiculos[copyVeic].categoria]);
                        printf("\n\t  >    Valor da Di�ria: R$ %.2f\n", preco[veiculos[copyVeic].categoria]);
                        /*fim verifica��o de cadastro existente e continua��o da loca��o*/
                        printf("\n\t  >    Data da loca��o (dd mm aaaa): ");
                        scanf("%d %d %d", &locacoes[posicaoHelper].dataLoc.dia, &locacoes[posicaoHelper].dataLoc.mes, &locacoes[posicaoHelper].dataLoc.ano);
                        fflush(stdin);
                        printf("\n\t  >    Hora da loca��o (hh mm): ");
                        scanf("%d %d", &locacoes[posicaoHelper].horaLoc.hora, &locacoes[posicaoHelper].horaLoc.min);
                        fflush(stdin);
                        printf("\n\t  >    Data da devolu��o (dd mm aaaa): ");
                        scanf("%d %d %d", &locacoes[posicaoHelper].dataDev.dia, &locacoes[posicaoHelper].dataDev.mes, &locacoes[posicaoHelper].dataDev.ano);
                        fflush(stdin);
                        locacoes[posicaoHelper].horaDev.hora = locacoes[posicaoHelper].horaLoc.hora;
                        locacoes[posicaoHelper].horaDev.min = locacoes[posicaoHelper].horaLoc.min;
                        printf("\n\t  >    Pertence a Ongs Parceiras? (1 - SIM | 0 - N�O): ");
                        scanf("%d", &locacoes[posicaoHelper].ongParceira);
                        fflush(stdin);

                        /*calculos de pre�o*/
                        int totalDias = dist_dias(locacoes[posicaoHelper].dataLoc, locacoes[posicaoHelper].dataDev);
                        locacoes[posicaoHelper].preco = totalDias * preco[veiculos[copyVeic].categoria];
                        printf("\n\t  >    Pre�o inicial: %.2f", locacoes[posicaoHelper].preco);
                        printf("\n");
                        float descTotal = 0;
                        int idade;
                        /*calculo de idade*/
                        idade = calcIdade(clientes[copyCli].dataNasc.dia, clientes[copyCli].dataNasc.mes,clientes[copyCli].dataNasc.ano);
                        /*FIM - calculo de idade*/
                        if(idade>59){
                            descTotal = descTotal + 0.1;
                        printf("\n\t  >    Desconto aplicado: IDOSO - 10 por cento ");
                        }
                        if(locacoes[posicaoHelper].ongParceira == 1){
                            descTotal = descTotal + 0.05;
                            printf("\n\t  >    Desconto aplicado: ONG PARCEIRA - 5 por cento ");
                        }
                        printf("\n\n\t  >    Desconto total aplicado: %.0f por cento",descTotal*100);
                        float valorDesc = locacoes[posicaoHelper].preco*descTotal;
                        locacoes[posicaoHelper].preco = locacoes[posicaoHelper].preco - valorDesc;
                        printf("\n\n\t  >    Pre�o final: %.2f", locacoes[posicaoHelper].preco);
                        /*FIM - calculos de pre�o*/
                         system("cls");
                        /*RESUMO LOCA��O*/
                        printf("\n\n\t       INFORMA��ES DA LOCA��O\n");
                        printf("\n\t       CPF: %s",locacoes[posicaoHelper].cpf);
                        printf("\n\t       Nome: %s %s", clientes[copyCli].nome, clientes[copyCli].sobreNome);
                        printf("\n\n\t       Placa: %s",locacoes[posicaoHelper].placa);
                        printf("\n\t       Fabricante: %s", veiculos[copyVeic].fabricante);
                        printf("\n\t       Modelo: %s", veiculos[copyVeic].modelo);
                        printf("\n\n\t       Data de loca��o: %d/%d/%d",locacoes[posicaoHelper].dataLoc.dia,locacoes[posicaoHelper].dataLoc.mes,locacoes[posicaoHelper].dataLoc.ano);
                        if(locacoes[posicaoHelper].horaLoc.min<10){
                        printf("\n\t       Hora da loca��o: %d:%d0", locacoes[posicaoHelper].horaLoc.hora,locacoes[posicaoHelper].horaLoc.min);}else{
                        printf("\n\t       Hora da loca��o: %d:%d", locacoes[posicaoHelper].horaLoc.hora,locacoes[posicaoHelper].horaLoc.min);
                        }
                        printf("\n\t       Data de devolu��o: %d/%d/%d",locacoes[posicaoHelper].dataDev.dia,locacoes[posicaoHelper].dataDev.mes,locacoes[posicaoHelper].dataDev.ano);
                        if(locacoes[posicaoHelper].horaDev.min<10){
                        printf("\n\t       Hora limite da devolu��o: %d:%d0", locacoes[posicaoHelper].horaDev.hora,locacoes[posicaoHelper].horaDev.min);}else{
                        printf("\n\t       Hora limite da devolu��o: %d:%d", locacoes[posicaoHelper].horaDev.hora,locacoes[posicaoHelper].horaDev.min);
                        }
                        printf("\n\n\t       Valor bruto: R$ %.2f", locacoes[posicaoHelper].preco + valorDesc);
                        if(idade>59){
                        printf("\n\t       >Desconto aplicado: IDOSO - 10 por cento");
                        }
                        if(locacoes[posicaoHelper].ongParceira == 1){
                        printf("\n\t       >Desconto aplicado: ONG PARCEIRA - 5 por cento");
                        }
                        printf("\n\t       Valor total de descontos aplicados: R$ %.2f", valorDesc);
                        printf("\n\n\t       Valor final: R$ %.2f", locacoes[posicaoHelper].preco);

                        printf("\n\n\t       Caso seja ultrapassada a data ou hora limites para devolu��o do ve�culo, ser� cobrada uma multa proporcional as horas de atraso, de acordo com o valor da di�ria SEM DESCONTOS.\n O valor da multa ser� calculado na data de entrega do ve�culo.");
                        printf("\n\n\t       Ao confirmar a loca��o, o cliente concorda com os termos e dados acima apresentados.");
                        /*FIM - RESUMO LOCA��O*/

                        int confirma = 0;
                        do{
                                printf("\n\n\t  ||     CONFIRMA LOCA��O?");
                                printf("\n\t  ||   > 1 SIM");
                                printf("\n\t  ||   > 0 N�O");
                                printf("\n\t  ||   > ");
                                scanf("%d", &confirma);
                                fflush(stdin);
                        }while(confirma!=0 && confirma!=1);

                        if(confirma==1){/*efetua o registro do cadastro no arquivo e atualiza as variaveis de controle*/

                            locacoes[posicaoHelper].devolvido = 1; //adiciona a informa��o de veiculo retirado
                            veiculos[copyVeic].reserva = 1; //atualiza statuos do veiculo na tabela de cadastros
                            controle_cadastros_loc++; //ganha +1 para inicializar o vetor com tamanho correto
                            locacoes_cadastrados++; //a cada novo cadastro esse contador ganha +1
                            posicao_locacao++; //ganha +1 para n�o sobrescrever nenhum cadastro anterior

                            /*PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 4*/
                            char url4[]="bdVeiculos.mrb"; //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE VEICULOS CADASTRADOS
                            FILE *arq4; //VARIAVEL DO TIPO "FILE"
                            arq4 = fopen(url4, "w");//ABRE PARA ESCRITA
                            if(arq4 == NULL){
                                    printf("Erro ao abrir o arquivo \"bdVeiculos.mrb\"!\n");
                            }
                            else{
                                    for(int i = 0; i < veiculos_cadastrados; i++){

                                  /*FABRICANTE*/fprintf(arq4, "%s ", veiculos[i].fabricante);
                                  /*MODELO    */fprintf(arq4, "%s ", veiculos[i].modelo);
                                  /*CATEGORIA */fprintf(arq4, "%d ", veiculos[i].categoria);
                                  /*CAMBIO    */fprintf(arq4, "%d ", veiculos[i].cambio);
                                  /*ANO       */fprintf(arq4, "%d ", veiculos[i].anoFabricacao);
                                  /*PLACA     */fprintf(arq4, "%s ", veiculos[i].placa);
                                  /*DISPONIVEL*/fprintf(arq4, "%d\n", veiculos[i].reserva);
                                    }
                           }
                            fclose(arq4); //fecha o arquivo
                            /*FIM DO PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 4*/

                            /*PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 5*/
                            char url[]="controleCadastrosLocacao.mrb"; //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE CLIENTES CADASTRADOS
                            FILE *arq; //VARIAVEL DO TIPO "FILE"
                            arq = fopen(url, "w");//ABRE PARA ESCRITA
                            if(arq == NULL){
                                    printf("Erro ao abrir o arquivo \"controleCadastrosLocacao.mrb\"!\n");
                            }
                            else{
                                    fprintf(arq, "%d\n", controle_cadastros_loc);//atualiza variavel de controle
                            }
                            fclose(arq); //fecha o arquivo
                            /*FIM DO PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 5*/

                            /*PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 6*/
                            char url2[]="bdLocacoes.mrb"; //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE CLIENTES CADASTRADOS
                            FILE *arq2; //VARIAVEL DO TIPO "FILE"
                            arq2 = fopen(url2, "a");//ABRE PARA ESCRITA SEM APAGAR DADOS ANTERIORES
                            if(arq2 == NULL){
                                    printf("Erro ao abrir o arquivo \"bdLocacoes.mrb\"!\n");
                            }
                            else{
                          /*CPF   */fprintf(arq2, "%s ", locacoes[posicaoHelper].cpf);
                          /*PLACA */fprintf(arq2, "%s ", locacoes[posicaoHelper].placa);
                          /*DT LOC*/fprintf(arq2, "%d %d %d ", locacoes[posicaoHelper].dataLoc.dia,locacoes[posicaoHelper].dataLoc.mes,locacoes[posicaoHelper].dataLoc.ano);
                          /*DT DEV*/fprintf(arq2, "%d %d %d ", locacoes[posicaoHelper].dataDev.dia,locacoes[posicaoHelper].dataDev.mes,locacoes[posicaoHelper].dataDev.ano);
                          /*HH LOC*/fprintf(arq2, "%d %d ", locacoes[posicaoHelper].horaLoc.hora,locacoes[posicaoHelper].horaLoc.min);
                          /*HH DEV*/fprintf(arq2, "%d %d ", locacoes[posicaoHelper].horaDev.hora,locacoes[posicaoHelper].horaDev.min);
                          /*STATUS*/fprintf(arq2, "%d ", locacoes[posicaoHelper].devolvido);
                          /*PRE�O */fprintf(arq2, "%.2f ", locacoes[posicaoHelper].preco);
                          /*ONG P */fprintf(arq2, "%d\n", locacoes[posicaoHelper].ongParceira);
                           }
                            fclose(arq2); //fecha o arquivo
                            /*FIM DO PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 6*/
                            system("cls");
                            printf("\n\t|||     ||||||  /||||\\  //||\\\\  |||   ");     /*LOGO DA EMPRESA*/
                            printf("\n\t|||     |||     ||  |/  ||  ||  |||   ");
                            printf("\n\t|||     |||||   || __   ||||||  |||   ");
                            printf("\n\t|||     |||     || \\\\\\  ||  ||  |||   ");
                            printf("\n\t||||||  ||||||  \\||||/  ||  ||  ||||||");
                            printf("\n\t _   __       _____        __  __   _ ");
                            printf("\n\t| \\ |   |\\  |   |     .   /   /  \\ | \\");
                            printf("\n\t|-/ |-  | \\ |   |    /_\\  |   |--| |-/");
                            printf("\n\t| \\ |__ |  \\|   |    | |  \\__ |  | | \\\n");

                            printf("\n");
                            printf("\n\t     LOCA��O REALIZADA COM SUCESSO!\n");

                            /*RESUMO LOCA��O*/
                            printf("\n\n\t       INFORMA��ES DA LOCA��O\n");
                            printf("\n\t       CPF: %s",locacoes[posicaoHelper].cpf);
                            printf("\n\t       Nome: %s %s", clientes[copyCli].nome, clientes[copyCli].sobreNome);
                            printf("\n\n\t       Placa: %s",locacoes[posicaoHelper].placa);
                            printf("\n\t       Fabricante: %s", veiculos[copyVeic].fabricante);
                            printf("\n\t       Modelo: %s", veiculos[copyVeic].modelo);
                            printf("\n\n\t       Data de loca��o: %d/%d/%d",locacoes[posicaoHelper].dataLoc.dia,locacoes[posicaoHelper].dataLoc.mes,locacoes[posicaoHelper].dataLoc.ano);
                            if(locacoes[posicaoHelper].horaLoc.min<10){
                            printf("\n\t       Hora da loca��o: %d:%d0", locacoes[posicaoHelper].horaLoc.hora,locacoes[posicaoHelper].horaLoc.min);}else{
                            printf("\n\t       Hora da loca��o: %d:%d", locacoes[posicaoHelper].horaLoc.hora,locacoes[posicaoHelper].horaLoc.min);
                            }
                            printf("\n\t       Data de devolu��o: %d/%d/%d",locacoes[posicaoHelper].dataDev.dia,locacoes[posicaoHelper].dataDev.mes,locacoes[posicaoHelper].dataDev.ano);
                            if(locacoes[posicaoHelper].horaDev.min<10){
                            printf("\n\t       Hora limite da devolu��o: %d:%d0", locacoes[posicaoHelper].horaDev.hora,locacoes[posicaoHelper].horaDev.min);}else{
                            printf("\n\t       Hora limite da devolu��o: %d:%d", locacoes[posicaoHelper].horaDev.hora,locacoes[posicaoHelper].horaDev.min);
                            }
                            printf("\n\n\t       Valor bruto: R$ %.2f", locacoes[posicaoHelper].preco + valorDesc);
                            if(idade>59){
                            printf("\n\t       >Desconto aplicado: IDOSO - 10 por cento");
                            }
                            if(locacoes[posicaoHelper].ongParceira == 1){
                            printf("\n\t       >Desconto aplicado: ONG PARCEIRA - 5 por cento");
                            }
                            printf("\n\t       Valor total de descontos aplicados: R$ %.2f", valorDesc);
                            printf("\n\n\t       Valor final: R$ %.2f\n\t       ", locacoes[posicaoHelper].preco);
                            /*FIM - RESUMO LOCA��O*/

                            system("PAUSE");
                        }
                    }
                }//FIM SWITCH
            }else{//FIM IF VEICULO
                printf("\n\t       Este ve�culo n�o est� cadastrado. Favor realizar o cadastro.\n");
                system("PAUSE");
            }
        }else if(ckCliente == 2)//FIM IF CLIENTE
        {
            printf("\n\t       Este cliente n�o est� cadastrado. Favor realizar o cadastro.\n");
            system("PAUSE");
        }
}

    void cadastrarCliente(){ /*FUN��O RESPONS�VEL POR REALIZAR O CADASTRO DE UM NOVO CLIENTE*/

    /*atualiza o espa�o de mem�ria para incluir um novo cadastro*/
    clientes = (Cliente *) realloc(clientes, (controle_cadastros + 1) * sizeof(Cliente));

    int posicaoHelper = posicao_cliente + 1; /*VARI�VEL AUXILIADORA PARA NOVO CADASTRO*/
    int varVolta = 22117435;
        printf("\n\t       CADASTRO DE CLIENTE\n");
        printf("\n\t  >    CPF (apenas n�meros): ");
        gets(clientes[posicaoHelper].cpf);
        /*inicio verifica��o de cadastro existente*/
        for(int i = 0; i < clientes_cadastrados; i++){ //percorre todo o vetor de clientes at;e o ultimo cadastro
            if(strcmp(clientes[posicaoHelper].cpf,clientes[i].cpf) == 0){//verifica se o CPF � o mesmo. Retorna 0 caso seja
                system("cls"); // limpa conteudo anterior
                printf("\n\n\t                   ESTE CPF J� EST� CADASTRADO COM OS DADOS DO CLIENTE ABAIXO:   \n");
                printf("\n\t                   CLIENTE CADASTRADO   \n");
                printf("\n\t              >    Nome: %s", clientes[i].nome);
                printf("\n\t              >    Sobrenome: %s", clientes[i].sobreNome);
                printf("\n\t              >    Data de nascimento: %d/%d/%d", clientes[i].dataNasc.dia,clientes[i].dataNasc.mes,clientes[i].dataNasc.ano);
                printf("\n\t              >    CNH: %s", clientes[i].cnh);
                printf("\n\t              >    CPF: %s", clientes[i].cpf);
                printf("\n\t              >    CEP: %s", clientes[i].cep);
                printf("\n\t              >    N�mero: %s", clientes[i].numRes);
                printf("\n\t              >    Telefone: %s", clientes[i].telefone);
                printf("\n\t              >    DIGITE 0 (ZERO) PARA VOLTAR AO MENU ANTERIOR: ");
                scanf("%d", &varVolta);
                fflush(stdin);
                break; //encerra a busca depois de encontrar
            }
        }
        switch(varVolta)
        {
            case 22117435:{
                /*fim verifica��o de cadastro existente e continua��o do cadastro*/
                printf("\n\t  >    Primeiro Nome: ");
                gets(clientes[posicaoHelper].nome);
                printf("\n\t  >    Sobrenome: ");
                gets(clientes[posicaoHelper].sobreNome);
                printf("\n\t  >    Data de nascimento (dd mm aaaa): ");
                scanf("%d %d %d", &clientes[posicaoHelper].dataNasc.dia, &clientes[posicaoHelper].dataNasc.mes, &clientes[posicaoHelper].dataNasc.ano);
                fflush(stdin);
                printf("\n\t  >    CNH (apenas n�meros): ");
                gets(clientes[posicaoHelper].cnh);
                printf("\n\t  >    CEP (apenas n�meros): ");
                gets(clientes[posicaoHelper].cep);
                printf("\n\t  >    N�mero da Resid�ncia: ");
                gets(clientes[posicaoHelper].numRes);
                printf("\n\t  >    Telefone (apenas n�meros): ");
                gets(clientes[posicaoHelper].telefone);

                int confirma = 0;
                printf("\n\n\t  ||     CONFIRMA?");
                printf("\n\t  ||   > 1 SIM");
                printf("\n\t  ||   > 0 N�O");
                printf("\n\t  ||   > ");
                scanf("%d", &confirma);
                fflush(stdin);

                if(confirma==1){/*efetua o registro do cadastro no arquivo e atualiza as variaveis de controle*/

                    controle_cadastros++; //ganha +1 para inicializar o vetor com tamanho correto
                    clientes_cadastrados++; //a cada novo cadastro esse contador ganha +1
                    posicao_cliente++; //ganha +1 para n�o sobrescrever nenhum cadastro anterior

                    /*PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 1*/
                    char url[]="controleCadastrosClientes.mrb"; //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE CLIENTES CADASTRADOS
                    FILE *arq; //VARIAVEL DO TIPO "FILE"
                    arq = fopen(url, "w");//ABRE PARA ESCRITA
                    if(arq == NULL){
                            printf("Erro ao abrir o arquivo \"controleCadastrosClientes.mrb\"!\n");
                    }
                    else{
                            fprintf(arq, "%d\n", controle_cadastros);//atualiza variavel de controle
                    }
                    fclose(arq); //fecha o arquivo
                    /*FIM DO PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 1*/

                    /*PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 2*/
                    char url2[]="bdClientes.mrb"; //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE CLIENTES CADASTRADOS
                    FILE *arq2; //VARIAVEL DO TIPO "FILE"
                    arq2 = fopen(url2, "a");//ABRE PARA ESCRITA SEM APAGAR DADOS ANTERIORES
                    if(arq2 == NULL){
                            printf("Erro ao abrir o arquivo \"bdClientes.mrb\"!\n");
                    }
                    else{
                  /*NOME  */fprintf(arq2, "%s ", clientes[posicao_cliente].nome);
                  /*S.NOME*/fprintf(arq2, "%s ", clientes[posicao_cliente].sobreNome);
                  /*NASCM.*/fprintf(arq2, "%d %d %d ", clientes[posicao_cliente].dataNasc.dia,clientes[posicao_cliente].dataNasc.mes,clientes[posicao_cliente].dataNasc.ano);
                  /*CNH   */fprintf(arq2, "%s ", clientes[posicao_cliente].cnh);
                  /*CPF   */fprintf(arq2, "%s ", clientes[posicao_cliente].cpf);
                  /*CEP   */fprintf(arq2, "%s ", clientes[posicao_cliente].cep);
                  /*N.RES */fprintf(arq2, "%s ", clientes[posicao_cliente].numRes);
                  /*TEL.  */fprintf(arq2, "%s\n", clientes[posicao_cliente].telefone);
                   }
                    fclose(arq2); //fecha o arquivo
                    /*FIM DO PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 2*/
                }
            }
        }
}

    void cadastrarVeiculo(){ /*FUN��O RESPONS�VEL POR REALIZAR O CADASTRO DE UM NOVO VEICULO*/

    /*atualiza o espa�o de mem�ria para incluir um novo cadastro*/
    veiculos = (Veiculo *) realloc(veiculos, (controle_cadastros_veic + 1) * sizeof(Veiculo));

    int posicaoHelper = posicao_veiculo + 1; /*VARI�VEL AUXILIADORA PARA NOVO CADASTRO*/
    int varVolta = 22117435;
        printf("\n\t       CADASTRO DE VEICULOS\n");
        printf("\n\t  >    PLACA (XXX9999): ");
        gets(veiculos[posicaoHelper].placa);
        /*inicio verifica��o de cadastro existente*/
        for(int i = 0; i < veiculos_cadastrados; i++){ //percorre todo o vetor de veiculos ate o ultimo cadastro
            if(strcasecmp(veiculos[posicaoHelper].placa,veiculos[i].placa) == 0){//verifica se a PLACA � a mesma. Retorna 0 caso seja
                system("cls"); // limpa conteudo anterior
                printf("\n\n\t                   ESTA PLACA J� EST� CADASTRADA COM OS DADOS DO VEICULO ABAIXO:   \n");
                printf("\n\t                   VEICULO CADASTRADO   \n");
                printf("\n\t              >    Fabricante: %s", veiculos[i].fabricante);
                printf("\n\t              >    Modelo: %s", veiculos[i].modelo);
                printf("\n\t              >    Categoria: %s", categoria[veiculos[i].categoria]);
                printf("\n\t              >    Cambio: %s", cambio[veiculos[i].cambio]);
                printf("\n\t              >    Ano: %d", veiculos[i].anoFabricacao);
                printf("\n\t              >    Placa: %s", veiculos[i].placa);
                printf("\n\t              >    Disponibilidade: %s", reserva[veiculos[i].reserva]);
                printf("\n\t              >    DIGITE 0 (ZERO) PARA VOLTAR AO MENU ANTERIOR: ");
                scanf("%d", &varVolta);
                fflush(stdin);
                break; //encerra a busca depois de encontrar
            }
        }
        switch(varVolta)
        {
            case 22117435:{
                int ctrlCategoria = 999; /*VARIAVEL UTILIZADA PARA O TRATAMENTO DE ERROS*/
                int ctrlCambio = 999;    /*VARIAVEL UTILIZADA PARA O TRATAMENTO DE ERROS*/
                void selecionaCategoria(){
                for(int i = 0; i < tamanhoCategoria;i++){
                        printf("\n\t  >    %d - %s ", i,categoria[i]);
                    }
                    printf("\n\t  >     ");
                    scanf("%d", &veiculos[posicaoHelper].categoria);
                    fflush(stdin);
                    ctrlCategoria = veiculos[posicaoHelper].categoria;
                }
                void selecionaCambio(){
                for(int i = 0; i < tamanhoCambio;i++){
                        printf("\n\t  >    %d - %s ", i,cambio[i]);
                    }
                    printf("\n\t  >     ");
                    scanf("%d", &veiculos[posicaoHelper].cambio);
                    fflush(stdin);
                    ctrlCambio = veiculos[posicaoHelper].cambio;
                }

                /*fim verifica��o de cadastro existente e continua��o do cadastro*/
                printf("\n\t  >    Fabricante: ");
                gets(veiculos[posicaoHelper].fabricante);
                printf("\n\t  >    Modelo: ");
                gets(veiculos[posicaoHelper].modelo);
                printf("\n\t  >    Selecione a Categoria:");
                    selecionaCategoria();
                    while(ctrlCategoria>=tamanhoCategoria){
                        printf("\n\t       Categoria %d � inv�lida. Selecione novamente:", ctrlCategoria);
                        selecionaCategoria();
                    }
                printf("\n\t  >    Cambio: ");
                    selecionaCambio();
                    while(ctrlCambio>=tamanhoCambio){
                        printf("\n\t       Cambio %d � inv�lido. Selecione novamente:", ctrlCambio);
                        selecionaCambio();
                    }
                printf("\n\t  >    Ano de fabrica��o (aaaa): ");
                scanf("%d", &veiculos[posicaoHelper].anoFabricacao);
                fflush(stdin);
                /*ATRIBUI RESERVA COMO DISPON�VEL*/
                veiculos[posicaoHelper].reserva = 0;

                int confirma = 0;
                printf("\n\n\t  ||     CONFIRMA?");
                printf("\n\t  ||   > 1 SIM");
                printf("\n\t  ||   > 0 N�O");
                printf("\n\t  ||   > ");
                scanf("%d", &confirma);
                fflush(stdin);

                if(confirma==1){/*efetua o registro do cadastro no arquivo e atualiza as variaveis de controle*/

                    controle_cadastros_veic++; //ganha +1 para inicializar o vetor com tamanho correto
                    veiculos_cadastrados++; //a cada novo cadastro esse contador ganha +1
                    posicao_veiculo++; //ganha +1 para n�o sobrescrever nenhum cadastro anterior

                    /*PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 3*/
                    char url[]="controleCadastrosVeiculos.mrb"; //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE CLIENTES CADASTRADOS
                    FILE *arq; //VARIAVEL DO TIPO "FILE"
                    arq = fopen(url, "w");//ABRE PARA ESCRITA
                    if(arq == NULL){
                            printf("Erro ao abrir o arquivo \"controleCadastrosVeiculos.mrb\"!\n");
                    }
                    else{
                            fprintf(arq, "%d\n", controle_cadastros_veic);//atualiza variavel de controle
                    }
                    fclose(arq); //fecha o arquivo
                    /*FIM DO PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 3*/

                    /*PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 4*/
                    char url2[]="bdVeiculos.mrb"; //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE VEICULOS CADASTRADOS
                    FILE *arq2; //VARIAVEL DO TIPO "FILE"
                    arq2 = fopen(url2, "a");//ABRE PARA ESCRITA SEM APAGAR DADOS ANTERIORES
                    if(arq2 == NULL){
                            printf("Erro ao abrir o arquivo \"bdVeiculos.mrb\"!\n");
                    }
                    else{
                  /*FABRICANTE*/fprintf(arq2, "%s ", veiculos[posicao_veiculo].fabricante);
                  /*MODELO    */fprintf(arq2, "%s ", veiculos[posicao_veiculo].modelo);
                  /*CATEGORIA */fprintf(arq2, "%d ", veiculos[posicao_veiculo].categoria);
                  /*CAMBIO    */fprintf(arq2, "%d ", veiculos[posicao_veiculo].cambio);
                  /*ANO       */fprintf(arq2, "%d ", veiculos[posicao_veiculo].anoFabricacao);
                  /*PLACA     */fprintf(arq2, "%s ", veiculos[posicao_veiculo].placa);
                  /*DISPONIVEL*/fprintf(arq2, "%d\n", veiculos[posicao_veiculo].reserva);
                   }
                    fclose(arq2); //fecha o arquivo
                    /*FIM DO PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 4*/
                }
            }
        }
}

    void apagarClienteCadastrado(){ /*FUN��O RESPONS�VEL POR REMOVER O CADASTRO DE UM CLIENTE*/

    char cpfProcurado[12];
    int encontrado = 0;

    printf("\n\t    REMOVER CLIENTE\n");
    printf("\n\t  > Digite o CPF do Cliente (apenas n�meros): ");
    gets(cpfProcurado);

    for(int i = 0; i < clientes_cadastrados; i++){ //percorre todo o vetor de clientes at;e o ultimo cadastro

        if(strcmp(cpfProcurado,clientes[i].cpf) == 0){//verifica se o CPF � o mesmo. Retorna 0 caso seja

            encontrado = 1; //confirma que encontrou o cliente

            system("cls"); // limpa conteudo anterior
            printf("\n\t                   DADOS DO CLIENTE QUE SER� REMOVIDO DO CADASTRO:   \n");
            printf("\n\t              >    Nome: %s", clientes[i].nome);
            printf("\n\t              >    Sobrenome: %s", clientes[i].sobreNome);
            printf("\n\t              >    Data de nascimento: %d/%d/%d", clientes[i].dataNasc.dia,clientes[i].dataNasc.mes,clientes[i].dataNasc.ano);
            printf("\n\t              >    CNH: %s", clientes[i].cnh);
            printf("\n\t              >    CPF: %s", clientes[i].cpf);
            printf("\n\t              >    CEP: %s", clientes[i].cep);
            printf("\n\t              >    N�mero: %s", clientes[i].numRes);
            printf("\n\t              >    Telefone: %s", clientes[i].telefone);

            int confirmaRemov = 0; /*confirma exclus�o do cadastro*/
            printf("\n\n\t              ||     REALMENTE DESEJA REMOVER?");
            printf("\n\t              ||   > 1 SIM");
            printf("\n\t              ||   > 0 N�O");
            printf("\n\t              ||   > ");
            scanf("%d", &confirmaRemov);
            fflush(stdin);

            if(confirmaRemov==1){ /*VERIFICA��O PARA EXCLUIR CLIENTE*/
                    int j;
                    for(j = i + 1; j < clientes_cadastrados; j++){ /*SOBREP�E OS DADOS REMOVENDO O CLIENTE SELECIONADO DO VETOR*/
                        clientes[j-1] = clientes[j];
                    }
                    clientes_cadastrados--; /*ATUALIZA AS VARIAVEIS DE CONTROLE*/
                    controle_cadastros--;
                    posicao_cliente--;

            /*PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 1*/
            char url[]="controleCadastrosClientes.mrb"; //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE CLIENTES CADASTRADOS
            FILE *arq; //VARIAVEL DO TIPO "FILE"
            arq = fopen(url, "w");//ABRE PARA ESCRITA
            if(arq == NULL){
                    printf("Erro ao abrir o arquivo \"controleCadastrosClientes.mrb\"!\n");
            }
            else{
                    fprintf(arq, "%d\n", controle_cadastros);//atualiza variavel de controle
            }
            fclose(arq); //fecha o arquivo
            /*FIM DO PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 1*/

            /*PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 2*/
            char url2[]="bdClientes.mrb"; //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE CLIENTES CADASTRADOS
            FILE *arq2; //VARIAVEL DO TIPO "FILE"
            arq2 = fopen(url2, "w");//ABRE PARA ESCRITA
            if(arq2 == NULL){
                    printf("Erro ao abrir o arquivo \"bdClientes.mrb\"!\n");
            }
            else{
                    for(int i = 0; i < clientes_cadastrados; i++){

              /*NOME  */fprintf(arq2, "%s ", clientes[i].nome);
              /*S.NOME*/fprintf(arq2, "%s ", clientes[i].sobreNome);
              /*NASCM.*/fprintf(arq2, "%d %d %d ", clientes[i].dataNasc.dia,clientes[i].dataNasc.mes,clientes[i].dataNasc.ano);
              /*CNH   */fprintf(arq2, "%s ", clientes[i].cnh);
              /*CPF   */fprintf(arq2, "%s ", clientes[i].cpf);
              /*CEP   */fprintf(arq2, "%s ", clientes[i].cep);
              /*N.RES */fprintf(arq2, "%s ", clientes[i].numRes);
              /*TEL.  */fprintf(arq2, "%s\n", clientes[i].telefone);
                    }
           }
            fclose(arq2); //fecha o arquivo
            /*FIM DO PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 2*/

                    printf("\n\n\t              ||     CLIENTE REMOVIDO COM SUCESSO!");
                    }
            break; //encerra a busca
        }
        printf("\n\t  PESQUISANDO CADASTRO, POR FAVOR AGUARDE... ");
    }

    if(encontrado==0){//confirma se o cliente n�o esta cadastrado
            system("cls"); // limpa conteudo anterior
            printf("\n\n\t                   CLIENTE N�O CADASTRADO");
            printf("\n\t              >    CPF: %s", cpfProcurado);
        }

        int confirma = 0;
        printf("\n\n\t              ||     DESEJA REMOVER OUTRO CLIENTE?");
        printf("\n\t              ||   > 1 SIM");
        printf("\n\t              ||   > 0 N�O");
        printf("\n\t              ||   > ");
        scanf("%d", &confirma);
        fflush(stdin);

     if(confirma==1){ /*VERIFICA��O PARA APAGAR OU N�O UM OUTRO CLIENTE*/
           apagarClienteCadastrado();
        }
}

    void apagarVeiculoCadastrado(){ /*FUN��O RESPONS�VEL POR REMOVER O CADASTRO DE UM VEICULO*/

    char placaProcurado[12];
    int encontrado = 0;
    printf("\n\t    REMOVER VE�CULO\n");
    printf("\n\t  > Digite a PLACA do ve�culo (XXX9999): ");
    gets(placaProcurado);

    for(int i = 0; i < veiculos_cadastrados; i++){ //percorre todo o vetor de veiculos at;e o ultimo cadastro

        if(strcasecmp(placaProcurado,veiculos[i].placa) == 0){//verifica se a PLACA � a mesma. Retorna 0 caso seja

            encontrado = 1; //confirma que encontrou o veiculo
            system("cls"); // limpa conteudo anterior
            printf("\n\t                   DADOS DO VE�CULO QUE SER� REMOVIDO DO CADASTRO:   \n");
            printf("\n\t              >    Fabricante: %s", veiculos[i].fabricante);
            printf("\n\t              >    Modelo: %s", veiculos[i].modelo);
            printf("\n\t              >    Categoria: %s", categoria[veiculos[i].categoria]);
            printf("\n\t              >    Cambio: %s", cambio[veiculos[i].cambio]);
            printf("\n\t              >    Ano: %d", veiculos[i].anoFabricacao);
            printf("\n\t              >    Placa: %s", veiculos[i].placa);
            printf("\n\t              >    Disponibilidade: %s", reserva[veiculos[i].reserva]);

            int confirmaRemov = 0; /*confirma exclus�o do cadastro*/
            if(veiculos[i].reserva == 1){
                printf("\n\n\t              ||     O VE�CULO EST� ALUGADO. IMPOSS�VEL REMOVER.\n");
                system("PAUSE");
            }else{
                printf("\n\n\t              ||     REALMENTE DESEJA REMOVER?");
                printf("\n\t              ||   > 1 SIM");
                printf("\n\t              ||   > 0 N�O");
                printf("\n\t              ||   > ");
                scanf("%d", &confirmaRemov);
                fflush(stdin);
            }

            if(confirmaRemov==1){ /*VERIFICA��O PARA EXCLUIR VEICULO*/
                    int j;
                    for(j = i + 1; j < veiculos_cadastrados; j++){ /*SOBREP�E OS DADOS REMOVENDO O VEICULO SELECIONADO DO VETOR*/
                        veiculos[j-1] = veiculos[j];
                    }
                    veiculos_cadastrados--; /*ATUALIZA AS VARIAVEIS DE CONTROLE*/
                    controle_cadastros_veic--;
                    posicao_veiculo--;

            /*PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 3*/
            char url[]="controleCadastrosVeiculos.mrb"; //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE VEICULOS CADASTRADOS
            FILE *arq; //VARIAVEL DO TIPO "FILE"
            arq = fopen(url, "w");//ABRE PARA ESCRITA
            if(arq == NULL){
                    printf("Erro ao abrir o arquivo \"controleCadastrosVeiculos.mrb\"!\n");
            }
            else{
                    fprintf(arq, "%d\n", controle_cadastros_veic);//atualiza variavel de controle
            }
            fclose(arq); //fecha o arquivo
            /*FIM DO PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 3*/

            /*PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 4*/
            char url2[]="bdVeiculos.mrb"; //NOME DO ARQUIVO QUE ARMAZENA OS DADOS DE VEICULOS CADASTRADOS
            FILE *arq2; //VARIAVEL DO TIPO "FILE"
            arq2 = fopen(url2, "w");//ABRE PARA ESCRITA
            if(arq2 == NULL){
                    printf("Erro ao abrir o arquivo \"bdVeiculos.mrb\"!\n");
            }
            else{
                    for(int i = 0; i < veiculos_cadastrados; i++){

                  /*FABRICANTE*/fprintf(arq2, "%s ", veiculos[i].fabricante);
                  /*MODELO    */fprintf(arq2, "%s ", veiculos[i].modelo);
                  /*CATEGORIA */fprintf(arq2, "%d ", veiculos[i].categoria);
                  /*CAMBIO    */fprintf(arq2, "%d ", veiculos[i].cambio);
                  /*ANO       */fprintf(arq2, "%d ", veiculos[i].anoFabricacao);
                  /*PLACA     */fprintf(arq2, "%s ", veiculos[i].placa);
                  /*DISPONIVEL*/fprintf(arq2, "%d\n", veiculos[i].reserva);
                    }
           }
            fclose(arq2); //fecha o arquivo
            /*FIM DO PROCESSO DE GRAVA��O DO ARQUIVO DE CONTROLE 4*/

                    printf("\n\n\t              ||     VE�CULO REMOVIDO COM SUCESSO!");
                    }
            break; //encerra a busca
        }
        printf("\n\t  PESQUISANDO CADASTRO, POR FAVOR AGUARDE... ");
    }

    if(encontrado==0){//confirma se o veiculo n�o esta cadastrado
            system("cls"); // limpa conteudo anterior
            printf("\n\n\t                   VE�CULO N�O CADASTRADO");
            printf("\n\t              >    PLACA: %s", placaProcurado);
        }

        int confirma = 0;
        printf("\n\n\t              ||     DESEJA REMOVER OUTRO VE�CULO?");
        printf("\n\t              ||   > 1 SIM");
        printf("\n\t              ||   > 0 N�O");
        printf("\n\t              ||   > ");
        scanf("%d", &confirma);
        fflush(stdin);

     if(confirma==1){ /*VERIFICA��O PARA APAGAR OU N�O UM OUTRO VEICULO*/
           apagarVeiculoCadastrado();
        }
}

    void consultarVeiculoCadastrado(){
        char* placaConsulta;
        int encontrado2 = 0;
        printf("\n\t       CONSULTA DE VEICULOS\n");
        printf("\n\t  >    PLACA (XXX9999): ");
        gets(placaConsulta);
        /*inicio verifica��o de cadastro existente*/
        for(int i = 0; i < veiculos_cadastrados; i++){ //percorre todo o vetor de veiculos ate o ultimo cadastro
            if(strcasecmp(placaConsulta,veiculos[i].placa) == 0){//verifica se a PLACA � a mesma. Retorna 0 caso seja
                system("cls"); // limpa conteudo anterior
                encontrado2 = 1;
                printf("\n\t                   VEICULO CADASTRADO   \n");
                printf("\n\t              >    Fabricante: %s", veiculos[i].fabricante);
                printf("\n\t              >    Modelo: %s", veiculos[i].modelo);
                printf("\n\t              >    Categoria: %s", categoria[veiculos[i].categoria]);
                printf("\n\t              >    Cambio: %s", cambio[veiculos[i].cambio]);
                printf("\n\t              >    Ano: %d", veiculos[i].anoFabricacao);
                printf("\n\t              >    Placa: %s", veiculos[i].placa);
                printf("\n\t              >    Disponibilidade: %s", reserva[veiculos[i].reserva]);
                break; //encerra a busca depois de encontrar
            }
            printf("\n\t  PESQUISANDO CADASTRO, POR FAVOR AGUARDE... ");
        }

         if(encontrado2==0){//confirma se o cliente n�o esta cadastrado
            system("cls"); // limpa conteudo anterior
            printf("\n\n\t                   VE�CULO N�O CADASTRADO");
            printf("\n\t              >    PLACA: %s", placaConsulta);
        }

        int confirma = 0;
        printf("\n\n\t  ||     DESEJA CONSULTAR OUTRO VE�CULO?");
        printf("\n\t  ||   > 1 SIM");
        printf("\n\t  ||   > 0 N�O");
        printf("\n\t  ||   > ");
        scanf("%d", &confirma);
        fflush(stdin);
        if(confirma==1){ /*VERIFICA��O PARA CONSULTAR OU N�O UM OUTRO VE�CULO*/
           consultarVeiculoCadastrado();
        }
}

    void consultarVeiculosDisponiveis(){ /*FUN��O RESPONS�VEL POR LISTAR OS VE�CULOS DISPON�VEIS POR CATEGORIA*/

        int categoriaSelecionada = 99;
        int carCount = 0;
        printf("\n\t  >    Selecione a Categoria para consulta:");
        for(int i = 0; i < tamanhoCategoria;i++){
                        printf("\n\t  >    %d - %s ", i,categoria[i]);
                    }
        printf("\n\t  >     ");
        scanf("%d", &categoriaSelecionada);
        fflush(stdin);

        printf("\n\n\t  ||     LISTANDO APENAS OS VE�CULOS DISPON�VEIS");
        printf("\n\t  ||     CATEGORIA %d - %s", categoriaSelecionada, categoria[categoriaSelecionada]);
        for(int i = 0; i < veiculos_cadastrados; i++){
                if(categoriaSelecionada == veiculos[i].categoria && 0 == veiculos[i].reserva){ /*LISTA VE�CULOS DE UMA CATEGORIA E DISPON�VEIS*/
                printf("\n\t                  ");
                printf("\n\t              >    Fabricante: %s", veiculos[i].fabricante);
                printf("\n\t              >    Modelo: %s", veiculos[i].modelo);
                printf("\n\t              >    Categoria: %s", categoria[veiculos[i].categoria]);
                printf("\n\t              >    Cambio: %s", cambio[veiculos[i].cambio]);
                printf("\n\t              >    Ano: %d", veiculos[i].anoFabricacao);
                printf("\n\t              >    Placa: %s", veiculos[i].placa);
                printf("\n\t              >    Disponibilidade: %s", reserva[veiculos[i].reserva]);
                carCount++;
                }
        }

        int confirma = 0;
        printf("\n\n\t  ||     FIM DA BUSCA. ENCONTRADO(S) %d VE�CULO(S).", carCount);
        printf("\n\t  ||     DESEJA CONSULTAR OUTRA CATEGORIA?");
        printf("\n\t  ||   > 1 SIM");
        printf("\n\t  ||   > 0 N�O");
        printf("\n\t  ||   > ");
        scanf("%d", &confirma);
        fflush(stdin);

     if(confirma==1){ /*VERIFICA��O PARA CONSULTAR OU N�O UMA OUTRA CATEGORIA*/
           consultarVeiculosDisponiveis();
        }
}

   void listarTudo(){ /*FUN��O RESPONS�VEL POR LISTAR TODOS OS VE�CULOS E LOCA\C�ES EM ABERTO*/

        int carCount = 0;

        system("cls");
        printf("\n\n\t\t\t\t\t\t  ||     LISTANDO TODOS OS VE�CULOS DISPON�VEIS\n");
        for(int i = 0; i < veiculos_cadastrados; i++){ /*LISTA VEICULOS DISPON�VEIS*/
                if(veiculos[i].reserva==0){
                        carCount++;
                        printf("\n%d", carCount);
                        printf("| %s", reserva[veiculos[i].reserva]);
                        printf("\t| Placa: %s", veiculos[i].placa);
                        printf("\t| Ano: %d", veiculos[i].anoFabricacao);
                        printf("\t| Cambio: %s", cambio[veiculos[i].cambio]);
                        printf("\t| Categoria: %s", categoria[veiculos[i].categoria]);
                        printf("\t| Fabrc: %s", veiculos[i].fabricante);
                        printf("\t| Mdl: %s\n", veiculos[i].modelo);
                }
        }
        printf("\n\n\t\t\t\t\t\t  ||     LISTANDO OS VE�CULOS ALUGADOS\n");

        for(int j = 0; j < veiculos_cadastrados; j++){ /*LISTA VEICULOS ALUGADOS*/
                if(veiculos[j].reserva==1){
                        carCount++;
                        char* placaHelper = veiculos[j].placa;
                        for(int i = 0; i < locacoes_cadastrados; i++){ //percorre todo o vetor de locacoes ate o ultimo cadastro
                            if((strcasecmp(placaHelper,locacoes[i].placa) == 0) && (1 == locacoes[i].devolvido)){//verifica se o veiculo est� dispon�vel
                            printf("\n%d", carCount);
                            printf("| %s", reserva[veiculos[j].reserva]);
                            printf("\t| Data Devolu��o: %d/%d/%d", locacoes[i].dataDev.dia,locacoes[i].dataDev.mes,locacoes[i].dataDev.ano);
                            if(locacoes[i].horaDev.min<10){
                            printf("\t| Hora Devolu��o: %d:%d0", locacoes[i].horaDev.hora,locacoes[i].horaDev.min);}else{
                            printf("\t| Hora Devolu��o: %d:%d", locacoes[i].horaDev.hora,locacoes[i].horaDev.min);
                            }
                            printf("\t| Placa: %s", veiculos[j].placa);
                            printf("\t| Categoria: %s", categoria[veiculos[j].categoria]);
                            printf("\t| Fabrc: %s", veiculos[j].fabricante);
                            printf("\t| Mdl: %s\n", veiculos[j].modelo);
                        }
                }
        }
        }
        printf("\n\n\t\t  ||     FIM DA BUSCA. ENCONTRADO(S) %d VE�CULO(S). ", carCount);
        system("PAUSE");
}

    void consultarClienteCadastrado(){ /*FUN��O RESPONS�VEL POR CONSULTAR O CADASTRO DE UM CLIENTE*/

    char cpfProcurado[12];
    int encontrado = 0;

    printf("\n\t    CONSULTAR CLIENTE\n");
    printf("\n\t  > Digite o CPF do Cliente (apenas n�meros): ");
    gets(cpfProcurado);

    for(int i = 0; i < clientes_cadastrados; i++){ //percorre todo o vetor de clientes at;e o ultimo cadastro

        if(strcmp(cpfProcurado,clientes[i].cpf) == 0){//verifica se o CPF � o mesmo. Retorna 0 caso seja

            encontrado = 1; //confirma que encontrou o cliente

            system("cls"); // limpa conteudo anterior
            printf("\n\t                   CLIENTE CADASTRADO   \n");
            printf("\n\t              >    Nome: %s", clientes[i].nome);
            printf("\n\t              >    Sobrenome: %s", clientes[i].sobreNome);
            printf("\n\t              >    Data de nascimento: %d/%d/%d", clientes[i].dataNasc.dia,clientes[i].dataNasc.mes,clientes[i].dataNasc.ano);
            printf("\n\t              >    CNH: %s", clientes[i].cnh);
            printf("\n\t              >    CPF: %s", clientes[i].cpf);
            printf("\n\t              >    CEP: %s", clientes[i].cep);
            printf("\n\t              >    N�mero: %s", clientes[i].numRes);
            printf("\n\t              >    Telefone: %s", clientes[i].telefone);
            break; //encerra a busca depois de encontrar

        }
        printf("\n\t  PESQUISANDO CADASTRO, POR FAVOR AGUARDE... ");
    }

    if(encontrado==0){//confirma se o cliente n�o esta cadastrado
            system("cls"); // limpa conteudo anterior
            printf("\n\n\t                   CLIENTE N�O CADASTRADO");
            printf("\n\t              >    CPF: %s", cpfProcurado);
        }

        int confirma = 0;
        printf("\n\n\t  ||     DESEJA CONSULTAR OUTRO CLIENTE?");
        printf("\n\t  ||   > 1 SIM");
        printf("\n\t  ||   > 0 N�O");
        printf("\n\t  ||   > ");
        scanf("%d", &confirma);
        fflush(stdin);

     if(confirma==1){ /*VERIFICA��O PARA CONSULTAR OU N�O UM OUTRO CLIENTE*/
           consultarClienteCadastrado();
        }
}

    void menuCliente(){ /*FUN��O RESPONS�VEL POR MONTAR O MENU DE CLIENTES*/

    int ck = 3;
    while(ck != 0 )//ENQUANTO A VARI�VEL DE CONTROLE FOR DIFERENTE DE ZERO O MENU CONTINUA DISPONIVEL
    {
        system("cls");
        printf("\n\t|||     ||||||  /||||\\  //||\\\\  |||   ");     /*CLIENTES*/
        printf("\n\t|||     |||     ||  |/  ||  ||  |||   ");
        printf("\n\t|||     |||||   || __   ||||||  |||   ");
        printf("\n\t|||     |||     || \\\\\\  ||  ||  |||   ");
        printf("\n\t||||||  ||||||  \\||||/  ||  ||  ||||||");
        printf("\n\t _   __       _____        __  __   _ ");
        printf("\n\t| \\ |   |\\  |   |     .   /   /  \\ | \\");
        printf("\n\t|-/ |-  | \\ |   |    /_\\  |   |--| |-/");
        printf("\n\t| \\ |__ |  \\|   |    | |  \\__ |  | | \\\n");
        printf("\n\t CLIENTES\n");
        printf("\n\t Digite a op��o desejada:\n\t > 1 para CADASTRAR UM NOVO CLIENTE\n\t > 2 para CONSULTAR O CADASTRO DE UM CLIENTE\n\t > 3 para REMOVER UM CLIENTE DO CADASTRO\n\t > 0 para VOLTAR ao menu principal\n\t > ");
        scanf("%d", &ck);
        fflush(stdin);

        switch(ck){
            case 1:{
                cadastrarCliente();
            break;
            }
            case 2:{
                consultarClienteCadastrado();
            break;
            }
            case 3:{
                apagarClienteCadastrado();
            break;
            }
            case 0:{
                printf("\nRetornando ao menu anterior\n\n");
                system("cls");
            break;
            }
            default:{
                printf(" %d n�o � uma op��o valida!\n", ck);
            break;
            }
        }
    }
    }

    void menuVeiculos(){ /*FUN��O RESPONS�VEL POR MONTAR O MENU DE VE�CULOS*/

    int ck = 3;
    while(ck != 0 )//ENQUANTO A VARI�VEL DE CONTROLE FOR DIFERENTE DE ZERO O MENU CONTINUA DISPONIVEL
    {
        system("cls");
        printf("\n\t|||     ||||||  /||||\\  //||\\\\  |||   ");     /*CLIENTES*/
        printf("\n\t|||     |||     ||  |/  ||  ||  |||   ");
        printf("\n\t|||     |||||   || __   ||||||  |||   ");
        printf("\n\t|||     |||     || \\\\\\  ||  ||  |||   ");
        printf("\n\t||||||  ||||||  \\||||/  ||  ||  ||||||");
        printf("\n\t _   __       _____        __  __   _ ");
        printf("\n\t| \\ |   |\\  |   |     .   /   /  \\ | \\");
        printf("\n\t|-/ |-  | \\ |   |    /_\\  |   |--| |-/");
        printf("\n\t| \\ |__ |  \\|   |    | |  \\__ |  | | \\\n");
        printf("\n\t VE�CULOS\n");
        printf("\n\t Digite a op��o desejada:\n\t > 1 para CADASTRAR NOVO VE�CULO\n\t > 2 para CONSULTAR OS VE�CULOS DISPON�VEIS POR CATEGORIA\n\t > 3 para CONSULTAR UM VE�CULO PELA PLACA\n\t > 4 para LISTAR TODOS OS VE�CULOS CADASTRADOS\n\t > 5 para REMOVER UM VE�CULO DO CADASTRO\n\t > 0 para VOLTAR ao menu principal\n\t > ");
        scanf("%d", &ck);
        fflush(stdin);

        switch(ck){
            case 1:{
                cadastrarVeiculo();
            break;
            }
            case 2:{
                consultarVeiculosDisponiveis();
            break;
            }
            case 3:{
                consultarVeiculoCadastrado();
            break;
            }
            case 4:{
                listarTudo();
            break;
            }
            case 5:{
                apagarVeiculoCadastrado();
            break;
            }
            case 0:{
                printf("\nRetornando ao menu anterior\n\n");
                system("cls");
            break;
            }
            default:{
                printf(" %d n�o � uma op��o valida!\n", ck);
            break;
            }
        }
    }
    }

    void menuLocacao(){ /*FUN��O RESPONS�VEL POR MONTAR O MENU DE LOCA��ES*/

    int ck = 3;
    while(ck != 0 )//ENQUANTO A VARI�VEL DE CONTROLE FOR DIFERENTE DE ZERO O MENU CONTINUA DISPONIVEL
    {
        system("cls");
        printf("\n\t|||     ||||||  /||||\\  //||\\\\  |||   ");     /*LOCA��ES*/
        printf("\n\t|||     |||     ||  |/  ||  ||  |||   ");
        printf("\n\t|||     |||||   || __   ||||||  |||   ");
        printf("\n\t|||     |||     || \\\\\\  ||  ||  |||   ");
        printf("\n\t||||||  ||||||  \\||||/  ||  ||  ||||||");
        printf("\n\t _   __       _____        __  __   _ ");
        printf("\n\t| \\ |   |\\  |   |     .   /   /  \\ | \\");
        printf("\n\t|-/ |-  | \\ |   |    /_\\  |   |--| |-/");
        printf("\n\t| \\ |__ |  \\|   |    | |  \\__ |  | | \\\n");
        printf("\n\t LOCA��O\n");
        printf("\n\t Digite a op��o desejada:\n\t > 1 para CONSULTAR VE�CULOS DISPON�VEIS\n\t > 2 para CONSULTAR TODOS OS VE�CULOS E LOCA��ES\n\t > 3 para FAZER UMA NOVA LOCA��O\n\t > 4 para DEVOLVER UM VE�CULO ALUGADO\n\t > 5 para LISTAR PRE�OS POR CATEGORIA\n\t > 0 para VOLTAR ao menu principal\n\t > ");
        scanf("%d", &ck);
        fflush(stdin);

        switch(ck){
            case 1:{
                consultarVeiculosDisponiveis(); /*CHAMA CONSULTA DE VE�CULOS*/
            break;
            }
            case 2:{
                listarTudo();/*CHAMA CONSULTA DE VE�CULOS E LOCA��ES*/
            break;
            }
            case 3:{
                novaLocacao();/*CHAMA NOVA LOCA��O*/
            break;
            }
            case 4:{
                devolverVeiculo();/*CHAMA DEVOLU��O DE VE�CULOS*/
            break;
            }
            case 5:{
                listarPrecos();/*LISTA PRE�OS POR CATEGORIA*/
            break;
            }
            case 0:{
                printf("\nRetornando ao menu anterior\n\n");
                system("cls");
            break;
            }
            default:{
                printf(" %d n�o � uma op��o valida!\n", ck);
            break;
            }
        }
    }
    }

int main(){
    system("color 70");                               /*INICIALIZA��O DA INTERFACE GR�FICA -  CONFIGURA��O DE CORES*/
    system("title Legal Rent a Car - Vers. 1.0.3");   /*INICIALIZA��O DA INTERFACE GR�FICA -  BARRA DE T�TULO*/
    setlocale(LC_ALL,"Portuguese");                   /*INICIALIZA��O DA INTERFACE GR�FICA -  CONFIGURA��O DE IDIOMA*/

    char pws[5];
    char bkp[5];
    char usr[3];
    while((strcmp("senha",pws) != 0) || (strcmp("mrb",usr) != 0))
    {
        system("cls");
        system("date/t");                                 /*INICIALIZA��O DA INTERFACE GR�FICA -  DATA ATUAL*/
        system("ver");                                    /*INICIALIZA��O DA INTERFACE GR�FICA -  VERS�O DO SISTEMA OPERACIONAL*/
        printf("\n\t|||     ||||||  /||||\\  //||\\\\  |||   ");     /*NOME DA EMPRESA*/
        printf("\n\t|||     |||     ||  |/  ||  ||  |||   ");
        printf("\n\t|||     |||||   || __   ||||||  |||   ");
        printf("\n\t|||     |||     || \\\\\\  ||  ||  |||   ");
        printf("\n\t||||||  ||||||  \\||||/  ||  ||  ||||||");
        printf("\n\t _   __       _____        __  __   _ ");
        printf("\n\t| \\ |   |\\  |   |     .   /   /  \\ | \\");
        printf("\n\t|-/ |-  | \\ |   |    /_\\  |   |--| |-/");
        printf("\n\t| \\ |__ |  \\|   |    | |  \\__ |  | | \\\n");
        printf("\n\t LOGIN DE USU�RIO\n ");
        printf("\n\t USU�RIO: ");
        gets(usr);
        printf("\n\t SENHA: ");
        /*INICIO MASCARA*/
        int i;
        int j;
        for(j = 0 , i=getch(); i != '\r'; i=getch() , j++)
        {
        putchar(MASK); /* mostra a masca a cada tecla digitada */
        if(j == 5) {break;} /*limitador para n�o estourar a variavel pws*/
        pws[j] = i;  /* insere o caractere original na variavel */
        }
        /*FIM MASCARA*/
        if ((strcmp("senha",pws) != 0) || (strcmp("mrb",usr) != 0))
        {
            printf("\n\n\t USU�RIO OU SENHA INCORRETOS.\n\t ");
            system("PAUSE");
        }
    }

    keybd_event ( VK_MENU, 0x36, 0, 0 );
    keybd_event ( VK_RETURN, 0x1C, 0, 0 );
    keybd_event ( VK_RETURN, 0x1C, KEYEVENTF_KEYUP, 0 );
    keybd_event ( VK_MENU, 0x38, KEYEVENTF_KEYUP, 0 );
    lerArquivo();                                     /*EXECUTA ROTINA QUE RECUPERA AS �LTIMAS INFORMA��ES SALVAS NO ARQUIVO DE CLIENTES*/
    lerArquivoVeic();                                 /*EXECUTA ROTINA QUE RECUPERA AS �LTIMAS INFORMA��ES SALVAS NO ARQUIVO DE VEICULOS*/
    lerArquivoLoc();                                  /*EXECUTA ROTINA QUE RECUPERA AS �LTIMAS INFORMA��ES SALVAS NO ARQUIVO DE LOCA��ES*/

    int ck = 3;                 /*MONTAGEM DO MENU PRINCIPAL*/
    while(ck != 0 )             /*FAZ A VERIFICA��O DA OP��O SELECIONADA*/
    {
        system("cls");
        system("date/t");                                 /*INICIALIZA��O DA INTERFACE GR�FICA -  DATA ATUAL*/
        system("ver");                                    /*INICIALIZA��O DA INTERFACE GR�FICA -  VERS�O DO SISTEMA OPERACIONAL*/
        printf("\n\t|||     ||||||  /||||\\  //||\\\\  |||   ");     /*NOME DA EMPRESA*/
        printf("\n\t|||     |||     ||  |/  ||  ||  |||   ");
        printf("\n\t|||     |||||   || __   ||||||  |||   ");
        printf("\n\t|||     |||     || \\\\\\  ||  ||  |||   ");
        printf("\n\t||||||  ||||||  \\||||/  ||  ||  ||||||");
        printf("\n\t _   __       _____        __  __   _ ");
        printf("\n\t| \\ |   |\\  |   |     .   /   /  \\ | \\");
        printf("\n\t|-/ |-  | \\ |   |    /_\\  |   |--| |-/");
        printf("\n\t| \\ |__ |  \\|   |    | |  \\__ |  | | \\\n");
        printf("\n\t MENU PRINCIPAL\n ");
        printf("\n\t Digite a op��o desejada:\n\t > 1 para CLIENTES\n\t > 2 para VE�CULOS\n\t > 3 para LOCA��ES\n\t > 0 para SAIR\n\t > ");
        scanf("%d", &ck);
        fflush(stdin);

        switch(ck){
            case 1:{
                menuCliente(); /*CHAMA O MENU DE CLIENTES*/
            break;
            }
            case 2:{
                menuVeiculos(); /*CHAMA O MENU DE VEICULOS*/
            break;
            }
            case 3:{
                                /*CHAMA O MENU DE LOCA��ES*/
                menuLocacao();
            break;
            }
            case 0:{
                system("cls");
                printf("\n ###  PROGRAMA FINALIZADO  ###\n\n");
                system("exit");
            break;
            }
            default:{
                system("cls");
                printf("\n %d n�o � uma op��o v�lida!\n", ck);
            break;
            }
        }
    }
return 0;
}

