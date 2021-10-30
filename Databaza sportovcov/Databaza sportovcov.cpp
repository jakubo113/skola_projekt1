// Databaza sportovcov.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <string.h>

#define FILENAME "databaza.txt"

static void help();
static void fetch();
static void add();
static void search();
static void remove();

//štruktúra
struct record {
	char firstname[10];
	char lastname[10];
	int  height;
	int  weight;
	int  age;
};

//hlavný program 
int _tmain(int argc, _TCHAR* argv[])
{
	int choice = 0;

	// hlavny cyklus (ziskavanie operacie + jej vykonanie)
	while (choice != 5) {
		// vypise sa help
		help();

		// ziskanie cisla operacie
		printf("choice> ");
		scanf_s("%d", &choice);

		if (choice < 1 || choice > 5) {
			while (getchar()!='\n'); // odstanenie moznosti potencionalneho zacyklenia
			printf("\nNespravna volba! Skus to znova!\n\n");
		}

		switch (choice) {
			case 1: fetch();
				break;
			case 2: add();
				break;
			case 3: search();
				break;
			case 4: remove();
				break;
		}
	}

	return 0;
}

static void help()
{
	printf("=======================\n");
	printf("        HOW TO\n");
	printf("=======================\n");
	printf("1 Zoznam sportovcov\n");
	printf("2 Pridat sportovca\n");
	printf("3 Vyhladat sportovca\n");
	printf("4 Zmazat sportovca\n");
	printf("5 Ukoncit program\n");
	printf("=======================\n\n");
}

// zobrazi vsetkych sportovcov
static void fetch()
{
	FILE *f;
	struct record r;
	char *format = "%s %s %d %d %d";
	int id = 1;

	fopen_s(&f, FILENAME, "r");

	if (f == 0) {
		printf("Databaza neexistuje!\n");
		return;
	}

	printf("\n");

	// postupne (po jednom riadku) si ziskam vsetky zaznami zo suboru *f (databaza.txt)
	while (fscanf_s(f, format, r.firstname, 10, r.lastname, 10, &r.age, &r.height, &r.weight) != EOF) {
		printf("ID: %d MENO: %s %s, VEK: %d, VYSKA: %d, VAHA: %d\n", id++, r.firstname, r.lastname, r.age, r.height, r.weight);
	}

	printf("\n");
	fclose(f);
}

// pridanie noveho sportovca do db
static void add()
{
	FILE *f;
	struct record r;
	char *format = "%s %s %d %d %d";

	fopen_s(&f, FILENAME, "a+");

	printf("meno> ");
	scanf_s("%s", r.firstname, 10);

	printf("priezvisko> ");
	scanf_s("%s", r.lastname, 10);

	printf("vek> ");
	scanf_s("%d", &r.age);

	printf("vyska> ");
	scanf_s("%d", &r.height);

	printf("vaha> ");
	scanf_s("%d", &r.weight);

	// na koniec suboru vlozim novy zaznam (udaje)
	fprintf_s(f, "%s %s %d %d %d\n", r.firstname, r.lastname, r.age, r.height, r.weight);
	
	fclose(f);
}

// vyhladavanie sportovca podla mena, alebo priezviska
static void search()
{
	FILE *f;
	struct record r;
	int choice = 0, finded = 0;
	char needle[10];
	char *format = "%s %s %d %d %d";
	char *haystack;

	printf("\nvyhladavat podla mena[1], priezviska[2]> ");
	scanf_s("%d", &choice);

	if (choice != 1 && choice != 2) {
		while (getchar()!='\n');	// vyriesenie problemu s potencionalnym zacyklenim
		printf("\nNeplatna operacia\n\n");
		return;
	}

	printf("hladany retazec> ");
	scanf_s("%s", needle, 10);

	fopen_s(&f, FILENAME, "r");
	printf("\n");

	// postupne prechadzam kazdy riadok (zaznam) v subore
	while (fscanf_s(f, format, r.firstname, 10, r.lastname, 10, &r.age, &r.height, &r.weight) != EOF) {
		// zistim si, ci chcem vyhladavat v menach, alebo v priezviskach
		if (choice == 1) {
			haystack = r.firstname;
		} else if (choice == 2) {
			haystack = r.lastname;
		}
		
		// zistim si, ci meno/priezvisko obsahuje hladany retazec
		if (strstr(haystack, needle) != NULL) {
			printf("MENO: %s %s, VEK: %d, VYSKA: %d, VAHA: %d\n", r.firstname, r.lastname, r.age, r.height, r.weight);
			finded = 1;
		}
	}

	// pokial som nenasiel co som hladal, tak sa vypise error
	if (finded != 1) {
		printf("ZIADNE VYSLEDKY\n");
	}

	printf("\n");
	fclose(f);
}

// vymanie sportovca podla jeho id
static void remove()
{
	FILE *f_in, *f_out;
	struct record r;
	char *format = "%s %s %d %d %d";
	int id_choice, id_item = 1, was_deleted = 0;

	printf("zadaj id sportovca> ");
	scanf_s("%d", &id_choice);
	while (getchar()!='\n');	// kvoli potencionalnemu zacykleniu

	fopen_s(&f_in, FILENAME, "r");  // otvorim si moju databazu (databaza.txt)
	fopen_s(&f_out, "tmp", "w");  // vytvorim si novy docasny subor "tmp" (potom sa zmaze)

	// prechadza sa subor databaza.txt
	while (fscanf_s(f_in, format, r.firstname, 10, r.lastname, 10, &r.age, &r.height, &r.weight) != EOF) {
		// zistim si, ci sa idm ktore chcem zmazat rovna id tohoto konkretneho zaznamu
		if (id_choice == id_item++) {
			was_deleted = 1;
			continue;
		}
		// do noveho/docasneho suboru tmp sa ulozi zaznam (ak sa jeho id NErovna id, ktore chcem zmazat!)
		fprintf_s(f_out, "%s %s %d %d %d\n", r.firstname, r.lastname, r.age, r.height, r.weight);
	}

	// zatvorim a nasledne otvorim subor databaza.txt, pretoze ju potrebujem prepisat
	fclose(f_in);
	fopen_s(&f_in, FILENAME, "w");

	// zatvorim a nasledne otvorim subor tmp
	fclose(f_out);
	fopen_s(&f_out, "tmp", "r");

	// zo suboru tmp prekopirujem vsetky zaznami do suboru databaza.txt (v tmp sa uz nenachadza zmazany zaznam)
	while (fscanf_s(f_out, format, r.firstname, 10, r.lastname, 10, &r.age, &r.height, &r.weight) != EOF) {
		fprintf_s(f_in, "%s %s %d %d %d\n", r.firstname, r.lastname, r.age, r.height, r.weight);
	}

	if (was_deleted != 1) {
		printf("Sportovec s tymto id neexistuje!\n");
	}

	fclose(f_in);
	fclose(f_out);

	// zmazem docasny subor tmp, pretoze ho uz nepotrebujem
	remove("tmp");
}