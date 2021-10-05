# Les 1: Pure C <!-- omit in toc -->

## Inhoudstafel <!-- omit in toc -->

- [Video](#video)
- [Introductie](#introductie)
- [Eerste C programma](#eerste-c-programma)
  - [Uitvoeren van een C programma](#uitvoeren-van-een-c-programma)
- [Simpel sommatieprogramma](#simpel-sommatieprogramma)
  - [Variabelen in C](#variabelen-in-c)
- [Functies in C](#functies-in-c)
  - [Functiedefinities](#functiedefinities)

## Video

[![Bekijk de video](https://img.youtube.com/vi/_KXa459UUiA/hqdefault.jpg)](https://www.youtube.com/watch?v=_KXa459UUiA)

## Introductie

Machinetalen (x86, RISC-V, DRAMA, ...) zijn de fundamentale talen waarmee machines geprogrammeerd worden.
Het is essentieel om deze talen te begrijpen en te beheersen om een volledig begrip te krijgen over de werking van een computer.

Grote programma's schrijven in een machinetaal is echter tijdsintensief en complex.
Een programma in machinecode bevat vaak patronen die telkens opnieuw terugkomen.
Denk bijvoorbeeld aan het pushen van waarden naar de call stack (DRAMA: Berg op STapel) bij iedere functieoproep.
Vele van deze patronen zijn telkens opnieuw hetzelfde, en kunnen dus ook automatisch gegenereerd worden.

De programmeertaal C biedt hier een oplossing voor.
C is één van de meest fundamentele programmeertalen.
Het is typisch de allereerste abstractielaag bovenop machinecode.
Programma's geschreven in C worden door *compilers* (vertalers) vertaald naar machinecode.
Machinecode kan dus automatisch gegenereerd worden op basis van een C-programma.

> :warning: C lijkt qua syntax meer op andere programmeertalen (Python, Java, JavaScript, ...) dan op machinetalen.
> Je hebt functies, variabelen, lussen, enzovoort.
> Correct programmeren in C vergt echter kennis die gelijkaardig is aan de kennis die je nodig hebt om te programmeren in machinetalen.
> Je moet kennis hebben van de geheugenlayout van je machine, werken met geheugenadressen, begrijpen hoe functie-oproepen werken, enzovoort.
> Probeer deze zittingen dus niet te *speedrunnen*.
> Indien je bepaalde concepten enkel imiteert en niet begrijpt, zul je gegarandeerd op een later moment in de problemen komen.

## Eerste C programma

Een C programma start typisch in de functie `main`.
De return-waarde van main geeft aan of het programma al dan niet correct is uitgevoerd.
We noemen dit de exit code van een programma.
De exit code `0` wil zeggen dat het programma correct is uitgevoerd.

* Schrijf onderstaande code in een tekstbestand genaamd `my-first-c-program.c`.

```C
int main(void)
{
    return 0;
}
```

### Uitvoeren van een C programma

Een C-programma kan niet rechtstreeks uitgevoerd worden.
Het moet eerst vertaald worden naar machinecode.
Hiervoor gebruiken we een *compiler* (vertaler).

* Compileer het C-programma met de standaard C compiler op Ubuntu: `gcc`

```bash
gcc my-first-c-program.c
```

* Verifieer dat er een bestand `a.out` is gegenereerd door `gcc` en dat dit bestand uitvoerbaar is. De `x` in de file permissions staat voor executable of uitvoerbaar.

```bash
ls -la
```

* Uitvoerbare bestanden bevatten machinecode. We kunnen verifiëren dat onze main-functie naar machinecode is vertaald door gebruik te maken van het programma `gdb` (de GNU debugger):

```bash
$ gdb a.out -batch -ex 'set disassembly-flavor intel' -ex 'disassemble main'

Dump of assembler code for function main:
   0x0000000000001129 <+0>:     endbr64
   0x000000000000112d <+4>:     push   rbp
   0x000000000000112e <+5>:     mov    rbp,rsp
   0x0000000000001131 <+8>:     mov    eax,0x0
   0x0000000000001136 <+13>:    pop    rbp
   0x0000000000001137 <+14>:    ret
End of assembler dump.
```

De machinecode die jullie hier zien is x86-64 machinecode, die uitgevoerd kan worden op 64-bit x86-processoren.
Laten we dit programma eens in detail bekijken.
In commentaar plaatsen we de equivalente DRAMA-instructies.

```nasm
endbr64         ;onbelangrijke instructie voor deze sessie
push   rbp      ;BST R8 | Voeg de oude frame pointer (omgevingswijzer) toe aan de stack/stapel
mov    rbp,rsp  ;HIA R8, R9 | Laad de huidige waarde van de stack pointer (stapelwijzer) in de frame pointer (omgevingswijzer)
mov    eax,0x0  ;HIA.w R0, 0 | Schrijf de waarde 0x0 (decimaal: 0) in register eax (het return-value register)
pop    rbp      ;HST R8 | Laad de waarde op de top van de stack (stapel) in de frame pointer (omgevingswijzer)
ret             ;KTG    | Spring naar de waarde op de top van de stack en pop deze waarde
```

> :information_source: Bij het vertalen van C code naar machinecode volg je typisch *calling conventions*. Dit zijn afspraken die je volgt, bijvoorbeeld welke registers gebruikt worden om gekende waarden (de stack pointer, frame pointer) in op te slaan. DRAMA heeft andere calling conventions dan x86. In DRAMA wordt de base pointer/omgevingswijzer aangepast *voor* een functieoproep, in x86 gebeurt dit in de functie zelf. In DRAMA wordt deze vervolgens ook hersteld na terugkeer van de functieoproep, in x86 net voor terugkeer.

Merk op dat het dus niet de C-broncode is die rechtstreeks uitgevoerd wordt (dat is niet mogelijk, een computer begrijpt geen C).
Het gecompileerde programma wordt door de machine uitgevoerd.

* Voer nu het programma uit.

```bash
./a.out
```

* Lees de exit-code van het laatst uitgevoerde programma uit. Dit zou de waarde 0 moeten geven.

```bash
echo $?
```

>:information_source: De exit-code moet je uitlezen zonder eerst andere commando's in de terminal uit te voeren. Bij elk commando dat je uitvoert wordt de exit-code opnieuw overschreven door de code van het laatst uitgevoerde programma.

* **Oefening** Pas de exit code van `my-first-c-program.c` aan naar het getal 43 en verifieer dat deze aanpassing werkt.

> :bulb: Wanneer je zaken aanpast in de broncode van een C-programma, vergeet dan niet om je broncode opnieuw te compilen naar machinecode! Het lijkt misschien logisch, maar dit wordt vaak over het hoofd gezien.

## Simpel sommatieprogramma

<!-- TODO naam sectie aanpassen -->

Het huidige programma doet nog niet zo veel.
Laten we een simpel programma schrijven dat de som uitrekent van de getallen `1..n`

```C
int main(void)
{
    int n = 10;
    int result = 0;
    for (int i = 1; i <= n; i++)
    {
        result += i;
    }
    return result;
}
```

> :bulb: Merk op dat elke regel in C wordt beëindigt door een `;` en elke blok code wordt omvat tussen `{` en `}`. De C-syntax is zeer strikt. Indien je een fout maakt zal de C-compiler je programma niet vertalen.

* **Oefening** Compileer en voer het bovenstaande programma uit. Verifieer dat de exit-code van het programma gelijk is aan de som van de getallen `1..10`.

### Variabelen in C

Merk op dat elke variabelenaam voorafgegaan wordt door `int`.
Dit is een datatype.
Elke variabele in C heeft een vast datatype.
Dit datatype wordt vastgelegd op het moment dat je de variabele definieert.

De C-compiler gebruikt datatypes onder andere om exact te bepalen hoeveel ruimte (bytes) er nodig zijn om een bepaalde variabele te bewaren.
Zo kan een compiler bijvoorbeeld weten of de variabele bewaard kan worden in een register, of hoeveel bytes er op de stack (stapel) gereserveerd moeten worden wanneer de variabele daar geplaatst moet worden.

Op datatypes gaan we dieper in gedurende de derde C-les.
Voorlopig is het voldoende te weten dat een `int` (*integer*) een geheel getal bevat.

* **Oefening** Schrijf een C programma dat de faculteit berekent van een getal `n` (dus `n!`) en dit teruggeeft via de exit code.

## Functies in C

De [manhattan distance](https://en.wikipedia.org/wiki/Taxicab_geometry) tussen twee coördinaten `(x1, y1)` en `(x2, y2)` is de kortste afstand die je moet afleggen om van het ene coördinaat naar het andere te wandelen, indien je niet diagonaal kan wandelen (voor een betere uitleg verwijzen we naar Wikipedia). De formule om deze afstand tussen twee punten te berekenen is `|x2 - x1| + |y2 - y1|`.

Onderstaand C-programma berekent de manhattan distance tussen twee coördinaten:

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

int manhattan_distance(int x1, int x2, int y1, int y2)
{
    return absolute_value(x2 - x1) + absolute_value(y2 - y1);
}

int main(void)
{
    return manhattan_distance(10, 0, 0, 10);
}
```

Laten we eens kijken naar de syntax van de drie functies in dit bestand.

* De functie `absolute_value` neemt als parameter een variabele van het type `int`, en geeft als resultaat ook een `int` terug.
* De functie `manhattan_distance` heeft vier parameters van het type `int` en geeft als resultaat ook een `int` terug.
* De functie `main` heeft geen enkele parameter. Dit wordt aangegeven door het keyword `void`. Main geeft ook als resultaat een `int` terug.

> :warning: Een parameterlijst leeg laten in C is niet hetzelfde als expliciet `void` te schrijven tussen de haakjes. De lege parameterlijst wordt beschouwd als een *ongespecifieerd* aantal parameters. [Meer info](https://softwareengineering.stackexchange.com/questions/286490/what-is-the-difference-between-function-and-functionvoid)

Tijd om het programma uit te voeren.

* Compileer het programma en voer het uit. Verifieer dat de exit code van het programma gelijk is aan `20`.

Laten we eens kijken hoe functie-oproepen vertaald worden in C.

* Bekijk de machinecode van de functie `manhattan_distance`

```bash
gdb a.out -batch -ex 'set disassembly-flavor intel' -ex 'disassemble manhattan_distance'
```

Laten we dit eens vertalen naar DRAMA:

```nasm
endbr64                             ;niet belangrijk
push   rbp                          ;BST R8
mov    rbp,rsp                      :HIA R8, R9
push   rbx                          :BST R1
sub    rsp,0x10                     ;AFT.w R9, 16
mov    DWORD PTR [rbp-0xc],edi      ;BIG R4, -12(R8)  
mov    DWORD PTR [rbp-0x10],esi     ;BIG R5, -16(R8)
mov    DWORD PTR [rbp-0x14],edx     ;BIG R3, -20(R8)
mov    DWORD PTR [rbp-0x18],ecx     ;BIG R2, -24(R8)
mov    eax,DWORD PTR [rbp-0x10]     ;HIA R0, -16(R8)
sub    eax,DWORD PTR [rbp-0xc]      ;AFT.d R0, -12(R8)
mov    edi,eax                      ;HIA R4, R0
call   0x1129 <absolute_value>      ;SBR absolute_value
mov    ebx,eax                      ;HIA R1, R0
mov    eax,DWORD PTR [rbp-0x18]     ;HIA R0, -24(R8)
sub    eax,DWORD PTR [rbp-0x14]     ;AFT.d R0, -20(R8)
mov    edi,eax                      ;HIA R4, R0
call   0x1129 <absolute_value>      ;SBR absolute_value
add    eax,ebx                      ;OPT R0, R1
add    rsp,0x10                     ;OPT.w R9, 16
pop    rbx                          ;HST R1
pop    rbp                          ;HST R8
ret                                 ;KTG
```

> :information_source: Getallen die starten met de prefix `0x` zijn hexadecimale getallen! `0x10` is dus niet gelijk aan het decimale getal `10` maar wel het decimale getal `16`.

<!-- ignore linter -->

> :information_source: In DRAMA neemt een `int` slechts 1 geheugencel in. In x86 is elke geheugencel 1 byte groot en wordt een `int` voorgesteld door 4 bytes, dus 4 geheugencellen. Daarom is het verschil tussen de verschillende adressen dus telkens 4 in plaats van 1.

We zien hier dat de parameters `x1`, `x2`, `y1` en `y2` via registers `edi`, `esi`, `edx` en `ecx` werden doorgegeven.
Aan het begin van de functie werden al deze parameters op de call stack (stapel) bewaard.
Vervolgens wordt het register `eax` gebruikt om `x2 - x1` te berekenen.
Via `edi` wordt dit resultaat doorgegeven aan de functie `absolute_value`.
Dit herhaalt zich voor `y2 - y1`.
Het resultaat van de functie wordt uiteindelijk in `eax` geplaatst.

Het volledig begrijpen van machinecode is uiteraard meer iets voor het vak SOCS.
Wat wel *belangrijk* is, is dat je ziet dat functievariabelen (parameters of lokale variabele) bewaard worden op de call stack.
Dit heeft grote gevolgen voor wat wel en niet mogelijk is in C.
We komen hier in toekomstige sessies zeker op terug.

* **Oefening** Schrijf een C-programma met een functie die de [rij van fibonacci](https://en.wikipedia.org/wiki/Fibonacci_number) recursief uitrekent. Geef het 10de fibonaccigetal terug als exit code van het programma.

### Functiedefinities

C (de opvolger van de programmeertaal B [[1]](https://en.wikipedia.org/wiki/C_(programming_language))) is een zeer oude taal, ontwikkeld in 1972 en 1973.
De machines waarop C compilers vroeger werkten hadden niet veel geheugen.
Dat had invloed op de manier waarop C gecompileerd werd.

Een C-compiler werkte traditioneel als volgt: regel per regel werd een bestand ingelezen. Elke regel werd rechtstreeks omgezet naar een regel machinecode, dat bewaard werd in het outputbestand.
De C-compiler moest elke regel kunnen vertalen zonder verder te moeten kijken in het bestand dat gelezen werd - dit zou inefficiënt zijn en te veel geheugen vergen.

Hierdoor kan enkel de informatie die de C compiler reeds eerder in het bestand tegengekomen is, gebruikt worden bij de vertaling van een regel code.
Dit leidt ons tot een belangrijke regel in C: een functie mag niet opgeroepen worden voordat deze functie *gedeclareerd* is.
De compiler kan namelijk onmogelijk een functie-oproep vertalen naar een functie die nog onbekend is.

* Wissel de functies `manhattan_distance` en `absolute_value` van plaats in het voorgaande C-programma. Probeer dit te compileren.

```bash
$ gcc functions.c
functions.c: In function ‘manhattan_distance’:
functions.c:3:12: warning: implicit declaration of function ‘absolute_value’ [-Wimplicit-function-declaration]
    3 |     return absolute_value(x2 - x1) + absolute_value(y2 - y1);
      |
```

De compiler geeft een waarschuwing: de functie `absolute_value` werd *impliciet* gedeclareerd.
Dit wil zeggen dat de compiler de functie niet herkent (de functiedefinitie staat pas later in het bestand), en vervolgens op eigen houtje een functiedeclaratie genereert.
**Doe dit nooit!**
Dit is een vorm van [*undefined behavior*](https://en.wikipedia.org/wiki/Undefined_behavior).
De C-standaard specifieert niet wat er moet gebeuren in deze situatie.
Ook al zal een goede compiler waarschijnlijk doen wat je verwacht dat er zou moeten gebeuren, dit is helemaal geen garantie.
Undefined behavior moet ten alle tijden vermeden worden.
Het kan leiden tot zeer lastige bugs in je programma.

Om te verhinderen dat we dit soort fouten maken, gaan we ons compilatiecommando uitbreiden.

* Probeer je programma opnieuw te compileren met de flags `-Wall -Werror -pedantic-errors -std=c17`:

```bash
$ gcc functions.c -Wall -Werror -pedantic-errors -std=c17
functions.c: In function ‘manhattan_distance’:
functions.c:3:12: error: implicit declaration of function ‘absolute_value’ [-Wimplicit-function-declaration]
    3 |     return absolute_value(x2 - x1) + absolute_value(y2 - y1);
      |            ^~~~~~~~~~~~~~
```

Deze compilatieflags hebben het volgende effect:

* `-Wall` zet alle waarschuwingen die een compiler kan geven aan.
* `-Werror` zorgt ervoor dat waarschuwingen omgezet worden in errors. Je wordt gedwongen om code met een waarschuwing te corrigeren.
* `-std=c17` zorgt ervoor dat onze C-code moet voldoen aan de regels van de nieuwste C-standaard (uit 2017).
* `-pedantic-error` zorgt ervoor dat code die niet voldoet aan de standaarden ook compiler errors genereert

Ons foute programma kan met deze strikte instellingen niet meer gecompileerd worden.
Gebruik in de toekomst *altijd* bovenstaande flags.
De regels van C breken is nooit een goed idee.

Tijd om ons foutieve programma te repareren.
We gaan de functies echter niet terug van plaats verwisselen.
In plaats daarvan maken we gebruik van een expliciete functiedeclaratie.

Met een functiedeclaratie kan je een functie *declareren* zonder deze te *definiëren*.
Je zegt tegen de compiler dat de functie bestaat en dat de implementatie (definitie) van deze functie pas op een later moment volgt.

De functiedeclaratie van de functie `absolute_value` schrijf je als volgt:

```c
int absolute_value(int a);
```

* Voeg bovenstaande functiedeclaratie toe bovenaan je C-bronbestand. Merk op dat het programma terug compileert zonder fouten.

* **Oefening** Schrijf een `C`-programma dat berekent of een getal even of oneven is, gebaseerd op onderstaand python-programma. Zorg ervoor dat je het programma kan compileren met de flags `-Wall -Werror -pedantic-errors -std=c17`.

```python
def even(n):
    if n == 0:
        return 1
    else:
        return odd(n-1)

def odd(n):
    if n == 0:
        return 0
    else:
        return even(n-1)
```

<!-- TODO move this to next session

> :bulb: De `gcc` compiler zal standaard je bronbestand van begin tot eind lezen, regel per regel, en elke regel vertalen naar machinecode.
> Dit zorgt voor een zo kort mogelijke compilatietijd.
> Het is echter mogelijk om gcc te vragen om meer tijd te steken in compilatie en de machinecode te *optimaliseren*.
> Dit kan je doen met de optimalisatieflags van gcc.
> Compileer je bestand met het commando `gcc bestand.c -O3` en bekijk de nieuwe machinecode.
> Wat valt op? Waarom werkt dit?
 
-->
