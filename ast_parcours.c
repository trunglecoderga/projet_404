
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "type_ast.h"
#include "table_symbole.h"
#include "ast_parcours.h"


 //extern void insererTS(char *idf, int v,Couple TS[], int *NbSymb) ;

void aff_operateur(TypeOperateur op){
	switch (op) {
		case N_PLUS:
			printf("+") ;
			break;
		case N_MOINS:
		case U_MOINS:
			printf("-") ;
			break;
		case N_MUL:
			printf("*") ;
			break;
		case N_DIV:
			printf("/");
			break;
		
	} 
}

void afficher_ast(Ast expr) {
	if(expr == NULL){
		printf("fin");
		return;
	}
	//printf("idf:%s\n",expr->ident);
	//printf("arbre nature: %d\n",expr->nature);
	switch (expr->nature) {
		case OPERATION:
			printf("(");
			afficher_ast(expr->gauche);
			aff_operateur(expr->operateur) ;
			afficher_ast(expr->droite);
			printf(")");
			break ;
		case VALEUR:
			printf("%d", expr->valeur);
			break ;
		case N_IDF :
			printf("%s", expr->ident);
			break;
		case N_AFF:
			//afficher_ast(expr)
			afficher_ast(expr->gauche);
			printf(":=");
			afficher_ast(expr->droite);
			break;
		case N_SEPINST:
			afficher_ast(expr->gauche);
			printf(";\n");
			afficher_ast(expr->droite);					
			break;
		case N_LIRE:
			printf("lire(%s)",expr->gauche->ident);
			//afficher_ast(expr->gauche);
			break;
		case N_ECRIRE:
			printf("ecrire");
			afficher_ast(expr->gauche);
			break;
	}
}

int evaluation(Ast expr) {
      
	int Vg, Vd;
	switch (expr->nature)
	{
	case VALEUR:
		return expr->valeur;
		break;
	case OPERATION:
		Vg = evaluation(expr->gauche);
		Vd = evaluation(expr->droite);
		switch (expr->operateur)
		{
		case N_PLUS:
			return Vg + Vd;
			break;
		case N_MOINS:
			return Vg - Vd;
			break;
		case N_MUL:
			return Vg*Vd;
			break;
		case N_DIV:
			return Vg/Vd;
			break;
		default:
			break;
		}
	default:
		break;
	}
	return -1 ;
}

void interpreter(Ast A){
	switch(A->nature){
		case N_SEPINST:
			interpreter_aff(A->gauche);
			interpreter_aff(A->droite);
			break;
		case N_AFF :
			interpreter_aff(A);
			break;
		case N_LIRE:
			interpreter_lire(A);
			break;
		case N_ECRIRE:
			interpreter_ecrire(A);
			break;
		case N_IF:
			interpreter_si_alors_sinon(A);
			break;
		default:
			printf("erreur zyegauzeg");
			break;
	}
}


void interpreter_aff(Ast A){
	char idf[20];
	int v;
	strcpy(idf,A->gauche->ident);
	if(strlen(idf)==0){
		return;
	}
	v = evaluation(A->droite);
	insererTS(idf,v,TS,&NbSymb);
}



void interpreter_lire(Ast A){
    int v;
    printf("lecture entier: \n");
    scanf("%d",&v);
    insererTS(A->gauche->ident,v,TS,&NbSymb);
    //afficheTS(TS,NbSymb);
}

void interpreter_ecrire(Ast A){
    int v;
    v = evaluation(A->gauche);
    printf("ecriture : %d\n",v);
}

void interpreter_si_alors_sinon(Ast A) {
	//printf("interpretation:\n");
	//printf("valeur = %d\n",A->gauche->nature);
	int condition = valeur_booleenne(A->gauche);
	if (condition==1) {
		interpreter(A->milieu->gauche);	
		return;
	} else if (condition == 0) {
		interpreter(A->droite->gauche);
	}

}

int valeur_booleenne(Ast A){
	int valeurg, valeurd;
	valeurg = evaluation(A->gauche);
	//printf("valg = %d\n",valeurg);
	valeurd = evaluation(A->droite);
	//printf("vald = %d\n",valeurd);
	//printf("nature comparaison:%d\n",A->nature);
	switch (A->cond) {
		case N_SUP:
			return  valeurg > valeurd;
			break;
		case N_SUPEGAL:
			return valeurg >= valeurd;
			break;
		case N_INF:
			return valeurg < valeurd;
			break;
		case N_INFEGAL:
			return valeurg <= valeurd;
			break;
		case N_EGAL:
			return valeurg == valeurd;
			break;
		case N_DIFF:
			return valeurg != valeurd;
			break;
		default:
			printf("erreur concernant évaluation de la condition\n");
			exit(1);
	}
}