Simulator de algoritmi de caching

Sunt simulaþi doi algoritmi de caching pentru a fi comparaþi.
Avem un cache cu douã niveluei (RAM ºi SSD).
Fiºierele sunt doar citite, nu ºi modificate (exemplu: server web).

Primul algoritm: când se solicitã un fiºier care nu e în cache, se încearcã aducerea lui
în RAM (dacã are loc) ºi apoi în SSD (dacã are loc). Dacã nu are loc în niciunul din aceste
medii de stocare, se verificã dacã are dimensiunea mai micã decât memoria RAM ºi se ºterg de
aici atâtea fiºiere încât noul fiºier sã aibã loc ºi acesta de copiazã în RAM. Dacã fiºierul
nu ar putea încãpea în RAM se verificã dacã are dimensiunea mai micã decât SSD-ul ºi se ºterg
de aici atâtea fiºiere încât noul fiºier sã aibã loc ºi acesta de copiazã în SSD. Dacã 
fiºierul nu ar încãpea nici în SSD, se lasã pe HDD. ªtergerea fiºierelor din RAM ºi SSD se
face începând cu cele mai vechi fiºiere introduse acolo.
Al doilea algoritm este similar cu primul, doar cã fiºerele care au maxim o dimensiunea datã
(de exmplu fiºiere .css. .html) se copiazã doar în RAM, iar celelalte se copiazã doar pe SSD,
pentru o îmbunãtãþire a performaþei.

Date de intrare:
Files.txt, care conþine fiºierele care vor fi acesate, sub forma unei perechi 
"index_fiºier mãrime_fiªier" pe câte o linie.
Profile.txt care conþine pe prima linie dimensiunea memoriei RAM, pe a doua linie dimensiunea
memoriei SSD (HDD-ul are o dimensiune irelevantã, din moment ce conþine toate fiºierele care
vor fi accesate), pe a treia linie dimensiunea maximã a fiºierelor care vor fi copieate în RAM
de algoritmul 2 ºi pe a patra linie ordinea în care vor fi accesate fiºierele.
Times.txt conþine pe prima linie timpul de acces al unui fiºier care se aflã în RAM, SSD, 
respectiv HDD, pe a doua linie rata de citire din RAM, SSD, HDD (timpul se citire se obþine
împãrþind dimensiunea la aceastã ratã) ºi pe a treia linie timpul de copiere de pe HDD în RAM,
respectiv SSD.

Datele de ieºire: câte un fiºier pentru fiecare algoritm care conþine linii de trei numere ce
reprezintã indexul fiºerului, dimesiunea sa ºi timpul în care a fost citit acel fiºier. Pe 
ultima linie se aflã timpul total în care au fost citite fiºierele.

Timp de accesare fiºier din RAM sau SSD = timp acces + dimensiune / ratã citire.
Timp de accesare fiºier din HDD = timp acces + dimensiune / ratã citire + timp copiere * dimensiune.