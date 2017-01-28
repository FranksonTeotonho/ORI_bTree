#include <stdio.h>
#include <stdlib.h>

#define T  2
#define MAXP  2 * T - 1
#define MAXF 2 * T

//defini��o de um n�.
typedef struct node{
    int n; 										//N�mero de chaves
    int chaves[MAXP+1]; 						//Vetor de Chaves
    struct node *filhos[MAXF+1]; 			//Filhos (Vetor de ponteiros)
    int eFolha; 								//0 = n�o � folha, 1 = � folha
}node;


// Ponteiro para n� que ser� a ra�z da Arvore
typedef struct bTree{
    node *raiz;
}bTree;

//Fun��o criabTree(bTree *t):
//Cria a arvore, alocando espa�o para o n� raiz, coloca n�mero de elmentos = 0
void criabTree(bTree *t){
    node *x;

    x = (node*) calloc(1, sizeof(node)); 	//Aloca primeiro n�
    x->n = 0;									//n�mero de elementos = 0
    x->eFolha = 1;								//eFolha = true

    t->raiz = x;								//coloca esse n� como ra�z
}

//Fun��o divideFilhobTree(node *x, int i)
//divide o n� de acordo com o parametro i (que diz onde selecionar)
void divideFilhobTree(node *x, int i){
    node *z, *y;
    int j;

    z = (node*) calloc(1, sizeof(node));  	//Aloca espa�o para o novo n�

    y = x->filhos[i];							//troca o filho de X na posi��o i de lugar

    z->eFolha = y->eFolha;						//se o n� original era folha, o novo tamb�m ser�

    z->n = T-1;									//n�mero de elementos do novo n� = T - 1

    for (j = 1; j < T; j++){                    //Copia as chaves da metade pra frente para o novo n�
            z->chaves[j] = y->chaves[j+T];
    }

    if (!y->eFolha){							//Se o n� n�o for folha:
        for (j = 1; j <= T; j++){				//Copia os filhos do n� original a partir de T+1 at� 2T para o novo n�
            z->filhos[j] = y->filhos[j+T];
        }
    }
    y->n = T-1;									//n�mero de elementos = T-1

    for (j = (x->n)+1; j >i; j-- ){				//desloca os filhos em 1 posi�ao
        x->filhos[j+1] = x->filhos[j];
    }

    x->filhos[i+1] = z;							//Filho a direita de X passa a ser o novo n�

    for (j= x->n; j >= i; j--){					//desloca as chaves de x em 1 posi��o
        x->chaves[j+1] = x->chaves[j];
    }

    x->chaves[i] = y->chaves[T];				//sobe a chave do meio
    x->n = (x->n)+1;							//corrige o n�mero de chaves em X (j� que uma chave subiu)
}


//Fun��o void inserebTree(bTree *t, int k)
//Essa fun��o � basicamente o controle da inser��o de chaves, "decidindo" se a chave ser� inserida em um n� existente
//ou se ser� alocado um novo n� para arrumar a arvore(crescimento da arvore na raiz).
//RETORNO: A fun��o n�o tem retorno
void inserebTree(bTree *t, int k){
    node *r, *s;

    r = t->raiz; 								//Definimos r como a raiz da arvore

    if (r->n == MAXP){							//Se a raiz tiver o n�mero maximo de elementos:
        s = (node*) malloc(sizeof(node));//Aloca um novo n� s
        s->eFolha = 0;							//Atribui a ele o estado e n�o folha
        s->n = 0;								//Atribui a ele o n�mero de elementos zero
        s->filhos[1] = r;						//Torna-se a raiz filha desse novo n�
        t->raiz = s;							//Torna-se esse n� a nova raiz da arvore

        divideFilhobTree(s, 1);				//Chama-se a fun��o de dividir o n� na posi��o 1
        inserebTreeNaoCheio(s, k);			//Chama-se a fun��o para inserir a chave k no novo n� que nunca estar� cheio (nessa etapa)
    }
    else										//Se a raiz n�o tiver o n�mero maximo de elementos
        inserebTreeNaoCheio(r, k);			//Chama-se a fun��o para inserir a chave k em r j� que ela n�o est� cheia
}


//Fun��o node* busca_bTree(node *x, int k, int *pos)
//Essa fun��o tem como objetivo buscar uma chave na arvore retornando o n� e a posi��o em que a chave se encontra
//RETORNO: A fun��o retorna o n� em que a chave foi encontrada(se foi encontrada) e a posi��o � passada por refer�ncia
node* busca_bTree(node *x, int k, int *pos){
    int i = 1;									//Inicializa-se uma variavel index no valor 1 (primeira posi��o de um n�)

    while(i <= x->n && k > x->chaves[i]){		//Incrementa-se o i at� a chave que ele indexe seja maior que a chave a ser inserida...
        i += 1;									//... ou at� que se chegue na �ltima posi��o do vetor
    }

    if (i <= x->n && k == x->chaves[i]){		//Se quando a condi��o acima parar o elemento for a chave busca:
        *pos = i;								//Retorna-se a posi��o i por refer�ncia
        /*printf("\n********** Elemento encontrado **********\n");
        printf("\t\tNumero: %d\n\t\tPosicao: %d\n", x->chaves[*pos], *pos);
        printf("*****************************************\n");
        */
        return x;								//Retorna-se o n�

    }
    else if (x->eFolha){						//Se n�o for a chave buscada e o n� for folha
        *pos = -1;								//Retorna-se -1 como posi��o por refer�ncia
        /*printf("\n********* Elemento nao encontrado **********\n");
        */
        return NULL;							//Retorna-se NULL no lugar do n�, j� que a chave n�o foi encontrada e n�o h� filhos para buscar
    }
    else										//Sen�o busca-se a chave no filho de posi��o i (j� que k seria menor que o elemento seguinte do n� atual)
        return busca_bTree(x->filhos[i], k, pos);//Chama-se a busca no filho de posi��o indexada i

}


//Fun��o inserebTreeNaoCheio (node *x, int k){
//insere em um n� n�o cheio
// � a �nica fun��o que de fato insere chaves na �rvore, pois a fun��o inserebTree faz todas as verifica��es e divis�es
// e ent�o chama a fun��o inserebTreeNaoCheio
void inserebTreeNaoCheio (node *x, int k){
    int i, pos;
    i = x->n;									//O index i recebe o n�mero de elementos do n�

    busca_bTree(x, k, &pos);					//Verifica se a chave a ser inserida j� n�o est� na �rvore

    if (pos == -1){								//Se pos = -1, quer dizer que a chave n�o existe, ent�o eu a insiro
        if(x->eFolha){							//Se x for folha:
            while (i >= 1 && k < x->chaves[i]){ //Enquanto i for maior ou igual a 1 E a chave a ser inserida menor que a chave na posi��o i
                x->chaves[i+1] = x->chaves[i];	//Abre espa�o pra chave ser inserida na posi��o correta
                i -=1;
            }
            x->chaves[i+1] = k;					//Coloco a chave na posi��o correta
			int pos_ins = i+1;
            x->n = (x->n)+1;					//atualiza a quantidade de chaves no n�
            printf ("Inserido %d no no: ", k);
            for (i = 0; i < x->n; i++){			//Imprime todas as chaves contidas no n� onde a chave foi inserida
                printf("%d - ", x->chaves[i+1]);
            }
            printf ("Na posicao: %d \n", pos_ins);


        }
        else {									//Se o n� x n�o for folha:
            while (i >= 1 && k < x->chaves[i]){ //Enquanto o index for maior ou igual a 1 E a chave a ser inserida menor que a chave na posi��o i
                i -=1;
            }
            i += 1;								//Ajusto o i para ter a posi��o correta
            if (x->filhos[i]->n == MAXP){		//Se o n�mero de elementos no filho de x na posi��o i for o m�ximo (2T - 1):
                divideFilhobTree(x, i);		//Divido  o n�
                if (k > x->chaves[i])			//Se a chave a ser inserida for maior que a chave de x na posi��o i
                    i+= 1;						//Incrementa-se o index para inserir no filho da direita da chave
            }
            inserebTreeNaoCheio(x->filhos[i], k);//insiro na posi��o correta
        }
    }
}


int main(int argc, char *argv[]){
    int pos;
    bTree arvore;
    criabTree(&arvore);

    inserebTree(&arvore, 1);
    inserebTree(&arvore, 7);
    inserebTree(&arvore, 9);
    inserebTree(&arvore, 24);
    inserebTree(&arvore, 40);
    inserebTree(&arvore, 41);
    inserebTree(&arvore, 45);
    inserebTree(&arvore, 60);
    inserebTree(&arvore, 75);
    inserebTree(&arvore, 90);
    inserebTree(&arvore, 50);
    inserebTree(&arvore, 65);



    return 0;
}

