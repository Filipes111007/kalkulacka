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

UZEL *vytvor_uzel(char znamenko, double hodnota) { // vytvori uzel
    UZEL *u;
    u = (UZEL *) malloc(sizeof(UZEL)); // alokace paměti
    if (u != NULL) {
        u->znamenko = znamenko; // hodnoty struktury
        u->hodnota = hodnota; // mohou být jiné, dle deklarace
        u->dalsi = NULL;
        u->predchozi = NULL;
        DEBUG_PRINT("Vytvořen uzel: znamenko='%c', hodnota=%g\n", znamenko, hodnota);
    } else {
        DEBUG_PRINT("CHYBA: Nepodařilo se alokovat paměť pro uzel!\n");
    }
    return u; 
}

void vypis_uzel(UZEL *u){
    if(u != NULL){
        if (u->znamenko == 'g'){
            printf("%g", u->hodnota);
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

UZEL *vypocitej_vysledek(SEZNAM *s) { //rekurzivní funkce pro výpočet výsledku
    int zavorek = 0; // proměnná pro sledování závorek
    s->akt = s->zacatek; // nastavíme ukazatel na začátek seznamu
    if (s->delka == 1) {
        return s->zacatek; // pokud je v seznamu jen jeden uzel, vrátíme jeho hodnotu
    }
    while (s->akt != NULL) {
        if (s->akt->znamenko == '('){
            zavorek++; // zvýšíme počet závorek
        } else if (s->akt->znamenko == ')') {
            zavorek--; // snížíme počet závorek
            if (zavorek < 0) { // pokud je počet závorek záporný, je chyba v závorkách
                fprintf(stderr, "Chyba: Neplatné závorky v příkladu.\n");
                return NULL; // vrátíme NULL pro chybu
            }
            if(zavorek == 0){
            SEZNAM *podseznam = init_seznam(); // vytvoříme nový seznam pro podvýraz
            UZEL *poduzel = s->zacatek; // začneme od začátku seznamu
            while (poduzel->znamenko != '(' && poduzel != s->akt) { // dokud nenarazíme na začínající závorku
                poduzel = poduzel->dalsi; // posuneme se na další uzel
            }
            UZEL *zacatek_zavorek = poduzel; // uložíme si začátek závorek
            poduzel = poduzel->dalsi; // posuneme se na další uzel, který by měl být uvnitř závorek
            while(poduzel != s->akt){
                UZEL *novy_uzel = vytvor_uzel(poduzel->znamenko, poduzel->hodnota); // vytvoříme nový uzel
                if (novy_uzel != NULL) {
                    pridej_uzel(podseznam, novy_uzel); // přidáme uzel do podseznamu
                } else {
                    fprintf(stderr, "Chyba při vytváření uzlu.\n");
                    zrus_seznam(podseznam);
                    return NULL;
                }
                poduzel = poduzel->dalsi; // posuneme se na další uzel
            }
            DEBUG_PRINT("Zpracovávám podseznam uvnitř závorek:\n");
            UZEL *vysledek = vypocitej_vysledek(podseznam); // rekurzivně zpracujeme podseznam
            if (vysledek == NULL) {
                fprintf(stderr, "Chyba při výpočtu podvýrazu.\n");
                zrus_seznam(podseznam);
                return NULL;
            }            
            // Uložíme si hodnotu výsledku
            double hodnota_vysledku = vysledek->hodnota;
            zrus_seznam(podseznam);

            zacatek_zavorek->znamenko = 'g';
            zacatek_zavorek->hodnota = hodnota_vysledku;

            // Bezpečné mazání
            UZEL *konec = s->akt;
            UZEL *next_after_konec = konec->dalsi;
            UZEL *mazany = zacatek_zavorek->dalsi;
            while (mazany != NULL && mazany != next_after_konec) {
                UZEL *dalsi = mazany->dalsi;
                zrus_uzel(s, mazany);
                mazany = dalsi;
            }
            zacatek_zavorek->dalsi = next_after_konec;
            if (next_after_konec) next_after_konec->predchozi = zacatek_zavorek;

            DEBUG_PRINT("Výsledek podvýrazu: %g\n", zacatek_zavorek->hodnota);

            // Resetujeme a začneme znovu od začátku
            s->akt = s->zacatek;
            zavorek = 0;
            continue;
            }
        }
        
        s->akt = s->akt->dalsi; // posuneme se na další uzel
    }
    s->akt = s->zacatek; // nastavíme ukazatel na začátek seznamu
    while (s->akt != NULL) {
        if (s->akt->znamenko == '*'){
            s->akt->hodnota = s->akt->predchozi->hodnota * s->akt->dalsi->hodnota;
            s->akt->znamenko = 'g'; // změníme znamenko na 'g' pro číslo
            // odstraníme uzel předchozí a následující
            DEBUG_PRINT("Vypočítáno: %g * %g = %g\n", 
                            s->akt->predchozi->hodnota, 
                            s->akt->dalsi->hodnota, 
                            s->akt->hodnota);
            zrus_uzel(s, s->akt->predchozi);
            zrus_uzel(s, s->akt->dalsi);
                
        }
        if (s->akt->znamenko == '/'){
            s->akt->hodnota = s->akt->predchozi->hodnota / s->akt->dalsi->hodnota;
            s->akt->znamenko = 'g'; // změníme znamenko na 'g' pro číslo
            // odstraníme uzel předchozí a následující
            DEBUG_PRINT("Vypočítáno: %g / %g = %g\n", 
                            s->akt->predchozi->hodnota, 
                            s->akt->dalsi->hodnota, 
                            s->akt->hodnota);
            zrus_uzel(s, s->akt->predchozi);
            zrus_uzel(s, s->akt->dalsi);
        }
        s->akt = s->akt->dalsi; // posuneme se na další uzel
    }
    s->akt = s->zacatek; // nastavíme ukazatel na začátek seznamu
    while (s->akt != NULL) {
        if (s->akt->znamenko == '+'){
            s->akt->hodnota = s->akt->predchozi->hodnota + s->akt->dalsi->hodnota;
            s->akt->znamenko = 'g'; // změníme znamenko na 'g' pro číslo
            // odstraníme uzel předchozí a následující
            DEBUG_PRINT("Vypočítáno: %g + %g = %g\n", 
                            s->akt->predchozi->hodnota, 
                            s->akt->dalsi->hodnota, 
                            s->akt->hodnota);
            zrus_uzel(s, s->akt->predchozi);
            zrus_uzel(s, s->akt->dalsi);
        }
        if (s->akt->znamenko == '-'){
            s->akt->hodnota = s->akt->predchozi->hodnota - s->akt->dalsi->hodnota;
            s->akt->znamenko = 'g'; // změníme znamenko na 'g' pro číslo
            // odstraníme uzel předchozí a následující
            DEBUG_PRINT("Vypočítáno: %g - %g = %g\n", 
                            s->akt->predchozi->hodnota, 
                            s->akt->dalsi->hodnota, 
                            s->akt->hodnota);
            zrus_uzel(s, s->akt->predchozi);
            zrus_uzel(s, s->akt->dalsi);
        }
        s->akt = s->akt->dalsi; // posuneme se na další uzel
    }
    return s->zacatek;
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

    double cislo = 0;
    double pocetCisel = 1;
    int je_desetinne = 0; // proměnná pro kontrolu desetinné čárky
    int delka_prikladu = 0; // proměnná pro délku příkladu

    for(i = delka; i > 0; i--){
        if(priklad[i-1] != ' ' && priklad[i-1] != '+' && priklad[i-1] != '-' && priklad[i-1] != '*' && priklad[i-1] != '/' && priklad[i-1] != '(' && priklad[i-1] != ')'){
            if(priklad[i-1] == '.' || priklad[i-1] == ','){
                if (je_desetinne) {
                    fprintf(stderr, "Chyba: Více než jedna desetinná čárka v čísle.\n");
                    zrus_seznam(seznam);
                    return 1; // vrátíme chybu
                }
                je_desetinne = 1; // nastavíme, že jsme narazili na desetinnou čárku
                cislo /= pocetCisel; // pokud je tečka, posuneme desetinnou čárku
                pocetCisel = 1;
            } else{
                cislo += pocetCisel * (priklad[i-1] - '0');
                pocetCisel *= 10;
            }
        } else{
            je_desetinne = 0; // resetujeme desetinnou čárku pro další číslo
            // Vytvoříme uzel s číslem pouze pokud bylo skutečně načteno nějaké číslo
            if (pocetCisel > 1) { // pocetCisel > 1 znamená, že bylo načteno alespoň jedno číslo
                uzel = vytvor_uzel('g', cislo);
                if (uzel != NULL) {
                    pridej_uzel(seznam, uzel);
                    delka_prikladu += 1;
                } else {
                    fprintf(stderr, "Chyba při vytváření uzlu.\n");
                    zrus_seznam(seznam);
                    return 1;
                }
            }
            
            // Vytvoříme uzel s operátorem/závorkou pouze pokud to není mezera
            if (priklad[i-1] != ' ') {
                uzel = vytvor_uzel(priklad[i-1], 0);
                if (uzel != NULL) {
                    pridej_uzel(seznam, uzel);
                    delka_prikladu += 1;
                } else {
                    fprintf(stderr, "Chyba při vytváření uzlu.\n");
                    zrus_seznam(seznam);
                    return 1;
                }
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
    //kontrola počtu závorek
    int zavorky = 0;
    while(seznam->akt != NULL) {
        if (seznam->akt->znamenko == '(') {
            zavorky++;
        } else if (seznam->akt->znamenko == ')') {
            zavorky--;
        }
        seznam->akt = seznam->akt->dalsi; // posuneme se na další uzel
    }
    if (zavorky != 0) {
        fprintf(stderr, "Chyba: Neplatný počet závorek v příkladu.\n");
        zrus_seznam(seznam);
        return 1; // vrátíme chybu
    }
    seznam->zacatek = vypocitej_vysledek(seznam); // vypočítáme výsledek
    if (seznam->zacatek == NULL) {
        fprintf(stderr, "Chyba při výpočtu výsledku.\n");
        zrus_seznam(seznam);
        return 1;
    }

    printf("Výsledek: ");
    printf("%g", seznam->zacatek->hodnota); // vypíšeme výsledek
    printf("\n");
    zrus_seznam(seznam); // uvolnění paměti
    DEBUG_PRINT("Seznam byl úspěšně uvolněn.\n");
    return 0;
}