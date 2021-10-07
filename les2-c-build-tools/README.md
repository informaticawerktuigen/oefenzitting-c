# C Build Tools <!-- omit in toc -->

## Inhoudstafel <!-- omit in toc -->

- [Video](#video)
- [Introductie](#introductie)
- [gcc](#gcc)
- [Preprocessor](#preprocessor)
  - [Header files (.h)](#header-files-h)
  - [Andere preprocessorcommando's](#andere-preprocessorcommandos)
- [Compiler](#compiler)
  - [Object files (.o)](#object-files-o)
- [Linker](#linker)
- [Libraries](#libraries)
  - [Hello, world!](#hello-world)
  - [Dynamic linking](#dynamic-linking)
- [Korte samenvatting](#korte-samenvatting)
- [FizzBuzz](#fizzbuzz)

## Video

[![Bekijk de video](https://img.youtube.com/vi/69xYSOAfD08/hqdefault.jpg)](https://www.youtube.com/watch?v=69xYSOAfD08)

## Introductie

In deze les leren we werken met verschillende tools die gebruikt worden bij het omzetten van C-broncode naar werkende programma's.

> :warning: Deze les bevat veel informatie en minder oefeningen. Neem je tijd om door deze les te gaan en voer elk commando ook zelf uit. Verifieer de outputs. Het is de bedoeling dat je de verschillende C tools snapt. Je kan op je projectverdediging gevraagd worden om een bepaalde tool uit deze sessie te gebruiken.

## gcc

In de eerste les hebben we enkele simpele C programma's geschreven die we compileerden met behulp van het programma `gcc`.
`gcc` is echter veel meer dan gewoon een compiler.

> :information_source: Initieel stond `gcc` voor de GNU C compiler.
Vandaag staat het voor GNU Compiler Collection. [[bron]](https://en.wikipedia.org/wiki/GNU_Compiler_Collection)

`gcc` bevat onder andere een preprocessor (voorvertaler), compiler (vertaler) en een linker (verbinder).
Deze onderdelen worden elk ingezet, in vaste volgorde, bij de omzetting van een C bronbestand naar een `ELF` (Executable Linkable Format, een uitvoerbaar bestand).

## Preprocessor

De eerste fase gedurende de compilatie van een C programma is de preprocessorfase.
De preprocessor zet één of meerdere C bronbestanden om in een compilation unit.

De preprocessor heeft een eigen programmeertaal.
Elk preprocessor commando begint met een `#`.
Het meest voorkomende commando is `#include`.
Het `#include` commando wordt gevolgd door een bestandsnaam.
Wanneer de preprocessor `#include "<filename>"` tegenkomt zal de preprocessor deze regel vervangen door de volledige inhoud van het meegegeven bestand.

Om dit te illustreren kijken we naar de bestanden in de folder `les2-c-tools/src/preprocessor`.

`main.c`

```c
int main(void)
{
#include "1.c"
```

`1.c`

```c
return 0;
#include "2.c"
```

`2.c`

```c
}
```

* Probeer met `gcc` de bestanden `1.c` of `2.c` te compileren.
Dit zijn geen welgevormde C-bestanden.

```bash
$ gcc 1.c
1.c:1:1: error: expected identifier or ‘(’ before ‘return’
    1 | return 0;
      | ^~~~~~
In file included from 1.c:2:
2.c:1:1: error: expected identifier or ‘(’ before ‘}’ token
    1 | }
      | ^
$ gcc 2.c
2.c:1:1: error: expected identifier or ‘(’ before ‘}’ token
    1 | }
      | ^
```

De errors klagen over het ontbreken van haakjes en acoolades, kortom simpele syntax-errors.

* Probeer nu het programma `main.c` compileren.

```bash
$ gcc main.c
$ ./a.out
$ echo $?
0
```

Dit werkt wel.
`main.c` is correct, omdat de C preprocessor in `main.c` de inhoud van het bestand `1.c` plakt.
In `1.c` staat ook een preprocessor commando, dat ervoor zorgt dat de inhoud van bestand `2.c` in `1.c` geplakt wordt.

Met de `-E` flag kunnen we aan gcc vragen enkel de preprocessorstap uit te voeren.

* Voer het commando `gcc -E main.c` uit.

```bash
$ gcc -E main.c
# 1 "main.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "main.c"
int main(void)
{
# 1 "1.c" 1
return 0;
# 1 "2.c" 1
}
# 2 "1.c" 2
# 3 "main.c" 2
```

De output van `gcc -E` is een compilation unit.
De `#`-regels in deze output bevatten metadata en wordt voornamelijk door de `C` compiler gebruikt om duidelijkere error messages te genereren.

Indien we enkel kijken naar de regels die gecompileerd (vertaald naar machinecode) zullen worden, dus de regels zonder `#`, zien we ons ondertussen welbekende C-programma:

```bash
$ gcc -E main.c | grep -v "#"
int main(void)
{
return 0;
}
```

### Header files (.h)

Een functie-implementatie over verschillende bestanden verdelen wil je in de praktijk nooit doen.
Hoewel het mogelijk is, is het zeer onoverzichtelijk.
`#include`-statements in C worden bijna uitsluitend gebruikt om te werken met header-files.

Herinner je uit vorige sessie ons simpele functieprogramma:

`les1-purec/src/functions.c`

```c
int absolute_value(int a); // <- Functiedeclaratie

int manhattan_distance(int x1, int x2, int y1, int y2)
{
    return absolute_value(x2 - x1) + absolute_value(y2 - y1);
}

int absolute_value(int a) // <- Functiedefinitie
{
    if (a < 0)
    {
        return -a;
    }
    else
    {
        return a;
    }
}

int main(void)
{
    return manhattan_distance(10, 0, 0, 10);
}
```

De functie `absolute_value` werd eerst *gedeclareerd*, vervolgens kon de functie `manhattan_distance` deze functie oproepen, en ten slotte werd `absolute_value` *gedefinieerd*.

Een standaardpraktijk bij het schrijven van `C`-programma's is om functiedeclaraties te schrijven in een header-file.
Een header-file (een `.h` bestand) bevat alle functies die gedeclareerd moeten worden.

`les2-c-tools/src/header-files/functions.h`

```C
int absolute_value(int a);
int manhattan_distance(int x1, int x2, int y1, int y2);
```

Vervolgens wordt deze header-file geïnclude aan de start van het `.c` bestand waarin de functiedefinities staan:

`les2-c-tools/src/header-files/functions.c`

```C
#include "functions.h"

int manhattan_distance(int x1, int x2, int y1, int y2)
{
    return absolute_value(x2 - x1) + absolute_value(y2 - y1);
}

int absolute_value(int a)
{
    if (a < 0)
    {
        return -a;
    }
    else
    {
        return a;
    }
}

int main(void)
{
    return manhattan_distance(10, 0, 0, 10);
}
```
Indien we `functions.c` laten preprocessen zien we dat de declaraties inderdaad boven de definities geplaatst worden.

* Bekijk de output van onderstaand commando

```bash
gcc functions.c -E | grep -v "#"
```

* Compileer `functions.c` met `-Wall -Werror -std=c17 -pedantic-errors` en verifieer dat dit inderdaad zonder fouten compileert.

Door een header-file te gebruiken maak je een expliciete splitsing van functiedeclaraties en definities.
Je moet je vervolgens nooit zorgen maken over de volgorde waarmee je functies oproept.
In het gedeelte over de linker zullen we ook zien dat header-files nodig zijn om functies te kunnen gebruiken in meerdere compilation units.

### Andere preprocessorcommando's

De preprocessor bevat een hoop andere commando's.
Het is een programmeertaal op zich.
Je kan de preprocessor onder andere gebruiken om macro's te implementeren.
Het kan interessant zijn om eens door [Wikipedia](https://en.wikipedia.org/wiki/C_preprocessor) te scrollen om te kijken wat zoal mogelijk is.

Bekijk het onderstaande stukje C-code:

```c
#define A return 1
#define B return 0
#define C
int main(void){
#ifdef C
    A;
#endif
    B;
}
```

Kan je voorspellen wat de output is van dit programma?

* **Oefening** Gebruik de preprocessor (eventueel met `grep -v` om alle metadata te verwijderen) om te kijken welke code bovenstaand programma effectief zal doorgeven aan de compiler.




## Compiler

Tot nu toe hebben we de compiler (vertaler) telkens voorgesteld als een programma dat voor ons een executable (uitvoerbaar bestand) genereert.
Dat is echter geen correcte voorstelling.
De compiler is enkel verantwoordelijk voor het vertalen van C-broncode naar machinecode.
De compiler genereert geen executable bestanden (dat doet de linker), maar object files.

### Object files (.o)

Bestanden met de extensie `.o` op Linux zijn object files.
Deze bestanden bevatten de machinecodevertaling van een compilation unit, maar zijn geen uitvoerbare bestanden.

Eén belangrijk verschil tussen een executable en een `.o`-file is dat een `.o`-file verwijzigingen of 'referenties' naar functies kan bevatten zonder de bijhorende machinecode van deze functies.

Laten we dit meteen expliciet maken.
In de folder `les2-c-tools/src/compiler` zit opnieuw het bestand `functions.c`.
Ditmaal is de functie `absolute_value` echter niet gedefinieerd.
Indien we onze preprocessor een compilation unit laten genereren, zien we het volgende:

```bash
$ gcc -E functions.c | grep -v "#"
int manhattan_distance(int x1, int x2, int y1, int y2);
int absolute_value(int a);

int manhattan_distance(int x1, int x2, int y1, int y2)
{
    return absolute_value(x2 - x1) + absolute_value(y2 - y1);
}

int main(void)
{
    return manhattan_distance(10, 0, 0, 10);
}
```

Onze functie `absolute_value` heeft geen definitie.
Wanneer we `functions.c` proberen omzetten in een uitvoerbaar bestand, krijgen we een error:

``` $ gcc functions.c
/usr/bin/ld: /tmp/ccRIXrjI.o: in function `manhattan_distance':
functions.c:(.text+0x22): undefined reference to `absolute_value'
/usr/bin/ld: functions.c:(.text+0x31): undefined reference to `absolute_value'
collect2: error: ld returned 1 exit status
```

Een aandachtige lezer merkt misschien op dat deze error message er zeer anders uitziet dan de error messages die we eerder kregen.
Dit is namelijk een error message van de *linker* (verbinder).
In de volgende sectie zal duidelijk worden hoe dit komt.

Het feit dat de linker uitgevoerd wordt, wil zeggen dat de preprocessor en de compiler met succes hun werk hebben kunnen uitvoeren.
Met de `-c`-flag kunnen we `gcc` expliciet vragen om enkel te compileren, een `.o`-bestand te genereren, en daar te stoppen.
We genereren dus nog geen uitvoerbaar bestand.

* Voer `gcc -c functions.c` uit en merk op dat er een nieuw bestand is aangemaakt: `functions.o`

```bash
$ gcc -c functions.c
$ ls -la functions.o
-rw-rw-r-- 1 kobe kobe 1784 Nov 26 14:39 functions.o
```

Merk op dat dit bestand niet uitvoerbaar is.
Het bevat echter wél machinecode:

```bash
$ objdump --disassemble=main -Mintel functions.o

functions.o:     file format elf64-x86-64


Disassembly of section .text:

000000000000003e <main>:
  3e:   f3 0f 1e fa             endbr64 
  42:   55                      push   rbp
  43:   48 89 e5                mov    rbp,rsp
  46:   b9 0a 00 00 00          mov    ecx,0xa
  4b:   ba 00 00 00 00          mov    edx,0x0
  50:   be 00 00 00 00          mov    esi,0x0
  55:   bf 0a 00 00 00          mov    edi,0xa
  5a:   e8 00 00 00 00          call   5f <main+0x21>
  5f:   5d                      pop    rbp
  60:   c3                      ret
```

> :information_source: In vorige sessie hebben we `gdb` gebruikt om te *disassemblen*, nu gebruiken we het programma `objdump`. Beide zijn bruikbaar.

Elke `.o`-file bevat een symbol table.
Deze symbol table bevat informatie over de symbolen (functies, labels, globale variabelen, ...) die gedeclareerd worden in het bestand.

Laten we eens kijken naar de symbol table van onze `.o` file.

* Voer het commando `objdump -t functions.o` uit:

```bash
$ objdump -t functions.o

functions.o:     file format elf64-x86-64

SYMBOL TABLE:
0000000000000000 l    df *ABS*  0000000000000000 functions.c
0000000000000000 l    d  .text  0000000000000000 .text
0000000000000000 l    d  .data  0000000000000000 .data
0000000000000000 l    d  .bss   0000000000000000 .bss
0000000000000000 l    d  .note.GNU-stack        0000000000000000 .note.GNU-stack
0000000000000000 l    d  .note.gnu.property     0000000000000000 .note.gnu.property
0000000000000000 l    d  .eh_frame      0000000000000000 .eh_frame
0000000000000000 l    d  .comment       0000000000000000 .comment
0000000000000000 g     F .text  000000000000003e manhattan_distance
0000000000000000         *UND*  0000000000000000 _GLOBAL_OFFSET_TABLE_
0000000000000000         *UND*  0000000000000000 absolute_value
000000000000003e g     F .text  0000000000000023 main
```

> :information_source: De volledige betekenis van de output van het commando `objdump` kan je vinden met het commando `man objdump` indien dit je interesseert. In deze sessie kijken we enkel naar een klein gedeelte.

* Kijk met onderstaand commando naar de symbol table entry voor de functie `manhattan_distance`:

```bash
$ objdump -t functions.o | grep manhattan_distance
0000000000000000 g     F .text  000000000000003e manhattan_distance
```

In deze regel betekent:

* `F` dat het symbool `manhattan_distance` een functie is
* `g` dat het een globaal symbool is
* `.text` geeft aan in welke sectie van het bestand het symbool gedefinieerd staat. Zo bevat `.text` code, `.data` globale variabelen, `.rodata` read-only data, enzovoort.
* `3e` is de grootte in bytes (hexadecimaal) van de machinecode van deze functie

Al deze informatie zal belangrijk zijn voor de *linker* (verbinder), die deze info zal gebruiken om een uitvoerbaar bestand te genereren.

* Kijk nu in de symbol table naar de functie `absolute_value`:

```bash
$ objdump -t functions.o | grep manhattan_distane
0000000000000000         *UND*  0000000000000000 absolute_value
```

Deze `*UND*` geeft aan dat de functie `absolute_value` wel gebruikt wordt in het bestand (gedeclareerd/opgeroepen) maar dat de functiedefinitie van deze functie niet gekend is.
Er is geen vertaalde machinecode die bij deze functie hoort.
Dit leidt ons meteen tot de laatste tool in onze opeenvolging van compilatietools, de *linker*.

## Linker

De taak van de *linker* is om verschillende object files met elkaar te verbinden, om zo een uitvoerbaar bestand te genereren.
Een *linker* gebruikt de symbol tables van verschillende `.o` files om deze samen te voegen.

In onze vorige sectie hebben we `functions.o` gegenereerd.
Hier ontbrak de functiedefinitie van `absolute_value`.
De definitie van deze functie hebben we verplaatst naar het bestand:

`les2-c-tools/src/linker/math.c`

```c
int absolute_value(int a)
{
    if (a < 0)
    {
        return -a;
    }
    else
    {
        return a;
    }
}
```

* Compileer dit bestand (gebruik `-c` om enkel te compileren en niet te linken)

```bash
$ gcc -c math.c
$ ls
math.c  math.o
```

* Bekijk de entry `absolute_value` in de symbol table van `math.o`

```bash
$ objdump -t math.o | grep absolute_value
0000000000000000 g     F .text  000000000000001d absolute_value
```

In deze `.o`-file staat onze ontbrekende functie dus wel gedefinieerd.

* Gebruik de linker `ld` om de bestanden `functions.o` en `math.o` aan elkaar te linken (verbinden) en zo een uitvoerbaar bestand te genereren.

```bash
$ ld math.o ../compiler/functions.o
ld: warning: cannot find entry symbol _start; defaulting to 0000000000401000
$ ls
a.out  math.c  math.o
```

`ld` geeft ons een waarschuwing over het symbool `_start` dat niet gevonden kan worden, maar ons bestand `a.out` is wel gegenereerd.

* Voer `a.out` uit

```bash
$ ./a.out
Segmentation fault (core dumped)
```

Dit lijkt niet te werken. Op de specifieke foutmelding komen we later terug, maar wat hier misloopt is dat de linker wel alle functies heeft samengevoegd, maar niet weet waar het startpunt van je programma is.
Er wordt gegokt dat dit het adres `401000` is (zie de warning message van ld).
Indien we kijken wat er op dit adres staat, zien we:

```bash
$ objdump -t a.out | grep 401000
0000000000401000 l    d  .text  0000000000000000 .text
0000000000401000 g     F .text  000000000000001d absolute_value
```

De uitvoering start dus bij de functie `absolute_value` en daar loopt het mis.

Wanneer we `gcc` gebruiken om een programma te maken, lost `gcc` deze problemen voor ons op.
`gcc` zal zelf enkele functies toevoegen aan je executable, waaronder een functie die vervolgens `main` oproept.
Vervolgens zal `gcc` het startpunt intern doorgeven aan `ld`, de linker.

Tijd om ons programma dus te linken met hulp van `gcc`.

* Voer onderstaand commando uit om `functions.o` en `math.o` te linken met `gcc`

```bash
$ gcc math.o ../compiler/functions.o
$ ./a.out
$ echo $?
20
```

We hebben onze object-files succesvol samengevoegd!

* **Oefening** Breid het bestand `math.c` uit met je eigen implementatie van de functies fibonacci en factorial (zie les 1). Maak een header-bestand `math.h` waarin deze functies gedeclareerd worden. Maak een bestand `main.c` met daarin enkel een `main`-functie, en roep vanuit `main` de verschillende functies in `math.c` op. Genereer `math.o` en `main.o` met de compiler. Link deze `.o`-files vervolgens en genereer zo een uitvoerbaar bestand.

## Libraries

Een belangrijke reden waarom al deze stappen zo expliciet worden opgesplitst is om het mogelijk te maken modulair te programmeren.

Neem bijvoorbeeld het gebruik van libraries.
Dit zijn verzamelingen van functies die je gebruikt in een project, typisch geschreven door andere programmeurs (je kan uiteraard ook zelf libraries schrijven).

Om een library te gebruiken kan je de bijhorende header file (`.h`, bevat alle functiedeclaraties) van deze library includen.
De C-preprocessor zal alle declaraties van de library bovenaan je compilation unit zetten.
De compiler vertaalt je compilation unit naar een `.o` file, zonder de volledige library te moeten compileren.
Vervolgens kan de library zelf, die eventueel al eerder door iemand anders gecompileerd was, gelinkt worden aan je programma.

In de laatste oefening van vorige sessie heb je in essentie een soort `math`-library gemaakt.
Verschillende programma's zouden van deze library gebruik kunnen maken door `math.h` te includen en te linken aan `math.o`.

### Hello, world!

Nu we gesproken hebben over libraries, kunnen we eindelijk kijken naar de [C standard library](https://en.wikipedia.org/wiki/C_standard_library).
Deze library biedt een verzameling functies aan die een C-programma de mogelijkheid geven om te interageren met het systeem waarop het actief is.
Output lezen en schrijven naar bestanden of de terminal, het aanmaken van processen, enzovoort, dit soort functies worden aangeboden door functies in de standard library.

Een besturingssysteem komt vaak gebundeld met een implementatie van de C standaard library,
Op Ubuntu heet deze library simpelweg `libc`.
De header files kan je vinden door bijvoorbeeld te typen:

```bash
$ whereis stdlib.h
stdlib: /usr/include/stdlib.h
```

Op mijn systeem zitten deze headers dus in `/usr/include` (`stdlib.h` is maar één van de vele headers in de standard library).
De gecompileerde versie van deze library kan je vinden met het commando

```bash
$ whereis libc
libc: /usr/lib/x86_64-linux-gnu/libc.so /usr/lib/x86_64-linux-gnu/libc.a /usr/share/man/man7/libc.7.gz
```

Tot nog toe hebben we het gebruik van deze library expres vermeden. Tijd om eindelijk een `Hello, world!`-programma te schrijven:

```c
#include <stdio.h>

int main(void)
{
    printf("Hello, world!");
}
```

* Zet bovenstaand programma om in machinecode (`gcc` zal automatisch preprocessen, compileren en linken) en voer uit:

```bash
$ gcc hello-world.c
$ ./a.out
Hello, world!
```

In dit Hello, world! programma starten we met het includen van de header-file `stdio.h` uit de standaard library.
In `stdio.h` staat de functie `printf` gedeclareerd.

We gebruiken hierbij `<` en `>` om deze bestanden te includen, in plaats van de dubbele aanhalingstekens die we eerder zagen.
Met `<` en `>` gaat je preprocessor het bestand zoeken in de systeemfolder (op mijn machine de folder `/usr/include`).

Wanneer we kijken naar de symbolen in het `a.out` bestand zien we echter iets verrassend:

* Zoek met `objdump` en `grep` naar het symbool `printf` in `a.out`

```bash
objdump -t a.out | grep printf
0000000000000000       F *UND*  0000000000000000              printf@@GLIBC_2.2.5
```

De definitie van de functie `printf`, een functie in de C standaard library (`libc`), werd niet gevonden in het bestand.
Toch werkt het programma en heeft de linker ons geen foutmelding gegeven.

### Dynamic linking

De `libc` library is dynamisch gelinkt.
Dat wil zeggen dat de functiedefinities, dus de machinecode waarmee deze functies geïmplementeerd worden, niet in het uitvoerbaar bestand zelf staan.
Pas op het moment dat het programma door een *loader* (lader) in het geheugen geladen wordt, dus op het moment dat je het programma uitvoert, worden deze functies gelinkt.

Aangezien vele programma's gebruik maken van `libc`, is dynamische linking een logische keuze.
Je zou de machinecode van `libc` in elk uitvoerbaar bestand kunnen plakken, maar dat is telkens opnieuw dezelfde code en zou alleen maar veel plaats innemen.
Dynamische linking zorgt hier dus voor een kleiner uitvoerbaar bestand.

Met het commando `ldd` kan je nakijken welke libraries dynamisch gelinkt zijn aan een executable bestand.

* Voer `ldd` uit op je Hello, world! programma

```bash
$ ldd a.out
linux-vdso.so.1 (0x00007ffc3e1d8000)
libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f4886633000)
/lib64/ld-linux-x86-64.so.2 (0x00007f4886841000)
```

Hier zie je `libc` als één van je drie dynamisch gelinkte libraries.
`vdso` beschouwen we niet in deze sessie. De derde library, `ld-linux-x86-64.so.2` bevat de code van de dynamische linker zelf, die door de loader uitgevoerd zal worden om de andere dynamische libraries correct te linken op het moment dat het programma wordt uitgevoerd.

## Korte samenvatting

Na deze lange uitleg kan het gebeuren dat je door de bomen het bos niet meer ziet.

Daarom deze beknopte samenvatting van de zaken die we zonet hebben besproken.
Het vertalen van een C programma, mogelijks bestaand uit meerdere `.c` en `.h` bestanden, gebeurt als volgt:

1. De preprocessor verwerkt alle preprocessor commando's (startend met `#`) en genereert een compilation unit (compilatie-eenheid);
2. De compiler vertaalt deze compilatie-eenheid naar machinecode (output: `.o` bestanden). Het is mogelijk dat het `.o` bestand verwijst naar functies waarvan
de machinecode niet aanwezig is in hetzelfde `.o` bestand.
3. De linker genereert een uitvoerbaar bestand door `.o` files met elkaar te combineren (output: `ELF` bestand). Het is nog steeds mogelijk dat hierin de machinecode van library-functies niet aanwezig is, indien deze *dynamisch gelinked* zijn.
4. De loader start een programma door de machinecode van een bestand in het werkgeheugen te laden en zorgt op dat moment voor dat dynamisch gelinkte libraries ook ingeladen worden.

Het begrip van deze verschillende stappen is belangrijk bij het schrijven van C-programma's.
Let bijvoorbeeld op foutboodschappen.
Worden deze gegenereerd door de preprocessor, compiler of de linker?
Dit kan je al veel informatie geven over wat er misgaat in je programma.

## FizzBuzz

Deze laatste oefening is een variant op een programmeeroefening die vaak gevraagd wordt op sollicitatiegesprekken, om te controleren of een sollicitant effectief kan programmeren.

De opgave is een programma te schrijven dat alle getallen van 1 tot en met 100 print naar de console, behalve:

* Voor elk veelvoud van 3 print je het woord *Fizz*;
* Voor elk veelvoud van 5 print je het woord *Buzz*;
* Voor veelvouden van 3 en 5 print je *FizzBuzz*.

Een correcte output is dus

```bash
$ ./fizzbuzz
1
2
Fizz
4
Buzz
Fizz
7
8
Fizz
Buzz
11
Fizz
13
14
FizzBuzz
...
```

Voeg hiervoor een functie `is_multiple` toe aan `math.c`, die twee integers `x` en `y` vergelijkt. De functie moet `1` teruggeven indien `x` een veelvoud is van `y` en `0` in alle andere gevallen.

Implementeer de FizzBuzz-functie zelf in een `main.c` bestand.

> :bulb: Met `printf("%d\n", naam_van_int);` kan je een int printen naar de console, gevolgd door een newline (`\n`). Op de exacte syntax van `printf` komen we in een toekomstige sessie terug.
