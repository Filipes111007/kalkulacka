#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seznam.h"

#define BUFFER 256

// Debug makro pro snadné debug výpisy
#ifdef DEBUG
    #define DEBUG_PRINT(fmt, ...) printf("[DEBUG] " fmt, ##__VA_ARGS__)
#else
    #define DEBUG_PRINT(fmt, ...) // Prázdné makro pro release verzi
#endif

SEZNAM *init_seznam(){  // vytvori seznam
    SEZNAM *s;
    s = (SEZNAM *)malloc(sizeof(SEZNAM));
    if (s == NULL) { // kontrola úspěšnosti alokace
        fprintf(stderr, "Chyba\n");
        exit(EXIT_FAILURE);
    }
    s->zacatek = NULL;
    s->konec = NULL;
    s->akt = NULL;
    s->delka = 0;
    return s;
} 

void vypis_seznam(SEZNAM *s){
    s->akt = s->zacatek;
    while(s->akt != NULL){
        vypis_uzel(s->akt);
        s->akt = s->akt->dalsi;
    } 
}

void zrus_seznam(SEZNAM *s){
    UZEL *uzel;
    s->akt = s->zacatek;
    while(s->akt != NULL){
        uzel = s->akt->dalsi;
        free((UZEL *)s->akt);
        s->akt = uzel;
        s->zacatek = uzel;
    }
    free((SEZNAM *)s);
    s = NULL;
}

void pridej_uzel(SEZNAM *s, UZEL *u){ //prida uzel na konec
    DEBUG_PRINT("Přidávám uzel do seznamu (délka před přidáním: %d)\n", s->delka);
    if (s->delka == 0) {
        s->zacatek = u;
        s->konec = u;
        s->akt = u;
        s->delka = 1;
        u->dalsi = NULL;
        u->predchozi = NULL;
        DEBUG_PRINT("První uzel přidán do seznamu\n");
    } else {
        u->predchozi = s->konec;
        s->konec->dalsi = u;
        s->konec = u;
        s->delka++;
        u->dalsi = NULL;
        DEBUG_PRINT("Uzel přidán na konec seznamu (nová délka: %d)\n", s->delka);
    }
}

UZEL *vytvor_uzel(char znamenko, int hodnota) { // vytvori uzel
    UZEL *u;
    u = (UZEL *) malloc(sizeof(UZEL)); // alokace paměti
    if (u != NULL) {
        u->znamenko = znamenko; // hodnoty struktury
        u->hodnota = hodnota; // mohou být jiné, dle deklarace
        u->dalsi = NULL;
        u->predchozi = NULL;
        DEBUG_PRINT("Vytvořen uzel: znamenko='%c', hodnota=%d\n", znamenko, hodnota);
    } else {
        DEBUG_PRINT("CHYBA: Nepodařilo se alokovat paměť pro uzel!\n");
    }
    return u; 
}

void vypis_uzel(UZEL *u){
    if(u != NULL){
        if (u->znamenko == 'g'){
            printf("%d", u->hodnota);
        }
        else {
            printf("%c", u->znamenko);
        }
    }
}

void otoc_seznam(SEZNAM *s) { // otočí seznam
    UZEL *predchozi = NULL;
    UZEL *aktualni = s->zacatek;
    UZEL *nove_konec = NULL;

    while (aktualni != NULL) {
        UZEL *dalsi = aktualni->dalsi; // uložíme si další uzel
        
        // Prohodíme ukazatele dalsi a predchozi
        aktualni->dalsi = aktualni->predchozi;
        aktualni->predchozi = dalsi;
        
        predchozi = aktualni; // posuneme se o krok dál
        aktualni = dalsi; // posuneme se o krok dál
    }

    nove_konec = s->zacatek; // původní začátek se stane koncem
    s->zacatek = predchozi; // nový začátek je poslední zpracovaný uzel
    s->konec = nove_konec; // aktualizujeme konec seznamu
}

void zrus_uzel(SEZNAM *s, UZEL *u) { // uvolní uzel
    if (u == NULL) {
        return; // nic neděláme, pokud je uzel NULL
    }
    
    if (u->predchozi != NULL) {
        u->predchozi->dalsi = u->dalsi; // propojení předchozího uzlu s dalším
    } else {
        s->zacatek = u->dalsi; // pokud je to první uzel, posuneme začátek
    }

    if (u->dalsi != NULL) {
        u->dalsi->predchozi = u->predchozi; // propojení dalšího uzlu s předchozím
    } else {
        s->konec = u->predchozi; // pokud je to poslední uzel, posuneme konec
    }

    free(u); // uvolnění paměti uzlu
    s->delka--; // snížení délky seznamu
}


int main(){
    char priklad[BUFFER];
    int i = 0;
    char znak;
    int delka;
    SEZNAM *seznam = init_seznam(); // inicializace seznamu
    UZEL *uzel;
    printf("Zadejte příklad:\n");
    while (i < BUFFER - 1 && (znak = getchar()) != '\n' && znak != EOF) {
        priklad[i++] = znak;
    }
    delka = i;
    priklad[i] = '\0'; // Null-terminate the string
    #ifdef DEBUG
    printf("Zadali jste: ");
    for (int j = 0; j < i; j++) {
        printf("%c", priklad[j]);
    }
    printf("\n");
    printf("Délka zadaného příkladu: %d znaků\n", delka);
    #endif
    DEBUG_PRINT("Začínám parsování výrazu: '%s'\n", priklad);

    int cislo = 0;
    int pocetCisel = 1;
    int delka_prikladu = 0; // proměnná pro délku příkladu

    for(i = delka; i > 0; i--){
        if(priklad[i-1] != ' ' && priklad[i-1] != '+' && priklad[i-1] != '-' && priklad[i-1] != '*' && priklad[i-1] != '/'){
            cislo += pocetCisel * (priklad[i-1] - '0');
            pocetCisel *= 10;
        } else{
            uzel = vytvor_uzel('g', cislo);
            if (uzel != NULL) {
                pridej_uzel(seznam, uzel);
                delka_prikladu += 1; // zvýšení délky příkladu
            } else {
                fprintf(stderr, "Chyba při vytváření uzlu.\n");
                zrus_seznam(seznam);
                return 1;
            }
            uzel = vytvor_uzel(priklad[i-1], 0);
            if (uzel != NULL) {
                pridej_uzel(seznam, uzel);
                delka_prikladu += 1; // zvýšení délky příkladu
            } else {
                fprintf(stderr, "Chyba při vytváření uzlu.\n");
                zrus_seznam(seznam);
                return 1;
            }
            cislo = 0;
            pocetCisel = 1;
        }
    }
    
    // Zpracování posledního čísla (které nebylo zpracováno v cyklu)
    if (cislo > 0 || pocetCisel > 1) {
        uzel = vytvor_uzel('g', cislo);
        if (uzel != NULL) {
            pridej_uzel(seznam, uzel);
            delka_prikladu += 1; // zvýšení délky příkladu
        } else {
            fprintf(stderr, "Chyba při vytváření uzlu.\n");
            zrus_seznam(seznam);
            return 1;
        }
    }
    DEBUG_PRINT("Délka zpracovaného příkladu: %d\n", delka_prikladu);
    otoc_seznam(seznam); // otočení seznamu
    DEBUG_PRINT("Seznam po otočení:\n");
    #ifdef DEBUG
    vypis_seznam(seznam);
    printf("\n");
    #endif
    seznam->akt = seznam->zacatek; // nastavíme ukazatel na začátek seznamu
    while (seznam->akt != NULL) {
        if (seznam->akt->znamenko == '*'){
            seznam->akt->hodnota = seznam->akt->predchozi->hodnota * seznam->akt->dalsi->hodnota;
            seznam->akt->znamenko = 'g'; // změníme znamenko na 'g' pro číslo
            // odstraníme uzel předchozí a následující
            zrus_uzel(seznam, seznam->akt->predchozi);
            zrus_uzel(seznam, seznam->akt->dalsi);
            if (uzel != NULL) {
                DEBUG_PRINT("Vypočítáno: %d * %d = %d\n", 
                            seznam->akt->predchozi->hodnota, 
                            seznam->akt->dalsi->hodnota, 
                            seznam->akt->hodnota);
            } else {
                fprintf(stderr, "Chyba při vytváření uzlu.\n");
                zrus_seznam(seznam);
                return 1;
            }
        }
        if (seznam->akt->znamenko == '/'){
            seznam->akt->hodnota = seznam->akt->predchozi->hodnota / seznam->akt->dalsi->hodnota;
            seznam->akt->znamenko = 'g'; // změníme znamenko na 'g' pro číslo
            // odstraníme uzel předchozí a následující
            zrus_uzel(seznam, seznam->akt->predchozi);
            zrus_uzel(seznam, seznam->akt->dalsi);
            if (uzel != NULL) {
                DEBUG_PRINT("Vypočítáno: %d / %d = %d\n", 
                            seznam->akt->predchozi->hodnota, 
                            seznam->akt->dalsi->hodnota, 
                            seznam->akt->hodnota);
            } else {
                fprintf(stderr, "Chyba při vytváření uzlu.\n");
                zrus_seznam(seznam);
                return 1;
            }
        }
        seznam->akt = seznam->akt->dalsi; // posuneme se na další uzel
    }
    seznam->akt = seznam->zacatek; // nastavíme ukazatel na začátek seznamu
    while (seznam->akt != NULL) {
        if (seznam->akt->znamenko == '+'){
            seznam->akt->hodnota = seznam->akt->predchozi->hodnota + seznam->akt->dalsi->hodnota;
            seznam->akt->znamenko = 'g'; // změníme znamenko na 'g' pro číslo
            // odstraníme uzel předchozí a následující
            zrus_uzel(seznam, seznam->akt->predchozi);
            zrus_uzel(seznam, seznam->akt->dalsi);
            if (uzel != NULL) {
                DEBUG_PRINT("Vypočítáno: %d + %d = %d\n", 
                            seznam->akt->predchozi->hodnota, 
                            seznam->akt->dalsi->hodnota, 
                            seznam->akt->hodnota);
            } else {
                fprintf(stderr, "Chyba při vytváření uzlu.\n");
                zrus_seznam(seznam);
                return 1;
            }
        }
        if (seznam->akt->znamenko == '-'){
            seznam->akt->hodnota = seznam->akt->predchozi->hodnota - seznam->akt->dalsi->hodnota;
            seznam->akt->znamenko = 'g'; // změníme znamenko na 'g' pro číslo
            // odstraníme uzel předchozí a následující
            zrus_uzel(seznam, seznam->akt->predchozi);
            zrus_uzel(seznam, seznam->akt->dalsi);
            if (uzel != NULL) {
                DEBUG_PRINT("Vypočítáno: %d - %d = %d\n", 
                            seznam->akt->predchozi->hodnota, 
                            seznam->akt->dalsi->hodnota, 
                            seznam->akt->hodnota);
            } else {
                fprintf(stderr, "Chyba při vytváření uzlu.\n");
                zrus_seznam(seznam);
                return 1;
            }
        }
        seznam->akt = seznam->akt->dalsi; // posuneme se na další uzel
    }
    seznam->akt = seznam->zacatek; // nastavíme ukazatel na začátek seznamu
    printf("Výsledek: ");
    printf("%d", seznam->akt->hodnota); // vypíšeme výsledek
    printf("\n");
    zrus_seznam(seznam); // uvolnění paměti
    DEBUG_PRINT("Seznam byl úspěšně uvolněn.\n");
    return 0;
}