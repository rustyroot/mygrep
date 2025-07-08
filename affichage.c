void parcours_postfixe (arbre_t* arbre) {
    if (arbre != NULL) {
        parcours_postfixe(arbre->fils_gauche);
        parcours_postfixe(arbre->fils_droit);
        printf("%c\n", arbre->c);
    }
}

void afficher_arbre_recursivement (arbre_t* arbre) {
    assert(arbre != NULL);
    switch (arbre->c) {
        case '@': // Concaténation
            afficher_arbre_recursivement(arbre->fils_gauche);
            afficher_arbre_recursivement(arbre->fils_droit);
            break;
        case '|': // Union
            printf("(");
            afficher_arbre_recursivement(arbre->fils_gauche);
            printf("|");
            afficher_arbre_recursivement(arbre->fils_droit);
            printf(")");
            break;
        case '*': // Etoile
            printf("(");
            afficher_arbre_recursivement(arbre->fils_gauche);
            printf(")*");
            break;
        case '?': // Binaire : e? -> l'expression e zéro ou une fois
            printf("(");
            afficher_arbre_recursivement(arbre->fils_gauche);
            printf(")?");
            break;
        case '.': // Joker : désigne n'importe quel caractère
            printf(".");
            break;
        default:  // Lettre
            printf("%c", arbre->c);
            break;
    }
}

void afficher_arbre (arbre_t* arbre) {
    if (arbre == NULL) {
        printf("arbre vide\n");
    }
    else {
        afficher_arbre_recursivement(arbre);
        printf("\n");
    }
}

void afficher_liste_lettres (liste_lettres_t* liste) {
    while (liste != NULL) {
        printf("%c ", liste->head);
        liste = liste->tail;
    }
    printf("\n");
}

void afficher_liste_facteurs (liste_facteurs_t* liste) {
    while (liste != NULL) {
        printf("%c%c ", liste->head.first, liste->head.second);
        liste = liste->tail;
    }
    printf("\n");
}

void afficher_liste_entiers (liste_entiers_t* liste) {
    while (liste != NULL) {
        printf("%d ", liste->head);
        liste = liste->tail;
    }
    printf("\n");
}

void afficher_alphabet (alphabet_t* alphabet) {
    printf("{");
    int nb_lettres_affichees = 0;
    for (int a = 0; a < 256; a++) {
        if (alphabet->lettres_to_index[a] != -1) {
            printf("%c", (char)a);
            nb_lettres_affichees++;
            if (nb_lettres_affichees < alphabet->taille) printf(", ");             
        }
        
    }
    printf("}\n");
}

void afficher_afg (afg_t* afg) {
    // sigma 
    char* index_to_lettres = malloc(sizeof(char) * afg->sigma->taille);
    printf("sigma = ");
    printf("{");
    int index = 0;
    for (int i = 0; i < 256; i++) {
        if (afg->sigma->lettres_to_index[i] != -1) {
            char a = (char)i;
            printf("%c", a);
            index_to_lettres[index] = a;
            index++;
            if (index < afg->sigma->taille) printf(", ");             
        }
        
    }
    printf("}\n");

    // Q
    printf("|Q| = %d\n", afg->taille_Q);

    // I
    printf("I = {");
    for (int q = 0; q < afg->taille_Q; q++) {
        if (afg->initiaux[q]) printf("%d ", q);
    }
    printf("}\n");

    // F
    printf("F = {");
    for (int q = 0; q < afg->taille_Q; q++) {
        if (afg->finaux[q]) printf("%d ", q);
    }
    printf("}\n");

    // delta
    printf("delta :\n");
    for (int q = 0; q < afg->taille_Q; q++) {
        printf("  delta(%d, *)\n", q);
        for (int i = 0; i < afg->sigma->taille; i++) {
            char a = index_to_lettres[i];
            printf("    delta(%d, %c) = ", q, a);
            afficher_liste_entiers(afg->delta[q][i]);
        }
    }

    printf("sigma-transitions :\n");
    for (int q = 0; q < afg->taille_Q; q++) {
        printf("  delta(%d, .) = ", q);
        afficher_liste_entiers(afg->sigma_transitions[q]);
    }

    free(index_to_lettres);
}

void afficher_tableau_bool(int taille, bool* tableau) {
    printf("[");
    for (int i = 0; i < taille; i++) {
        printf("%d", tableau[i]);
        if (i < taille-1) printf(", ");
    }
    printf("]\n");
}