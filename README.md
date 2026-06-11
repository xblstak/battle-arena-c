# Battle Arena – Simulácia súboja armád v C

Konzolová aplikácia v jazyku C simulujúca súboj dvoch armád na bojovom poli. Projekt pozostáva z dvoch nadväzujúcich zadaní:

- **Zadanie 4 (Battle Arena 1.0)** – konfigurácia armád zo štandardného vstupu, simulácia súboja
- **Zadanie 5 (Battle Arena 2.0)** – rozšírenie o načítavanie predmetov z JSON súboru a armád z textových súborov

## Čo projekt robí

**Zadanie 4**
- Používateľ nakonfiguruje dve armády (1–5 jednotiek každá) zo štandardného vstupu
- Každá jednotka má meno, HP a 1–2 predmety z databázy (útok, obrana, dosah, polomer)
- Súboj prebieha po kolách — jednotky sa navzájom zasahujú podľa svojich predmetov
- Podpora útoku s polomerom (poškodenie viacerých nepriateľov naraz)
- Výsledok: víťazstvo armády 1, armády 2, alebo remíza

**Zadanie 5**
- Predmety sa načítavajú dynamicky z JSON súboru (vlastný parser, `malloc`/`realloc`)
- Armády sa načítavajú z textových súborov zadaných ako argumenty príkazového riadku
- Rozšírená validácia vstupu s chybovými hláseniami

## Štruktúra repozitára

```
z4.c        – Battle Arena 1.0
z5.c        – Battle Arena 2.0
data.h            – definície štruktúr a databáza predmetov
```

## Použité technológie
- Jazyk C (štandardná knižnica) – `stdio.h`, `stdlib.h`, `string.h`
- Dynamická správa pamäte – `malloc`, `realloc`, `free`
- Kompilátor GCC
