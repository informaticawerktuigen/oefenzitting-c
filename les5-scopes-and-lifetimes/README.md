# Lifetimes en scopes <!-- omit in toc -->

## Inhoudstafel <!-- omit in toc -->

- [Video](#video)
- [Introductie](#introductie)
- [Blokken code](#blokken-code)
  - [Functiedefinities](#functiedefinities)
- [Globale scope](#globale-scope)
- [Dynamische geheugenallocatie](#dynamische-levensduur)
  - [malloc](#malloc)
  - [free](#free)
  - [Out of memory](#out-of-memory)
  - [Memory leak](#memory-leak)

## Video

[![Bekijk de video](https://img.youtube.com/vi/ti8-oMvOlO4/hqdefault.jpg)](https://www.youtube.com/watch?v=ti8-oMvOlO4)
## Introductie

In deze sessie gaan we kijken naar de levensduur (*lifetimes*) en *scopes* van variabelen in C.

De scope van een variabele bepaalt de delen code waarvoor de variabele zichtbaar is.
Een stuk code kan een variabele gebruiken indien deze variabele *in scope* (in het vizier) is.

## Blokken code

In C definieer je een blok code met `{ }`.
Alles tussen deze accolades vormt 1 blok.

Blokken code hebben rechtstreeks invloed op de levensduur en de scope van een variabele:

1. Een variabele is enkel zichtbaar (*in scope*) in het blok code waarin de variabele gedefinieerd wordt, in en alle geneste blokken/subblokken van deze blok.
2. Indien je een variabele op een standaard manier (zonder `static`) definieert in een blok code, bestaat deze variabele enkel tot het einde van deze blok. De variabele heeft een *automatische levensduur* die eindigt wanneer het blok eindigt.

We geven een voorbeeld.

* Probeer onderstaand bestand te compileren.

```C
int main(void)
{
    {
        int a = 0;
    }
    return a;
}
```

Je krijgt een foutboodschap te zien: `a`  is niet gedeclareerd in de functie `main`.

Er zijn hier in feite twee problemen:

1. `a` wordt aangemaakt in een blok code.
Op het moment dat het blok eindigt, stopt `a` met bestaan, want `a` heeft een *automatische levensduur*;
2. `a` is niet zichtbaar (niet "in scope") in de regel code met het return-statement. `a` is namelijk niet in hetzelfde blok gedefinieerd, noch in een omsluitend blok.

> :bulb: Je kan de accolades beschouwen als een soort éénrichtingsgordijn. Het is mogelijk om van binnen (tussen de accolades) naar buiten te kijken, en dus de variabelen te zien in omsluitende blokken. Omgekeerd is echter niet mogelijk. Code buiten het blok kan niet naar binnen kijken.

Merk op dat dit geldt voor elke blok code dat je schrijft. Onderstaande code vertoont exact hetzelfde probleem:

```C
int main(void)
{
    if (1 > 0)
    {
        int a = 0;
    }
    return a;
}
```

Misschien denken we dat we de compiler te slim af kunnen zijn.
Vorige les hebben we namelijk geleerd over pointers, variabelen die geheugenadressen bevatten.
Wat als we een pointer `p` aanmaken in het blok van `main` en vervolgens in ons genest blok het adres van de lokale variabele `a` in `p` bewaren.
We kunnen daarna toch gewoon de waarde van `a` uitlezen met behulp van het adres in `p`?

* Bekijk onderstaande code en voer uit

```C
#include <stdio.h>

int main(void)
{
    int *p;
    {
        int a = 42;
        p = &a;
    }
    printf("The value of a is %d\n", *p);
    return 0;
}
```

We hebben één van de twee problemen opgelost, namijk het scoping-probleem.
Variabele `a` moet niet zichtbaar zijn indien we het adres van variabele `a` kennen.
De C-compiler lijkt akkoord te zijn.

*Op mijn machine* krijg ik de volgende output:

```bash
$ gcc undefined-behavior.c
$ ./a.out
The value of a is 42
```

Eureka!
We zijn de compiler te slim af geweest.
Helaas hebben we een programma geschreven dat *undefined behavior* vertoont.
Het tweede probleem hebben we namelijk nooit opgelost: de variabele `a` *bestaat niet meer*.

Op vele machines zal het probleem meteen duidelijk worden door het compilatiecommando aan te passen:

```bash
$ gcc -O3 undefined-behavior.c
$ ./a.out
The value of a is 0
```

Exact hetzelfde programma heeft een andere output.
Beide outputs zijn correcte outputs voor het programma.
Elke mogelijke output van het programma is in feite correct.
Het gedrag van het programma is namelijk *undefined*.
Een adres aanspreken van een variabele die niet (meer) bestaat is *undefined behavior*.

> :information_source: Met de flag `-03` zeg je tegen `gcc` dat de gegenereerde code geoptimalizeerd mag worden om ze zo snel mogelijk uit te voeren.
`gcc` is een slimme compiler die zal zien dat het hele blok code dat we geschreven hebben in feite helemaal niets doet.
`p` start met een niet-geïnitialiseerde waarde en eindigt met een niet-geïnitialiseerde waarde.
Een niet-geïnitialiseerd adres aanspreken kan eender welk resultaat teruggeven.
In plaats van effectief een geheugenoperatie uit te voeren, kiest `gcc` dus om gewoon `0` te gebruiken als resultaat.
Dit is *correct* gedrag van gcc, ons programma is fout.

> :information_source: Het is *mogelijk* om de levensduur van een lokale variabele te veranderen met het keyword `static`. We gaan dit concept in deze lessen niet bekijken, je zal het in IW niet nodig hebben.

### Functiedefinities

Merk op dat een functiedefinitie ook een blok code gebruikt.
Deze blokken volgen dezelfde regels.
Alle variabelen gedeclareerd in een functie zijn *lokaal* en stoppen met bestaan aan het einde van de functie.
Ze zijn niet in *scope* voor code buiten de functie.

Wanneer we een C-functie oproepen, wordt een kopie gemaakt van de *argumenten* die je meegeeft aan de functie.
De kopieën worden bewaard in de *parameters* van de functie.
De functie zelf kan de waarden van de parameters wijzigen, maar dit verandert *niets* aan de waarden van de *argumenten* die initieel meegegeven werden in de functie-oproep.
Dit noemen we *call-by-value* of *pass-by-value*.

> :information_source: Een *parameter* is een variabele die in een functiedeclaratie gedefinieerd wordt. In onderstaande code zijn `a` en `b` parameters van de functie `swap`. Een *argument* is de waarde die meegegeven wordt bij het oproepen van deze functie. `x` en `y` worden hieronder gebruikt als *argumenten* voor de functie.

We geven een voorbeeld:

```C
#include <stdio.h>

void swap(int a, int b)
{
    int c = a;
    a = b;
    b = c;
}

int main(void)
{
    int x = 0;
    int y = 42;

    printf("Before swap: x = %d, y = %d\n", x, y);
    swap(x, y);
    printf(" After swap: x = %d, y = %d\n", x, y);
    return 0;
}
```

```bash
$ gcc swap-fail.c
$ ./a.out
Before swap: x = 0, y = 42
 After swap: x = 0, y = 42
```

De waarde van de argumenten `x` en `y` zijn niet gewijzigd.
De waarden zijn namelijk gekopieerd naar de parameters `a` en `b`.
Die parameters zijn echter gestopt met bestaan na de functie-oproep.
De levensduur van een parameter stopt aan het einde van een functie.

Toch hebben we jullie in vorige sessie gevraagd een swapfunctie te schrijven met behulp van pointers:

```c
#include <stdio.h>

void swap(int *a, int *b)
{
    int c = *a;
    *a = *b;
    *b = c;
}

int main(void)
{
    int x = 0;
    int y = 42;

    printf("Before swap: x = %d, y = %d\n", x, y);
    swap(&x, &y);
    printf(" After swap: x = %d, y = %d\n", x, y);
    return 0;
}
```

Wanneer we dit uitvoeren zien we inderdaad:

```bash
Before swap: x = 0, y = 42
 After swap: x = 42, y = 0
```

Dit komt omdat de argumenten die we meegaven niet `x` en `y` zijn, maar de *adressen* van `x` en `y`.
Indien we deze adressen zouden wijzigen in de swapfunctie door de waarden van
de parameters `a` en `b` te wijzigen, zou dat niet het gewenste resultaat
opleveren. Ook hier werd er namelijk een kopie gemaakt van de *adressen* en
zouden we bijgevolg de kopie wijzigen.

> :warning: Bovenstaand voorbeeld wordt vaak **foutief** *pass-by-reference* genoemd. Dat is fout. *pass-by-reference* bestaat niet in C. Een *reference* bestaat niet in C (wel in C++). Voor de geïnteresseerden: [het verschil tussen pointers en references](https://stackoverflow.com/questions/57483/what-are-the-differences-between-a-pointer-variable-and-a-reference-variable-in).

> :question: **Geef het einde van de levensduur aan van alle variabelen (`a`, `b`, `c`, `x`, `y`) in bovenstaande swapcode. Zet een comment met de naam van de variabele in de laatste regel code waar de variabele nog in leven is.**

## Globale scope

In C kennen we ook een globale scope.
Indien we een variabele of functie declareren die niet omsloten wordt door een blok code, is deze variabele of functie in-scope (zichtbaar) voor *alle* code in het bestand.
Een globale variabele blijft bestaan zolang het programma actief is.
De variabele heeft een *statische* levensduur.

We geven een voorbeeld.

* Compileer het volgende programma en voer uit. Merk op dat de variabele `function_call_counter` overal gebruikt kan worden.

```C
#include <stdio.h>

int function_call_counter = 0;

int is_odd(int n);

int is_even(int n)
{

    function_call_counter++;
    if (n < 0)
        n = -n;
    
    if (n == 0)
    {
        return 1;
    }
    else
    {
        return is_odd(n - 1);
    }
}

int is_odd(int n)
{
    function_call_counter++;
    if (n == 0)
    {
        return 0;
    }
    else
    {
        return is_even(n - 1);
    }
}

int main()
{
    int n = 21;
    if (is_even(n))
    {
        printf("%d is even\n", n);
    }
    else
    {
        printf("%d is odd\n", n);
    }
    printf("Checking the parity of %d only took us %d function calls!\n", n, function_call_counter);
    return 0;
}
```

> :warning: Het gebruik van globals *kan* leiden tot slechte code. [Hier](https://stackoverflow.com/questions/484635/are-global-variables-bad) kan je een interessante discussie vinden over dit topic.

> **:question: Probeer bovenstaande code te herschrijven zonder gebruik te moeten maken van een global variable. Hint: je kan gebruik maken van een lokale variabele in main waarvan je het adres doorgeeft aan de functies.**

## Dynamische geheugenallocatie

Tijd om terug te springen naar één van de eerste voorbeelden deze les:

```C
#include <stdio.h>

int main(void)
{
    int *p;
    {
        int a = 42;
        p = &a;
    }
    printf("The value of a is %d\n", *p);
    return 0;
}
```

Herinner je dat dit gedrag *undefined behavior* vertoonde omdat de levensduur van `a` stopt op het moment dat het blok code eindigt.

### malloc

Er is een manier om dit probleem op te lossen.
We kunnen een object aanmaken met een levensduur die niet automatisch beheerd wordt.
De levensduur van dat object is volledig onder de controle van de programmeur zelf.
Hiervoor gebruiken we de functie `void *malloc(size_t size);` gedeclareerd in de header `<stdlib.h>.`

`malloc` reserveert genoeg plaats in het geheugen om een object in te bewaren van grootte `size`.
De `sizeof` operator kan gebruikt worden om de grootte van een C-type te vinden.
Om specifieker te zijn, met `sizeof` vinden we de grootte die nodig is om een C-object te maken van een bepaald type.

> :information_source: Meestal wordt het resultaat van `sizeof` uitgedrukt in bytes. Dit is echter geen garantie. Het resultaat van `sizeof` is een `size_t` en `size_t` is een datatype dat gegarandeerd groot genoeg moet zijn om de grootte van een object in te kunnen bewaren.

De return-waarde van `malloc` heeft type `void *`. Dit wil zeggen, een pointer naar een onbekend datatype.
De return-waarde van `malloc` is het adres van het dynamisch gealloceerde object *tenzij* er geen geheugen meer is.
Op de out-of-memory situatie komen we terug.

Tijd om eerst de werking van `malloc` te verduidelijken met een voorbeeld.
We lossen het levensduurprobleem uit het vorige voorbeeld op:

```C
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p;
    {
        int *a = malloc(sizeof(int));
        *a = 42;
        p = a;
    }
    printf("The value of a is %d\n", *p);
    return 0;
}
```

Indien we dit programma compileren en uitvoeren krijgen we het correcte resultaat, zelfs met optimalisaties.

De expressie `malloc(sizeof(int))` reserveert een object ter grootte van een `int` in het geheugen.
Het adres van dit object wordt bewaard in de variabele `a`.
Het type van `a` definiëren we dus als `int *`.
`a` zal namelijk het geheugenadres van een int bevatten.

Met `*a = 42` schrijven we de waarde `42` naar het zonet gereserveerde adres.
Vervolgens kopiëren we dit adres naar de pointer `p`.
Merk op dat de levensduur van `a` nog steeds *automatisch* is en eindigt op het einde van het blok code.
We hebben de waarde van `a` gelukkig gekopieerd naar `p`, dus nu bevat `p` een verwijzing naar ons dynamisch gealloceerde object.

In de `printf`-functie lezen we de waarde van dit object.
Het object bestaat nog steeds dus we hebben geen *undefined behavior*.

### free

Een dynamisch gealloceerd object blijft leven, en blijft dus gereserveerd in het geheugen, tot het programma de functie `void free(void *ptr);` uitvoert.
Het adres van het object wordt meegegeven als argument bij het oproepen van `free`.

Het onderstaande programma vertoont *opnieuw* undefined behavior:

```c
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p;
    {
        int *a = malloc(sizeof(int));
        *a = 42;
        p = a;
    }
    free(p);
    printf("The value of a is %d\n", *p);
    return 0;
}
```

De levensduur van het gereserveerde object, waarvan het adres in `p` bewaard zit, wordt beëindigt door de oproep naar `free`.
Merk op dat `p` nog steeds bestaat, de waarde van `p` zal zelfs onveranderd zijn door de oproep naar `free`.
Het adres dat bewaard is in `p` is echter ongeldig gemaakt.
Mijn machine geeft de volgende output zelfs zonder optimalizatie:

```bash
$ gcc malloc-undefined.c
$ ./a.out
The value of a is 0
```

> **:question: Geef in bovenstaande (foute) code opnieuw het einde van de levensduur van de variabelen aan in comments. Annoteer daarnaast ook de levensduur van het gereserveerde geheugenobject. Je mag dit zelf een naam geven.**

### Out of memory

Elk van de `malloc` programma's die we net getoond hebben *kunnen* undefined behavior vertonen.
Er is geen garantie dat er genoeg geheugen beschikbaar is in een machine om aan de `malloc`-oproep te kunnen voldoen.
Indien `malloc` er niet in slaagt om genoeg geheugen te reserveren, geeft `malloc` als resultaat een *null pointer*.
Op vele machines wordt de *null pointer* voorgesteld door het adres 0, maar dit is geen verplichting.

Het dereferencen van de `null pointer` is *undefined behavior*.
Op Ubuntu zal je de melding `segmentation fault` krijgen, omdat de `null pointer` zal verwijzen naar geheugen waar je geen toegang tot hebt.

In C-code kan je de *null pointer* voorstellen door `NULL` te schrijven.
We corrigeren ons eerder geschreven programma.

```c
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p;
    {
        int *a = malloc(sizeof(int));
        if (a == NULL)
        {
            return -1;
        }

        *a = 42;
        p = a;
    }
    printf("The value of a is %d\n", *p);
    free(p);
    p = NULL;
    return 0;
}
```

Dit programma voldoet aan enkele *good practices* bij het gebruik van dynamisch gealloceerd geheugen:

* Controleer dat het resultaat van `malloc` niet `NULL` is;
* Zorg ervoor dat geheugen, gealloceerd door `malloc`, ook terug met `free` vrijgegeven wordt.
* Laat pointers die vrijgegegeven zijn met `free` verwijzen naar `NULL`. Zo kan je op een later moment (door te vergelijken met `NULL`) verifiëren of de pointer al dan niet naar gealloceerd geheugen verwijst. Free doet dit *niet* automatisch.

Enkel door de return-waarde van `malloc` te controleren kunnen we zeker zijn dat dit programma geen *undefined behavior* zal vertonen.

> :information_source: De status code van ons programma is `-1` indien `malloc` geen geheugen kan reserveren, om aan te geven dat ons programma niet correct is uitgevoerd.

### Memory leak

Manueel geheugenbeheer met `malloc` en `free` is uitdagend.
Memory leaks zijn een enorm veel voorkomend probleem in C-programma's.

Een memory leak treedt op wanneer dynamisch gealloceerd geheugen niet terug wordt vrijgegeven wanneer het niet meer gebruikt zal worden.

Neem bijvoorbeeld onderstaande code:

```C
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *p;
    for (int i = 0; i < 10; i++)
    {
        p = malloc(sizeof(int));
        *p = i;
    }
    free(p);
    return 0;
}
```

De code reserveert 10 dynamische gealloceerde `int` objecten en bewaart de waarde van `i` in elk van deze objecten.
Het adres dat malloc teruggeeft wordt echter elke iteratie overschreven.
Op het einde van de lus hebben we enkel toegang tot het adres van de laatst gereserveerde `int`.
Het is vervolgens onmogelijk om de eerste 9 `int` objecten door te geven aan de functie `free`.
De variabelen zullen eeuwig (tot het einde van het programma) blijven bestaan.

Memory leaks zijn problematisch in programma's die gedurende een lange tijd actief moeten zijn.
Een memory leak wordt veroorzaakt wanneer een programma geheugen vraagt dat niet
terug wordt vrijgegeven.
Hoe langer het programma actief is, hoe meer geheugen het zal vragen.
Op een bepaald moment zal het geheugen opgeraken.

Het grote probleem met memory leaks is dat ze vaak lastig zijn om te detecteren of te vermijden.
Code kan zeer veel paden volgen, en in elk mogelijk pad moet een object dat gealloceerd is met `malloc` op een andere plaats vrijgegeven moeten worden met `free`.

Om problemen zoals memory leaks, null pointer dereferences, undefined behavior enzovoort te vermijden worden gehele nieuwe programmeertalen ontwikkeld, zoals bijvoorbeeld [Rust](https://www.rust-lang.org/).
De meeste van deze zogenaamde "memory-safe" talen leggen veel meer regels op via het typesysteem, waardoor een compiler veel meer kans heeft fouten terug te vinden.
Dit soort talen zal je echter pas later in de opleiding tegenkomen.
Vertrouwd zijn met C en de complexiteit van C zal je vormen tot een betere programmeur en zal je appreciate voor alternatieve, veilige(re) talen in de toekomst waarschijnlijk doen vergroten.
