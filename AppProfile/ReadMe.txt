Profiler de fisiere

Profilerul furnizeaza o lista cu fisierele accesate in urma unor comenzi. De asemenea afiseaza numarul de afisari per fisier.

Programul care genereaza lista de fisiere este "open.c"(gcc open.c -o open) si se foloseste dupa cum urmeaza:

Intrare
  1. Se introduc comenzile dorite in scriptul 'script.sh'
 
  2. Se executa 'strace' pentru a afla apeluri de sistem si se dumpeaza in fisier cu comanda
         strace -t -f -o dump.txt -e trace=open sh script.sh

Iesire
  3. Se executa open si acesta va genera 2 liste cu fisiere
        file_all.txt    - va contine fiecare fisier deschis si timestampul la care a avut loc accesul
        files_stat.txt  - va contine o lista de tipul: <fisier, numar de accesari>

