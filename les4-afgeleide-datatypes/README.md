# Afgeleide datatypes <!-- omit in toc -->

## Inhoudstafel <!-- omit in toc -->

- [Video](#video)
- [Introductie](#introductie)
- [stdint.h](#stdinth)
- [Pointers](#pointers)
  - [Adresoperator](#adresoperator)
- [Pointer dereference](#pointer-dereference)
- [Verzameltypes](#verzameltypes)
  - [Arrays](#arrays)
    - [Lengte van een array](#lengte-van-een-array)
    - [Arrays als parameters](#arrays-als-parameters)
  - [Strings](#strings)
  - [Structs](#structs)

## Video

[![Bekijk de video](https://img.youtube.com/vi/XxlZ4nYIqng/hqdefault.jpg)](https://www.youtube.com/watch?v=XxlZ4nYIqng)

## Introductie

Tot nog toe hebben we enkel numerieke datatypes bekeken, met name de meest traditionele types: `char`, `short`, `int`, `long,` ,`long long` in `signed` en `unsigned` varianten.
Misschien heb je je wel gestoord aan de onzekerheid die deze types met zich meebrengen.
Hoe groot zullen ze zijn, welke representatie zullen ze gebruiken voor negatieve getallen, is een `char` nu standaard `signed` of `unsigned`, ... .

> :information_source: In de eerste versie van vorige les werd aangehaald dat negatieve getallen in C voorgesteld worden door 2-complement. Dit klopt niet. Compilerschrijvers mogen volgens de C-standaard kiezen uit 2-complement, 1-complement en sign-magnitude.

In deze sessie gaan we afgeleide (derived) datatypes bekijken.
Dit zijn types die op basis van voorgaande simpele types meer ingewikkelde datatypes samenstellen.
Eerst gaan we echter bovenstaande problematiek verlichten.

## stdint.h

Om duidelijkheid te scheppen in de wereld van ingewikkelde numerieke types is in de `C99 standard library` de header [`<stdint.h>`](https://en.wikibooks.org/wiki/C_Programming/stdint.h#cite_note-pubs-1) toegevoegd.

Deze biedt ons enkele zéér duidelijke types om mee te werken:

* `uint8_t`: een unsigned int van 8 bits groot
* `int8_t`: een signed int van 8 bits groot, wel *verplicht* in 2-complementrepresentatie
* `uint16_t`: een unsigned int van 16 bits groot
* `int16_t`: een signed int van 16 bits groot, wel *verplicht* in 2-complementsrepresentatie
* `uint32_t`: een unsigned int van 32 bits groot
* `int32_t`: een signed int van 32 bits groot, wel *verplicht* in 2-complementsrepresentatie
* `uint64_t`: een unsigned int van 64 bits groot
* `int64_t`: een signed int van 64 bits groot, wel *verplicht* in 2-complementsrepresentatie

Het is vaak een goed idee om met dit soort types te werken.
Ze maken de onderliggende voorstelling zeer duidelijk, waardoor je onverwacht gedrag kan vermijden.

## Pointers

Het eerste afgeleide datatype dat we zullen bespreken is waarschijnlijk de koning van de afgeleide types: de pointer (wijzer).
Pointers vormen een struikelblok voor vele startende C-programmeurs.

Door een variabele in C het datatype pointer toe te kennen, zeggen we dat de bits die bewaard worden in deze variabele een *geheugenadres* voorstellen.

> :information_source: Een geheugenadres kan veel betekenissen hebben. C legt niet vast hoe een geheugenadres er moet uitzien. De compiler moet dit zelf beslissen aan de hand van de onderliggende architectuur.

Pointers wijzen meestal naar een specifiek type:

* Het datatype _pointer naar een `int`_ schrijven we in C als het type `int *`. 
* De waarde van een variabele met type `int *` wordt geïnterpreteerd als het _geheugenadres_ van een `int` object.
* Het datatype _pointer naar een `char`_ schrijven we in C als het type `char *`
* De waarde van een variabele met type `char *` wordt geïnterpreteerd als het _geheugenadres_ van een `char` object.
* ...
  
Er is één uitzondering:

* Het datatype _pointer naar niets_ schrijven we in C als het type `void *`
* De waarde van een variabele met `void *` wordt geïnterpreteerd als het _geheugenadres_ van een onbekend type

Voor *elk* type in C hebben we een bijhorend pointer-datatype.
Merk echter op dat pointers zelf ook datatypes zijn. Dus:

* Het datatype _pointer naar een pointer naar een int_ schrijven we in C als het type `int **`.
* De waarde van een variabele met type `int **` wordt geïnterpreteerd als het _geheugenadres_ van een `int *` object (m.a.w. het adres van een adres).

Deze definities werken dus recursief.

### Adresoperator

Nu je weet wat het pointertype betekent, vraag je jezelf misschien af: hoe kom je nu aan geheugenadressen?

Om het adres van een variabele te vinden gebruiken we de *adress operator* `&`.
Het resultaat kunnen we bewaren in een pointer.

* Voer onderstaand programma uit. Welk adres krijgt de variabele `a` toegewezen op jouw machine?

```c
#include <stdio.h>

int main(void)
{
    int a = 0;
    int *p = &a;
    printf("The value of variable a is: %d\n", a);
    printf("The memory address of variable a is: %p\n", p);
    return 0;
}
```

> :bulb: Met de format specifier `%p` laat je de meegegeven waarde interpreteren als geheugenadres.

> **:question: Schrijf een C-programma dat variabelen declareert van het type `char`, `char *` en `long`. Geef deze variabelen initieel geen waarde. Print het adres van deze variabelen. Ken nadien deze variabelen een waarde toe en print het adres opnieuw. Verwacht je dat dit adres zal veranderen? Voer uit en kijk na.**

## Pointer dereference

Pointers bevatten geheugenadressen.
Op die geheugenadressen staan data.
Deze data kunnen opgevraagd worden door middel van de pointer.
Dit doen we met de *dereference operator* `*`.

> :warning: Merk op dat we `*` ook gebruiken in de naam van een pointer datatype, bij het declareren van een variabele. C gebruikt `*` niet alleen voor de vermenigvuldiging maar ook als dereference operator. Dit kan verwarrend zijn. Probeer goed de verschillen te begrijpen.

Laten we ons vorige programma lichtjes uitbreiden.

* Compileer onderstaand programma en voer uit.

```c
#include <stdio.h>

int main(void)
{
    int a = 0;
    int *p = &a;
    printf("The value of variable a is: %d\n", a);
    printf("The memory address of variable a is: %p\n", p);
    printf("The value stored at the memory address in p is: %d\n", *p);

    *p = 10;
    printf("The value of variable a is: %d\n", a);
    printf("The memory address of variable a is: %p\n", p);
    printf("The value stored at the memory address in p is: %d\n", *p);
    return 0;
}
```

Merk op dat we de waarde van `p` niet aanpassen.
Het bewaarde adres in `p` blijft hetzelfde.
Met `*p` voeren we een dereference uit van de variabele `p`.
`*p = 10` schrijft dus de waarde `10` naar de locatie in het geheugen waarvan het adres opgeslagen is in `p`.

> **:question: Vervang de regel `*p = 10` in bovenstaand programma door `p = 10`. Compileer het programma en voer uit. Loopt alles goed?**

Bovenstaande oefening illustreert een veelvoorkomende fout die beginnende C-programmeurs maken.
Indien je de waarde van een pointer rechtstreeks aanpast, overschrijf je het adres bewaard in de pointer.
Je hebt waarschijnlijk:

1. geen idee wat er op geheugenadres 10 in jouw machine opgeslagen zit;
2. of het adres 10 wel een geldig adres is op jouw machine

Indien je deze tutorial volgt op een Ubuntu-machine op een computer met een `x86` architectuur is de kans groot dat je zonet de foutmelding `segmentation fault` kreeg.
Over deze melding is het voorlopig genoeg dat je begrijpt dat deze voorkomt op het moment dat je een adres probeert te *dereferencen* waar je geen toegang tot hebt. In Ubuntu zal dit adres gereserveerd zijn door de kernel (deel van je besturingssysteem) en zal je processor geconfigureerd zijn om een fout te genereren wanneer je een gereserveerd adres probeert aan te spreken.

> :bulb: Indien je te maken krijgt met een segmentation fault in je C-programma, kijk dan of je ergens werkt met pointers. Zo ja, is het een goed idee om na te kijken wat de waarden van die pointers zijn. De kans is groot dat een bepaalde pointer een incorrecte waarde heeft en vervolgens gedereferenced wordt. Een andere mogelijke oorzaak kan zijn dat je een recursieve functie schrijft die in een oneindige lus belandt is. Op dat moment zal de stack blijven groeien tot de stack pointer op een bepaald punt een gereserveerd adres zal proberen aanspreken.

> **:question: Schrijf een functie `void swap(int *a, int* b)` die de waarden van twee variabelen `a` en `b` met elkaar wisselt. Roep deze functie op vanuit `main` en controleer na de functieoproep de waarden van de meegegeven variabelen. Zou je dit ook kunnen schrijven zonder pointers? Waarom wel/niet?**

## Verzameltypes

Het nut van pointers is gemakkelijker in te zien wanneer we enkele types introduceren die vaak gepaard gaan met het gebruik van pointers.
Laten we starten met een type dat je waarschijnlijk kent vanuit andere talen: de *array*.

### Arrays

Een array stelt een verzameling van waarden van hetzelfde type voor, die naast elkaar in het geheugen bewaard worden.
De lengte van de array is ook onderdeel van het datatype.

Net zoals de vele pointertypes zijn er ook voor elk datatype in C overeenkomstige array-types:

* Het datatype `int [15]` verwijst naar een verzameling van 15 `int` objecten die naast elkaar in geheugen zijn gealloceerd.
* Het datatype `int *[3]` verwijst naar een array van 3 `int *` objecten (dus een array van 3 _pointers naar ints_) die naast elkaar in geheugen zijn gealloceerd
* Het datatype `char[10]` verwijst naar een verzameling van 10 `char` objecten die naast elkaar in geheugen zijn gealloceerd

De _syntax_ om een variabele te definiëren met een array type is ietwat specialer.

* Met `int arr[3];` definieer je een variabele `arr` met als type een _array die 3 elementen bevat met type `int`_
* Met `int *arr[3];` definieer je een variabele `arr` met als type een _array die 3 elementen bevat met type `int*`_

Arrays kunnen geïnitializeerd worden met behulp van `{ }`.

* Bekijk onderstaand codevoorbeeld. Compileer en voer uit.

```C
#include <stdio.h>

int main(void)
{
    int arr[5] = {1, 10, 100, 1000, 10000};
    for (int i = 0; i < 5; i++)
    {
        printf("Element %d of arr has value %d\n", i, arr[i]);
    }
    return 0;
}
```

> **:question: Verander de uitdrukking `i < 5` in bovenstaand programma door `i < 10`. Wat verwacht je dat er zal gebeuren? Compileer en voer uit. Probeer het resultaat van je programma te verklaren.**

#### Lengte van een array

Een array is niet meer dan een verzameling van waarden van hetzelfde type, naast elkaar in het geheugen.
In bovenstaande oefening merkte je dat, na de array zelf, gewoon andere waarden in het geheugen zaten.

Vele high-level talen werken met lijsten (die soms verwarrend genoeg arrays worden genoemd) die van lengte kunnen veranderen.
In die talen heb je een lengte-functie om te achterhalen hoe lang een lijst is.
In C is dit soort functie schrijven voor arrays in het algemeen niet mogelijk.

> :information_source: Datatypes in C worden door de compiler gebruikt om machinecode te genereren (at _compile time_). Op het moment dat een programma uitgevoerd wordt (_at runtime_), bestaan deze datatypes niet meer. Denk aan de assemblycode die je schrijft in SOCS - de types bestaan daar niet meer.  Functies worden _at runtime_ uitgevoerd, maar de informatie over de lengte van een array is enkel beschikbaar at _compile time_, in het datatype. Je zou de lengte van de array kunnen bewaren in het eerste element van de array, maar dat zou plaats verspillen.

De lengte van een array zit in C geëncodeerd in het type.
Herinner je dat we met `sizeof` de grootte van een datatype kunnen opvragen.
`sizeof` kan ook gebruikt worden om de grootte van een array op te vragen.
We illustreren met code.
  
* Compileer onderstaande code en voer deze uit. Verander de lengte van de meegegeven array. Begrijp je hoe dit mechanisme werkt?

```c
#include <stdio.h>
#include <stdint.h>

int main(void)
{
    int arr[] = {1, 10, 100, 1000, 10000};
    size_t total_memory_size = sizeof(arr);
    size_t element_memory_size = sizeof(arr[0]);
    size_t amount_of_elements = total_memory_size / element_memory_size;
    printf("Total size of the array in memory: %lu\n", total_memory_size);
    printf("Total size of an array element in memory: %lu\n", element_memory_size);
    printf("Amount of elements in array: %lu\n", amount_of_elements);
    return 0;
}
```

> :information_source: Merk op dat we in bovenstaande code de lengte van de array niet _expliciet_ hebben meegegeven bij het aanmaken van de variabele. De compiler heeft dit automatisch kunnen bepalen op basis van de initialisatielijst. Indien je een array meteen initialiseert moet je de lengte niet meegeven.

> :information_source: Bovenstaande code gebruikt het type `size_t`. Dit type wordt gebruikt om de grootte van structuren te bewaren in C, zoals datatypes. Dit is het correcte type om te gebruiken om resultaten van `sizeof` in te bewaren.

> **:question: Schrijf een programma dat de som berekent van alle waarden in een array. Zorg ervoor dat je de lengte van de array automatisch berekent.**

#### Arrays als parameters

Het zou gemakkelijk zijn indien we het verhaal over de lengte van arrays hierboven konden beeïndigen.
Helaas is dat niet het geval.
Dat heeft te maken met de manier waarop arrays doorgegeven worden aan functies.

* Stel dat je een array aan een functie zou doorgeven in DRAMA. Hoe zou jij dit aanpakken?

Je _zou_ kunnen kiezen om elk element van de array op de _call stack_ te pushen.
Dat is echter **niet** wat er in C gebeurt.
Wanneer we in C de naam van een array gebruiken in een expressie, zonder deze naam te volgen door `[]`, wordt deze naam vervangen door het adres van het eerste element in de array<sup>1</sub>.

> :information_source: <sup>1</sup> De expressies `sizeof arr` of `&arr` zijn uitzonderingen op deze regel.

* Voer onderstaande code uit. Wat valt op?

```C
#include <stdio.h>

int main(void)
{
    int my_array[] = {1, 2, 3};
    printf("my_array: %p\n", my_array);
    printf("&my_array: %p\n", &my_array);
    printf("&my_array[0]: %p\n", &my_array[0]);
    return 0;
}
```

In C zijn arrays verzamelingen van elementen.
Namen van arrays zijn verwijzingen naar het adres van het eerste element.

Een functie schrijven met als parameter een array is niet mogelijk in C.
Je kan enkel functies schrijven met als parameter _het adres van een array_.
Het _adres van een array_ is gelijk aan het _adres van het eerste element_.

Stel dat je een array `arr` hebt van `int`-elementen.
Het eerste element is een `int`.
`arr` wordt omgezet naar het adres van het eerste element.
Indien je `arr` doorgeeft aan een functie, moet de parameter het type `int *` hebben, namelijk het adres van een int.

Onderstaande code toont de juiste manier om elementen van een generische array op te tellen met behulp van een functie.

* Compileer onderstaande code en voer uit.

```C
#include <stdio.h>

int array_sum(int *arr, size_t length)
{
    int sum = 0;
    for (size_t i = 0; i < length; i++)
    {
        sum += arr[i];
    }
    return sum;
}

int main(void)
{
    int arr[] = {1, 2, 3};
    size_t arr_len = sizeof(arr) / sizeof(arr[0]);
    int sum = array_sum(arr, arr_len);
    printf("Sum of array elements: %d\n", sum);
    return 0;
}
```

> **:question: Waarom geven we de lengte mee in bovenstaande functie? Kan je de lengteberekening ook uitvoeren binnen de functie zelf? Waarom wel/niet?**

### Strings

In C kan je een string aanmaken, zoals in vele talen, door tekst te schrijven tussen dubbele aanhalingstekens.
We hebben dit ondertussen al verschillende keren gedaan, bij het oproepen van print-functies.

C heeft echter geen datatype _string_.
De string `"abc"` wordt door C voorgesteld als een array van **vier** `char`-waarden: `a` (`97`, ascii), `b` (`98`, ascii) en `c` (`99`, ascii) en `0` (`NULL`, ascii).

Het laatste element `0` wordt ook wel eens de _terminating null byte_ genoemd.
Deze geeft het einde van een string aan.
Goed gevormde strings moeten steeds eindigen met een null byte.

Dit komt erg van pas bij het doorgeven van strings aan functies.
Aangezien strings gewoon speciale arrays zijn, krijgen functies opnieuw het adres mee van het eerste karakter van de string.
Het is in het geval van strings echter _wel_ mogelijk de lengte van de string te bepalen: zoek gewoon naar de null byte.

* Compileer onderstaande code en voer uit.

```C
#include <stdio.h>

int main(void)
{
    char bad_str[] = {'x', 'y', 'z'};
    const char *good_str1 = "abc";
    char good_str2[] = {'d', 'e', 'f', '\0'};
    char good_str3[] = {103, 104, 105, 0};

    printf("Bad: %s\n", bad_str);
    printf("Good 1: %s\n", good_str1);
    printf("Good 2: %s\n", good_str2);
    printf("Good 3: %s\n", good_str3);

    return 0;
}
```

> :bulb: Het type dat typisch aan strings wordt gegeven is `const char *`. Dit is een speciale `char *` waarbij het niet mogelijk is om de waarde waarnaar de char pointer verwijst (dus de eerste letter van de string) aan te passen.

>**:question: Verklaar de output van de foutief gevormde string. Dit kan variëren van compiler tot compiler.**

> **:question: Schrijf een functie `size_t strlen(const char* s)` die de lengte van een string berekent. Deze functie zit ook in `libc` en wordt gedeclareerd in de header `<string.h>`.**
> 
> **Wanneer je klaar bent kan je eens kijken naar de [implementatie in de standard library](https://github.com/bminor/glibc/blob/master/string/strlen.c). Het begin van de functie lijkt misschien op jouw implementatie.**

### Structs

Het laatste belangrijke datatype dat we zullen bespreken is de `struct`.
Met een struct kunnen we in zekere zin een eigen datatype samenstellen door een combinatie te maken van andere types.

* Bekijk onderstaand voorbeeld en voer uit:

```C
#include <stdio.h>

struct person
{
    const char *first_name;
    const char *last_name;
    int age;
};

void print_personal_information(struct person p)
{
    printf("First name: %s\n", p.first_name);
    printf("Last name: %s\n", p.last_name);
    printf("Age name: %d\n", p.age);
}

int main(void)
{
    struct person p;
    p.first_name = "Bob";
    p.last_name = "Marley";
    p.age = 36;
    print_personal_information(p);
    return 0;
}
```

> :warning: Merk op dat de naam van het declareerde type gelijk is aan `struct person`, niet gewoon `person`.

Het aanmaken van een `struct` zal ervoor zorgen dat er plaats wordt gereserveerd voor elk van zijn velden.
Deze zullen naast elkaar in het geheugen liggen.
De C-compiler mag lege ruimte voorzien tussen de verschillende velden van een struct.
Dit noemen we padding.

> **:question: Gebruik de sizeof operator om te achterhalen of bovenstaande struct gebruik maakt van padding.**

> **:question: Schrijf een programma waarin een struct gedefinieerd wordt die de x- en y-coördinaten van een punt bewaart. Schrijf vervolgens een functie die de [manhattan distance](https://en.wikipedia.org/wiki/Taxicab_geometry) (`|x1 - x2| + |y1 - y2|`) tussen twee punten berekent, gebruik makend van dit struct type.**
