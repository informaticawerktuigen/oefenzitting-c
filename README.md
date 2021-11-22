# C Tutorial <!-- omit in toc -->

## Inhoudstafel

- [Inhoudstafel](#inhoudstafel)
- [Werkomgeving](#werkomgeving)
- [Structuur oefenzitting](#structuur-oefenzitting)
- [Het project](#het-project)
- [Tijdsinvestering](#tijdsinvestering)
- [Lessen](#lessen)
  - [Les 1: pure C](#les-1-pure-c)
  - [Les 2: C Build Tools](#les-2-c-build-tools)
  - [Les 3: Bits & bytes](#les-3-bits--bytes)
  - [Les 4: Afgeleide datatypes](#les-4-afgeleide-datatypes)
  - [Les 5: Scopes en lifetimes](#les-5-scopes-en-lifetimes)
  - [Les 6: Dynamische structuren](#les-6-dynamische-structuren)
  - [Les 7: Systeeminteractie](#les-7-systeeminteractie)
  - [Les 8: Debugging](#les-8-debugging)
  - [Les 9: Makefiles en tests](#les-9-makefiles-en-tests)

## Werkomgeving

In deze lessen nemen we aan dat je werkt in Ubuntu of een andere UNIX-omgeving.
Windows wordt *niet* ondersteund.
Vele programma's die we in deze les en de komende lessen gebruiken werken niet standaard op Windows.
Ze wel werkend krijgen zal meer moeite kosten dan gewoon een Ubuntu-omgeving op te starten.
Het C-project zal ten slotte ook moeten werken in een Unix-omgeving.

Indien je nog steeds geen UNIX-omgeving hebt, volg dan deze tutorial: 
[Klaarzetten werkomgeving](https://github.com/informaticawerktuigen/klaarzetten-werkomgeving).

## Structuur oefenzitting

Deze oefenzitting heeft een niet-traditionele structuur.
De zitting zal bestaan uit negen korte lessen die elk een specifiek concept van de programmeertaal C uitlichten.

Het is de bedoeling dat je deze lessen de komende drie weken doorneemt.
Tijdens de eerste drie C-oefenzittingen krijg je de kans vragen te stellen over de lessen en de oefeningen.
Het is echter een goed idee hier op voorhand al aan te werken, zodat je de tijd in de zittingen gericht kan gebruiken om geholpen te worden.

Volg alle lessen in de opgegeven volgorde. Indien je op een later moment in je project zou vastlopen kan je altijd terug naar de specifieke les gaan waarin je probleem behandeld werd.

Stel ook zeker vragen op het [discussieforum](https://p.cygnus.cc.kuleuven.be/webapps/discussionboard/do/forum?action=list_threads&course_id=_1012669_1&nav=discussion_board_entry&conf_id=_1839407_1&forum_id=_1020024_1).

Tijdens de vierde oefenziting zullen jullie aan het project kunnen werken.

## Het project

Uitgebreide informatie over het project zal verschijnen na de eerste oefenzitting.

## Tijdsinvestering

Het gehele deel C is begroot op 1 studiepunt (30 uur) en wordt volledig afgewerkt in de komende drie weken.

De bedoeling is dat je elke les in ongeveer 1h30 kan afwerken (video kijken, les doorlopen, oefeningen maken).
In totaal geeft dat een tijdsinvestering van ongeveer 15 uur om de taal te leren.

Het project zal dus ook ongeveer 15 uur vergen.

**Begin op tijd aan het project**. Probeer dagelijks te werken zodra het project beschikbaar is.
De deadline van het project zal vallen op zondag 19/12 23h59, twee dagen na de laatste zitting.

> :warning: Hoewel je het laatste weekend (zaterdag 18/12 en zondag 19/12) de mogelijkheid zal hebben om verder te werken aan het project, merk op dat de meesten van jullie de laatste week van het semester een test hebben voor de vakken SOCS en BRI. De kans is dus groot dat je dat weekend zal willen gebruiken om te studeren voor die vakken. Werk dus op tijd dit project af.

## Lessen

### Les 1: pure C

In [deze les](https://github.com/informaticawerktuigen/oefenzitting-c/tree/main/les1-purec) leren jullie meer over de werking en de basissyntax van C.

### Les 2: C Build Tools

In [deze les](https://github.com/informaticawerktuigen/oefenzitting-c/tree/main/les2-c-build-tools) leren jullie meer over de verschillende tools die gebruikt worden bij het omzetten van een C-bestand naar een uitvoerbaar bestand.

### Les 3: Bits & bytes

In [deze les](https://github.com/informaticawerktuigen/oefenzitting-c/tree/main/les3-bits-and-bytes) leren jullie over de manier waarop C intern data voorstelt, door datatypes te gebruiken die een betekenis geven aan willekeurige bits en bytes.
De les bevat ook een uitgebreidere bespreking van verschillende operatoren in C, waaronder bitwise operatoren.

### Les 4: Afgeleide datatypes

In [deze les](https://github.com/informaticawerktuigen/oefenzitting-c/tree/main/les4-afgeleide-datatypes) beschouwen we enkele afgeleide datatypes: pointers, strings, arrays en structs.

### Les 5: Scopes en lifetimes

In [deze les](https://github.com/informaticawerktuigen/oefenzitting-c/tree/main/les5-scopes-and-lifetimes) gaan we kijken naar de levensduur van variabelen.
Dynamisch geheugenbeheer met behulp van *malloc* en *free* wordt onder andere geïntroduceerd.


### Les 6: Dynamische structuren

In [deze les](https://github.com/informaticawerktuigen/oefenzitting-c/tree/main/les6-dynamische-structuren) leren we hoe dynamisch geheugen ons toelaat om gegevensstructuren te bouwen die doorheen het programma groter of kleiner kunnen worden.
We bekijken het concept door de implementatie van lijsten te bestuderen.


### Les 7: Systeeminteractie

In [deze les](https://github.com/informaticawerktuigen/oefenzitting-c/tree/main/les7-system-interaction) leren we meer over I/O in C, vanuit de command line of via bestanden.

### Les 8: Debugging

In [deze les](https://github.com/informaticawerktuigen/oefenzitting-c/tree/main/les8-debugging) leer je meer over tools om fouten te zoeken in C-programma's, zoals `gdb`.

### Les 9: Makefiles en tests

In [deze les](https://github.com/informaticawerktuigen/oefenzitting-c/tree/main/les9-Makefiles-en-testing) leer je meer over Makefiles en het schrijven van testen.
