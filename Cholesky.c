#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void Decomposition(float *A, float *B, int N);
void TringularisationInferieure(float *B, float *s, float *y, int N);
void TringularisationSuperieure(float *B, float *y, float *x, int N);
void getData(const char *filename, float *A, float *s, int N);
void AffMatrice(float *mat, int N);
void AffVecteur(float *vec, int N);

int main() {
    int N = 4; // Taille de la matrice et des vecteurs

    // Allocation dynamique des matrices et vecteurs
    float *A = (float *)malloc(N * N * sizeof(float));
    float *B = (float *)calloc(N * N, sizeof(float)); // Initialise à 0
    float *s = (float *)malloc(N * sizeof(float));
    float *y = (float *)malloc(N * sizeof(float));
    float *x = (float *)malloc(N * sizeof(float));

    // Lecture des donnees depuis le fichier
    getData("data.txt", A, s, N);

    printf("Matrice A lue depuis le fichier :\n");
    AffMatrice(A, N);

    printf("\nVecteur s lu depuis le fichier :\n");
    AffVecteur(s, N);

    // etape 1 : Decomposition de Cholesky
    Decomposition(A, B, N);
    printf("\nMatrice B apres la decomposition :\n");
    AffMatrice(B, N);

    // etape 2 : Resolution de B * y = s
    TringularisationInferieure(B, s, y, N);
    printf("\nle vecteur y  de B * y = s :\n");
    AffVecteur(y, N);

    // etape 3 : Resolution de B^T * x = y
    TringularisationSuperieure(B, y, x, N);
    printf("\nVecteur x apres resolution de B^T * x = y :\n");
    AffVecteur(x, N);

    // Liberation de la memoire
    free(A);
    free(B);
    free(s);
    free(y);
    free(x);

    return 0;
}

// Fonction pour effectuer la decomposition pour calculer B

void Decomposition(float *A, float *B, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j <= i; j++) {
             float sum = 0;
             for (int k = 0; k < j; k++) {
                sum += B[i * N + k] * B[j * N + k];
            }
            if (i == j) {
                 B[i * N + j] = sqrtf(A[i * N + i] - sum);
            } else {
                 B[i * N + j] = (A[i * N + j] - sum) / B[j * N + j];
            }
        }
    }
}

// Resolution de B * y = s
void TringularisationInferieure(float *B, float *s, float *y, int N) {
    for (int i = 0; i < N; i++) {
         float sum = 0;
         for (int j = 0; j < i; j++) {
         sum += B[i * N + j] * y[j];
        }
     y[i] = (s[i] - sum) / B[i * N + i];
    }
}

// Resolution de B^T * x = y
void TringularisationSuperieure(float *B, float *y, float *x, int N) {
    for (int i = N - 1; i >= 0; i--) {
        float sum = 0;
        for (int j = i + 1; j < N; j++) {
            sum += B[j * N + i] * x[j];
        }
     x[i] = (y[i] - sum) / B[i * N + i];
    }
}

// Fonction pour lire toutes les donnees à partir d'un fichier
void getData(const char *filename, float *A, float *s, int N) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    int dimension;
    fscanf(file, "%d", &dimension);
    if (dimension != N) {
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Lecture de la matrice A
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fscanf(file, "%f", &A[i * N + j]);
        }
    }

    // Lecture du vecteur s
    for (int i = 0; i < N; i++) {
        fscanf(file, "%f", &s[i]);
    }

    fclose(file);
}

// Affichage d'une matrice
void AffMatrice(float *mat, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%f ", mat[i * N + j]);
        }
        printf("\n");
    }
}

// Affichage d'un vecteur
void AffVecteur(float *vec, int N) {
    for (int i = 0; i < N; i++) {
        printf("%f ", vec[i]);
    }
    printf("\n");
}