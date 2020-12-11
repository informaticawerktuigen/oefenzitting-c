# Makefiles en tests <!-- omit in toc -->

## Inhoudstafel <!-- omit in toc -->

- [Video](#video)
- [Introductie](#introductie)
- [Makefiles](#makefiles)
	- [Dependencies](#dependencies)
	- [Chains](#chains)
	- [Patterns](#patterns)
	- [Variables](#variables)
	- [C Makefile](#c-makefile)
- [Tests](#tests)
	- [Voorbeeldproject: Linked lists](#voorbeeldproject-linked-lists)
		- [Test suite](#test-suite)


## Video

[![Bekijk de video](https://img.youtube.com/vi/tMGpCLuSv3M/hqdefault.jpg)](https://www.youtube.com/watch?v=tMGpCLuSv3M)

## Introductie

Een tool die helpt bij automatisatie van compilatie, linking en ook testing van projecten, is de build tool `make`.
Deze tool zullen we in deze les introduceren.
Vervolgens wordt kort gepraat over testen in C en hoe `make` ons kan helpen deze tests te automatiseren.

> :warning: `make` en tests zijn niet direct gerelateerd aan elkaar. In de toekomst zullen dit waarschijnlijk twee aparte lessen worden. Dit jaar hebben we ervoor gekozen om deze kort samen te bespreken.

## Makefiles

`make` is een programma waarmee automatisch bestanden gegenereerd kunnen worden volgens een voorgeprogrammeerde procedure.
Indien je een project hebt, waarbij bepaalde bestanden automatisch gegenereerd worden door het uitvoeren van bepaalde commando's op andere bestanden (denk aan compilatie), kan `make` een zeer goede tool zijn om te gebruiken.

`make` werkt op basis van een bestand genaamd `Makefile`.
Wanneer we het commando `make` uitvoeren, zal gezocht worden naar een `Makefile`.

Laten we starten met een simpel voorbeeld.

```Makefile
hello_world:
	echo "Hello, world!" > $@
```

In deze simpele `Makefile` schrijven we een script dat het bestand `hello_world` genereert.
`hello_world` is een voorbeeld van een *make target*, een bestand dat gegenereerd moet worden door `make` op basis van de `Makefile`.
De manier waarop `hello_world` gegenereerd moet worden, wordt beschreven in het commando eronder :`echo "Hello, world! > $@`.
In dit commando zal `$@` worden vervangen door de naam van het target, dus `hello_world`.

Het commando `make` zal een `Makefile` standaard uitvoeren door te zoeken naar het eerste target en vervolgens de commando's uitvoeren die horen bij dat target.

Laten we bovenstaande `Makefile` eens uittesten:

```console
foo@bar:examples/make/simple$ ls
Makefile
foo@bar:examples/make/simple$ make
echo "Hello, world!" > hello_world
foo@bar:examples/make/simple$ ls
hello_world  Makefile
foo@bar:examples/make/simple$ cat hello_world 
Hello, world!
```

`make` zal de commando's die het uitvoert printen naar de `stdout`.
Zo kan je meteen zien welke commando's uitgevoerd werden.
Zoals verwacht is een bestand `hello_world` aangemaakt met het `echo` commando.

Stel dat we `make` nogmaals uitvoeren:

```console
foo@bar:examples/make/simple$ make
make: 'hello_world' is up to date.
```

`make` zal het echo-commando niet opnieuw uitvoeren, het targetbestand `hello_world` bestaat al en is blijkbaar *up to date*.
Om uit te leggen wat `make` bedoelt met *up to date*, geven we een iets complexer voorbeeld.

### Dependencies

```Makefile
word_count: count_me
	wc -w $< > $@
```

Bovenstaande `Makefile` genereert een target file genaamd `word_count`.
We zien echter een tweede bestand gespecifieerd na de dubbelpunt, namelijk `count_me`.
`count_me`, ook een bestand, is een dependency van `word_count`.
Dat wil zeggen dat je het bestand `count_me` nodig hebt om `word_count` te genereren.

Het commando zelf voert `wc -w` (telt het aantal woorden) uit op bestand `$<`.
`$<` zal vervangen worden door de eerste dependency.
Indien we `make` uitvoeren, zien we:

```console
foo@bar:examples/make/dependency$ ls
count_me  Makefile
foo@bar:examples/make/dependency$ cat count_me 
How many words does this file contain?
foo@bar:examples/make/dependency$ make
wc -w count_me > word_count
foo@bar:examples/make/dependency$ ls
count_me  Makefile  word_count
foo@bar:examples/make/dependency$ cat word_count 
7 count_me
```

Onze `Makefile` telt dus het aantal woorden in `count_me` en maakt een nieuw bestand `word_count` met deze informatie.
Indien we opnieuw `make` uitvoeren, zien we hetzelfde resultaat als eerder:

```console
foo@bar:examples/make/dependency$ make
make: 'word_count' is up to date.
```

`make` ziet geen reden om het commando opnieuw uit te voeren.
`count_me` is namelijk nog steeds hetzelfde bestand, het resultaat zal dus ook niet anders zijn.
Wanneer we `count_me` echter aanpassen, zal `make` het commando *wel* opnieuw uitvoeren:

```console
foo@bar:examples/make/dependency$ echo "And now?" > count_me
foo@bar:examples/make/dependency$ cat count_me 
And now?
foo@bar:examples/make/dependency$ make
wc -w count_me > word_count
foo@bar:examples/make/dependency$ cat word_count 
2 count_me
```

Merk dus op dat `make` kijkt naar de *dependencies* om te beslissen of het commando opnieuw uitgevoerd moet worden.
Indien de dependencies niet gewijzigd zijn sinds de laatste uitvoering van `make`, zal het commando niet opnieuw uitgevoerd moeten worden.
`make` doet dit door te kijken naar de datum van de laatste aanpassing van een bestand.
Indien een dependency aangepast werd na de laatste wijziging van de target, zal het target opnieuw gegenereerd worden.

> :information_source: Indien je `make` gebruikt om C code te compileren, wil je dat telkens wanneer een `.c` bestand gewijzigd wordt, het overeenkomstige `.o` bestand ook opnieuw gegenereerd wordt.

> **:question: Schrijf een Makefile die een bestand `line_count` aanmaakt met daarin het aantal lijnen van een inputbestand. Genereer het bestand met `make`, voeg nadien een lijn toe aan het inputbestand en voer `make` opnieuw uit.**

### Chains

Indien een *dependency* niet bestaat, zal `make` proberen om deze dependency te genereren.
`make` gaat kijken of er in de `Makefile` een *regel* staat over hoe dit bestand gegenereerd kan worden.

```Makefile
word_count: count_me
	wc -w $< > $@

count_me:
	echo "Try to count this!" > $@
```

Stel dat we bovenstaande `Makefile` in een lege folder uitvoeren.
`make` kan `word_count` niet genereren, want `count_me` bestaat niet.
Er is echter een *rule* waarmee `count_me` gegenereerd kan worden.
`make` zal eerst `count_me` genereren met deze *rule* en pas nadien *word_count* genereren.

```console
foo@bar:examples/make/chains$ ls
Makefile
foo@bar:examples/make/chains$ make
echo "Try to count this!" > count_me
wc -w count_me > word_count
foo@bar:examples/make/chains$ ls
count_me  Makefile  word_count
```

Merk op dat `make` dus *chains* (kettingen) maakt van dependencies.

> :information_source: Om een executable te maken op basis van `.c` broncode voer je typisch twee stappen uit. Compileer de `.c` files naar `.o` files, en link vervolgens de `.o` files tot een executable. Deze procedure is met `make` eenvoudig te automatiseren met behulp van chains.

### Patterns

Stel dat we onze word counter `Makefile` willen toepassen op verschillende bestanden met dezelfde extensie.
Elk bestand met extensie `.countme` moet geteld worden.
De telling moet bewaard worden in een bestand met extensie `.counted`.
Dit kan eenvoudig, met behulp van *patterns*:

```Makefile
.PHONY: count
count: 1.counted 2.counted 3.counted

%.counted: %.countme
	wc -w $< > $@
```

Deze `Makefile` heeft 1 target: `count`.
`count` is echter een `PHONY` target.
Hiermee geven we aan dat we geen bestand willen genereren met de naam `count`, we willen enkel de commando's van het target uitvoeren.
Dit target heeft echter ook geen bijhorende commando's.

Het `PHONY` target heeft wel een lijst van dependencies: `1.counted`, `2.counted` en `3.counted`.
Aangezien alle dependencies van een target gegenereerd moeten worden, zorgt het `.PHONY` target ervoor dat `1.counted`, `2.counted` en `3.counted` gegenereerd zullen worden.
Zonder een `PHONY` target te gebruiken hadden we drie aparte targets moeten opstellen.

Verder heeft deze `Makefile` een regel die gebruikt maakt van *pattern matching*.
`%.counted` matcht met elk bestand dat eindigt op `.counted`.
Met `%.counted: %.countme` zeggen we dat, om een bestand met de extensie `.counted` te genereren, we als dependency een bestand met dezelfde naam hebben, maar dan met extensie `%.countme`.

Indien we deze `Makefile` uitvoeren in een folder met de bestanden `1.countme`, `2.countme` en `3.countme` zullen de overeenkomstige `.counted` bestanden automatisch gegenereerd worden:

```console
foo@bar:examples/make/patterns$ ls
1.countme  2.countme  3.countme  Makefile
foo@bar:examples/make/patterns$ make
wc -w 1.countme > 1.counted
wc -w 2.countme > 2.counted
wc -w 3.countme > 3.counted
foo@bar:examples/make/patterns$ ls
1.counted  1.countme  2.counted  2.countme  3.counted  3.countme  Makefile
```

> **:question: Een pattern dat je vaak zal tegenkomen in de `Makefile` van een C-project is `%.o: %.c`. Begrijp je nu wat dit betekent?**

### Variables

Bovenstaande `Makefile` werkt enkel door de verschillende `.counted` targets te hardcoden.
Met behulp van het commando `find` kunnen we echter automatisch alle bestanden die eindigen op `.countme` vinden:

```console
foo@bar:examples/make/variables$ find ./ -name "*.countme"
./3.countme
./2.countme
./1.countme
```

We kunnen dit commando rechtstreeks vanuit een `Makefile` gebruiken:

```Makefile
INPUT_FILES := $(shell find ./ -name "*.countme")
```

Deze regel definieert een variabele `INPUT_FILES` op basis van het resultaat van het shellcommando gespecifieerd met `$(shell <commando>)`.
In de folder `examples/make/variables` zal `INPUT_FILES` dus gelijk zijn aan `./3.countme ./2.countme ./1.countme`.

Dit zijn echter onze dependencies, niet onze targets.
Onze vorige `Makefile` specifieerde de targets `*.counted`, niet de `*.countme` files.
We kunnen de targets berekenen op basis van de input files, als volgt:

```Makefile
INPUT_FILES := $(shell find ./ -name "*.countme")
TARGETS := $(INPUT_FILES:.countme=.counted)
```

De variabele `TARGETS` zal hier dus gelijk zijn aan `./3.counted ./2.counted ./1.counted`, exact de lijst targets de we zoeken.
We kunnen de waarde van deze variabele gebruiken in een regel door `$(TARGETS)` te schrijven, dit geeft ons de volgende `Makefile`:

```Makefile
INPUT_FILES := $(shell find ./ -name "*.countme")
TARGETS := $(INPUT_FILES:.countme=.counted)

.PHONY: count
count: $(TARGETS)

%.counted: %.countme
	wc -w $< > $@
```

Deze `Makefile` zal *alle* bestanden met extensie `*.countme` omzetten naar bestanden met extensie `*.counted` door het `wc -w` commando te gebruiken:

```console
foo@bar:examples/make/variables$ ls
1.countme  2.countme  3.countme  Makefile
foo@bar:examples/make/variables$ make
wc -w 3.countme > 3.counted
wc -w 2.countme > 2.counted
wc -w 1.countme > 1.counted
foo@bar:examples/make/variables$ ls
1.counted  1.countme  2.counted  2.countme  3.counted  3.countme  Makefile
```

> **:question: Schrijf een `Makefile` die voor elk bestand met extensie `.countlines` een gelijknamig outputbestand genereert met extensie `.countedlines`. In het outputbestand wordt het aantal lijnen van het invoerbestand bewaard.**

### C Makefile

In de folder `examples/make/c` hebben we een voorbeeld geplaatst van een simpel `C`-project dat automatisch gecompileerd wordt met behulp van een `Makefile`.
De flags voor de compiler en linker bewaren we voor de duidelijkheid in een variabele.

```Makefile
SOURCE_FILES := $(shell find ./ -name "*.c")
OBJECT_FILES := $(SOURCE_FILES:.c=.o)
COMPILE_FLAGS := -Wall -Werror -std=c17 -pedantic
LINK_FLAGS := -lm
EXECUTABLE := my_executable

$(EXECUTABLE): $(OBJECT_FILES)
	gcc $^ -o $@ $(LINK_FLAGS)

%.o: %.c
	gcc -c $< $(COMPILE_FLAGS)

.PHONY: clean
clean:
	rm -f $(EXECUTABLE) $(OBJECT_FILES)
```

> :information_source: De linker flag `-lm` linkt de `math` library waarin de functies `pow` (machtsverheffing) en `sqrt` (vierkantswortel) gedefinieerd staan.

Deze `Makefile` compileert *alle* `.c` files in een folder en linkt deze samen tot een executable genaamd `my_executable`:

```console
foo@bar:examples/make/c$ ls
carthesian.c  carthesian.h  main.c  Makefile  print.c  print.h
foo@bar:examples/make/c$ make
gcc -c carthesian.c -Wall -Werror -std=c17 -pedantic
gcc -c print.c -Wall -Werror -std=c17 -pedantic
gcc -c main.c -Wall -Werror -std=c17 -pedantic
gcc carthesian.o print.o main.o -o my_executable -lm
foo@bar:examples/make/c$ ls
carthesian.c  carthesian.h  carthesian.o  main.c  main.o  Makefile  my_executable  print.c  print.h  print.o
```

Het `clean` target is een standaard `PHONY` target (genereert dus zelf geen bestand) dat gebruikt kan worden om alle automatisch gegenereerde bestanden automatisch te verwijderen:

```console
foo@bar:examples/make/c$ ls
carthesian.c  carthesian.h  carthesian.o  main.c  main.o  Makefile  my_executable  print.c  print.h  print.o
foo@bar:examples/make/c$ make clean
rm -f my_executable ./carthesian.o ./print.o ./main.o
foo@bar:examples/make/c$ ls
carthesian.c  carthesian.h  main.c  Makefile  print.c  print.h
```


<!-- all, clean -->

## Tests

Een zeer effectieve strategie bij het schrijven van goede code, is om je functionaliteit op te delen in kleine, korte functies.
Elke functie krijgt een eigen taak en doet exact één ding.
Een complexe functie zal vele kleinere functies oproepen, die op hun beurt eventueel nog kleinere functies oproepen, enzovoort.

Dit heeft vele voordelen.
Je code wordt overzichtelijker, meer leesbaar, je kan duplicatie in de code vermijden door functies te hergebruiken, ... .
Deze strategie gaat ook hand in hand met het schrijven van testen.

Wanneer je een functie schrijft met een zeer duidelijk afgeleinde taak, kan je eenvoudig code schrijven die test of deze functie werkt zoals je zou verwachten.
Geef voorbeeldinputs en controleer of de functies de verwachtte resultaten teruggeven.
Indien je dit consequent doet voor al je functies, zal je veel minder frequent tegen fouten aanlopen.

De tests kan je vervolgens op regelmatige basis uitvoeren, bijvoorbeeld na elke compilatie opnieuw.
Wanneer je code aanpast, kan een goede test suite je meteen vertellen welk deel van je programma niet meer werkt, en vaak ook waarom.

### Voorbeeldproject: Linked lists

In `examples/unit-tests` hebben we een voorbeeldproject aangemaakt.
Dit project gebruikt een *linked list* datastructuur.
De gegevensstructuur zelf wordt gecompileerd tot een library, zodat verschillende programma's gebruik kunnen maken van deze gegevensstructuur.

De gegevensstructuur wordt geïmplementeerd in de `lib/` folder.
De `Makefile` zal automatisch de C-bestanden in `lib/` compileren en linken tot een library `liblinkedlist.a`.
Vervolgens kan het programma, met broncode in `src/`, gebruik maken van deze gegevensstructuur.
De `Makefile` zal de code in `src/` compileren en linken aan `liblinkedlist.a`.

#### Test suite

Het project heeft echter ook een uitgebreide test suite
Het bestand `test/linked-list-test.c` bevat *unit tests* voor elke linked list functie (behalve de printfunctie).
[Unit tests](https://en.wikipedia.org/wiki/Unit_testing) gaan op een geautomatiseerde wijze bepaalde *units* in je programma (bijvoorbeeld functies) uitgebreid testen.

Hier zie je een voorbeeld van de tests die uitgevoerd worden voor `list_append`:

```C
void list_append_test()
{
    //We don't use list_length since it might not yet be implemented
    struct List *list = list_create();
    assert(list != NULL);

    fprintf(stdout, " - Testing empty list\n");
    assert(list_append(list, 1) == OK);

    struct ListNode *node = list->first;
    assert(node->value == 1);
    assert(node->next == NULL);

    fprintf(stdout, " - Testing nonempty list\n");
    assert(list_append(list, 2) == OK);

    node = node->next;
    assert(list->first->next == node);
    assert(node->next == NULL);
    assert(node->value == 2);

    assert(list_append(list, 3) == OK);

    node = node->next;
    assert(list->first->next->next == node);
    assert(node->next == NULL);
    assert(node->value == 3);

    fprintf(stdout, " - Testing uninitialized list\n");
    assert(list_append(NULL, 0) == UNINITIALIZED_LIST);
}
```

Merk op dat het gebruik van de functie `list_append` getest wordt op verschillende manieren.
Met `assert` controleren we telkens of de oproep naar `list_append` het gewenste effect had.

> :information_source: `assert` in C controleert of de meegegeven conditie al dan niet `true` is. Indien de conditie `false` is, stopt het programma op dat punt met uitvoeren en wordt de falende lijn gerapporteerd naar de output.

> :warning: Wanneer je `assert` gebruikt in een test suite moet je goed opletten. Indien de flag `-DNDEBUG` gebruikt wordt om je programma te compileren worden `assert` statements genegeerd. Deze simpele test suite mag dus nooit met die flag gecompileerd worden. Grote projecten maken typisch gebruik van test frameworks met een verzameling speciale functies die gebruikt kunnen worden op de manier waarop wij hier `assert` gebruiken. In de testsuite van het project gebruiken we een *macro* in plaats van `assert`.

Stel dat we de implementatie van `list_append` even kapot maken.
We vervangen deze door:

```C
status list_append(struct List *list, int value)
{
    return OK;
}
```

Indien we nu ons project builden met `make`, zien we dit resultaat:

```console
foo@bar:examples/unit-tests$ make
gcc -c lib/linked-list.c -o lib/linked-list.o -Wall -Werror -std=c17 -pedantic
ar rcs liblinkedlist.a lib/linked-list.o
gcc test/linked-list-tests.o liblinkedlist.a -o test_suite 
./test_suite
Starting unit tests...

Starting list_create_test
[OK] list_create

Starting list_append_test
 - Testing empty list
[ERROR] list_append

Starting list_length_test
 - Testing length after appends
test_suite: test/linked-list-tests.c:56: list_length_test: Assertion `list_length(list) == i' failed.
[ERROR] list_length

Starting list_get_test
 - Testing after appends
test_suite: test/linked-list-tests.c:74: list_get_test: Assertion `list_get(list, i, &retval) == OK' failed.
[ERROR] list_get

Starting list_remove_item_test
 - Testing on empty list
 - Testing removal of first element
test_suite: test/linked-list-tests.c:102: list_remove_item_test: Assertion `list_remove_item(list, 0) == OK' failed.
[ERROR] list_remove_item

Starting list_delete_test
 - Testing empty list
 - Testing uninitialized list
[OK] list_delete

Starting list_insert_test
 - Testing out of bounds accesses
 - Testing empty list insert
 - Testing front insert
 - Testing back insert
 - Testing middle insert
[OK] list_insert

Unit tests complete (3/7 succesful).
make: *** [Makefile:36: test_suite] Error 255
```

Onze testsuite faalt meteen op verschillende punten.
Slechts 3/7 tests werken.
Dat komt omdat de tests voor `list_get`, `list_length` en `list_remove_item` ook gebruik maken van `list_append`.

Wat misschien opvalt is dat onze testsuite uitgevoerd wordt door het commando `make`.
In de Makefile hebben we hier expliciet om gevraagd:

```Makefile
$(TEST_EXECUTABLE): $(TEST_OBJECT_FILES) $(LIBRARY)
	$(CC) $^ -o $@ $(LINK_FLAGS)
	./$@
```

Meteen na het builden van de test suite wordt deze uitgevoerd.
Indien deze test suite faalt, *stopt* make met verder builden.
Het programma dat onze linked list gebruikt, wordt niet meer gecompileerd.
Het heeft namelijk geen zin om een executable te genereren indien er nog fouten in onze library zitten.

Merk dus op dat deze test-suite ons beschermt bij het maken van fouten.
Ons programma wil niet meer builden wanneer we een foute library hebben.
Indien je een goede test suite hebt die automatisch uitgevoerd wordt elke keer dat de code opnieuw gecompileerd wordt, kan je fouten zeer snel detecteren.
Indien de `list_append` wijziging niet gedetecteerd zou worden, zou het programma dat `liblinkedlist.a` gebruikt niet meer werken.
De oorzaak dan terugtracen naar `list_append` zou al een stuk moeilijker kunnen zijn.


> :information_source: In toekomstige vakken zal goed testen uitgebreid besproken en aangeleerd worden.
Het doel van deze sectie is om jullie al eens een eerste keer te tonen hoe tests van C code kunnen werken, en hoe nuttig ze kunnen zijn.
Daarnaast illustreert dit voorbeeldproject nogmaals hoe een `Makefile` handig gebruikt kan worden om bepaalde procedures te automatiseren.

> **:question: Lees de code van de test suite om te kijken hoe je functies kan testen in C. Kijk hoe het voorbeeldproject werkt en voeg expres kleine fouten toe aan de voorbeeldimplementatie van de linked list. Kijk hoe de test suite deze fouten opvangt door na fouten te introduceren het project opnieuw te builden met `make`**

> **:question: Kan je een fout introduceren in de linked list library die onze testsuite niet kan detecteren? Indien dit lukt, pas dan de test suite aan om ook deze fout te detecteren.**
