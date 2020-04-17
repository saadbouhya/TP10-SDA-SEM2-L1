////////////////////////// TP10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prod.h"



void produireUnFichierTXT(char *); //prototype (voir sous le main)


/******** **********************************************************/
/*    fonction : lireNumeroProchaineCommande                       */
/*    description  : lire l'entier contenu dans le fichier 	   */
/*	numNextFact 						   */
/*    paramètres : aucun				           */
/*    retourne : un entier qui est le numero de la premiere        */
/*  	commande à traiter					   */
/*    condition : le fichier numNextFact doit exister 		   */
/*			dans le repertoire de l'exe		   */
/*    auteur : kub						   */
/*    date : 29/3/20						   */
/*******************************************************************/
int lireNumeroProchaineCommande() 
{
FILE *f;
int N;
f=fopen("numNextFact","r"); 
fread(&N,sizeof(int),1,f);
fclose(f);
//printf("\n--->lu N=%d",N);
return N;
}


/*******************************************************************/
/*    fonction : convertirNenChaine4 	                           */
/*    description  : convertir l'entier en une chaine	 	   */
/*    paramètre : N l'entier à convertir en chaine		   */
/*    paramètre : N4 la chaine contenant le resultat		   */
/*    retourne : RIEN					           */
/*    condition : 1<=N<=9999			 		   */
/*    condition : N4 doit etre une chaine de 5 char ou 		   */
/*    auteur : kub						   */
/*    date : 29/3/20						   */
/*******************************************************************/

void convertirNenChaine4(int N,char *N4) 
{ // si N=1234 alors N4 sera égal à "1234"
int m,c,d,u;
char cm,cc,cd,cu;//chiffre des milliers, chiffre des centaines ...
m=N/1000;cm=48+m;
N%=1000;
c=N/100;cc=48+c;
N%=100;
d=N/10;cd=48+d;
u=N%10;
cu=48+u;
N4[0]=cm;N4[1]=cc;N4[2]=cd;N4[3]=cu;N4[4]='\0';
}



/*******************************************************************/
/*    fonction : chercherReference	(à compléter)              */
/*    paramètre : ref : la reference du produit 		   */
/* 				 à rechercher dans produits.txt    */
/*    paramètre : lib : chaine de caractere qui contiendra	   */
/* 				 le libelle du produit	           */
/*    retourne : le prix unitaire du produit 		           */
/*    condition : on suppose que la reference existe dans le       */
/*				fichier produits.txt		   */
/*    auteur : kub						   */
/*    date : 29/3/20						   */
/*******************************************************************/
float chercherReference(int ref,char *lib)
{
//printf("%d",ref);
//printf("%s",lib);
//à coder : ouvrir produits.txt en lecture , le parcourir jusqu'à
// trouver la refence ref. Dans ce cas, on aura aussi son prix unitaire et son 
// libelle (qui seront renvoyés). On fermera alors le fichier produits.txt
FILE *fic;
T_Produit p;

fic=fopen("produits.txt","rt");
if(fic!=NULL)
{
	do
	{
		fscanf(fic,"%d %s %f",&(p.reference),p.libelle,&(p.prixU));
		if(!(feof(fic)))
		{
			if(p.reference==(unsigned)ref)
			{
				strcpy(lib,p.libelle);
				fclose(fic);
				return p.prixU;
			}
		}		
	}while(!(feof(fic)));
	
}
else 
	printf("erreur à l'ouverture du fichier, veuillez ajouter le fichier produits.txt\n"); 
return -1;
}













/*******************************************************************/
/*    fonction : lireCommande	(à compléter)                      */
/*    paramètre : pf l'adresse du fichier de commande à lire	   */
/*    retourne : RIEN					           */
/*    auteur : kub						   */
/*    date : 29/3/20						   */
/*******************************************************************/
void lireCommande(FILE * pf, FILE * facture)// le fichier pointé par pf est deja ouvert
{
char nomClient[TAILLE];
T_Produit p;
float Total=0;

p.reference=0;
p.prixU=0.0;
strcpy(p.libelle,"??");
p.quantite=0;

fscanf(pf,"%s",nomClient);//la commande contient tjrs le nom du client
// en premiere ligne
fprintf(facture,"\n-->client = %s",nomClient);
do
	{
	// à partir de la ligne2, toutes les lignes sont en 2 morceaux :
	// un int suivi d'un int
	//2 chaines sont lues et implicitement converties en int
	fscanf(pf,"%d %d",&(p.reference),&(p.quantite));
	
	
	//Partie 1 Etape 1 (voir sujet) : pour afficher la facture à l'écran
	if (!feof(pf))
		{
		fprintf(facture,"\n-->ref = %d  qtt=%d",p.reference,p.quantite);
		// à écrire :
		p.prixU= chercherReference(p.reference,p.libelle); 
		fprintf(facture,"\n\t-->lib = %s  prixUnit=%f",p.libelle,p.prixU);
		Total += (p.quantite) * (p.prixU);
		
		//calculez aussi puis afficher le total de la facture
		}



	}while(!feof(pf));

	fprintf(facture, "\n\t TOTAL=%f€\n", Total);

}

void lireCommande2(FILE * pf,FILE *facture)// le fichier pointé par pf est deja ouvert
{
char nomClient[TAILLE];
T_Produit p;
T_Produit p2;
FILE *fic=NULL;
FILE *fic2=NULL;
FILE *ficAlerte=NULL;
float Total=0;
int quantite=0;
p.reference=0;
p.prixU=0.0;
strcpy(p.libelle,"??");
p.quantite=0;
fic=fopen("stock.txt","rt");
fic2=fopen("stockbis.txt","wt");
fscanf(pf,"%s",nomClient);//la commande contient tjrs le nom du client
fprintf(facture,"%s\n",nomClient);
do
{
	fscanf(pf,"%d %d",&(p.reference),&(p.quantite));
	if (!feof(pf))
	{
		if(chercherReference(p.reference,p.libelle)!=-1)		
		{	
			do
			{					
				fscanf(fic,"%d %d",&(p2.reference),&(p2.quantite)); 
				if(!feof(fic))
				{				
					if(p2.reference==p.reference)
					{
						quantite=p2.quantite-p.quantite;	
						if(quantite<0)
						{
							fprintf(fic2,"%d %d \n",p2.reference,0);
							p.quantite=p2.quantite;
						}
						else
						fprintf(fic2,"%d %d \n",p2.reference,quantite);
					}			
					else
						fprintf(fic2,"%d %d \n",p2.reference,p2.quantite);
				}
			}while(!feof(fic));
			rewind(fic);
			rewind(fic2);
			system("cp stockbis.txt stock.txt");
			p.prixU= chercherReference(p.reference,p.libelle);
			fprintf(facture,"\n%d %s (PU=%f€) ::%f€",p.quantite,p.libelle,p.prixU,p.prixU*p.quantite); 
			Total+=p.prixU*p.quantite;
		//calculez aussi puis afficher le total de la facture
		}else
		{
				fprintf(facture,"\n \n ---> %d ---> on s'excuse monsieur, on n'a pas cette reference parmi nos produits !\n",p.reference);
		}
	}	
	}while(!feof(pf));
	fclose(fic);
	fclose(fic2);
	fprintf(facture,"\n\n\t Prix Totale de La commande : %f\n",Total);
	fic=fopen("stock.txt","rt");
	ficAlerte=fopen("Alerte.txt","wt");
	do
	{
		fscanf(fic,"%d %d",&(p.reference),&(p.quantite));
		if(!feof(fic))
		{
			if(p.quantite <= 0)
			fprintf(ficAlerte,"%d \n",p.reference);	
		}
	}while(!feof(fic));
	fclose(fic);
	fclose(ficAlerte);
}






/*******************************************************************/
/*    fonction : lireLesCommandes 	                           */
/*    description  : ouvrir 1 par 1 tous les fichiers de commande  */
/*    description  : (commandes pas encore traitées)	 	   */
/*    description  : appel à la fonction lireCommande pour chacune */
/* 			 des commandes			 	   */
/*    description  : mettre à jour le fichier numNextFact 	   */
/*    paramètre : aucun						   */
/*    retourne : RIEN					           */
/*    condition : 1<=N<=9999			 		   */
/*    condition : N4 doit etre une chaine de 5 char ou 		   */
/*    auteur : kub						   */
/*    date : 29/3/20						   */
/*******************************************************************/

void lireLesCommandes() //cette fonction ouvre tous les fichiers commandeXXXX.txt avec XXXX démarrant à N lu dans numNextFact
{
FILE *ficCommande=NULL;
FILE *ficFacture=NULL;
int FINI=0;
int N = lireNumeroProchaineCommande(); //numero de la premiere commande qui sera lue et traitee lors de cette execution
char NNNN[5];
char nomCommande[29];
char nomFacture[29];

do //ce do while prend fin dès que fichier commandeXXXX.txt est absent 
	{
	strcpy(nomCommande,"./commandes/commande");//pour lire ds le bon repertoire
	strcpy(nomFacture, "./factures/facture");
	convertirNenChaine4(N,NNNN);//convertion d'un int en chaine de 4 
	//printf("\n==>%s<==",NNNN);
	ficCommande=NULL;
	ficFacture=NULL;
	strcat(nomCommande,NNNN);
	strcat(nomCommande,".txt");//le nom du fichier est formé
	strcat(nomFacture, NNNN);
	strcat(nomFacture, ".txt");
	
	//printf("\n traitement de  %s",nomCommande);
	
	ficCommande=fopen(nomCommande,"rt");//mode rt = "read" et "text"
	ficFacture=fopen(nomFacture,"wt");
	if (ficCommande!=NULL)
		{
			if(ficFacture!=NULL) {
			 // le fichier commandeNNNN.txt existe
			printf("\n fichier %s present",nomCommande);
			lireCommande(ficCommande,ficFacture); // à vous de coder cette fonction lors de ce TP10
			fclose(ficCommande);
			fclose(ficFacture);
			}
		}
	else
		{
			
			printf("\n toutes les commandes presentes ont ete traitees.");
			printf("\n\n Les factures sont disponibles dans le dossier factures");
			FILE *f=fopen("numNextFact","w"); // on va ecrire la valeur de N dans numNextFact 
			fwrite(&N,1,sizeof(int),f);
			fclose(f);
			
			FINI=1;
						
		}

	N++;
	}while(FINI==0);		

}


/**********************************************************/

void lireLesCommandes2() //cette fonction ouvre tous les fichiers commandeXXXX.txt avec XXXX démarrant à N lu dans numNextFact
{
FILE *ficCommande=NULL;
FILE *ficFacture=NULL;
int FINI=0;
int N = lireNumeroProchaineCommande(); //numero de la premiere commande qui sera lue et traitee lors de cette execution
char NNNN[5];
char nomCommande[29];
char nomFacture[29];

do //ce do while prend fin dès que fichier commandeXXXX.txt est absent 
	{
	strcpy(nomCommande,"./commandes/commande");//pour lire ds le bon repertoire
	strcpy(nomFacture, "./factures/facture");
	convertirNenChaine4(N,NNNN);//convertion d'un int en chaine de 4 
	//printf("\n==>%s<==",NNNN);
	ficCommande=NULL;
	ficFacture=NULL;
	strcat(nomCommande,NNNN);
	strcat(nomCommande,".txt");//le nom du fichier est formé
	strcat(nomFacture, NNNN);
	strcat(nomFacture, ".txt");
	
	//printf("\n traitement de  %s",nomCommande);
	
	ficCommande=fopen(nomCommande,"rt");//mode rt = "read" et "text"
	ficFacture=fopen(nomFacture,"wt");
	if (ficCommande!=NULL)
		{
			if(ficFacture!=NULL) {
			 // le fichier commandeNNNN.txt existe
			printf("\n fichier %s present",nomCommande);
			lireCommande2(ficCommande,ficFacture); // à vous de coder cette fonction lors de ce TP10
			fclose(ficCommande);
			fclose(ficFacture);
			}
		}
	else
		{
			
			printf("\n toutes les commandes presentes ont ete traitees.");
			printf("\n\n Les factures sont disponibles dans le dossier factures");
			FILE *f=fopen("numNextFact","w"); // on va ecrire la valeur de N dans numNextFact 
			fwrite(&N,1,sizeof(int),f);
			fclose(f);
			
			FINI=1;
						
		}

	N++;
	}while(FINI==0);		

}


/**************************************************************************/
/*        TP10         */
/**************************************************************************/
int main()
{
	int choix;
//ci dessous :
// creation d un fichier d'un seul int nommé umNnextFact et contenant l'int 1
// code à utiliser pour réinitialiser numNextFact à 1 si besoin au cours du TP 
	
	FILE *f;int N=1;
	f=fopen("numNextFact","w");
	fwrite(&N,1,sizeof(int),f);
	fclose(f);
	
printf("1. Produire les factures sans modification de stock.\n");
printf("2. Produire les factures avec modification de stock.\n");
scanf("%d", &choix);
switch (choix)
{
case 1:
	lireLesCommandes();
	break;
case 2:
	lireLesCommandes2();
default: 
	break;
}



	//PARTIE 1 du TP : sans Gestion de stock
	//lireLesCommandes();
	//lecture de tous les fichiers commandeXXXX.txt (cette fonction doit 
	//produire les factures des commandes non traitées jusqu'à maintenant)	
	

	//PARTIE 2 du TP : même sujet que PARTIE 1 MAIS avec Gestion de stock
	//copiez coller votre travail précédent puis modifiez le  
	//lireLesCommandes2(); 
	// le fichier stock.txt devra évoluer au fur et à mesure des commandes 
	//traitees (voir explications dans le sujet)
	


	//ci dessous : code illustrant comment créer et écrire dans un fichier 
	//txt
	//enlever le commentaire si vous voulez tester
	//produireUnFichierTXT("monfic.txt");
	//cette fonction sera utilisee à partir de la PARTIE1 ETAPE 2 (quand vos 
	//factures seront bien formées et affichées à l'écran (partie 1 etape 1) 
	//il suffira de modifier/remplacer ces affichages à l'écran par des 
	//ecritures dans le fichier facture correspondant à la commande

	return 0;
}


void produireUnFichierTXT(char *nomFichier)
{
int qtt=10;
float prixU=12.30;
float total=prixU*qtt;
char libelle[50],nom[50];
FILE *fic=NULL;

strcpy(libelle,"ardoise_20*30");
strcpy(nom,"Client : TOTO");

fic=fopen(nomFichier,"wt");
if (fic!=NULL) //le fichier est bien ouvert
	{
	fputs(nom,fic);
	fprintf(fic,"\n%d %s (PU=%f€) ::%f€",qtt,libelle,prixU,total);//ici, pas de & sur les parametres de type int et float (cf ma remarque dans la video2)
	fclose(fic);

	}
else printf("\n impossible de créer le fichier !! ");

}
