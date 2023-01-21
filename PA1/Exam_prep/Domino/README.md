# Dominos
EN

We have triangle, square or pentagon shaped dominoes. The dice contain numbers that can be positive, negative or zero and may be repeated on the dice.

The program receives dice written as threes/fours/fives of numbers up to EOF. Subsequently, he has the task of listing the number of different dice.

Dice are the same if they contain the same numbers in the same order, but they may be turned arbitrarily. Thus:

[4,8,1,3] = [4,8,1,3]
[1,2,3] = [2,3,1] → rotated by 120°
[2,5,7,2,5] = [7,2,5,2,5] → rotated twice by 72°
[1,2,3] != [1,3,2]

---

CZ

Máme kostky domina ve tvaru trojúhelníku, čtverce nebo pětiúhelníku. Kostky obsahují čísla, která můžou být kladná, záporná i nulová a na kostce se smí opakovat.

Program dostává kostky zapsané jako trojici/čtveřici/pětici čísel až do EOF. Následně má za úkol vypsat počet různých kostek.

Kostky jsou stejné, pokud obsahují stejná čísla ve stejném pořadí, smí být ale libovolně natočené. Tedy:

[4,8,1,3] = [4,8,1,3]
[1,2,3] = [2,3,1] → otočeno o 120°
[2,5,7,2,5] = [7,2,5,2,5] → otočeno dvakrát o 72°
[1,2,3] != [1,3,2]