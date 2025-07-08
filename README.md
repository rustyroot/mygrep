# PROJET GREP - Version étoilée

###### Eliott Houget

## Introduction

Voici mon projet mygrep. Les parties les plus importantes sont :
- le sommaire car il est le squelette de ma reflexion et de mon code.
- la partie 2. car il y a toutes les idées.
- la discussion autour du vide (cf. *4.B.c.i*) 

## Sommaire 

1. Conventions
    - document capytale
    - fonctions d'affichages
    - int, char, lettre et meta_char
2. Algorithme
    1. Algorithme grossier
    2. Algorithme détaillé
    3. linéariser et délinéariser des jokers ?!
    4. Pourquoi Berry-Sethi ?
    5. *`main`*
3. Expression régulière
    1. Représentation 
        1. Arité des opérateurs
            - *`arite`*
        2. Définition du type **`arbre_t`**
            - `arbre_s`
            - *`liberer_arbre`*
        3. Construction d'un arbre à partir de la notation polonaise inversée
            - inserer un opérateur dans un arbre : *`inserer`*
            - *`arboriser`*
4. Construction d'un automate 
    1. Linéarisation d'une expression regulière
        1. Compter les éléments utiles (lettres, jokers)
            - *`compter_lettres`*
            - *`compter_jokers`*
        2. Linéariser
            - *`lineariser`*
            - *`lineariser_recursivement`*
    2. Calcul des ensembles P, D et F
        1. liste et ensemble de lettres 
            1. **`liste_lettres_t`**
                - `liste_lettres_s`
                - *`liberer_liste_lettres`*
                - *`ajouter_lettre`*
            2. **`ensemble_lettres_t`**
                - *`union_ensemble_lettres`*
        2. liste et ensemble de facteurs
            1. **`facteurs_t`**
                - `facteur_s`
                - *`are_equal`*
                - *`greater_than`*
            2. **`liste_facteurs_t`**
                - `liste_facteurs_s`
                - *`liberer_liste_facteurs`*
                - *`ajouter_facteur`*
            3. **`ensemble_facteurs_t`**
                - *`union_ensemble_facteurs`*
        3. Vide et epsilon
            1. Cas du langage vide
            2. *`contient_epsilon`* 
        4. *`calculer_P`*
        5. *`calculer_D`*
        6. Calculer F
            1. *`fusion`*
            2. *`calculer_F`*
    5. Construction de l'automate des facteurs
        1. Automates Finis déterministes : **`afd_t`**
            - `afd_s`
            - *`liberer_afd`*
            - *`creer_afd`*
    4. Délinéariserisation
        1. Automates finis "généralisés"
            1. **`liste_entiers_t`**
                - `liste_entiers_s`
                - *`liberer_liste_entiers`*
                - *`ajouter_entier`*
            2. **`alphabet_t`**
                - `alphabet_s`
                - *`creer_alphabet`*
                - *`creer_alphabet_recursivement`*
            3. **`afg_t`**
                - `struct afg_s`
                - *`liberer_afg`*
                - *`creer_afg`*
        2. Délinéariser
            - *`delineariser_afd`*
            - *`delineariser_afd_recursivement`*
    5. *`berry_sethi`*
    6. *`construire_automate`*
5. Lire un mot dans un automate fini "généralisé"
    - *`modifier_tableau_avec_liste_entiers`*
    - *`afg_reconnait`*
6. Test
    1. Question 2
    2. Question 3
    3. batterie de test supplémentaire
7. Commentaires

## 1. Conventions

### Concernant ce document

- Les __fonctions__ sont en italique et encadrée : *`fonction`*
- Les __types du C__ sont en gras et encadrée : **`type`**
- Les __types construits__ sont en gras, encadrée, et ont le suffixe *_t* : **`type_t`**
- Les __structures__ sont encadrée et ont le suffixe *_s* : `structure_s`

### Concernant les fonctions d'affichages

Aucune fonction d'affichage ne sera fournie dans le code et aucune ne sera présentée ici. Néanmoins chaque type dispose de sa fonction d'affichage associée dans ma version de travail afin de réaliser un débuggage régulier et de qualité.

### int, char, lettre et meta_char

- les **`int`** et les **`char`** sont les types C classiques et seront souvent mélangés, j'ai essayé le plus souvent de caster explicitement les types afin de clarifier le code au maximum.
- On utilisera le terme `lettre` pour designer les éléments d'un alphabet (l'objet théorique).
- les `meta_char` sont des variables de type **`char`** mais qui non pas vocation à être affichées et qui ne doivent pas rentrer en collision avec les autres caractères utilisés. On utilisera donc les éléments suivants dans le code :
```C
#define ascii_table_size 128
#define first_meta_char (char)ascii_table_size

int meta_char_to_int (char a) {
    return (int)(a - first_meta_char);
}
```

## 2. Algorithme

### A. Algoritme grossier
1. construire l'automate A avec l'expression régulière
2. Pour chaque ligne du fichier
3.    Si la ligne est reconnue par A
4.        afficher la ligne
### B. Algorithme détaillé
1.  Construire l'arbre représentant l'expression régulière
2.  n <- Compter le nombre de lettre et de joker 
3.  Linéariser l'expression regulière
4.  Calculer P
5.  Calculer D
6.  Calculer F
7.  A <- Créer un automate fini déterministe sans transition à n+1 états
8.  Faire les branchements à l'aide de P, D et F
9.  A' <- Délinéariser A
10. Pour chaque ligne du fichier
11.    Si la ligne est reconnu par A'
12.        afficher la ligne
### C. linéariser et délinéariser des jokers ?!

Voilà la seule idée intéressante que j'apporte (le reste c'est l'algorithme Berry-Sethi classique implémenté en C). Lors de la linéarisation et du calcul des ensembles P, D et F, les jokers seront considérés comme des lettres "classiques". Ainsi une lors de la délinéarisation l'automate fini déterministe devient un automate fini "généralisé", car les jokers apparaissent comme des $\Sigma$-transitions.

Une $\Sigma$-transitions est une transition qui reconnait toutes les lettres.

### D. Pourquoi Berry-Sethi ?

Premièrement, j'ai fortement été influencé par le TP-C (j'ai commencé ce TP-DM le même soir). Deuxièmement, ça permet d'avoir des automates "pas trop gros", c'est-à-dire avec "pas trop d'états" (exactement le nombre de lettres et de jokers + 1 états). Le nombre d'états de l'automate créer est important car pour lire un mot m dans un automate fini "généralisé" à n états, on a une complexité temporelle en O(|m|n²). Troisièmement, c'est cool et pédagogique d'implementer Berry-Sethi en C.

### E. fonction *`main`*

```C
int main (int argc, char* argv[]) { // (1)

    if (argc > 2) { // (2)
        afg_t* automate = construire_automate(argv[1]); // (3)
        ///// (4)
        FILE* fichier = fopen(argv[2], "r");
        size_t taille_zone_tampon = 256;
        char* zone_tampon = malloc(sizeof(char) * taille_zone_tampon);
        ///// (5)
        while (getline(&zone_tampon, &taille_zone_tampon, fichier) != EOF) {
            if (afg_reconnait(automate, zone_tampon)) {
                printf("%s", zone_tampon);
            }
        }
        ///// (6)
        free(zone_tampon);
        fclose(fichier);
        liberer_afg(automate);
    }

    return 0;
}
```

1. cf. feuille "Syntaxe : la fonction main" 
2. On choisit de ne rien faire si il n'y a pas assez d'arguments, d'en ignorer s'il y en a trop et de laisser le programme planter si l'utilisateur fournie n'importe quoi
3. l'argument 1 est l'expression regulière en notation polonaise fourni par l'utilisateur (pour le type **`afg_t`**, cf. *4.D.a.iii*; pour *`construire_automate`* cf. *4.F*)
4. On ouvre le fichier en lecture, ici on fait le choix (arbitraire) d'allouer un buffer de taille 256 * 8  = 2048 bits, pour les lignes de plus de 256 caractères (\n et \0 compris) le comportement est indéfini.
5. Ces lignes traduisent les lignes 10, 11 et 12 de l'algorithme donné en pseudo-code ci-dessus.
6. Je m'engage à ce qu'il n'y ait aucune fuite mémoire.

On a donc besoin des librairies suivantes :
```C
#include <stddef.h> // pour le type size_t
#include <stdlib.h> // pour malloc
#include <stdio.h> // pour la gestion du fichier
```

## 3. Expression regulière

### A. Représentation

L'expression régulière donnée en paramètre est en notation polonaise inversée (notation postfixe) ainsi on va représenter notre expression comme un arbre dont les sommets intérieurs seront les opérateurs `@`, `|`, `*` et `?`, et les feuilles seront les lettres et les jokers `.`. Donc les expressions sont des arbres binaires étiquetés par des **`char`**.

#### a. Arité des opérateurs

```C
int arite (char c) {
    switch (c) {
        case '@': // Concaténation
            return 2;
        case '|': // Union
            return 2;
        case '*': // Etoile
            return 1;
        case '?': // Binaire : e? -> l'expression e zéro ou une fois
            return 1;
        case '.': // Joker : désigne n'importe quel caractère
            return 0;
        default:  // Lettre
            return 0;
    }
}
```

Je profite de cette fonction qui s'explique elle-même pour présenter notre meilleur ami : le switch-case.

#### b. Définition du type arbre_t

```C
typedef struct arbre_s {
    char c;
    struct arbre_s* fils_gauche;
    struct arbre_s* fils_droit;
    int libertes; // indique le nombre de feuille encore libre
} arbre_t;
```

Pas de surprise pour les trois premiers champs, le dernier va être utile lors de la construction de l'arbre.

```C
void liberer_arbre (arbre_t* arbre) {
    if (arbre != NULL) {
        liberer_arbre(arbre->fils_gauche);
        liberer_arbre(arbre->fils_droit);
    }
    free(arbre);
}
```

Les fonctions de libérations seront fournis par souci d'exhaustivité mais jamais expliquées car triviales.

#### c. Construction d'un arbre à partir de la notation polonaise inversée

```C
arbre_t* inserer (arbre_t* arbre_courant, char c) {
    if (arbre_courant == NULL) {
        arbre_t* nouvelle_arbre = malloc(sizeof(arbre_t));
        nouvelle_arbre->c = c;
        nouvelle_arbre->fils_gauche = NULL;
        nouvelle_arbre->fils_droit = NULL;
        nouvelle_arbre->libertes = arite(c); // (1)
        return nouvelle_arbre;
    }
    else {
        switch (arbre_courant->libertes) {
            case 2:
                arbre_courant->fils_droit = inserer(arbre_courant->fils_droit, c);
                if (arbre_courant->fils_droit->libertes == 0) arbre_courant->libertes--;
                return arbre_courant;
            case 1: // (2)
                arbre_courant->fils_gauche = inserer(arbre_courant->fils_gauche, c);
                if (arbre_courant->fils_gauche->libertes == 0) arbre_courant->libertes--;
                return arbre_courant;
            default: // (3)
                if (arbre_courant->libertes == 0) 
                    fprintf(stderr,"ERREUR : tentative d'insertion dans un arbre complet.\n");
                else 
                    fprintf(stderr, "ERREUR : le nombre de libertés de l'opérateur a la valeur interdite %d.\n", arbre_courant->libertes);
                return arbre_courant;
        }
    }
}
```

On procède récursivement pour inserer un opérateur (ou lettre) dans un arbre. Le cas de base est l'insertion dans l'arbre vide `NULL`. (1) L'arbre initialisé peut alors avoir autant de fils que l'arité de l'opérateur inseré le permet. Pour insérer dans un arbre non vide on regarde la liberté de sa racine, si ses deux fils sont libre alors on place d'abord à droite afin de respecter la reconstrution d'un arbre affiché par parcours postfixe. (2) **attention si un opérateur n'a qu'un fils celui-ci sera à gauche**. (3) le premier cas d'erreur peut être déclenché par l'utilisateur (pas doué). Le second indiquerait plutôt un problème dans le code (ça n'arrive jamais)

```C
arbre_t* arboriser (char* expression_polonaise_inverse) {
    int index = strlen(expression_polonaise_inverse)-1;
    arbre_t* arbre_courant = NULL;
    while (index >= 0) {
        arbre_courant = inserer(arbre_courant, expression_polonaise_inverse[index]);
        index--;
    }
    return arbre_courant;
}
```

Pour transformer l'expression polonaire inversé en arbre on insére caractère par caractère de droite à gauche.
Donc on a besoin de string.h.
```C
#include <string.h> // pour strlen (oui juste pour ça)
```

## 4. Construction d'un automate

### A. Linéarisation d'expression regulière

#### a. Compter les éléments utiles (lettres, jokers)

```C
int compter_lettres (arbre_t* arbre) {
    if (arbre == NULL) return 0;
    else switch (arbre->c) {
        case '@': 
            return compter_lettres(arbre->fils_droit) + compter_lettres(arbre->fils_gauche);
        case '|': 
            return compter_lettres(arbre->fils_droit) + compter_lettres(arbre->fils_gauche);
        case '*': 
            return compter_lettres(arbre->fils_gauche);
        case '?': // Binaire : e? -> l'expression e zéro ou une fois
            return compter_lettres(arbre->fils_gauche);
        case '.': // Joker : désigne n'importe quel caractère
            return 0;
        default:  // Lettre
            return 1;
    }
}

int compter_jokers (arbre_t* arbre) {
    if (arbre == NULL) return 0;
    else switch (arbre->c) {
        case '@': 
            return compter_jokers(arbre->fils_droit) + compter_jokers(arbre->fils_gauche);
        case '|': 
            return compter_jokers(arbre->fils_droit) + compter_jokers(arbre->fils_gauche);
        case '*': 
            return compter_jokers(arbre->fils_gauche);
        case '?': // Binaire : e? -> l'expression e zéro ou une fois
            return compter_jokers(arbre->fils_gauche);
        case '.': // Joker : désigne n'importe quel caractère
            return 1;
        default:  // Lettre
            return 0;
    }
}
```

Deux fonctions très similaires triviales. 

#### b. Linéariser

```C
char* lineariser (arbre_t* arbre) {
    int nb_lettres = compter_lettres(arbre);
    int nb_jokers = compter_jokers(arbre);
    char* tableau_de_delinearisation = malloc(sizeof(char) * (nb_lettres+nb_jokers)); // (1)
    int index = 0; // (2)
    if (arbre != NULL) lineariser_recursivement(arbre, tableau_de_delinearisation, &index);
    return tableau_de_delinearisation;
}
```

```C
void lineariser_recursivement (arbre_t* arbre, char* tab, int* index_ptr) {
    assert(arbre != NULL);
    switch (arbre->c) {
        case '@': 
            lineariser_recursivement(arbre->fils_gauche, tab, index_ptr);
            lineariser_recursivement(arbre->fils_droit, tab, index_ptr);
            break;
        case '|': 
            lineariser_recursivement(arbre->fils_gauche, tab, index_ptr);
            lineariser_recursivement(arbre->fils_droit, tab, index_ptr);
            break;
        case '*': 
            lineariser_recursivement(arbre->fils_gauche, tab, index_ptr);
            break;
        case '?': // Binaire : e? -> l'expression e zéro ou une fois
            lineariser_recursivement(arbre->fils_gauche, tab, index_ptr);
            break;
        case '.': // Joker : désigne n'importe quel caractère // (3)
            tab[*index_ptr] = arbre->c;
            arbre->c = first_meta_char + *index_ptr; // (4)
            (*index_ptr)++;
            break;
        default:  // Lettre
            tab[*index_ptr] = arbre->c;
            arbre->c = first_meta_char + *index_ptr; // (4)
            (*index_ptr)++;
            break;
    }
}
```

1. Ce tableau est l'élément le plus important de ce couple de fonctions, l'arbre de l'expression regulière est toujours parcouru de la même manière, ainsi on peut construire un tableau qui relie la place du meta_char dans l'arbre linéarisé et la lettre (ou le joker) qu'il remplace. Cela sera utile pour délinéariser.
2. Cette index est déclaré dans la fonction principal et on donne un pointeur vers celui-ci dans la fonction auxiliaire, cela permet d'avoir une variable "globale" pour tout appel de *`lineariser_recursivement`*
3. Ce cas n'est pas utile à strictement parler, mais rend à mon sens le code plus clair pour un humain.
4. Les meta_char font alors tout leur sens. Il ne sont pas des char que l'on souhaite manipuler mais les "linéarisés" des lettres et jokers de l'expression régulière.
- (la fonctions "auxiliaire" est (évidemment) déclarée avant la fonction "principale" dans le .c, on ne le reprécisera pas dans la suite)

On rajoute en en-tête 
```C
#include <assert.h>
```

### B. Calcul des ensembles P, D et F

#### a. liste et ensemble de lettres 

##### i. **`liste_lettres_t`**

```C
typedef struct liste_lettres_s { // (1)
    char head;
    struct liste_lettres_s* tail;
} liste_lettres_t;

void liberer_liste_lettres (liste_lettres_t* liste) {
    if (liste != NULL) {
        liberer_liste_lettres(liste->tail);
        free(liste);
    }
}

liste_lettres_t* ajouter_lettre (liste_lettres_t* liste, char lettre) { // (2)
    liste_lettres_t* nouvelle_liste = malloc(sizeof(liste_lettres_t));
    nouvelle_liste->head = lettre;
    nouvelle_liste->tail = liste;
    return nouvelle_liste;
}
```

1. Construction d'un type de listes chainées classiques.
2. Deux choses importantes sur cette fonction :
    1. Elle renvoie une nouvelle liste, et ne modifie pas celle donnée en argument. On aurait pu implémenter une fonction void demandant un pointeur vers une liste en argument et modifier la cible du pointeur vers la nouvelle liste. Mais cette construction est important pour la suite.
    2. On ne fait aucun test sur ce qui est ajouté, on a donc aucune hypothèse sur l'ordre ou la présence de doublons dans les éléments de la liste.

##### ii. **`ensemble_lettres_t`**

```C
typedef liste_lettres_t ensemble_lettres_t;
```

Oui, le type **`ensemble_lettres_t`** n'est qu'un renommage du type **`liste_lettres_t`**. Mais cela permet un code plus clair car les ensembles de lettres sont les listes de lettres contenant une famille strictement croissante.

```C
ensemble_lettres_t* union_ensemble_lettres (ensemble_lettres_t* ensemble_lettres1, ensemble_lettres_t* ensemble_lettres2) {
    if (ensemble_lettres1 != NULL && ensemble_lettres2 != NULL) { // (1)
        if (ensemble_lettres1->head == ensemble_lettres2->head) { // (A)
            return ajouter_lettre(union_ensemble_lettres(ensemble_lettres1->tail, ensemble_lettres2->tail), ensemble_lettres2->head);
        }
        else if (ensemble_lettres1->head < ensemble_lettres2->head) { // (B)
            return ajouter_lettre(union_ensemble_lettres(ensemble_lettres1->tail, ensemble_lettres2), ensemble_lettres1->head);
        }
        else { // (C)
            return ajouter_lettre(union_ensemble_lettres(ensemble_lettres1, ensemble_lettres2->tail), ensemble_lettres2->head);
        }
    }
    else if (ensemble_lettres1 == NULL && ensemble_lettres2 == NULL) { // (2)
        return NULL;
    }
    else if (ensemble_lettres1 == NULL) { // (3)
        return ajouter_lettre(union_ensemble_lettres(ensemble_lettres1, ensemble_lettres2->tail), ensemble_lettres2->head);
    }
    else { // (4)
        return ajouter_lettre(union_ensemble_lettres(ensemble_lettres1->tail, ensemble_lettres2), ensemble_lettres1->head);
    }
}
```

- Justifions et expliquons la correction de cette fonction sous l'hypothèse que les ensembles donnés en argument sont comme décrits ci-dessus. On procède récursivement par disjonction de cas en fonction de si les listes sont vides ou non (NULL ou non).
1. Si les deux listes sont non vides
    1. Si les deux listes commencent par la même lettres, alors on jette la première lettre de la liste 1 et on ajoute la première lettre de la liste 2. Plus précisement on ajoute la lettre en tête de l'union du reste des deux listes. Cela permet d'assurer qu'il n'y aura pas de doublons et que les plus petites lettres (dans l'ordre lexicographique) seront en début de liste, c'est-à-dire que la liste renvoyée est strictement croissante. Voilà pourquoi on a construit *`ajouter_lettre`* ainsi.
    2. Si la première lettre de la liste 1 est plus petite que celle de la liste 2, alors on l'ajoute en tête de l'union de la liste 2 et du reste de la liste 1.
    3. même chose que B en symétrique.
2. Faire l'union de deux ensembles vides donne l'ensemble vide.
3. Par les test précédents on est sûr que ensemble_lettres2 n'est pas vide alors on peut ajouter sa première lettre.
4. même chose que 3 en symétrique.

#### b. liste et ensemble de facteurs

##### i. **`facteurs_t`**

```C
typedef struct facteur_s {
    char first;
    char second;
} facteur_t;

bool are_equal (facteur_t facteur1, facteur_t facteur2) {
    return (facteur1.first == facteur2.first) && (facteur1.second == facteur2.second);
}

bool greater_than (facteur_t facteur1, facteur_t facteur2) {
    if (facteur1.first > facteur2.first) return true;
    else if (facteur1.first == facteur2.first) {
        return facteur1.second > facteur2.second; 
    }
    else return false;
}
```

Avant de faire des listes et ensembles de facteurs, il nous faut construire un type **`facteur_t`** et des outils pour le manipuler. On peut comparer des **`char`** avec les opérateurs classiques car ce ne sont que des "int cachés" mais il nous faut réimplémenter des comparateur pour ce nouveau type. 

##### ii. **`liste_facteurs_t`**

```C
typedef struct liste_facteurs_s {
    facteur_t head;
    struct liste_facteurs_s* tail;
} liste_facteurs_t;

void liberer_liste_facteurs (liste_facteurs_t* liste) {
    if (liste != NULL) {
        liberer_liste_facteurs(liste->tail);
        free(liste);
    }
}

liste_facteurs_t* ajouter_facteur (liste_facteurs_t* liste, facteur_t facteur) {
    liste_facteurs_t* nouvelle_liste = malloc(sizeof(liste_facteurs_t));
    nouvelle_liste->head = facteur;
    nouvelle_liste->tail = liste;
    return nouvelle_liste;
}
```

Même commentaires que pour les listes de lettres (cf. 4.B.a.i).

##### iii. **`ensemble_facteurs_t`**

```C
typedef liste_facteurs_t ensemble_facteurs_t;
// Les ensembles de facteurs sont les listes de facteurs contenants une famille strictement croissante

ensemble_facteurs_t* union_ensemble_facteurs (ensemble_facteurs_t* ensemble_facteurs1, ensemble_facteurs_t* ensemble_facteurs2) {
    if (ensemble_facteurs1 != NULL && ensemble_facteurs2 != NULL) {
        if (are_equal(ensemble_facteurs1->head, ensemble_facteurs2->head)) {
            ensemble_facteurs1 = ensemble_facteurs1->tail;
            return ajouter_facteur(union_ensemble_facteurs(ensemble_facteurs1, ensemble_facteurs2->tail), ensemble_facteurs2->head);
        }
        else if (greater_than(ensemble_facteurs1->head, ensemble_facteurs2->head)) {
            return ajouter_facteur(union_ensemble_facteurs(ensemble_facteurs1->tail, ensemble_facteurs2), ensemble_facteurs1->head);
        }
        else {
            return ajouter_facteur(union_ensemble_facteurs(ensemble_facteurs1, ensemble_facteurs2->tail), ensemble_facteurs2->head);
        }
    }
    else if (ensemble_facteurs1 == NULL && ensemble_facteurs2 == NULL) {
        return NULL;
    }
    else if (ensemble_facteurs1 == NULL) {
        return ajouter_facteur(union_ensemble_facteurs(ensemble_facteurs1, ensemble_facteurs2->tail), ensemble_facteurs2->head);
    }
    else {
        return ajouter_facteur(union_ensemble_facteurs(ensemble_facteurs1->tail, ensemble_facteurs2), ensemble_facteurs1->head);
    }
}
```

Même chose que pour les ensembles de lettres.

#### c. Vide et epsilon

##### i.  Cas du langage vide

Aucune expression régulière avec `@`, `|`, `*`, `?`, `.` et des lettres ne représente le langage vide. Seul l'expression régulière vide représente le langage vide. De plus `./mygrep '' entree.txt` doit produire le même résultat que `grep -E '^$' entree.txt`, c'est-à-dire afficher les lignes ne contenant que les caractère \n ou aucun caractère. Or la construction de l'automate présenté fait que l'expression vide `''` créer un arbre vide (NULL) et par suite un automate à un seul état initiale mais pas final. Par conséquence, on a pas besoin d'implémenter une fonction *`est_vide`* comme on s'attendrait à le faire pour calculer P, D, et F. Pour savoir si l'automate dérivé de l'arbre NULL reconnait le mot vide (c'est-à-dire "\n\0" ou "\0") on rajoute 
```C
if (afg->taille_Q == 1 && (mot[0] == '\n' || mot[0] == '\0')) return true;
```
à *`afg_reconnait`*.

##### ii. *`contient_epsilon`*

```C
bool contient_epsilon (arbre_t* arbre) {
    if (arbre == NULL) return false;
    else switch (arbre->c) {
        case '@': // Concaténation
            return contient_epsilon(arbre->fils_gauche) && contient_epsilon(arbre->fils_droit);
        case '|': // Union
            return contient_epsilon(arbre->fils_gauche) || contient_epsilon(arbre->fils_droit);
        case '*': // Etoile
            return true;
        case '?': // Binaire : e? -> l'expression e zéro ou une fois
            return true;
        case '.': // Joker : désigne n'importe quel caractère
            return false;
        default:  // Lettre
            return false;
    }
}
```

Pas de surprise pour la constuction de cette fonction. À noter que *e?* est la même chose que *e|$\epsilon$* donc sa gestion ne pose pas de problème.

#### d. *`calculer_P`*

```C
ensemble_lettres_t* calculer_P (arbre_t* arbre) {
    if (arbre == NULL) return NULL;
    else switch (arbre->c) {
        ensemble_lettres_t* P_gauche; // (1)
        ensemble_lettres_t* P_droit;
        ensemble_lettres_t* P;
        case '@': // Concaténation // (2)
            P_gauche = calculer_P(arbre->fils_gauche); // (A)
            if (contient_epsilon(arbre->fils_gauche)) { // (B) 
                P_droit = calculer_P(arbre->fils_droit);
                P = union_ensemble_lettres(P_gauche, P_droit);
                liberer_liste_lettres(P_gauche); // (C)
                liberer_liste_lettres(P_droit);
                return P;
            }
            return P_gauche;
        case '|': // Union
            P_gauche = calculer_P(arbre->fils_gauche);
            P_droit = calculer_P(arbre->fils_droit);
            P = union_ensemble_lettres(P_gauche, P_droit);
            liberer_liste_lettres(P_gauche);
            liberer_liste_lettres(P_droit);
            return P;
        case '*': // Etoile
            return calculer_P(arbre->fils_gauche);
        case '?': // Binaire : e? -> l'expression e zéro ou une fois
            return calculer_P(arbre->fils_gauche);
        case '.': // Joker : désigne n'importe quel caractère
            return ajouter_lettre(NULL, '.');
        default:  // Lettre
            return ajouter_lettre(NULL, arbre->c);
    }
}
```

1. petite particularité la syntaxe C, on ne peut pas déclarer de variables dans le scope de case, on déclare donc tout ce qui nous sera utile pour la suite.
2. Détaillons (arbitrairement) le cas de la concaténation `@` :
    1. l'ensemble les premières lettres de e1@e2 contient les premières lettres de e1 donc on les calculs.
    2. Si $\epsilon \in$ e1 alors on rajoute les premières lettres de e2
    3. Et on oublie pas de prévenir les fuites mémoires 

#### d. *`calculer_D`*

```C
ensemble_lettres_t* calculer_D (arbre_t* arbre) {
    if (arbre == NULL) return NULL;
    else switch (arbre->c) {
        ensemble_lettres_t* D_gauche;
        ensemble_lettres_t* D_droit;
        ensemble_lettres_t* D;
        case '@': // Concaténation
            D_droit = calculer_D(arbre->fils_droit);
            if (contient_epsilon(arbre->fils_droit)) {
                D_gauche = calculer_D(arbre->fils_gauche);
                D = union_ensemble_lettres(D_gauche, D_droit);
                liberer_liste_lettres(D_gauche); 
                liberer_liste_lettres(D_droit);
                return D;
            }
            return D_droit;
        case '|': // Union
            D_gauche = calculer_D(arbre->fils_gauche);
            D_droit = calculer_D(arbre->fils_droit);
            D = union_ensemble_lettres(D_gauche, D_droit);
            liberer_liste_lettres(D_gauche);
            liberer_liste_lettres(D_droit);
            return D;
        case '*': // Etoile
            return calculer_D(arbre->fils_gauche);
        case '?': // Binaire : e? -> l'expression e zéro ou une fois
            return calculer_D(arbre->fils_gauche);
        case '.': // Joker : désigne n'importe quel caractère
            return ajouter_lettre(NULL, '.');
        default:  // Lettre
            return ajouter_lettre(NULL, arbre->c);
    }
}
```

*chercher-remplacer P par D*

#### e. Calculer F

##### i. *`fusion`*

```C
ensemble_facteurs_t* fusion (ensemble_lettres_t* ensemble_lettres1, ensemble_lettres_t* ensemble_lettres2) {
    ensemble_facteurs_t* ensemble_facteurs = NULL;
    while (ensemble_lettres1 != NULL) {
        ensemble_lettres_t* ensemble_lettres2_courant = ensemble_lettres2;
        while (ensemble_lettres2_courant != NULL) {
            facteur_t facteur = {ensemble_lettres1->head, ensemble_lettres2_courant->head};
            ensemble_facteurs = ajouter_facteur(ensemble_facteurs, facteur);
            ensemble_lettres2_courant = ensemble_lettres2_courant->tail;
        }
        ensemble_lettres1 = ensemble_lettres1->tail;
    }
    return ensemble_facteurs;
}
```

Produit cartésien classique. Les hypothèses sur l'ensemble de facteurs renvoyé sont vérifiées grâce aux hypothèses sur les ensembles de lettres donnés en argument.

##### ii. *`calculer_F`*

```C
ensemble_facteurs_t* calculer_F (arbre_t* arbre) {
    if (arbre == NULL) return NULL;
    else switch (arbre->c) {
        ensemble_lettres_t* D_gauche;
        ensemble_lettres_t* P_droit;
        ensemble_facteurs_t* F_milieu;
        ensemble_facteurs_t* F_gauche;
        ensemble_facteurs_t* F_droit;
        ensemble_facteurs_t* F_exterieur;
        ensemble_facteurs_t* F;
        case '@': // Concaténation
            D_gauche = calculer_D(arbre->fils_gauche);
            P_droit = calculer_P(arbre->fils_droit);
            F_milieu = fusion(D_gauche, P_droit);
            liberer_liste_lettres(D_gauche);
            liberer_liste_lettres(P_droit);
            F_gauche = calculer_F(arbre->fils_gauche);
            F_droit = calculer_F(arbre->fils_droit);
            F_exterieur = union_ensemble_facteurs(F_gauche, F_droit);
            liberer_liste_facteurs(F_gauche);
            liberer_liste_facteurs(F_droit);
            F = union_ensemble_facteurs(F_exterieur, F_milieu);
            liberer_liste_facteurs(F_exterieur);
            liberer_liste_facteurs(F_milieu);
            return F;
        case '|': // Union
            F_gauche = calculer_F(arbre->fils_gauche);
            F_droit = calculer_F(arbre->fils_droit);
            F = union_ensemble_facteurs(F_gauche, F_droit);
            liberer_liste_facteurs(F_gauche);
            liberer_liste_facteurs(F_droit);
            return F;
        case '*': // Etoile
            D_gauche = calculer_D(arbre->fils_gauche);
            P_droit = calculer_P(arbre->fils_gauche); // non, il n'y pas d'erreur sur cette ligne.
            F_milieu = fusion(D_gauche, P_droit);
            liberer_liste_lettres(D_gauche);
            liberer_liste_lettres(P_droit);
            F_exterieur = calculer_F(arbre->fils_gauche);
            F = union_ensemble_facteurs(F_exterieur, F_milieu);
            liberer_liste_facteurs(F_milieu);
            liberer_liste_facteurs(F_exterieur);
            return F;
        case '?': // Binaire : e? -> l'expression e zéro ou une fois
            return calculer_F(arbre->fils_gauche);
        case '.': // Joker : désigne n'importe quel caractère
            return NULL;
        default:  // Lettre
            return NULL;
    }
}
```

Calcul récursif de l'ensemble des facteurs de deux lettres comme détaillé dans le cours.

### C. Construction de l'automate des facteurs

#### a. Automates Finis déterministes : **`afd_t`**

```C
typedef struct afd_s {
    int taille_Sigma;
    int taille_Q;
    int initial;
    bool* finaux;
    int** delta;
} afd_t;

void liberer_afd (afd_t* afd) {
    free(afd->finaux);
    for (int q = 0; q < afd->taille_Q; q++) {
        free(afd->delta[q]);
    }
    free(afd->delta);
    free(afd);
}

afd_t* creer_afd (int taille_Sigma, int taille_Q) {
    afd_t* afd = malloc(sizeof(afd_t));
    afd->taille_Sigma = taille_Sigma;
    afd->taille_Q = taille_Q;
    afd->initial = 0; // (1)
    afd->finaux = malloc(sizeof(bool) * taille_Q);
    afd->delta = malloc(sizeof(int*) * taille_Q);
    for (int q = 0; q < taille_Q; q++) {
        afd->finaux[q] = false;
        afd->delta[q] = malloc(sizeof(int) * taille_Sigma);
        for (int a = 0; a < taille_Sigma; a++) {
            afd->delta[q][a] = -1; // (2)
        }
    }
    return afd;
}
```

1. L'état initial est par convention 0.
2. l'automate n'est pas complet; les transitions non définies sont initialisées vers "l'état" -1.

### D. Délinéariserisation

#### a. Automates finis "généralisés"

##### i. **`liste_entiers_t`**

```C
typedef struct liste_entiers_s {
    int head;
    struct liste_entiers_s* tail;
} liste_entiers_t;

void liberer_liste_entiers (liste_entiers_t* liste) {
    if (liste != NULL) {
        liberer_liste_entiers(liste->tail);
        free(liste);
    }
}

liste_entiers_t* ajouter_entier (liste_entiers_t* liste, int entier) {
    liste_entiers_t* nouvelle_liste = malloc(sizeof(liste_entiers_t));
    nouvelle_liste->head = entier;
    nouvelle_liste->tail = liste;
    return nouvelle_liste;
}
```

Même chose que pour **`liste_lettres_t`** et **`liste_facteurs_t`**.

##### ii. **`alphabet_t`**

```C
typedef struct alphabet_s {
    int taille;
    int lettres_to_index[ascii_table_size];
} alphabet_t;

alphabet_t* creer_alphabet (arbre_t* arbre) {
    alphabet_t* alphabet = malloc(sizeof(alphabet_t));
    alphabet->taille = 0;
    for (int k = 0; k < ascii_table_size; k++) alphabet->lettres_to_index[k] = -1;
    int index = 0;
    if (arbre != NULL) creer_alphabet_recursivement(arbre, alphabet, &index);
    return alphabet;
}

void creer_alphabet_recursivement(arbre_t* arbre, alphabet_t* alphabet, int* index_ptr) {
    switch (arbre->c) {
        case '@': // Concaténation
            creer_alphabet_recursivement(arbre->fils_gauche,alphabet, index_ptr);
            creer_alphabet_recursivement(arbre->fils_droit,alphabet, index_ptr);
            break;
        case '|': // Union
            creer_alphabet_recursivement(arbre->fils_gauche,alphabet, index_ptr);
            creer_alphabet_recursivement(arbre->fils_droit,alphabet, index_ptr);
            break;
        case '*': // Etoile
            creer_alphabet_recursivement(arbre->fils_gauche,alphabet, index_ptr);
            break;
        case '?': // Binaire : e? -> l'expression e zéro ou une fois
            creer_alphabet_recursivement(arbre->fils_gauche,alphabet, index_ptr);
            break;
        case '.': // Joker : désigne n'importe quel caractère
            break;
        default:  // Lettre
            if (alphabet->lettres_to_index[(int)arbre->c] == -1) {
                alphabet->lettres_to_index[(int)arbre->c] = *index_ptr;
                alphabet->taille++;
                (*index_ptr)++;
            }
            break;
    }
}
```

On crée un dictionnaire où les clés sont les lettres et les valeurs sont l'index dans l'alphabet.

##### iii. **`afg_t`**

```C
typedef struct afg_s {
    // Automate Fini "Généralisé" (particulier)
    // particulier car il possède toujours un unique état initial 
    // et seul des lettres ou des sigma-transition sont possibles
    alphabet_t* sigma;
    int taille_Q;
    bool* initiaux;
    bool* finaux;
    liste_entiers_t*** delta; // (1)
    liste_entiers_t** sigma_transitions; // (2)
} afg_t;

void liberer_afg (afg_t* afg) {
    free(afg->initiaux);
    free(afg->finaux);
    for (int q = 0; q < afg->taille_Q; q++) {
        for (int a = 0; a < afg->sigma->taille; a++) {
            liberer_liste_entiers(afg->delta[q][a]);
        }
        liberer_liste_entiers(afg->sigma_transitions[q]);
        free(afg->delta[q]);
    }
    free(afg->sigma);
    free(afg->sigma_transitions);
    free(afg->delta);
    free(afg);
}
```

1. Les transitions forment une matrice listes d'états, à la ligne $q$ et à la colonne $a$, on a la liste $\delta (q,a)$, $a$ n'est pas directement la lettre mais son index dans l'alphabet sigma.
2. sigma_transition\[$q$\] est la liste des états d'arrivés des $\Sigma$-transitions partant de l'état $q$.

```C
afg_t* creer_afg (alphabet_t* sigma, int taille_Q) {
    afg_t* afg = malloc(sizeof(afg_t));
    afg->taille_Q = taille_Q;
    afg->sigma = sigma;
    afg->initiaux = malloc(sizeof(bool) * taille_Q);
    afg->finaux = malloc(sizeof(bool) * taille_Q);
    afg->delta = malloc(sizeof(liste_entiers_t**) * taille_Q);
    afg->sigma_transitions = malloc(sizeof(liste_entiers_t*) * taille_Q);
    for (int q = 0; q < taille_Q; q++) {
        afg->initiaux[q] = false;
        afg->finaux[q] = false;
        afg->delta[q] = malloc(sizeof(liste_entiers_t*) * afg->sigma->taille);
        afg->sigma_transitions[q] = NULL;
        for (int a = 0; a < sigma->taille; a++) {
            afg->delta[q][a] = NULL;
        }
    }
    return afg;
}

```

On initialise proprement chaque champs du type **`afg_t`**. Il est notatemment important d'initialiser les listes à `NULL`.

#### b. Délinéariser

```C
afg_t* delineariser_afd (afd_t* afd, arbre_t* arbre, alphabet_t* sigma, char* tableau_de_delinearisation) {
    // /!\ l'arbre toujours linéarisé
    afg_t* afg = creer_afg(sigma, afd->taille_Q);

    // même état initial
    afg->initiaux[afd->initial] = true;

    // même états finaux
    for (int q = 0; q < afd->taille_Q; q++) {
        afg->finaux[q] = afd->finaux[q];
    }

    // On reconstruit les bonnes transitions
    int index = 0;
    if (arbre != NULL) delineariser_afd_recursivement(afd, arbre, tableau_de_delinearisation, &index, afg);

    // /!\ l'arbre a été linéarisé en même temps que l'afd
    return afg;
}
```

Les commentaires dans le code suffisent.

```C
void delineariser_afd_recursivement (afd_t* afd, arbre_t* arbre, char* tab, int* index_ptr, afg_t* afg) {
    assert(arbre != NULL);
    switch (arbre->c) {
        case '@': 
            delineariser_afd_recursivement(afd, arbre->fils_gauche, tab, index_ptr, afg);
            delineariser_afd_recursivement(afd, arbre->fils_droit, tab, index_ptr, afg);
            break;
        case '|': 
            delineariser_afd_recursivement(afd, arbre->fils_gauche, tab, index_ptr, afg);
            delineariser_afd_recursivement(afd, arbre->fils_droit, tab, index_ptr, afg);
            break;
        case '*': 
            delineariser_afd_recursivement(afd, arbre->fils_gauche, tab, index_ptr, afg);
            break;
        case '?': // Binaire : e? -> l'expression e zéro ou une fois
            delineariser_afd_recursivement(afd, arbre->fils_gauche, tab, index_ptr, afg);
            break;
        case '.': // Joker : désigne n'importe quel caractère
            fprintf(stderr, "Il ne peut y avoir de joker dans un arbre linéarisé\n"); // (1)
            assert(true);
            break;
        default:  // Lettre
            if (tab[*index_ptr] == '.') { // (2)
                for (int q = 0; q < afd->taille_Q; q++) {
                    int q_prime = afd->delta[q][meta_char_to_int(arbre->c)];
                    if (q_prime != -1) {
                        afg->sigma_transitions[q] = ajouter_entier(afg->sigma_transitions[q], q_prime);
                    }
                }
                arbre->c = '.';
            }
            else {
                for (int q = 0; q < afd->taille_Q; q++) { // (3)
                    int a_afd = meta_char_to_int(arbre->c);
                    int q_prime = afd->delta[q][a_afd];
                    if (q_prime != -1) {
                        int a_afg = afg->sigma->lettres_to_index[(int)tab[*index_ptr]];
                        afg->delta[q][a_afg] = ajouter_entier(afg->delta[q][a_afg], q_prime);
                    }
                }
                arbre->c = tab[*index_ptr];
            }
            (*index_ptr)++;
            break;
    }
}
```

1. Cette erreur ne peut pas être déclenchée par un comportement malvaillant de l'utilisateur, elle indiquerait plutôt un (gros) problème dans le code.
2. Si l'opérateur "non linéarisé" est un joker alors la transition dans l'AFD apparaît comme une $\Sigma$-transition dans l'AFG.
3. Si l'opérateur "non linéarisé" est une lettre alors la transition est conservée telle quelle.

### E. *`berry_sethi`*

```C
afg_t* berry_sethi (arbre_t* arbre) {
    // linéarisation de l'expression regulière
    int nb_lettres = compter_lettres(arbre);
    int nb_jokers = compter_jokers(arbre);
    alphabet_t* sigma = creer_alphabet(arbre);
    char* tableau_de_delinearisation = lineariser(arbre);

    // Calcul des ensembles P, D et F
    liste_lettres_t* P = calculer_P(arbre);
    liste_lettres_t* D = calculer_D(arbre);
    liste_facteurs_t* F = calculer_F(arbre);

    // initialisation afd vide avec pour état initial 0
    afd_t* afd = creer_afd(nb_lettres+nb_jokers, nb_lettres+nb_jokers+1);

    // gestion des états "initiaux"
    liste_lettres_t* initial_courant = P;
    while (initial_courant != NULL) {
        afd->delta[0][meta_char_to_int(initial_courant->head)]=meta_char_to_int(initial_courant->head)+1;//*
        initial_courant = initial_courant->tail;
    }
    liberer_liste_lettres(P);

    // gestion des états finaux
    liste_lettres_t* final_courant = D;
    while (final_courant != NULL) {
        afd->finaux[meta_char_to_int(final_courant->head)+1] = true; //*
        final_courant = final_courant->tail;
    }
    liberer_liste_lettres(D);
    if (contient_epsilon(arbre)) afd->finaux[0] = true;

    // gestion des transitions
    liste_facteurs_t* liaison_courante = F;
    while (liaison_courante != NULL) {
        int q = meta_char_to_int(liaison_courante->head.first)+1; //*
        int a = meta_char_to_int(liaison_courante->head.second);
        int q_prime = meta_char_to_int(liaison_courante->head.second)+1; //*
        afd->delta[q][a] = q_prime;
        liaison_courante = liaison_courante->tail;
    }
    liberer_liste_facteurs(F);

    // déliniarisation
    afg_t* afg = delineariser_afd(afd, arbre, sigma, tableau_de_delinearisation);

    // on fait le ménage et on renvoie
    free(tableau_de_delinearisation);
    liberer_afd(afd);
    return afg;
}
```

On rassemble toutes les éléments et fonctions que l'on a construit plus tôt. \
Les commentaires dans le codes suffisent. \
\* les `+1` sont ici car j'ai choisi que l'état initial soit l'état 0, ainsi il faut "décaler" tous les autres états.

### F. *`construire_automate`*

```C
afg_t* construire_automate(char* expression) {
    arbre_t* arbre = arboriser(expression);
    afg_t* automate = berry_sethi(arbre);
    liberer_arbre(arbre);
    return automate;
}
```

## 5. Lire un mot dans un automate fini "généralisé"

``` C
void modifier_tableau_avec_liste_entiers (bool* tableau, liste_entiers_t* liste) {
    while (liste != NULL) {
        tableau[liste->head] = true;
        liste = liste->tail;
    }
}
```

On représente les ensembles d'états avec des tableaux booléens, j'aurais pu créer un type **`ensemble_entiers_t`** sur le modèle de **`ensemble_lettres_t`** pour optimiser *`afg_reconnait`*. Et donc implémenter *`union_ensemble_entiers`*. C'est une piste d'améliorations possible.

```C
bool afg_reconnait (afg_t* afg, char* mot) {
    // ensemble d'états initiaux
    bool* meta_etat_courant = malloc(sizeof(bool)* afg->taille_Q);
    bool* prochain_meta_etat = malloc(sizeof(bool) * afg->taille_Q);
    for (int q = 0; q < afg->taille_Q; q++) {
        meta_etat_courant[q] = afg->initiaux[q];
        prochain_meta_etat[q] = false;
    }

    // lecture du mot
    int i = 0;
    while(mot[i] != '\0' && mot[i] != '\n') { // (1)

        // Calcul du prochain "meta-état"
        for (int q = 0; q < afg->taille_Q; q++) { // (2)
            if (meta_etat_courant[q]) {
                modifier_tableau_avec_liste_entiers(prochain_meta_etat, afg->sigma_transitions[q]); // (A)
                int a = afg->sigma->lettres_to_index[(int)mot[i]]; // (B)
                if (a != -1) { 
                    modifier_tableau_avec_liste_entiers(prochain_meta_etat, afg->delta[q][a]);
                }
            }
        }

        // Mise à jour du "méta-état" courant
        for (int q = 0; q < afg->taille_Q; q++) { // (3)
            meta_etat_courant[q] = prochain_meta_etat[q];
            prochain_meta_etat[q] = false;
        }

        i++;
    }

    // On regarde l'intersection entre l'ensemble d'état d'arrivés (le "méta-état" d'arrivé) et l'ensemble des états finaux de l'automate
    for (int q = 0; q < afg->taille_Q; q++) {
        if (meta_etat_courant[q] && afg->finaux[q]) {
            free(meta_etat_courant);
            free(prochain_meta_etat);
            return true;
        }
    }

    free(meta_etat_courant);
    free(prochain_meta_etat);
    return false;
}
```

Un "méta-état" est un ensemble d'états de l'automate.
Détaillons la lecture du mot dans l'automate :
1. On met à jour le "méta-état" courant pour chaque lettre du mot
2. Pour chaque état dans le "méta-état courant"
    1. On regarde les $\Sigma$-transitions
    2. Et si la lettre appartient à l'alphabet alors on regarde les transitions "classiques".
3. On met à jour le "méta-état" courant à l'aide du "méta-état" calculé.

## 6. Test

### A. Question 2

mots de la langue française qui contiennent à la fois un q et un w.
- `commande`
```bash
./mygrep '.*q.*w.*@@@@.*w.*q.*@@@@|' Donnees_grep/francais.txt 
```
- `sortie`
```
clownesque
clownesques
squaw
squaws
wisigothique
wisigothiques
```
- `temps`
```
real	0m0.186s
user	0m0.186s
sys	0m0.000s
```

Comparons avec la commandes grep :
- `commande`
```bash
grep -E '(q.*w)|(w.*q)' Donnees_grep/francais.txt 
```
- `sortie`
```
clownesque
clownesques
squaw
squaws
wisigothique
wisigothiques
```
- `temps`
```
real	0m0.005s
user	0m0.005s
sys	0m0.000s
```

### B. Question 3

- `commandes`
```bash
./mygrep 'aa?@*' Donnees_grep/aXXb.txt
```
```bash
grep -E '^(aa?)*$' Donnees_grep/aXXb.txt
```
- `temps ("réels")`
|XX|./mygrep|-grep|
|-|-|-|
|20|0.001s|0.002s|
|25|0.003s|0.001s|
|30|0.002s|0.003s|
|35|0.002s|0.002s|
|40|0.003s|0.002s|
|1000000*|0.025s|0.009s|

\*  - Pour ce test on a modifié légerement la fonction *`main`* :
```C
size_t taille_zone_tampon = 1000001;
```
et on a utiliser ce code (python) pour générer a1Mb.txt :
```python
file1 = open('a1Mb.txt', 'w')
a1Mb = "a"*(10**6)+"b"
file1.write(a1Mb)
file1.close()
```

- `commentaire` \
Sur des entrées "normales" les temps sont trop petits pour être comparés. Sur les plus "gros" test (français.txt et a1Mb.txt) on voit très clairement la rapidité de `grep` mais ça reste satisfaisant.

### C. batterie de test supplémentaire

`test.txt`

----
```
a

\n
\0
|
.
@
*
?
é
學
Eliott

```
----
(il y a une ligne vide à la fin)

On test, dans l'ordre des ligne :
1. un unique caractère
2. une ligne vide (donc juste '\n'). Pour tester le langage vide.
3. une ligne avec les caractères \ et n. Pour essayer de tromper l'ennemi.
4. idem
5. Un opérateur.
6. idem
7. idem
8. idem
9. idem
10. ascii étendu
11. l'idéogramme chinois (signifiant apprendre, connaissance, école...) de code unicode U+5B78
12. une ligne classique (le témoin)
13. une ligne vide (vraiment vide sans retour à la ligne)

**test `1`**
- `commande`
```bash
./mygrep '.' test.txt
```
- `sortie`
```
a
|
.
@
*
?
```
- `commande`
```bash
grep -E '^.$' test.txt
```
- `sortie`
```
a
|
.
@
*
?
é
學
```
- `commentaire` \
`mygrep` se comporte mal, mais comme prévu, il ne gère pas les caractère non ascii. 

**test `2`**
- `commande`
```bash
./mygrep '..@' test.txt
```
- `sortie`
```
\n
\0
é
```
- `commande`
```bash
grep -E '^..$' test.txt
```
- `sortie`
```
\n
\0
```
- `commentaire` \
Il semblerait que `é` soit considéré comme 2 caractères. Mais encore une fois mygrep échoue seulement là où on l'attend.

**test `3`**
- `commande`
```bash
./mygrep '' test.txt
```
- `sortie`
```


```
(2 lignes vides)
- `commande`
```bash
grep -E '^$' test.txt
```
- `sortie`
```


```
(idem, 2 lignes vides)
- `commentaire`\
Sans fautes; on a penser notre code pour ça (cf 4.B.c.i Cas du langage vide)

**test `4`**
- `commande`
```bash
./mygrep '.*' test.txt
```
- `sortie`
```
a

\n
\0
|
.
@
*
?
é
學
eliott


```
(il y a une ligne vide à la fin)
- `commande`
```bash
grep -E '^.*$' test.txt
```
- `sortie`
```
a

\n
\0
|
.
@
*
?
é
學
eliott


```
(idem, il y a une ligne vide à la fin)
- `commentaire` \
Je suis impressionné du résultat de ce test et je ne sais l'expliquer.

**test `5`**
- `commande`
```bash
./mygrep '...@@' test.txt
```
- `sortie`
```
Segmentation fault
```
- `commande`
```bash
grep -E '^...$' test.txt
```
- `sortie`
```
```
- `commentaire` \
Avec un débuggage supplémentaire on voit que l'erreur `Segmentation fault` est soulevée lors du traitement de `學`. Le bug n'est donc ni surprenant, ni inquiétant et doit être lié à l'encodage de `學`.

**test `6`**
- `commande`
```bash
./mygrep 'é' test.txt
```
- `sortie`
```
ERREUR : tentative d'insertion dans un arbre complet.
```
- `commande`
```bash
grep -E '^é$' test.txt
```
- `sortie`
```
é
```
- `commentaire` \
C'est cohérent avec le **test `2`**. À noté que l'erreur n'est pas bloquante dans le code donc si aucune ligne n'est affichée après l'erreur c'est qu'aucune ligne ne correspond au premier caractère mystère qui compose `é`.

**test `7`**
- `commande`
```bash
./mygrep '學' test.txt
```
- `sortie`
```
ERREUR : tentative d'insertion dans un arbre complet.
ERREUR : tentative d'insertion dans un arbre complet.
```
- `commande`
```bash
grep -E '^學$' test.txt
```
- `sortie`
```
學
```
- `commentaire` \
Plus surprenant, le programme se comporte comme si l'on essayait d'insérer 3 lettres dans un arbre. Avec l'addition des résultats des **test `2` et `5`**, je conjecture que le caractère `學` est encodé sur 25 à 31 bits, c'est à dire sctrictement plus de 3 octects et strictement moins de 4 octets.

**test `8`**
- `commande`
```bash
./mygrep '\.@' test.txt
```
- `sortie`
```
\n
\0
é
```
- `commande`
```bash
grep -E '^\\.$' test.txt
```
- `sortie`
```
\n
\0
```
- `commentaire` \
`\`, voilà l'identité du premier caractère mystère du couple de caractère qui forme `é`.

**test `9`**
- `commande`
```bash
time ./mygrep '.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@' Donnees_grep/a20b.txt
```
- `sortie`
```
aaaaaaaaaaaaaaaaaaaab

real	0m0.021s
user	0m0.021s
sys	0m0.000s
```
- `commande`
```bash
time grep -E '^.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?.?$' Donnees_grep/a20b.txt
```
- `sortie`
```
aaaaaaaaaaaaaaaaaaaab

real	0m0.012s
user	0m0.012s
sys	0m0.000s
```
- `commentaire`
Ce petit test permet de comparer `grep` et `mygrep` sur la taille de l'expression régulière et non sur la taille du fichier : il y a 100 `.?`. Le résultat est satisfaisant et même mieux qu'attendu.

**test `10`** (promis c'est le dernier)
- `commandes`
```bash
touch test2.txt
./mygrep '' test2.txt
rm test2.txt
```
- `sortie`
```
```
- `commande`
```bash
touch test2.txt
grep -E '^$' test2.txt
rm test2.txt
```
- `sortie`
```
```
- `commentaire` \
Plus vide que ça, on ne peut pas faire. Il est normal que `mygrep` reste correct dans car il y a **aucune** ligne, ainsi il ne rentre pas dans la boucle principale :
```C
while (getline(&zone_tampon, &taille_zone_tampon, fichier) != EOF) { ... }
```

## 7. Commentaires

Le code compile sans erreur ni avertissement avec :
```bash
gcc mygrep.c -o mygrep -Wall -Wextra -fsanitize=address -g
```

J'ai renoncé à la question 4. 

J'ai fait de mon mieux pour rendre ce document propre et lisible. Je crois que je comprends un peu mieux la frustration de rédiger de beaux corrigers de TP et de voir qu'ils ne sont pas lus. Bonne chance pour la correction de ce TP-DM. 
