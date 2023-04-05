# NightKnight

## Fet per: 
- Pere Carrillo
- Marc Ordóñez

## Execució
Per executar el joc, entrar al directori `Binari` i executar `NightKnight.exe`. Si surt algun error, instal·lar `vc_redist.x86.exe` i tornar a provar.

Per obrir la solució de visualStudio, obrir `Projecte/02-Bubble/02-Bubble.sln`.


## Funcionalitats implementades:
- Jugador marca les rajoles al passar per sobre
- Quan estan totes les rajoles marcades apareix una clau, que desbloqueja un cofre
- Quan el jugador toca el cofre desbloquejat, passa al seguent nivell
- Moviment bàsic del jugador
    - Dreta - Esquerra - Salt - Gravetat
        - `Coyote Jump`: Permetem al jugador saltar si just fa pocs frames que ha sortit d'una rajola
- Punxes, que fan perdre una vida al jugador
- Temps límit per cada nivell
- Tecles per saltar contingut
    - Invulnerabilitat: G
    - Fer apareixer la clau: K
    - Saltar a un nivell concret: '1' - '2' - ... - '6'
- Pantalles
    - Menú principal
    - Seleccionador de nivells
    - Joc
    - Pausa
    - Guanyar
    - Perdre
    - Crèdits
    - Instruccions
- 3 enemics, cadascun amb comportaments diferents
    - Esquelet: Es mou a dreta i esquerra. Quan no pot més s'atura i canvia de direcció
    - Vampir: Més lent que l'Esquelet però el mateix moviment. Es pot transformar en Ratpenat aleatoriament i volar. Al volar rebota contra tots els obstacles
    - Rata: És més ràpida que l'Esquelet però mes petita. Pot saltar a rajoles properes quan se li acaba el camí 
- 5 objectes diferents
    - Rellotge: Congela als enemics
    - Clau: Desbloqueja el cofre
    - Gemma: Afegeix 500 punts
    - Escut: Torna al player invencible durant uns instants
    - Cor: Guanya una vida
- 6 nivells diferents
- Plataformes verticals i horitzontals: Si el player està a sobre es mou amb ella
- So
    - Musica de fons
    - Efectes especials
- Interactivitat de les entitats del joc
    - Animacions del jugador
    - Animacions de tots els enemics
    - Pausa inicial
    - Animació transformació Vampir - Ratpenat
    - Animació Bonus puntuació final
    - Animació a l'entrar al cofre
    - Animació al morir
    - Animació de canvi de nivell
    - Menú interactiu per poder seleccionar nivells
    - Menú principal interactiu

## Instruccions
- Moviment Jugador: Fletxes esquerra ← i dreta →. Amunt ↑ per saltar.
- L'objectiu de cada nivell és pintar totes les rajoles del terra, agafar la clau que apareixerà i arribar al cofre, tot sense que s'acabi el temps.
- Si xoques amb un enemic perdràs una vida, si les perds totes s'acabarà el joc.
- Aniran apareixent items, cadascun té un poder diferent:
    - Rellotge: Atura el temps i els enemics durant 5 segons
    - Gemma: Et dona 500 punts extres
    - Escut: Transforma al jugador en robot i es torna invulnerable durant 5 segons
    - Cor: Et dona una vida extra
