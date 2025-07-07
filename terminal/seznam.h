#ifndef SEZNAM_H
#define SEZNAM_H


typedef struct _uzel { // PRVEK, UZEL, ITEM
    char znamenko; // hodnotová položka
    int hodnota; // hodnotová položka
    struct _uzel *dalsi; // odkaz na další uzel
    struct _uzel *predchozi; // odkaz na předchozí uzel (pro případ, že by bylo potřeba)
} UZEL;

typedef struct _seznam { // INFO
    UZEL *zacatek; //ukazatel na zacatek
    UZEL *konec; // ukazatel na konec
    UZEL *akt; // ukazovatko
    int delka; // delka seznamu
} SEZNAM;


SEZNAM *init_seznam(); // vytvori seznam
void vypis_seznam(SEZNAM *s);
void zrus_seznam(SEZNAM *s);
void pridej_uzel(SEZNAM *s, UZEL *u); //prida na konec
// void zatrid_uzel(SEZNAM *s, UZEL *u); //zatridi uzel
// SEZNAM *serad_seznam(SEZNAM *s); // seradi seznam
// UZEL *vyjmi_uzel(SEZNAM *s);
// UZEL *vyhledej(SEZNAM *s, char *nazev);
UZEL *vytvor_uzel(char znamenko, int hodnota);
void vypis_uzel(UZEL *u);
void otoc_seznam(SEZNAM *s); // otočí seznam
void zrus_uzel(SEZNAM *s, UZEL *u); // uvolní uzel
#endif