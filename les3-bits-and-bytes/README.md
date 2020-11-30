# Bits and bytes <!-- omit in toc -->

## Inhoudstafel <!-- omit in toc -->

- [Video](#video)
- [Introductie](#introductie)
- [Eenvoudige representaties](#eenvoudige-representaties)
  - [ASCII](#ascii)
- [C datatypes](#c-datatypes)
  - [int](#int)
  - [char](#char)
    - [ASCII](#ascii-1)
  - [unsigned](#unsigned)
  - [Andere basistypes](#andere-basistypes)
  - [sizeof](#sizeof)
  - [Type casting](#type-casting)
- [C operatoren](#c-operatoren)
  - [Arithmetische operaties](#arithmetische-operaties)
  - [Vergelijkende operatoren](#vergelijkende-operatoren)
  - [Logische operatoren](#logische-operatoren)
  - [Bitoperatoren](#bitoperatoren)
  - [Bitwise not](#bitwise-not)
  - [Bitwise and](#bitwise-and)
  - [Bitwise or](#bitwise-or)
  - [Bitwise xor](#bitwise-xor)
  - [Left bit shift](#left-bit-shift)
  - [Right bit shift](#right-bit-shift)
- [Masking](#masking)

## Video

> :warning: In deze video claim ik dat C de 2-complementsrepresentatie afdwingt voor negatieve getallen.
> Dit klopt niet en zal gecorrigeerd worden.
> Een C-compiler mag kiezen welke representatie gebruikt wordt.
> In de meeste gevallen (en op mijn machine) zal dit 2-complement zijn maar dit is *geen* garantie!
> Een C-compiler mag kiezen om sign-magnitude of 1-complement te gebruiken, of eender welke andere representatie, zolang deze consistent gekozen wordt.

[![Bekijk de video](https://img.youtube.com/vi/6MbIt705v7A/hqdefault.jpg)](https://www.youtube.com/watch?v=6MbIt705v7A)

## Introductie

Het geheugen van een computer bestaat, zoals we weten, uit een hoop bits die elk de waarde 1 of 0 kunnen bevatten.
Op een typische machine kan je deze bits in veelvouden van 8 (dus byte per byte) uitlezen of wegschrijven.

Alle gegevens die in op een computer kunnen worden voorgesteld worden uiteindelijk bewaard in bits en bytes. Tekst, lijsten, bestanden, machinecode, enzovoort.
Het is de manier waarop je de bytes interpreteert die bepaalt welke data er opgeslagen wordt.

## Eenvoudige representaties

Een simpel voorbeeld: neem nu de bits `1000 0001`.
Deze bits zouden we kunnen interpreteren als het decimaal getal: `129`.
Python zal hiermee akkoord gaan.

```python
$ python
>>> 0b10000001
129
```

> :information_source: Vanaf nu spreken we niet meer over linkse of rechtse bits, maar over de Most Significant Bits (MSB) of de Least Significant Bits (LSB). De meest linkse bits in onze binaire voorstelling zijn de bits die het meest significant zijn. Ze hebben de meeste invloed op de magnitude van het binaire getal. [[Meer info]](https://en.wikipedia.org/wiki/Bit_numbering)

Hoe zou je nu echter een negatief getal voorstellen?
We zouden kunnen beslissen dat de meest linkse bit aangeeft of het getal negatief of positief is, en de andere 7 bits het getal voorstellen.
In dat geval betekent zou `1000 0001` gelijk kunnen zijn aan het getal `-1`.
Dit noemen we de [*signed magnitude*](https://en.wikipedia.org/wiki/Signed_number_representations) representatie.

We kunnen ook andere regels volgen bij het interpreteren `1000 0001` als een negatief getal. Stel dat we de regels volgen van de *2-complement*-voorstelling, stellen deze bits het getal `-127` voor.
Hoe we tot dit getal komen kan je nakijken in je SOCS-cursus.

We hebben dezelfde bits ondertussen al kunnen interpreteren als 3 getallen: `129`, `-1`, `-127`. 
Merk dus op dat de manier waarop je bits interpreteert bepalen wat hun waarde is.
Meer niet.

### ASCII

Met 1 byte kunnen we 256 unieke waarden voorstellen.
We zouden dus ook kunnen beslissen dat elk van deze waarden een letter voorstelt.
Dit is wat gebeurt in `ASCII`:

![Ascii table image](https://upload.wikimedia.org/wikipedia/commons/1/1b/ASCII-Table-wide.svg)

Merk op dat standaard ASCII maar 127 waarden gebruikt.
Er zijn [uitbreidingen op ASCII](https://en.wikipedia.org/wiki/Extended_ASCII) die de volledige 256 waarden gebruiken.

Indien we de de byte `0011 1111` (decimaal: 63) interpreteren als ASCII-karakter stelt deze byte het karakter `?` voor.

## C datatypes

Een datatype in C wordt toegewezen aan een variabele (of functie) en geeft de manier aan waarop bytes in het geheugen geïnterpreteerd moeten worden.
Een datatype heeft telkens een vaste grootte, uitgedrukt in een hoeveelheid bytes.
Deze grootte kan variëren van machine tot machine.

### int

Neem nu bijvoorbeeld het type dat we reeds kennen: de `int` (integer, geheel getal).
Op een typische, moderne machine zal een `int` 4 bytes bevatten.
Dat wil zeggen dat wanneer je een `int` aanmaakt, 4 bytes geheugen worden gereserveerd.
Deze 4 bytes samen worden geïnterpreteerd als 1 getal: de waarde van de `int`.
De bytes worden geïnterpreteerd als een getal dat negatief kan zijn.
Negatieve getallen kunnen we echter op verschillende manieren voorstellen: de signed magnitude, 2-complement, 1-complement, ... .

C legt geen onderliggende representatie op.
De compiler kan zelf een keuze maken, waarschijnlijk gebaseerd op de onderliggende machine-architectuur.

Wat wel altijd waar is, is dat een `int` positieve en negatieve getallen kan bevatten.

> :warning: een `int` kan verschillen van grootte afhankelijk van de onderliggende machine-architectuur!

### char

Laten we eerst echter eens kijken naar een simpeler numeriek type: de `char`.
Een `char` is *bijna altijd* exact 1 byte groot.

> :information_source: Een char is `CHAR_BIT` groot. Volgens de C standaard is `CHAR_BIT >= 8`, volgens de `POSIX` standaard moet `CHAR_BIT == 8`. In de praktijk zal je waarschijnlijk nooit in contact komen met chars die niet exact 1 byte groot zijn.

Een `char` kan positieve en negatieve waarden bevatten.
Opnieuw kan C de binaire respresentatie van deze waarden kiezen.

Bekijk onderstaand programma.
De `printf`-regel zal de byte in de variabele `c` interpreteren volgens de intern gekozen representatie van mogelijks negatieve getallen en vervolgens de waarde van `c` uitprinten.

* Welke uitvoer verwacht je te zien?
* Zal dit programma in een oneindige lus terecht komen? Waarom wel/niet?
* Compileer het programma en voer het uit om na te kijken of je antwoorden correct waren.

```c
#include <stdio.h>

int main(void)
{
    for (char c = 1; c != 0; c++)
    {
        //%hhd -> signed number of 1 byte in size
        printf("%hhd ", c);
    }
    return 0;
}
```

> :warning: Bovenstaand `C`-programma maakt gebruik van *undefined behavior*. We doen dit om illustratieve redenen. Schrijf dit soort code nooit zelf!

Stel dat we ons programma opnieuw lichtjes aanpassen.
`%hhd` zorgde ervoor dat we de meegegeven variabele `c` interpreteerden als een signed number (getal dat positief of negatief kan zijn) van 1 byte groot.
Met `%hhu` zorgen we ervoor dat `c` wordt geïnterpreteerd als een getal dat *enkel* positief kan zijn.
We stellen dezelfde vragen:

* Zal dit programma in een oneindige lus terecht komen?
* Welke uitvoer verwacht je te zien?
* Compileer het programma en voer het uit om na te kijken of je antwoorden correct waren.

```c
#include <stdio.h>

int main(void)
{
    for (char c = 1; c != 0; c++)
    {
        //%hhd -> signed number of 1 byte in size
        printf("%hhu ", c);
    }
    return 0;
}
```

Tijd om te kijken wat bovenstaand programma effectief doet.
Laten we eens kijken naar de machinecode van het programma.
We laten hiervoor de `print`-statement tijdelijk weg, om enkel de lus te analyseren.

```bash
$ objdump --disassemble=main a.out -Mintel | grep -v -e '^$' | grep -v "section"
0000000000001129 <main>:
    1129:	f3 0f 1e fa          	endbr64
    112d:	55                   	push   rbp
    112e:	48 89 e5             	mov    rbp,rsp
    1131:	c6 45 ff 01          	mov    BYTE PTR [rbp-0x1],0x1
    1135:	eb 0a                	jmp    1141 <main+0x18>
    1137:	0f b6 45 ff          	movzx  eax,BYTE PTR [rbp-0x1]
    113b:	83 c0 01             	add    eax,0x1
    113e:	88 45 ff             	mov    BYTE PTR [rbp-0x1],al
    1141:	80 7d ff 00          	cmp    BYTE PTR [rbp-0x1],0x0
    1145:	75 f0                	jne    1137 <main+0xe>
    1147:	b8 00 00 00 00       	mov    eax,0x0
    114c:	5d                   	pop    rbp
    114d:	c3                   	ret  
```

Indien je deze code zou analyseren, zie je dat de variabele `c` bewaard wordt op de stack (offset -1 van `rbp`).
Elke lusiteratie wordt de waarde in het register `eax` geladen,
en verhoogd met `1` en terug bewaard op de stack.
Elke iteratie wordt gecontroleerd of de nieuwe waarde `0` is.
Zo ja, wordt de lus beeïndigd.

Je vraagt je misschien af hoe het mogelijk is dat de waarde van `c` ooit terug `0` wordt.
Stel dat je start met het binaire getal `0b00000000`.
Indien je 255 keer de waarde `1` optelt bij de getal, krijg je het getal `0b11111111`.

* Wat is de waarde van `0b11111111 + 1`?

Bovenstaande optelling veroorzaakt *arithmetische overflow*.
Er zijn niet genoeg bits om het resultaat voor te stellen.

> :information_source: Volgens de C standaard hangt de waarde van `0b11111111 + 1` af van het datatype. Indien het datatype *signed* is (dus negatieve waarden kan bevatten) mag een compiler eender welke waarde kiezen als resultaat. Dit is exact de *undefined behavior* die optreedt. De meeste compilers zullen waarschijnlijk kiezen voor de waarde `0`, maar dat is geen garantie. Indien het getal een *unsigned* getal is, dus nooit negatief kan zijn, is de waarde *wel* gedefinieerd in de standaard. Dan moet het resultaat gelijk zijn aan `0`.

Wat we zien gebeuren op mijn machine, indien je de assembly code in detail zou bekijken, is dat de waarde `0b11111111` bewaard wordt in register `eax`, dat 32-bit groot is.
Het resultaat van `0b11111111` + 1 is dus `0b100000000`, 9 bits lang, dit wordt bewaard in een 32-bit register.
Enkel de 8 minst significante bits worden echter bewaard op de call stack.
Dus - het resultaat dat bewaard wordt voor variabele `c` is `0b00000000`.
Daarom eindigt het programma.

In het algemeen kan je geen correct antwoord geven op de vraag of deze `C`-programma's ooit eindigen.
Doordat de optelling een vorm is van *undefined behavior* kan de C-compiler zelf kiezen hoe het opgelost wordt.
Het kan in theorie dus zijn dat op jouw machine je toch in een *infinite loop* terecht komt.

Wat we wel zeker weten is dat beide programma's minstens éénmaal, startend bij het getal `0b00000001`, elk mogelijk getal afliepen, tot `0b11111111`.
Door te printen met `%hhd` *interpreteerden* we deze bits volgens de interne representatie van negatieve getallen. Op de meeste machines zal dit de 2-complementrepresentatie zijn en zal dit er dus toe leiden dat er negatieve getallen verschijnen vanaf het moment dat de meest significante bit `1` wordt.
Door te printen met `%uud` *interpreteerden* we de bits als positieve getallen, dus zelfs indien de meest significante bit `1` was bleven we normale verhogingen zien.

#### ASCII

Laten we ten slotte nog een laatste variant bekijken van ons programma.
Met `%c` kunnen we de byte bewaard in `c` voorstellen als ASCII-karakter.

* Voer onderstaand programma uit en verklaar de output.

```c
#include <stdio.h>

int main(void)
{
    for (char c = 1; c != 0; c++)
    {
        //%c: interpreteer de byte als ASCII karakter
        printf("%c ", c);
    }
    return 0;
}
```

> :bulb: `char` wordt in de praktijk het vaakst gebruikt om ASCII-letters in te bewaren, vandaar ook de naam (character). Vergis je echter niet, `char` is een gewoon numerisch datatype van 1 byte groot. `char` en `int` verschillen enkel in het aantal bytes dat ze innemen.

### unsigned

Het sleutelwoord `unsigned` kan worden toegevoegd aan numerieke datatypes in C.
Hiermee zorgen we ervoor dat de waarde bewaard in het getal door `C` altijd als positief wordt geïnterpreteerd.

Je vraagt je misschien af wat het verschil dan net is? We hebben toch zonet, door middel van de print-functie, gekozen op welke manier we de bytes willen voorstellen.
Waarom maakt het voor C dan uit of de 8 bits een negatief of een positief getal voorstellen?

Het antwoord is eenvoudig uitgelegd met onderstaande code.

* Compileer onderstaande code en voer uit: 

```c
#include <stdio.h>

int main(void){
    signed char c1 = -1;
    unsigned char c2 = -1;

    if(c1 > 0){
        printf("c1 is positive\n");
    }else{
        printf("c1 is negative\n");
    }

    if(c2 > 0){
        printf("c2 is positive!\n");
    }else{
        printf("c2 is negative\n");
    }

    printf("c1 has value %hhd\n", c1);
    printf("c2 has value %hhd\n", c2);
    printf("c1 has value %hhu\n", c1);
    printf("c2 has value %hhu\n", c2);

    return 0;
}
```

```bash
$ ./a.out 
c1 is negative
c2 is positive
c1 has value -1
c2 has value -1
c1 has value 255
c2 has value 255
```

> :information_source: De C standaard is soms een gek beestje. Bij elk numeriek type wordt standaard aangenomen dat, indien je geen `unsigned` schrijft, het getal standaard `signed` is. De enige uitzondering is `char`. Indien een `char` niet wordt voorafgegaan door `signed` of `unsigned` mag een compiler zelf kiezen. De meeste compilers zullen waarschijnlijk `signed` kiezen.

Hoewel beide getallen exact dezelfde binaire waarde hebben (`0b11111111`), behandelt C `c1` als een negatief getal en `c2` als een positief getal.
`c2` is namelijk unsigned en kan dus nooit negatief zijn.
Wanneer we het getal met `print` gaan uitprinten, zeggen we tegen print, met behulp van de *format specifier* (`%hhu`, `%hhd`, `%d`, ...) hoe het getal moet geïnterpreteerd worden.
Dat kan echter verschillen van de interne interpretatie die C zelf gebruikt bij het uitvoeren van de code.

### Andere basistypes

Naast `char` en `int` hebben we ook nog `short`, `long` en `long long`. Elk van deze types bevatten gehele getallen die `signed` of `unsigned` kunnen zijn.

Daarnaast heb je ook `float` en `double` voor niet-gehele getallen.
Deze zullen we in deze lessenreeks niet behandelen.

Een zeer duidelijke tabel van de verschillende C-datatypes kan je vinden op [Wikipedia](https://en.wikipedia.org/wiki/C_data_types).
Telkens wordt ook aangegeven hoe je de waarden van die variabelen via een format specifier kan printen.

* Bekijk de tabel op Wikipedia. Enkel de tabel in *Main types* is voor deze les relevant. De floating point types behandelen we niet, pointers, arrays, structs en unions bekijken we in toekomstige lessen.

### sizeof

Met de operator `sizeof(datatype)` kan je achterhalen hoe groot een datatype exact is in een C-programma.
Dit hangt af van je compiler, die dit zal beslissen op basis van je onderliggende processorarchitectuur.

Het resultaat van `sizeof` geeft de grootte van het meegegeven datatype, uitgedrukt in *bytes*.

> :information_source: Om volledig correct te zijn geeft `sizeof` zijn resultaat in het aantal `char` variabelen nodig om het datatype te bewaren. `sizeof(char)` is dus gegarandeerd `1`. Aangezien een `char` zo goed als altijd 1 byte groot is, komt dit in de praktijk bijna altijd neer op het aantal bytes in een datatype. [[Meer info]](https://en.wikipedia.org/wiki/Sizeof)

`sizeof` is *geen* functie, maar een *operator*.
De compiler zal elk voorkomen van `sizeof(type)` vervangen door de grootte van het type.
Dit gebeurt volledig gedurende de compilatie zelf.
Er wordt nergens een functie opgeroepen.

Onderstaande code print het aantal bytes in een `int`-variabele:

```c
#include <stdio.h>

int main(void)
{
    printf("Size of int: %lu\n", sizeof(int));
    return 0;
}
```

* **Oefening** Schrijf een programma dat de grootte uitprint van alle numerieke types in C die gehele getallen bevatten. Gebruik de lijst op [Wikipedia](https://en.wikipedia.org/wiki/C_data_types) ter referentie.

### Type casting

Elke [expression](https://en.wikipedia.org/wiki/Expression_(computer_science)) (uitdrukking) in C heeft een impliciet datatype.
Indien `c` het type `char` heeft, zal de uitdrukking `c` uiteraard het type `char` hebben, en ook de uitdrukking `c + 1` zal het type `char` krijgen.

Stel dat we een `int i` hebben en een `char c`.
We schrijven de uitdrukking `i + c`.

* Wat is het type van deze expressie? Welk type zou jij kiezen?

De C compiler zal hier de logische keuze maken om het grootste type te kiezen, in dit geval zal de expressie dus het type `int` krijgen.
Indien je meer informatie wil kan je [deze post op StackOverflow](https://stackoverflow.com/questions/46073295/implicit-type-promotion-rules) lezen.

Het is mogelijk om de C-interpretatie van een [expressie](https://en.wikipedia.org/wiki/Expression_(computer_science)), bepaald door het datatype, manueel te wijzigen.
Dit gebeurt door middel van *type casting*.

Laten we ons eerdere programma lichtjes aanpassen.

* Compileer onderstaand programma en voer uit.

```c
#include <stdio.h>

int main(void){
    signed char c1 = -1;
    unsigned char c2 = -1;

    if(c1 > 0){
        printf("c1 is positive\n");
    }else{
        printf("c1 is negative\n");
    }

    //We typecast c2 to a signed char
    if((signed char) c2 > 0){
        printf("c2 is positive!\n");
    }else{
        printf("c2 is negative\n");
    }

    printf("c1 has value %hhd\n", c1);
    printf("c2 has value %hhd\n", c2);
    printf("c1 has value %hhu\n", c1);
    printf("c2 has value %hhu\n", c2);

    return 0;
}
```

Door `(datatype)` te schrijven voor een C expressie, veranderen we de C-interpretatie van die expressie.

> :warning: Het is niet mogelijk om het datatype van een variabele te wijzigen.
C-variabelen kunnen *nooit* van type veranderen!
Enkel expressies kunnen gecast worden naar een ander type.
Dit wijzigt niets aan het datatype van eventuele variabelen die voorkomen in de expressie.

## C operatoren

### Arithmetische operaties

De standaard C operatoren om te rekenen wijzen zichzelf uit. `+`, `-`, `*` en `/` voeren respectievelijk de optelling, aftrekking, vermenigvuldiging en deling uit.

Bij de deling moet je wel oppassen.
Neem het volgende programma:

```c
#include <stdio.h>

int main(void)
{
    int a = 9;
    a /= 2;
    printf("%d\n", a);
    return 0;
}
```

Aangezien `a` enkel gehele getallen kan bevatten, kan `4.5` als resultaat niet bewaard worden.
Het gedeelte na de komma zal gewoon weggelaten worden.
Bovenstaand programma print dus `4` af.

Indien je met kommagetallen wil werken, moet je een `float` of `double` gebruiken.
Daar gaan we in deze lessen niet verder op in.

In de laatste opgave van vorige oefenzitting heb je waarschijnlijk gewerkt met de `%`-operator.
`a % b` geeft de rest na deling van `a` door `b`.

Ten slotte hebben we nog de operatoren `++` en `--`.
De expressie `a++` telt `1` op bij de waarde van `a`.
De waarde `++a` doet dit ook.
Het resultaat van beide expressies verschilt echter.
De expressie `a++` evalueert naar de waarde `a`, de expressie `++a` evalueert meteen naar de waarde `a + 1`.
Voor `--` geldt het omgekeerde.

Dit kan het meest eenvoudig begrepen worden door te kijken naar het resultaat van onderstaande code:

```c
#include <stdio.h>

int main(void)
{
    int i = 0;
    int j = 0;
    printf("%d\n", i++);
    printf("%d\n", ++j);
    printf("%d\n", i);
    printf("%d\n", j);
    return 0;
}
```

* Voer bovenstaande code uit en zorg ervoor dat je het verschil tussen `++a` en `a++` begrijpt.
* **Oefening** Schrijf een programma met twee for-loops die elk de getallen `1 - 10` printen. Gebruik in het eerste programma de `++i` operator en in de tweede lus de `i++` operator.

### Vergelijkende operatoren

In C kunnen we twee variabelen vergelijken met de operatoren `==`, `!=`, `<`, `>`, `<=` of `>=`.
In vele talen zouden deze operatoren `true` of `false` als resultaat geven.
In C is dit niet het geval.

* Stel dat je de keuze kreeg. Hoe zou jij `true` of `false` binair voorstellen?

Onderstaand programma toont de werkelijke output van deze operatoren.

* Compileer onderstaand programma en voer het uit.

```c
#include <stdio.h>

int main(void){
    int i = 0;
    int j = 1;
    printf("equal: %d\n", i == j);
    printf("not equal: %d\n", i != j);
    printf("greater: %d\n", i > j);
    printf("less: %d\n", i < j);
    printf("less or equal: %d\n", i <= j);
    printf("bigger or equal: %d\n", i >= j);
    return 0;
}
```

In C wordt de `0`-waarde impliciet als `false` beschouwd.
Alle waarden die niet gelijk zijn aan `0`, zijn `true`.
If-statements kunnen dus uitgevoerd worden met behulp van numerieke waarden.

* Bekijk onderstaande code. Welke output verwacht je te zien?
* Compileer onderstaande code en voer uit. Kijk na of je verwachting klopten.

```c
#include <stdio.h>

int main(void){
    for(signed char c = -10; c < 10; c++)
    {
        if(c){
            printf("%hhd ", c);
        }
    }
    return 0;
}
```

### Logische operatoren

De logische operatoren `!`, `&&` en `||` staan respectievelijk voor de logische `not`, `and` en `or`-operaties.
Deze werken in vele talen enkel met booleaanse waarden.

Zoals we ondertussen weten bestaan er geen booleans in C.
Logische operatoren kan je gewoon uitvoeren op getallen.

* Compileer en voer onderstaand programma uit om te achterhalen hoe `!` omgaat met verschillende numerieke waarden.

```c
#include <stdio.h>

int main(void){
    for(signed char c = -10; c < 10; c++)
    {
        printf("%hhd ", !c);
    }
    return 0;
}
```

* Voorspel de output van onderstaand programma. Kijk nadien je voorspelling na door het programma te compileren en uit te voeren.

```c
#include <stdio.h>

int main(void){
    printf("%hhd\n", 1 && 66 && -33);
    printf("%hhd\n", 0 && -54 && 44);
    printf("%hhd\n", -12 || 0);
    printf("%hhd\n", !-10 || 0);
    return 0;
}
```

Merk op dat alle logische operatoren altijd de waarde `1` teruggeven als `true` en (uiteraard) de waarde `0` als false.
De operatoren zouden echter ook correct blijven werken indien ze telkens `-1`, `2` of eender welk getal zouden teruggeven dat niet `0` is.

### Bitoperatoren

De laatste soort operatoren die we in deze les zullen bespreken zijn de bitoperatoren.
Ook voor bitoperaties heb je een `not`, `and` en `or`. Deze bitwise operaties werken echter anders dan de logische operatoren die we net zagen.
De bitoperatoren voeren de `not`, `and` en `or` operaties uit op de invdividuele bits van de meegegeven getallen.

Indien de voorbeelden in deze secties niet voldoende zijn kan je altijd gaan kijken naar deze [Wikipediapagina](https://en.wikipedia.org/wiki/Bitwise_operation).

### Bitwise not

`~` is de bitwise not-operator.
Met `~a` inverteer je alle bits van de variabele `a`.
Het getal `0b110011` zou na applicatie van `~` gelijk zijn aan het getal `0b001100`.

* Kan je de output van onderstaand programma verklaren?

```c
#include <stdio.h>

int main(void)
{
    printf("%d\n", ~-10);
    return 0;
}
```

### Bitwise and

`&` is de bitwise and-operator.
Door `c = a & b` uit te voeren worden de individuele bits met dezelfde bit-index van twee variabelen `a` en `b` met elkaar vergeleken en bewaard in variabele `c`.
Er wordt gebruik gemaakt van volgende waarheidstabel om de resultaatbits te berekenen:

|   | 0 | 1 |
|---|---|---|
| 0 | 0 | 0 |
| 1 | 0 | 1 |

### Bitwise or

`|` is de bitwise or-operator.
Door `c = a | b` uit te voeren worden de individuele bits met dezelfde bit-index van twee variabelen `a` en `b` met elkaar vergeleken en bewaard in variabele `c`.
Er wordt gebruik gemaakt van volgende waarheidstabel om de resultaatbits te berekenen:

|   | 0 | 1 |
|---|---|---|
| 0 | 0 | 1 |
| 1 | 1 | 1 |

### Bitwise xor

`^` is de bitwise xor-operator.
Door `c = a ^ b` uit te voeren worden de individuele bits met dezelfde bit-index van twee variabelen `a` en `b` met elkaar vergeleken en bewaard in variabele `c`.
Er wordt gebruik gemaakt van volgende waarheidstabel om de resultaatbits te berekenen:

|   | 0 | 1 |
|---|---|---|
| 0 | 0 | 1 |
| 1 | 1 | 0 |

### Left bit shift

Bit shifts zijn complexer dan de voorgaande operatoren.
De `<<`-operator voert een linkse bit-shift uit, de `>>`-operator een rechtse bit-shift.

Met `x << n` verschuif je alle bits in `x`, `n` plaatsen naar links.
Het resultaat van `0b00110011 << 5` is bijvoorbeeld `0b11001100000`.
Merk op dat er (n) nullen geïntroduceerd worden als nieuwe bits.
Stel dat dit resultaat bewaard moet worden in een byte, zal het nieuwe resultaat gelijk zijn aan `0b01100000` (de least significant bits worden bewaard).

### Right bit shift

De `>>`-operator voert ook een bit-shift uit, maar nu naar rechts.
Dit werkt minder voorspelbaar dan een linkse bit-shift.

`0b11001100 >> 3` kan in principe twee resultaten hebben: `0b00011001` of `0b11111001`.

Indien het getal een `unsigned` getal is zal het resultaat gelijk zijn aan `0b00011001`. 
De drie least siginificant bits van het originele getal zijn verwijderd en vervangen door 3 most-significant bits elk met de waarde `0`.

Indien het getal echter `signed` is, heb je te maken met *undefined behavior*.
Het resultaat is afhankelijk van de keuze van de compiler.

Meer informatie kan je [hier](https://stackoverflow.com/questions/7622/are-the-shift-operators-arithmetic-or-logical-in-c) vinden.

## Masking

Binaire getallen worden in C vaak gebruikt om configuraties voor te stellen.

Stel je voor dat we een machine hebben met 8 lampjes.
Deze lampjes kunnen aan- en uitgezet worden door de verschillende bits van een byte op `1` of op `0` te zetten.

Door de char `c` met waarde `0b00000101` naar een register te schrijven, zetten we lamp 0 en lamp 2 aan.
Alle andere lampjes staan uit.

Stel dat we nu willen weten of een bepaalde lamp aan- of uitstaat vanuit code.
Om dit te achterhalen kunnen we gebruik maken van een mask.

Stel; je wil weten of lamp 3 aanstaat. Maak een binair getal met waarde `1` op de positie van lamp 3, en waarde `0` op elke andere plaatst. In dit geval dus `mask_l3 = 0b00001000` (merk op dat bits starten met index 0!). Dit getal noemen we een mask.

De mask kunnen we gebruiken voor verschillende doeleinden.
Indien we willen weten of lamp 3 actief is, kunnen we de waarde van onze huidige lampen uitlezen.
Stel `current_lamps = 0b11001100`.

* Overtuig jezelf dat de operatie `current_lamps & mask_l3` enkel gelijk zijn aan 0 (false) indien lamp 3 uitstaat. Indien lamp 3 aanstaat zal `current_lamps & mask_l3` niet gelijk zijn aan 0 (dus true) zijn.

Stel dat `current_lamps = 0b00000001`. Enkel lamp 0 staat dus aan.
Indien we ervoor willen zorgen dat lamp 3 aangezet wordt, kunnen we de `|`-operatie gebruiken.

* Overtuig jezelf dat de operatie `current_lamps = current_lamps | mask_l3` lamp 3 zal aanzetten. Wat is er mis met `current_lamps = current_lamps & mask_l3`?

Een mask kan zeer eenvoudig aangemaakt worden:
`mask_l3 = 1 << 3`, of in het algemeen `mask_ln = 1 << n`.

* Maak een mask voor lamp 2 en lamp 4. Kijk na of bovenstaande procedures blijven werken voor masks met meerdere `1`-waarden.
