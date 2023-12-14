# TME sujet semaine 4 : thread, atomic, mutex
Etudiant 1: 28705252  
Etudiant 2: 28633410

[Programmation Système Répartie et Concurrente : Homepage](https://pages.lip6.fr/Yann.Thierry-Mieg/PR)

[Sujet du TME en PDF](https://pages.lip6.fr/Yann.Thierry-Mieg/PR/tdtme4.pdf)

To use the project :
*  Go to an empty folder (not your workspace)
```
mkdir -p tme4 ; cd tme4 ;

```
*  Clone the project, 
```
git clone https://github.com/yanntm/PSCR-TME4.git

```
* Reconfigure project.
This project is compatible with [autoconf/automake](https://www.lrde.epita.fr/~adl/autotools.html), after cloning, use the mantra 
```
cd PSCR-TME4
autoreconf -vfi
./configure 
```
in the root folder to build makefiles for your project.
* Import in eclipse or open with your favorite editor
   * For eclipse, `File->Import->General->Existing projects into workspace` and point the folder tme4.
* To build, simply run `make` in the folder 
   * For eclipse use the "hammer" tool or "Project->Build Project".


Look at the contents of "configure.ac", "Makefile.am" and "src/Makefile.am" which are the input to autotools, and are relatively simple and easy to read.

(c) Sorbonne Université 2018-2019

---

### Question 2 :

Le comportement est incorrect, il est possible que plusieurs threads procèdent au transfert sur un même compte en débitant ou créditant. Par exemple si on choisit un nombre de compte petit, la probabilité d'avoir un datarace est très élevé.

### Question 4 :

Pour permettre la synchronisation :
On bloque d'abord le débiteur, durant cette section critique : 
on bloque le créditeur.

On exécute la fonction `debiter`, si elle renvoie vraie alors `crediter` est exécuté sur l'autre compte. Sinon, on ne fait rien
ensuite on débloque le crediteur.

Et on finit par débloquer le déditeur.


### Question 5 :
les threads se bloquent immédiatement, même avec un seul thread.Car on a décidé que on allait faire le transfert de façon simultanée. comme nous avons ajouté des lock sur le crediteur, lorsque le débit réussit, on exécute `crediter`, et au sein de cette fonction on fait un nouveau lock, mais le lock est appliqué sur le même mutex qui est dans `transfert`. Donc il y a blocage.

### Question 6 :
Il y a parfois des interblocages car le recursive_mutex fonctionne avec une liste d'attente en fifo. 
La ressource peut être bloqué et libéré plusieurs fois d'affilé en fonction du compteur. Dans certaine situation, nous pouvons avoir le même problème que précédemment.

Pour résoudre ce problème et éviter un interblocage, on peut utiliser la fonction try_lock() qui permet de vérifier si nous avons la main sur la ressource ou non.