#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define ascii_table_size 128
#define first_meta_char (char)ascii_table_size

int meta_char_to_int (char a) {
    return (int)(a - first_meta_char);
}

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

typedef struct arbre_s {
    char c;
    struct arbre_s* fils_gauche;
    struct arbre_s* fils_droit;
    int libertes; // indique le nombre de feuille encore libre
} arbre_t;

void liberer_arbre (arbre_t* arbre) {
    if (arbre != NULL) {
        liberer_arbre(arbre->fils_gauche);
        liberer_arbre(arbre->fils_droit);
    }
    free(arbre);
}

arbre_t* inserer (arbre_t* arbre_courant, char c) {
    if (arbre_courant == NULL) {
        arbre_t* nouvelle_arbre = malloc(sizeof(arbre_t));
        nouvelle_arbre->c = c;
        nouvelle_arbre->fils_gauche = NULL;
        nouvelle_arbre->fils_droit = NULL;
        nouvelle_arbre->libertes = arite(c);
        return nouvelle_arbre;
    }
    else {
        switch (arbre_courant->libertes) {
            case 2:
                arbre_courant->fils_droit = inserer(arbre_courant->fils_droit, c);
                if (arbre_courant->fils_droit->libertes == 0) arbre_courant->libertes--;
                return arbre_courant;
            case 1:
                arbre_courant->fils_gauche = inserer(arbre_courant->fils_gauche, c);
                if (arbre_courant->fils_gauche->libertes == 0) arbre_courant->libertes--;
                return arbre_courant;
            default: 
                if (arbre_courant->libertes == 0) 
                    fprintf(stderr,"ERREUR : tentative d'insertion dans un arbre complet.\n");
                else 
                    fprintf(stderr, "ERREUR : le nombre de libertés de l'opérateur a la valeur interdite %d.\n", arbre_courant->libertes);
                return arbre_courant;
        }
    }
}

arbre_t* arboriser (char* expression_polonaise_inverse) {
    int index = strlen(expression_polonaise_inverse)-1;
    arbre_t* arbre_courant = NULL;
    while (index >= 0) {
        arbre_courant = inserer(arbre_courant, expression_polonaise_inverse[index]);
        index--;
    }
    return arbre_courant;
}

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
        case '.': // Joker : désigne n'importe quel caractère
            tab[*index_ptr] = arbre->c;
            arbre->c = first_meta_char + *index_ptr;
            (*index_ptr)++;
            break;
        default:  // Lettre
            tab[*index_ptr] = arbre->c;
            arbre->c = first_meta_char + *index_ptr;
            (*index_ptr)++;
            break;
    }
}

char* lineariser (arbre_t* arbre) {
    int nb_lettres = compter_lettres(arbre);
    int nb_jokers = compter_jokers(arbre);
    char* tableau_de_delinearisation = malloc(sizeof(char) * (nb_lettres+nb_jokers));
    int index = 0;
    if (arbre != NULL) lineariser_recursivement(arbre, tableau_de_delinearisation, &index);
    return tableau_de_delinearisation;
}

typedef struct liste_lettres_s {
    char head;
    struct liste_lettres_s* tail;
} liste_lettres_t;

void liberer_liste_lettres (liste_lettres_t* liste) {
    if (liste != NULL) {
        liberer_liste_lettres(liste->tail);
        free(liste);
    }
}

liste_lettres_t* ajouter_lettre (liste_lettres_t* liste, char lettre) {
    liste_lettres_t* nouvelle_liste = malloc(sizeof(liste_lettres_t));
    nouvelle_liste->head = lettre;
    nouvelle_liste->tail = liste;
    return nouvelle_liste;
}

typedef liste_lettres_t ensemble_lettres_t;
// les ensembles de lettres sont les listes de lettres contenant une famille strictement croissante.

ensemble_lettres_t* union_ensemble_lettres (ensemble_lettres_t* ensemble_lettres1, ensemble_lettres_t* ensemble_lettres2) {
    if (ensemble_lettres1 != NULL && ensemble_lettres2 != NULL) {
        if (ensemble_lettres1->head == ensemble_lettres2->head) {
            return ajouter_lettre(union_ensemble_lettres(ensemble_lettres1->tail, ensemble_lettres2->tail), ensemble_lettres2->head);
        }
        else if (ensemble_lettres1->head < ensemble_lettres2->head) {
            return ajouter_lettre(union_ensemble_lettres(ensemble_lettres1->tail, ensemble_lettres2), ensemble_lettres1->head);
        }
        else {
            return ajouter_lettre(union_ensemble_lettres(ensemble_lettres1, ensemble_lettres2->tail), ensemble_lettres2->head);
        }
    }
    else if (ensemble_lettres1 == NULL && ensemble_lettres2 == NULL) {
        return NULL;
    }
    else if (ensemble_lettres1 == NULL) {
        return ajouter_lettre(union_ensemble_lettres(ensemble_lettres1, ensemble_lettres2->tail), ensemble_lettres2->head);
    }
    else {
        return ajouter_lettre(union_ensemble_lettres(ensemble_lettres1->tail, ensemble_lettres2), ensemble_lettres1->head);
    }
}

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

typedef liste_facteurs_t ensemble_facteurs_t;
// Les ensembles de facteurs sont les listes contenants une famille strictement croissante

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

ensemble_lettres_t* calculer_P (arbre_t* arbre) {
    if (arbre == NULL) return NULL;
    else switch (arbre->c) {
        ensemble_lettres_t* P_gauche;
        ensemble_lettres_t* P_droit;
        ensemble_lettres_t* P;
        case '@': // Concaténation
            P_gauche = calculer_P(arbre->fils_gauche);
            if (contient_epsilon(arbre->fils_gauche)) {
                P_droit = calculer_P(arbre->fils_droit);
                P = union_ensemble_lettres(P_gauche, P_droit);
                liberer_liste_lettres(P_gauche); 
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
    afd->initial = 0;
    afd->finaux = malloc(sizeof(bool) * taille_Q);
    afd->delta = malloc(sizeof(int*) * taille_Q);
    for (int q = 0; q < taille_Q; q++) {
        afd->finaux[q] = false;
        afd->delta[q] = malloc(sizeof(int) * taille_Sigma);
        for (int a = 0; a < taille_Sigma; a++) {
            afd->delta[q][a] = -1;
        }
    }
    return afd;
}

typedef struct liste_entiers_s {
    int head;
    struct liste_entiers_s* tail;
}liste_entiers_t;

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

typedef liste_entiers_t ensemble_entiers_t;
// les ensembles de entiers sont les listes de entiers contenant une famille strictement croissante.

ensemble_entiers_t* union_ensemble_entiers (ensemble_entiers_t* ensemble_entiers1, ensemble_entiers_t* ensemble_entiers2) {
    if (ensemble_entiers1 != NULL && ensemble_entiers2 != NULL) {
        if (ensemble_entiers1->head == ensemble_entiers2->head) {
            return ajouter_entier(union_ensemble_entiers(ensemble_entiers1->tail, ensemble_entiers2->tail), ensemble_entiers2->head);
        }
        else if (ensemble_entiers1->head < ensemble_entiers2->head) {
            return ajouter_entier(union_ensemble_entiers(ensemble_entiers1->tail, ensemble_entiers2), ensemble_entiers1->head);
        }
        else {
            return ajouter_entier(union_ensemble_entiers(ensemble_entiers1, ensemble_entiers2->tail), ensemble_entiers2->head);
        }
    }
    else if (ensemble_entiers1 == NULL && ensemble_entiers2 == NULL) {
        return NULL;
    }
    else if (ensemble_entiers1 == NULL) {
        return ajouter_entier(union_ensemble_entiers(ensemble_entiers1, ensemble_entiers2->tail), ensemble_entiers2->head);
    }
    else {
        return ajouter_entier(union_ensemble_entiers(ensemble_entiers1->tail, ensemble_entiers2), ensemble_entiers1->head);
    }
}

typedef struct alphabet_s {
    int taille;
    int lettres_to_index[ascii_table_size];
} alphabet_t;


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

alphabet_t* creer_alphabet (arbre_t* arbre) {
    alphabet_t* alphabet = malloc(sizeof(alphabet_t));
    alphabet->taille = 0;
    for (int k = 0; k < ascii_table_size; k++) alphabet->lettres_to_index[k] = -1;
    int index = 0;
    if (arbre != NULL) creer_alphabet_recursivement(arbre, alphabet, &index);
    return alphabet;
}

typedef struct afg_s {
    // Automate Fini "Généralisé" (particulier)
    // particulier car il possède toujours un unique état initial 
    // et seul des lettres ou des sigma-transition sont possibles
    alphabet_t* sigma;
    int taille_Q;
    bool* initiaux;
    bool* finaux;
    liste_entiers_t*** delta;
    liste_entiers_t** sigma_transitions;
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
            fprintf(stderr, "Il ne peut y avoir de joker dans un arbre linéarisé\n");
            assert(true);
            break;
        default:  // Lettre
            if (tab[*index_ptr] == '.') {
                for (int q = 0; q < afd->taille_Q; q++) {
                    int q_prime = afd->delta[q][meta_char_to_int(arbre->c)];
                    if (q_prime != -1) {
                        afg->sigma_transitions[q] = ajouter_entier(afg->sigma_transitions[q], q_prime);
                    }
                }
                arbre->c = '.';
            }
            else {
                for (int q = 0; q < afd->taille_Q; q++) {
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

afg_t* delineariser_afd (afd_t* afd, arbre_t* arbre, alphabet_t* sigma, char* tableau_de_delinearisation) {
    // /!\ l'arbre toujours linéarisé
    afg_t* afg = creer_afg(sigma, afd->taille_Q);

    // même état initial
    afg->initiaux[afd->initial] = true;

    // même états finaux
    for (int q = 0; q < afd->taille_Q; q++) {
        afg->finaux[q] = afd->finaux[q];
    }

    int index = 0;
    delineariser_afd_recursivement(afd, arbre, tableau_de_delinearisation, &index, afg);

    // /!\ l'arbre a été linéarisé en même temps que l'afd
    return afg;
}

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
        afd->delta[0][meta_char_to_int(initial_courant->head)] = meta_char_to_int(initial_courant->head)+1;
        initial_courant = initial_courant->tail;
    }
    liberer_liste_lettres(P);

    // gestion des états finaux
    liste_lettres_t* final_courant = D;
    while (final_courant != NULL) {
        afd->finaux[meta_char_to_int(final_courant->head)+1] = true;
        final_courant = final_courant->tail;
    }
    liberer_liste_lettres(D);
    if (contient_epsilon(arbre)) afd->finaux[0] = true;

    // gestion des transitions
    liste_facteurs_t* liaison_courante = F;
    while (liaison_courante != NULL) {
        int q = meta_char_to_int(liaison_courante->head.first)+1;
        int a = meta_char_to_int(liaison_courante->head.second);
        int q_prime = meta_char_to_int(liaison_courante->head.second)+1;
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

void modifier_tableau_avec_liste_entiers (bool* tableau, liste_entiers_t* liste) {
    while (liste != NULL) {
        tableau[liste->head] = true;
        liste = liste->tail;
    }
}

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
    while(mot[i] != '\0' && mot[i] != '\n') {

        // Calcul du prochain "meta-état"
        for (int q = 0; q < afg->taille_Q; q++) {
            if (meta_etat_courant[q]) {
                modifier_tableau_avec_liste_entiers(prochain_meta_etat, afg->sigma_transitions[q]);
                int a = afg->sigma->lettres_to_index[(int)mot[i]];
                if (a != -1) {
                    modifier_tableau_avec_liste_entiers(prochain_meta_etat, afg->delta[q][a]);
                }
            }
        }

        // Mise à jour du "méta-état" courant
        for (int q = 0; q < afg->taille_Q; q++) {
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

afg_t* construire_automate(char* expression) {
    arbre_t* arbre = arboriser(expression);
    afg_t* automate = berry_sethi(arbre);
    liberer_arbre(arbre);
    return automate;
}

int main (int argc, char* argv[]) {

    if (argc > 2) {
        afg_t* automate = construire_automate(argv[1]);
        FILE* fichier = fopen(argv[2], "r");
        size_t taille_zone_tampon = 256;
        char* zone_tampon = malloc(sizeof(char) * taille_zone_tampon);
        while (getline(&zone_tampon, &taille_zone_tampon, fichier) != EOF) {
            if (afg_reconnait(automate, zone_tampon)) {
                printf("%s", zone_tampon);
            }
        }
        free(zone_tampon);
        fclose(fichier);
        liberer_afg(automate);
    }

    return 0;
}