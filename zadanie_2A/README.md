Do synchronizacji użyto 2n+3 mutexów. Pojedyncze mutexy służą do blokowania 
zasobów: puchar z winem, tacka z ogórkami oraz aktualny stan króla. Pozostałe 
2n muteksów umieszczono w 2 tablicach o rozmiarze n. Pierwsza z nich 
`table_mutex` zawiera inforamcje o tym czy można podjąć jakąś akcje. W 
odniesieniu do prawdziwego życia można wyobrażać to sobie jako pytanie do 
sąsiada czy one zechce nas słuchać. lub czy kielich/talerzyk jest wolny. Jeśli 
tak to zaczynamy opowiadać. W przypadku gdy chcemy coś zjeść używamy tego 
samego mutexu aby sprawdzić czy sąsiad przypadkiem nie opowiada czegoś co 
powinniśmy słuchać lub nie je (wtedy musimy zaczekać aż zwolni naczynia). Jeśli 
udało nam się zablokować sąsiada to bierzemy odpowiednio kielich i talerzy i 
staramy się nałożyć sobie ogórka i nalać wina.
Przed rozpoczęciem oczekiwania na jedzenie a także przed rozpoczęciem 
opowiadania, sprawdzamy czy przypadkiem nie powinniśmy słuchać króla. Do tego 
celu użyto zmiennych warunkowych.
Te zmienne są też wykorzystane przy dostępie do jedzenia. W sumie w programie 
istnieją 4 zmienne warunkowe: po jednej do wina i ogórków (sprawdzanie czy jest 
coś jeszcze w dzbanie/talerzu) oraz po jednej do mówienia i jedzenia 
(sprawdzania czy król nie mówi).
Rozwiązanie inspirowane rowiązanie Problemu 5 filozofów autorstwa Dijkstry.