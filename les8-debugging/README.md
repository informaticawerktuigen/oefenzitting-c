# Debugging <!-- omit in toc -->

## Inhoudstafel <!-- omit in toc -->

- [Video](#video)
- [Introductie](#introductie)
- [Printf debugging](#printf-debugging)
- [`gdb`](#gdb)
  - [Single stepping](#single-stepping)
  - [Watchpoints](#watchpoints)
  - [Conditions](#conditions)
  - [Numerieke formaten](#numerieke-formaten)
  - [Call stack](#call-stack)
  - [Cheat sheet](#cheat-sheet)
- [`gdbgui`](#gdbgui)
- [`ddd`](#ddd)

## Video

[![Bekijk de video](https://img.youtube.com/vi/8ExxfhWyA_k/hqdefault.jpg)](https://www.youtube.com/watch?v=8ExxfhWyA_k)

## Introductie

Fouten opsporen in een C programma kan lastig zijn.
Een kleine fout in je code kan leiden tot een perfect werkend programma, dat toch volledig anders uitvoert dan je zou verwachten.
Achterhalen waar de fout net zit, is zeker niet eenvoudig.

Neem nu een zeer simpel voorbeeld.
Onderstaand programma wil elk element van een array printen, en daarnaast ook de buren van elk array element.
Het eerste element heeft natuurlijk geen linkerbuur.
Enkel indien `i - 1 >= 0` zullen we dus de linkerbuur printen.
Hetzelfde doen we voor het laatste element (`i + 1 < array_length`).

```C
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t array_length = sizeof(array) / sizeof(array[0]);

    for (unsigned i = 0; i < array_length; i++)
    {
        printf("My current value is: %d\n", array[i]);

        if (i - 1 >= 0)
        {
            printf("The value to my left is: %d\n", array[i - 1]);
        }

        if (i + 1 < array_length)
        {
            printf("The value to my right is %d\n", array[i + 1]);
        }

        printf("\n");
    }
    return 0;
}
```

Als ik dit programma compileer en uitvoer op mijn machine, krijg ik volgende boodschap:

```bash
$ gcc buggy.c
$ ./a.out
My current value is: 0
Bus error (core dumped)
```

> :information_source: Het kan zijn dat je op je eigen machine een andere foutboodschap krijgt, bijvoorbeeld `segmentation fault`.

Als ik deze foutboodschap opzoek krijg ik:

> In computing, a bus error is a fault raised by hardware, notifying an operating system (OS) that a process is trying to access memory that the CPU cannot physically address: an invalid address for the address bus, hence the name.
[[Bron: Wikipedia]](https://en.wikipedia.org/wiki/Bus_error)

Er wordt een adres aangesproken dat niet bestaat. Zeer vreemd.

> **:question: Kan je de fout zien in vorig programma? Probeer dit zeker, maar spendeer er niet té lang aan!**

## Printf debugging

Debugging met print statements, soms [printf debugging genoemd](https://stackoverflow.com/questions/189562/what-is-the-proper-name-for-doing-debugging-by-adding-print-statements) (naar `printf` in C), is een zeer legitieme manier van debuggen.

Foutopsporing gebeurt bijvoorbeeld vaak met behulp van [logs](https://en.wikipedia.org/wiki/Log_file), tekstbestanden waar programma's interne output schrijven, die bekeken kunnen worden op het moment dat een programma gecrasht is.

Op Linux gebeurt dit bijvoorbeeld voortdurend.
In de folder `/var/log` kan je in Ubuntu logfiles terugvinden van vele programma's. Voer bijvoorbeeld onderstaand commando uit om zo'n log te inspecteren:

```bash
$ cat /var/log/bootstrap.log | less
```

Indien je ongeveer weet waar een fout zich voordoet, kan het toevoegen van een print-statement een snelle en eenvoudige manier zijn om te achterhalen wat misloopt.

Laten we printf debugging eens uitproberen op ons buggy programma van de introductie.
Misschien had je al wel het vermoeden dat de code crasht in één van de twee `if`-cases.
Er wordt namelijk maar één iteratie uitgevoerd van de for-loop, alvorens de start van de tweede iteratie, crasht het programma.
Dat zien we aan het feit dat het initiële print-statement maar één keer wordt uitgevoerd.

Loopt er dus misschien iets mis met de waarde van `i`?
Laten we eens een print-statement plaatsen in beide cases:

```c
if (i - 1 >= 0)
{
    printf("i: %u\n", i);
    printf("The value to my left is: %d\n", array[i - 1]);
}

if (i + 1 < array_length)
{
    printf("i: %u\n", i);
    printf("The value to my right is %d\n", array[i + 1]);
}
```

Wanneer we deze code uitvoeren, krijgen we:

```bash
$ ./a.out 
My current value is: 0
i: 0
Bus error (core dumped)
```

Dit maakt ons niet veel wijzer. Welk van de twee print-statements is zonet uitgevoerd?
Je zou op dit moment misschien tot de conclusie komen dat het if-statement met conditie `i - 1 >= 0` niet uitgevoerd zou moeten worden (want `i == 0`), en dat de code dus crasht in het print-statement van het tweede if-statement.

Dit illustreert meteen een nadeel van deze stijl van debugging.
Indien je bovenstaande conclusie zou trekken, zou je op een volledig fout spoor terecht kunnen komen.
Laten we even beide print-statements annoteren:

```c
if (i - 1 >= 0)
{
    printf("(1) i: %u\n", i);
    printf("The value to my left is: %d\n", array[i - 1]);
}

if (i + 1 < array_length)
{
    printf("(2) i: %u\n", i);
    printf("The value to my right is %d\n", array[i + 1]);
}
```

Wanneer we dit uitvoeren zien we:

```bash
$ ./a.out 
My current value is: 0
(1) i: 0
Bus error (core dumped)
```

Het is dus toch zo dat het eerste if-statement uitgevoerd wordt.
Blijkbaar is `0 - 1 >= 0`?!

> **:question: Kan je op basis van deze info zien wat er misloopt in dit programma?**

Misschien heb je nog niet kunnen verklaren waarom het if-statement uitgevoerd wordt, maar besef je al wel dat de index die we meegeven aan de array incorrect zal zijn.
Element 0 heeft namelijk geen linkse buur, dat was net de reden van het if-statement.
Blijkbaar leidt `array[i - 1]` met `i == 0` hier dus tot de *bus error*.
Door dat statement uit te voeren ga je inderdaad een geheugenadres aanspreken, in dit geval blijkbaar een geheugenadres dat niet bestaat.

Tijd om de waarde van `i - 1` te printen en het mysterie op te lossen:

```c
if (i - 1 >= 0)
{
    printf("(1) i: %u\n", i - 1);
    printf("The value to my left is: %d\n", array[i - 1]);
}
```

Dit geeft als resultaat:

```bash
$ ./a.out 
My current value is: 0
(1) i: 4294967295
Bus error (core dumped)
```

> **:question: Indien je nog niet weet waarom deze waarde van `i` geprint wordt, probeer dit te achterhalen door nogmaals naar het originele programma te kijken. Hoe kan het dat `i - 1` gelijk is aan een (zeer groot) positief getal, wanneer `i` gelijk is aan `0`?**.

`unsigned` (hetzelfde als `unsigned int`) is een *unsigned* type en kan dus nooit negatief zijn.
Wanneer we `i - 1` berekenen op het moment dat `i == 0`, krijgen we in plaats van `-1` de waarde `UINT_MAX`, het grootst mogelijke getal dat voorgesteld kan worden door een `unsigned int` op de machine.
Door `UINT_MAX` te gebruiken als index in een array, spreken we het geheugenadres `&array[0] + UINT_MAX * sizeof(array[0])` aan.
Dit stelt op mijn machine geen geldig geheugenadres voor, dus krijg ik een *bus error*.

Hier het gecorrigeerde programma:

```C
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t array_length = sizeof(array) / sizeof(array[0]);

    for (unsigned i = 0; i < array_length; i++)
    {
        printf("My current value is: %d\n", array[i]);

        if (i > 0) //fix 1
        {
            printf("The value to my left is: %d\n", array[i - 1]);
        }

        if (i < array_length - 1) //fix 2
        {
            printf("The value to my right is %d\n", array[i + 1]);
        }
        printf("\n");
    }
    return 0;
}
```

Je kan verifiëren dat dit programma wel correct uitvoert.

> :question: In dit programma hebben we ook het tweede if-statement aangepast. Waarom kan `i + 1 < array_length` voor problemen zorgen? Kan `i < array_length - 1` ook mislopen?

## `gdb`

Het is ons gelukt voorgaand probleem op te lossem met behulp `print`-statements.
Het was echter ook mogelijk dat we op een dwaalspoor waren beland, door foute conclusies te trekken over het pad dat de code aan het nemen was.
Door gebruik te maken van een debugger kunnen we het probleem veel sneller achterhalen.

Laten we onze code compileren met de debugflag `-g`.
Deze flag zorgt ervoor dat `gcc` debuginformatie in de gegenereerde executable verwerkt.
Onder andere referenties naar de C-broncode worden mee in de executable bewaard.
Hierdoor kan een debugger tonen welke broncode ervoor gezorgd heeft dat een fout zich voordeed.

> :warning: Je kan debuggen zonder `-g` executable, maar indien je de mogelijkheid hebt om de `-g` flag te gebruiken, zal dit het een stuk eenvoudiger maken. Zonder `-g` zal je enkel namen van functies en machinecode te zien krijgen.

Compileer het foute programma met `gcc -g`:

```bash
$ gcc -g buggy.c
```

Vervolgens zullen we het programma met `gdb` debuggen.
Geef de naam van de executable die je wil debuggen als argument aan `gdb`:

```bash
$ gdb a.out
```

Het `gdb` commando opent een interactieve debugger.
Dit laat ons toe om commando's te typen waarmee we code kunnen uitvoeren, pauzeren en de waarden van variabelen, geheugenlocaties of registers kunnen inspecteren op verschillende plaatsen in de code.

Het meest simpele commando is `run`.

* Open `gdb` met `a.out` en voer het commando `run` uit.


```gdb
(gdb) run
Starting program: /home/kobe/courses/iw/oefenzitting-c/les8-debugging/examples/a.out 
My current value is: 0

Program received signal SIGBUS, Bus error.
0x000055555555521d in main () at buggy.c:15
15                  printf("The value to my left is: %d\n", array[i - 1]);
```

We krijgen al een stuk meer informatie vergeleken met een normale uitvoering van het programma.
`gdb` toont ons dat lijn 15 in de broncode ervoor zorgt dat ons programma crasht.
Dit simpele commando geeft ons dus al evenveel info als alle print-statements die we eerder geplaatst hebben om te achterhalen waar de crash zich voordeed.

We kunnen vervolgens proberen te achterhalen wat de reden is van de crash.
Met het `print` commando kan je een GDB-expressie uitvoeren en daarvan het resultaat printen.
Deze expressie kan gebruik maken van variabelen of zelfs rechtstreeks van registerwaarden.
Laten we de waarde van `i` en `i - 1` printen:

```gdb
(gdb) print i
$1 = 0
(gdb) print i - 1
$2 = 4294967295
```

We hebben nu dus alle informatie gevonden die we eerder reeds achterhaald hadden met print statements, maar op een veel snellere en efficiëntere manier.
We moesten onze broncode niet aanpassen om print statements toe te voegen en zo de fout te zoeken.
Op het moment dat we de fout vonden, moesten we ons ook niet bezig houden met het verwijderen van alle onnodige print-statements.

Simpelweg het programma een keertje uitvoeren in de debugger bleek voldoende te zijn.

### Single stepping

Met `gdb` kan je een programma ook regel per regel uitvoeren.

Onderstaand programma ziet er misschien mysterieus uit.
Een debugger kan ons helpen te begrijpen wat dit programma net doet.

```C
#include <stdio.h>

int main(void)
{
    unsigned i = 1;
    while (i)
    {
        i <<= 1;
    }
    return 0;
}
```

Compileer het programma met de debug-flag:

```bash
gcc -g single-step.c
```

Open het programma vervolgens met `gdb`:

```bash
gdb a.out
```

Indien we nu gewoon het commandu `run` zouden uitvoeren, zien we niets gebeuren.
Een correct werkend programma zal gewoon volledig uitvoeren, zonder speciale boodschap:

```gdb
(gdb) run
Starting program: /home/kobe/courses/iw/oefenzitting-c/les8-debugging/examples/a.out 
[Inferior 1 (process 29056) exited normally]
```

Indien we het programma willen *single steppen*, moeten we starten door een breakpoint te plaatsen.
Door een breakpoint (pauzepunt) te plaatsen op een regel code, zorg je ervoor dat het programma gepauzeerd zal worden net vóór de regel code wordt uitgevoerd.
We zetten een breakpoint op de functie `main`:

```gdb
(gdb) break main
Breakpoint 1 at 0x555555555129: file single-step.c, line 4.
```

> :information_source: Voer het commando `tui enable` uit in `gdb` om een grafische weergave van de broncode te krijgen. Met `tui disable` kan je terug uit deze modus gaan. Dit kan handig zijn om overzicht te houden gedurende het debuggen.

Indien we het programma nu uitvoeren, zal de uitvoering pauzeren net voor de eerste regel code die in de functie `main` wordt uitgevoerd.

>:information_source: Je kan ook een breakpoint plaatsen op een specifieke regel van een bronbestand met het commando `break filename:line`.

Wanneer we nu `run` uitvoeren, pauzeert het programma aan onze breakpoint:

```gdb
(gdb) run
Starting program: /home/kobe/courses/iw/oefenzitting-c/les8-debugging/examples/a.out 

Breakpoint 1, main () at single-step.c:4
4	{
```

Nu kunnen we de commando's `step` of `next` gebruiken om verder te gaan in de code.
Beide voeren de regel code uit waarop het programma op dit moment gepauzeerd is.
Indien de regel code een functieoproep is, zal `step` naar de eerste regel van die functie springen.
`next` zal de volledige functie in één keer uitvoeren.

> :information_source: Wanneer je library-functies tegenkomt zoals `printf` of `malloc` is het vaak een goed idee erover te stappen met `next`.
> Je bent waarschijnlijk niet geinteresseerd in de interne werking van die functies.

Laten we de eerste regel van `main` uitvoeren:

```gdb
(gdb) step
5	    unsigned i = 1;
(gdb) print i
$2 = 0
```

Bij elke uitvoering van `step`, wordt de *volgende* regel code uitgevoerd. Regel 5 (`unsigned i = 1;`) is dus nog *niet* uitgevoerd. `i` heeft nog steeds een willekeurige waarde, in dit geval `0`.

```gdb
(gdb) step
6	    while (i)
(gdb) print i
$3 = 1
```

Na regel 5 uit te voeren zien we dat `i` inderdaad de waarde `1` toegewezen kreeg.
`while(i)` staat klaar om uitgevoerd te worden.

### Watchpoints

Ons doel van deze debuggingsessie is om te achterhalen hoe de waarde van `i` evolueert gedurende de uitvoering van ons programma.
We kunnen met `step` alles stap voor stap uitvoeren en na elk `step` commando kunnen we manueel `print i` uitvoeren.

Er is echter een gemakkelijkere manier om de evolutie van `i` te bekijken doorheen het programma: met een *watchpoint*.
Door een watchpoint te plaatsen op een variabele, zorg je ervoor dat je programma automatisch gepauzeerd wordt op het moment dat de variabele van waarde verandert.

Laten we een watchpoint plaatsen op `i`:

```gdb
(gdb) watch i
Hardware watchpoint 2: i
```

Vervolgens kunnen we ons programma gewoon verder laten uitvoeren met `continue`.
Het programma zal pauzeren bij het eerstvolgende breakpoint (die zijn er niet meer), of wanneer `i` van waarde verandert:

```gdb
(gdb) continue
Continuing.

Hardware watchpoint 2: i

Old value = 1
New value = 2
main () at single-step.c:6
6	    while (i)
```

We krijgen de melding dat de watchpoint op `i` getriggerd is. 
`i` is dus van waarde veranderd.

Merk op dat we opnieuw gepauzeerd zijn aan het begin van de `while`-lus, maar dat dit niet de regel is die `i` heeft aangepast.
De regel die net uitgevoerd werd, was de regel die `i` heeft aangepast.
Aangezien we terug naar het begin van de while-loop zijn gesprongen, moet dit de laatste regel van de lus geweest zijn, dus `i <<= 1`.

`i <<= 1` heeft ervoor gezorgd dat de waarde van `i` van 1 naar 2 is veranderd.
Laten we continue nog een paar keer uitvoeren om te kijken wat verder zal gebeuren met `i`.
Het commando `c` is kort voor `continue`:

```gdb
(gdb) c
Continuing.

Hardware watchpoint 2: i

Old value = 2
New value = 4
main () at single-step.c:6
6	    while (i)
(gdb) c
Continuing.

Hardware watchpoint 2: i

Old value = 4
New value = 8
main () at single-step.c:6
6	    while (i)
(gdb) c
Continuing.

Hardware watchpoint 2: i

Old value = 8
New value = 16
main () at single-step.c:6
6	    while (i)
```

Blijkbaar verdubbelt de waarde van `i` bij elke uitvoering van `i <<= 1`.
`<<` voert een logische left shift uit van een binair getal.
We shiften een binair getal `1` positie naar links, [wat hetzelfde is als een vermenigvuldiging met 2](https://math.stackexchange.com/questions/1610667/why-shifting-left-1-bit-is-the-same-as-multiply-the-number-by-2).

### Conditions

Stel dat we willen achterhalen waarom bovenstaand programma niet in een oneindige lus terecht komt, kunnen we gebruik maken van een *condition*.

Met een *condition* kunnen we onze code laten pauzeren op het moment dat een bepaalde, zelf gekozen expressie `true` wordt.

De while-lus zal stoppen op het moment dat `i == 0`.
We zouden dus een condition kunnen toevoegen aan onze watchpoint, zodat de watchpoint enkel triggert indien `i == 0`.

Dit doen we als volgt:

```gdb
(gdb) info watchpoints
Num     Type           Disp Enb Address            What
2       hw watchpoint  keep y                      i
	breakpoint already hit 5 times
(gdb) condition 2 i == 0
(gdb) c
Continuing.

Hardware watchpoint 2: i

Old value = 2147483648
New value = 0
main () at single-step.c:6
6	    while (i)
```

Merk op dat we de conditie moeten toevoegen op basis van het nummer van de watchpoint.
Met `info watchpoints` konden we dit nummer opvragen.
Met `info breakpoints` kan je dit doen voor alle breakpoints.

We zien dus dat `i << 1 == 0` als `i == 2147483648`.
Op mijn machine is een `unsigned int` 32-bit groot.
Laten we het getal binair voorstellen:

```python
>>> bin(2147483648)
'0b10000000000000000000000000000000'
```

Nog één shift naar links zal ervoor zorgen dat de meest linkse `1` vervangen wordt door een 0, de nieuwe waarde zal dus `0` zijn.

### Numerieke formaten

Hierboven hebben we python gebruikt om de omzetting te maken tussen decimale en binaire waarden.
`gdb` kan hier ook rechtstreeks voor gebruikt worden.

Stel dat we het bovenstaande programma nogmaals met `gdb` uitvoeren:

```gdb
(gdb) break main
Breakpoint 1 at 0x1129: file single-step.c, line 4.
(gdb) run
Starting program: /home/kobe/courses/iw/oefenzitting-c/les8-debugging/examples/a.out 

Breakpoint 1, main () at single-step.c:4
4	{
(gdb) watch i
Hardware watchpoint 2: i
(gdb) c
Continuing.

Hardware watchpoint 2: i

Old value = 0
New value = 1
main () at single-step.c:6
6	    while (i)
(gdb) c
Continuing.

Hardware watchpoint 2: i

Old value = 1
New value = 2
main () at single-step.c:6
6	    while (i)
```

Met `print i` kunnen we de waarde van `i` printen op het huidige moment.

```gdb
(gdb) print i
$1 = 2
```

Met `print /x i` kunnen we de waarde van `i` als hexadecimaal printen:

```gdb
(gdb) print /x i
$2 = 0x2
```
Met `print /t i` kunnen we de waarde van `i` als binair getal printen:

```gdb
(gdb) print /t i
$3 = 10
```
Het commando `display` werkt net als het print-commando, maar zal deze print automatisch herhalen iedere keer dat het programma gepauzeerd wordt:

```gdb
(gdb) display /x i
1: /x i = 0x2
(gdb) display /t i
2: /t i = 10
(gdb) c
Continuing.

Hardware watchpoint 2: i

Old value = 2
New value = 4
main () at single-step.c:6
6	    while (i)
1: /x i = 0x4
2: /t i = 100
(gdb) c
Continuing.

Hardware watchpoint 2: i

Old value = 4
New value = 8
main () at single-step.c:6
6	    while (i)
1: /x i = 0x8
2: /t i = 1000
```

### Call stack

Code gecompileerd met `gcc` maakt gebruik van een [*call stack*](https://en.wikipedia.org/wiki/Call_stack) om lokale variabelen en argumenten van functies in te bewaren.
In SOCS wordt deze structuur de stapel genoemd.

De call stack geeft een soort geschiedenis van functie-oproepen die uitgevoerd werden om op een bepaald punt in code terecht te komen.
Je kan de call stack eenvoudig inspecteren met behulp van `gdb`.

We illustreren met behulp van een lichtjes gemodificeerde versie van `linked-list.c` uit les 6.
Het bestand `examples/linked-list-buggy.c` bevat een implementatie van linked lists, maar er zit een fout in.

Indien je dit programma compileert en uitvoert, krijg je het volgende resultaat:

```bash
$ gcc linked-list-buggy.c
$ ./a.out 
Segmentation fault (core dumped)
```

> **:question: Stel dat we je vroegen om de fout te zoeken in `linked-list-buggy.c`. Hoe zou je dit aanpakken op basis van bovenstaande output? Segmentation fault betekent dat je in je code een geheugenadres aanspreekt dat je niet mag aanspreken.**

Bovenstaande fout zoeken met behulp van `print`-statements zal niet eenvoudig zijn.
Een veel snellere aanpak is het programma uit te voeren met een debugger.

We recompilen het programma met de debugflag `-g` en voeren het uit met `gdb`:

```bash
$ gcc -g linked-list-buggy.c
$ gdb a.out
```

Vervolgens voeren we het programma uit.
`gdb` zal automatisch pauzeren op de lijn die crasht:

``` gdb
(gdb) run
Starting program: /home/kobe/courses/iw/oefenzitting-c/les8-debugging/examples/a.out 

Program received signal SIGSEGV, Segmentation fault.
0x00005555555552d3 in list_length (list=0x5555555592a0) at linked-list-buggy.c:61
61          while ((current = current->next) != NULL)
```

De output geeft al aan dat de fout zich voordoet op regel 61 van `linkd-list-buggy.c`, in de functie `list_length`.

Op dit moment kunnen we kijken welke oproepen ons tot dit punt hebben gebracht, met behulp van `backtrace`:

```gdb
(gdb) backtrace
#0  0x00005555555552d3 in list_length (list=0x5555555592a0) at linked-list-buggy.c:61
#1  0x0000555555555329 in list_print (list=0x5555555592a0) at linked-list-buggy.c:79
#2  0x0000555555555414 in main () at linked-list-buggy.c:117
```

De functie `main` heeft de functie `list_print` opgeroepen, die vervolgens de functie `list_length` heeft opgeroepen.

Met `backtrace full` kunnen we ook de waarden van alle lokale variabelen in elk *stack frame* (activatierecord) uitprinten:

```gdb
(gdb) backtrace full
#0  0x00005555555552d3 in list_length (list=0x5555555592a0) at linked-list-buggy.c:61
        length = 0
        current = 0x0
#1  0x0000555555555329 in list_print (list=0x5555555592a0) at linked-list-buggy.c:79
        current = 0x0
        length = 32767
#2  0x0000555555555414 in main () at linked-list-buggy.c:117
        list = 0x5555555592a0
```

We zien dat de waarde `current` in de huidige functie naar `0x0` verwijst.
De regel code die ons programma doet crashen was de regel `while ((current = current->next) != NULL)`.
Nu we weten dat `current` gelijk is aan `0x0` kunnen we zien wat er misloopt.
`0x0` is op mijn machine (en de meeste machines) gelijk aan `NULL` en dus een ongeldig adres.
Dit wordt gebruikt om het einde van de linked list aan te geven.
We voeren de operatie `current->next` uit met `current == 0x0`.
Dit zal ervoor zorgen dat we het adres `0` gaan dereferencen, dit mag niet, en we krijgen een *segmentation fault*.

De oplossing is simpel.
We vervangen onze oude while lus:

```C
while ((current = current->next) != NULL)
{
    length++;
}
```

Door de nieuwe while-lus:

```C
while (current != NULL)
{
    length++;
    current = current->next;
}
```

Zo kan `current` nooit `NULL` zijn op het moment dat we deze dereferencen met `->`.


### Cheat sheet

Een handig document met vele `gdb` commando's kan je [hier](https://darkdust.net/files/GDB%20Cheat%20Sheet.pdf) terugvinden.
Alternatieve documenten kan je vinden door te googlen op [`gdb cheat sheet`](https://www.google.com/search?q=gdb+cheat+sheet).
Daarnaast kan je rechtstreeks binnen `gdb` gebruik maken van de `help` om commando's te ontdekken.


## `gdbgui`

Het is mogelijk om `gdb` te gebruiken met behulp van een grafische user interface (GUI), door middel van het programma `gdbgui`.

De installatie-instructies kan je [hier](https://www.gdbgui.com/installation/) vinden.

Onderstaande commando's zouden moeten werken op Ubuntu:

```bash
$ sudo apt install python3 python3-venv python3-pip
$ python3 -m pip install --user pipx
$ python3 -m userpath append ~/.local/bin
```
*Herstart* vervolgens je console, en voer volgend commando uit:

```bash
$ pipx install gdbgui
```

Vervolgens kan je programma's debuggen met het commando `gdbgui naam-programma`.
Je zal nog steeds `gdb`-commando's kunnen gebruiken, maar ook een visuele weergave krijgen van de code en datastructuren.

## `ddd`

Indien ik een datastructuur zoals een linked list grafisch wil debuggen, gebruik ik nog het liefst `ddd`, ook een wrapper rond `gdb`.
De grafische interface van dit programma is een stuk ouder dan `gdbgui`, maar de visualisaite van datastructuren is op het moment van schrijven nog een stuk duidelijker.

`ddd` kan geïnstalleerd worden met:

```
$ sudo apt install ddd
```

Met `ddd` kan je een programma debuggen door het commando `ddd naam-programma` uit te voeren.
Instructies over het gebruik van `ddd` om datastructuren zoals gelinkte lijsten te visualiseren kan je vinden op het einde van de video die bij deze les hoort.
