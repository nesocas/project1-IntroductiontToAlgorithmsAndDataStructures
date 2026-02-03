/* iaed-23 - ist1107501 - project1 */

/*
* File: project1.c
* Author: Inês Fernandes Bentes
* Description: A program used to create bus stops, 
lines and connections.
*/

#include <stdio.h>
#include <string.h>

#define MAXp 10000
#define MAXPchar 51
#define MAXc 200
#define MAXCchar 21
#define MAXl 30000

struct paragem {
    char nome[MAXPchar];
    double latitude;
    double longitude;
};

struct carreira {
    char carreiraName[MAXCchar];
};

struct ligacao {
    char carLig[MAXCchar];
    char paragens[MAXPchar];
    char parVef[MAXPchar];
};

struct nomeLig {
    struct ligacao pars[MAXPchar];
    struct ligacao parsVef[MAXPchar]; 
};

/* global variables used across multiple functions */
struct paragem coordenadas[MAXp];
struct paragem nomeParagem[MAXPchar];
struct carreira nomeCarreira[MAXCchar];
struct nomeLig nomeCarLig[MAXCchar];
struct nomeLig paragenss[MAXPchar];

/* variables related to "carreira" commands */
int numCarreiras = 0, cPass=0, cdNeg=0;
/* variables related to "ligacao" and "interligacao" commands */
int numECnome=0, p2o=0, p1d=0, countLig=0, numPlig[MAXc], eL=0;
char nCar[MAXCchar], p1[MAXPchar], p2[MAXPchar];
float cost[MAXc], duration[MAXc], custo, duracao;
char* third_space;
char* second_space;
/* variables related to "paragem" commands */
int numParagens=0, numStopNames=0;
/* variables related to errors */
int numLFlag=0, p12Erro=0, numE=0, noP, vPchar=0;
char nomeErro[MAXp][MAXPchar], nomeNErro[MAXp][MAXPchar]; 
char erroLine[MAXc][MAXCchar], vParagem[MAXp][MAXPchar];

/* adds new bus stops */
void paragem(char stopName[MAXPchar], double lat, double longi) {
    struct paragem p;
    char stopNames[MAXp][MAXPchar];
    int i=0;

    if (strlen(stopName) >= MAXPchar) {
        return;
    }
    if (numParagens >= MAXp) {
        return;
    }
    
    /* compares stopNames to the ones that already exist */
    while (i < numStopNames) {
        if (strcmp(stopName, stopNames[i]) == 0) {
            strcpy(nomeErro[numE], stopName);
            numE++;
            return;
        }
        i++;
    }
    
    /* the names didn't match with any other previous names 
    are sent into stopName and it counts the number of new stops */
    strcpy(stopNames[numStopNames], stopName);
    numStopNames++;

    strcpy(nomeParagem[numParagens].nome, stopName);
    p.latitude = lat;
    p.longitude = longi;
    coordenadas[numParagens] = p;

    numParagens++;
}

/* if numE > 0 it means that a matching stop was found, 
it prints every single matching stop found until it ends */
void StopAlreadyExists(int numE) {
    int numEN;
    for (numEN=0; numE > 0 && numEN < numE; numEN++) {
        printf("%s: stop already exists.\n", nomeErro[numEN]);
    }
    return;
}
/* if numEF > 0 it means that no match was found, 
it prints every single stop that didn't match 
any coordinates/name added before */
void NoSuchStop(int numEF) {
    int numEN;
    for(numEN=0; numEF > 0 && numEN < numEF; numEN++) {
        printf("%s: no such stop.\n", nomeNErro[numEN]);
    }
    return;
}
/* if numLFlag > 0 it means that no match was found between 
line names and the one given to "ligacao" function, 
printing every single one */
void NoSuchLine(int numLFlag) {
    int numLF;
    for(numLF=0; numLFlag > 0 && numLF < numLFlag; numLF++) {
        printf("%s: no such line.\n", erroLine[numLF]);
    }
    return;
}
/* if noP > 0 it means that no match was found with 
the stop names that already exist, it will print 
every single one of those found in "vParagem" */
void NoSuchStopL(int vPchar, int noP) {
    int vPErro = 0;
    while (vPErro < vPchar && noP > 0) {
        printf("%s: no such stop.\n", vParagem[vPErro]);
        vPErro++;
    }
    return;
}
/* if p12Erro > 0 it means that neither stop given to 
"ligacao" and "origDest" functions are the origin/destination, 
therefore wont be able to associate the stop with the line */
void LinkError() {
    printf("link cannot be associated with bus line.\n");
    return;
}

/* adds bus lines */
void carreira(char carNome[MAXCchar]) {
    int i;
    if (strlen(carNome) >= MAXCchar) {
        return; 
    }
    if (numCarreiras >= MAXc) {
        return;
    }

    i = 0;
    while (numCarreiras > 0 && i < numCarreiras) {
        if (strcmp(nomeCarreira[i].carreiraName, carNome) == 0) {
            cPass++;
            return;
        }
        i++;
    }

    strcpy(nomeCarreira[numCarreiras].carreiraName, carNome);
    numCarreiras++;
    return;
}

/* sees which stop in "ligacao" function is the origin/destination 
and flags it */
void origDest(int numCar, int numPar, char p1[MAXPchar], char p2[MAXPchar]) {
    p1d=0; 
    p2o=0; 
    if (strcmp(nomeCarLig[numCar].pars[numPar-1].paragens, p1) == 0) {
        p1d++; 
        return;
    }
    if (strcmp(nomeCarLig[numCar].pars[0].paragens, p2) == 0) {
        p2o++;
        return;
    }
    if (strcmp(nomeCarLig[numCar].pars[numPar-1].paragens, p1) == 0 
        && strcmp(nomeCarLig[numCar].pars[0].paragens, p2) == 0) {
        p1d++;
        p2o++;
        return;
    }
    else {
        p12Erro++; /* link cannot be associated flag */
        return;
    }
}

/* adds connections */
void ligacao (char nCar[MAXCchar], char p1[MAXPchar], char p2[MAXPchar], 
float custo, float duracao) {
    int numEC=0, numErro=0, i=0, numCar=0, numPar=0, countLigN, pPass, e;

    if (strlen(nCar) >= MAXCchar) {
        return;
    }
    if (strlen(p1) >= MAXPchar || strlen(p2) >= MAXPchar) {
        return;
    }
    if (countLig >= MAXl) {
        return;
    }

    /* sees if nCar is a valid line name. */
    while (numEC < numCarreiras) {
        if (strcmp(nomeCarreira[numEC].carreiraName, nCar) == 0) {
            numErro++;
            break;
        }
        numEC++;
    }

    /* compares the names of lines that have been through this function previously
    to check if it is its first time in the function */
    countLigN = 0;
    pPass = 0;
    while (countLigN <= countLig && numErro != 0) {
        if (strcmp(nomeCarLig[countLigN].pars[countLigN].carLig, nCar) == 0) {
            pPass++;
            break;
        }
        countLigN++;
    }

    if (custo < 0 || duracao < 0) {
        cdNeg++;
        return;
    }

    /* if it is the first time its name will be added do nomeCarLig */
    if (numErro != 0 && pPass == 0 && numErro != 0) {
        strcpy(nomeCarLig[numECnome].pars[numECnome].carLig, nCar);
        numPlig[numECnome] += 2;
        e = numPlig[numECnome];
        strcpy(nomeCarLig[numECnome].pars[0].paragens, p1);
        strcpy(nomeCarLig[numECnome].pars[e-1].paragens, p2);
        cost[numECnome] = custo;
        duration[numECnome] = duracao;
        numECnome++;
    }
    
    if (pPass != 0) {
        /* number of line name given the first time this line 
        was given any stops */
        for(i=0; i < numECnome; i++) {
            if (strcmp(nomeCarLig[i].pars[i].carLig, nCar) == 0) {
                numCar = i;
                break;
            }
        }
        numPar = numPlig[numCar]; /* number of stops in this line */
        origDest(numCar, numPar, p1, p2);

        /* adds stops to the line based on which 
        stop (p1 or p2) is the origin or destination */
        i=1;
        if (p2o > 0 && p1d == 0) {
            i = 0;
            while (i < numPar) {
                strcpy(nomeCarLig[numCar].pars[numPar-i].paragens, 
                nomeCarLig[numCar].pars[numPar-i-1].paragens); 
                /* shifts all stops to the right */
                i++;
            }
            strcpy(nomeCarLig[numCar].pars[0].paragens, p1);
            cost[numCar] += custo;
            duration[numCar] += duracao;
            numPlig[numCar] += 1; /* increments number of stops */
        }
        if ((p1d > 0 && p2o == 0) || (p1d > 0 && p2o > 0)) {
            numPlig[numCar] += 1; 
            numPar = numPlig[numCar];
            cost[numCar] += custo;
            duration[numCar] += duracao;
            strcpy(nomeCarLig[numCar].pars[numPar-1].paragens, p2); 
            /* adds new stop to the end or makes a circular line */
        }
    } 

    /* if nCar is not a name declared before with "carreira" this triggers 
    the call of "NoSuchLine" error in the end */
    if (numErro == 0) {
        strcpy(erroLine[eL], nCar);
        eL++;
        numLFlag++;
        return;
    }

    countLig++; /* number of connections */
    return;
}

/* verifies that both stops exist, noP which will serve as a flag 
to call the error when the program ends. 
it will also put the stop names that don't exist in "vParagem" */
void vParagemF(char p1[MAXPchar], char p2[MAXPchar]) {
    int yesP1=0, yesP2=0, vP=0;
    noP = 0;
    while (vP < numParagens) {
        if (strcmp(nomeParagem[vP].nome, p1) == 0) {
            yesP1++;
            break;
        }
        vP++;
    }
    if (yesP1 == 0) {
        strcpy(vParagem[vPchar], p1);
        vPchar++;
        noP++;
    }

    vP = 0;
    while (vP < numParagens) {
        if (strcmp(nomeParagem[vP].nome, p2) == 0) {
            yesP2++;
            break;
        }
        vP++;
    }
    if (yesP2 == 0) {
        strcpy(vParagem[vPchar], p2);
        vPchar++;
        noP++;
    }

    return;
}

int main() {

    char input[BUFSIZ];
    /* main variables related "paragem" commands */
    double coordenadasLat[MAXp], coordenadasLongi[MAXp];
    char stopName[MAXPchar], nomeTemp[MAXPchar], stopNameComp[MAXPchar];
    int i, p=0, paragemFinal[MAXPchar], numComp = 0, numFinal = 0, numCompReset;
    /* main variables related "carreira" commands */
    char carNome[MAXCchar], numCnoP[MAXc][MAXCchar], tempNumC[MAXCchar]; 
    char tempNumCa[MAXCchar];
    int c=0, numCar=0, numCarLigs[MAXc];
    /* main variables related to "ligacao" or "interligacao" commands */
    int numPP=0, numVcar, numVpar, nParsVef, numI=0, countIC[MAXp];
    char paragemm[MAXp][MAXPchar];
    /* error variable avd variables to help with printing and 
    other statements */
    int numEF=0, invE=0;
    int a=0, b=0, d=0, f=0, g=0, j=0, k=0;
    /* variables for whitspaces count condition and other counts 
    related to input */
    int count = -2, countInp = 0, aspas=0, lenInp=0, countAspas; 
    int temAspas2, temAspas3; 
    /* variables for inverse commands */
    char inv[MAXCchar], inverso[MAXCchar];
    int invCar[MAXc];


    fgets(input, BUFSIZ, stdin);


    /* main statements before input is 'q' and program stops */
    while (input[0] != 'q') {

    count = 0;
    countInp = 0;
    aspas = 0;
    while (input[countInp] != '\0') {
        if (input[countInp] == '"' && (countInp == 0 
        || input[countInp-1] != '\\')) {
            aspas = !aspas;
        }
        else if (input[countInp] == ' ' && !aspas) {
            count++;
        }
        countInp++;
        } /* counts number of whitespaces in input and ignores those 
            in between " " */


        /* gives name (either without "" or with it) and calls "paragem" 
        function where a stop is created */
        while (count >= 3 && input[0] == 'p' && (input[2] == '"' 
        || input[2] != '"')) {
            if (input[2] == '"') {
                sscanf(input, "p \"%[^\"]\" %lf %lf", nomeTemp, 
                &coordenadasLat[numParagens], &coordenadasLongi[numParagens]);
                paragem(nomeTemp, coordenadasLat[numParagens], 
                coordenadasLongi[numParagens]);
            }
            else {
                sscanf(input, "p %s %lf %lf", stopName, 
                &coordenadasLat[numParagens], &coordenadasLongi[numParagens]);
                paragem(stopName, coordenadasLat[numParagens], 
                coordenadasLongi[numParagens]);
            }
            break;
        }

        /* creates new line */
        while (count == 1 && input[0] == 'c') {
            int i;
            sscanf(input, "c %s", carNome);
            carreira(carNome);
            i = 0;
            while (cPass != 0 && numECnome > 0 && i < numECnome) {
                if (strcmp(nomeCarLig[i].pars[i].carLig, carNome) == 0) {
                    numCarLigs[a] = i;
                    a++;
                    break;
                }
                i++;
            }

            /* when "c" + 1 argument is called to list its stops */
            if (a == 0) {
                strcpy(numCnoP[f], carNome);
                f++;
            }

            g = 0;
            while (a != 0) {
                if (strcmp(numCnoP[g], carNome) == 0) {
                    strcpy(tempNumC, numCnoP[g+1]);
                    while (g < f) {
                        strcpy(numCnoP[g], tempNumC);
                        strcpy(tempNumC, numCnoP[g+1]);
                        g++;
                    }
                    f--;
                }
                g++;
                break;
            }
            

            break;
        }

        /* inverso command to list stops */
        i = 0;
        while (count == 2 && input[0] == 'c') {
            cPass = 0;
            sscanf(input, "c %s %s", inv, inverso);
            carreira(inv);
            if (strcmp(inverso, "inverso") == 0 || strcmp(inverso, "invers") == 0 
            || strcmp(inverso, "inver") == 0 || strcmp(inverso, "inv") == 0 
            || strcmp(inverso, "inve")) {
                while (cPass != 0 && numECnome > 0 && i < numECnome) {
                    if (strcmp(nomeCarLig[i].pars[i].carLig, inv) == 0) {
                        invCar[k] = i;
                        k++;
                        break;
                    }
                    i++;
                }
            }
            else {
                invE++;
                break;
            }
            break;
        }
        
        /* when input is "p + 1 argument" this condition compares 
        the name given to every stop name 
        and stops when a match is found */
        i = 0;
        numCompReset = 0;
        while (input[0] == 'p' && count == 1 && i < numParagens) {
            if (input[2] == '"') {
                sscanf(input, "p \"%[^\"]\"", stopNameComp);
            }
            if (input[2] != '"') {
                sscanf(input, "p %s", stopNameComp);
            }
            
            if (strcmp(stopNameComp, nomeParagem[i].nome) == 0) {
                if (numComp < MAXPchar) {
                    paragemFinal[numComp] = i;
                    numComp++;
                    numCompReset++;
                }
                else {
                    return 0;
                }
            }
           i++;
        }

        /* creates a new connection, verifies with "vParagemF" 
        if both stops exist 
        and then creates the connection with "ligacao" */
        while (count == 5 && input[0] == 'l') {

            if (countLig >= MAXl) {
                break;
            }

            /* checks where double quotes are */
            second_space = strchr(input, ' ');
            second_space = strchr(second_space + 1, ' ');
            
            third_space = strchr(input, ' ');
            third_space = strchr(third_space + 1, ' ');
            third_space = strchr(third_space + 1, ' ');

            temAspas2 = (second_space[1] == '"');
            temAspas3 = (third_space[1] == '"');


            /* counts number of double quotes */
            i=0;
            countAspas = 0;
            lenInp = strlen(input);
            while (i < lenInp) {
                if (input[i] == '"') {
                    countAspas++;
                }
                i++;
            }

            /* scans input based on number and place of double quotes */
            if (temAspas3) {
                sscanf(input, "l %s %s \"%[^\"]\" %f %f", nCar, p1, p2, 
                &custo, &duracao);
            } 
            if (temAspas2) {
                sscanf(input, "l %s \"%[^\"]\" %s %f %f", nCar, p1, p2, 
                &custo, &duracao);
            }
            if (countAspas == 0) {
                sscanf(input, "l %s %s %s %f %f", nCar, p1, p2, &custo, 
                &duracao); 
            }
            else {
                sscanf(input, "l %s \"%[^\"]\" \"%[^\"]\" %f %f", nCar, p1, 
                p2, &custo, &duracao); 
            }

            vParagemF(p1, p2);
            if (noP > 0) {
                break;
            }
            ligacao(nCar, p1, p2, custo, duracao);
            break;
        }

        /* no match was found, stop doesn't exist */
        if (numCompReset == 0 && count == 1 && input[0] == 'p') {
            strcpy(nomeNErro[numEF], stopNameComp);
            numEF++;
        } 
        
        /* increments p to list all the stops once 'q' is called */
        if (input[0] == 'p' && count == 0) {
            p++;
        }

        /* increments c to list all lines once 'q' is called */
        if (input[0] == 'c' && count == 0) {
            c++;
        }

        /* "interligacao" command */
        if (count == 0) {

            while (numPP < numParagens) {
                if (numPP >= MAXp) {
                    break;
                }
                strcpy(paragemm[numPP], nomeParagem[numPP].nome);
                numVcar = 0;
                nParsVef = 0;
                while (numVcar < numECnome) {
                    numVpar = 0;
                    while (numVpar < numPlig[numVcar]) {
                        if (strcmp(nomeCarLig[numVcar].pars[numVpar].paragens, 
                            paragemm[numPP]) == 0) {
                            strcpy(paragenss[numPP].parsVef[nParsVef].parVef, 
                            nomeCarLig[numVcar].pars[numVcar].carLig);
                            countIC[numPP] += 1;
                            nParsVef++;
                        }
                        numVpar++;
                    }
                    numVcar++; 
                }
                numPP++;
            }
            if (input[0] == 'i') {
                numI++;
            }
        }

    fgets(input, 100, stdin);
}
    if (invE > 0) {
        printf("incorrect sort option.\n");
    }
    StopAlreadyExists(numE);
    NoSuchStop(numEF); 

    /* prints every "p + 1 argument" called */
    i = 0;
    while (numComp > 0 && i < numParagens && i < numComp) {
        numFinal = paragemFinal[i];
        printf("%16.12f %16.12f\n", coordenadas[numFinal].latitude, 
        coordenadas[numFinal].longitude);
        i++;
    }

    /* lists every stop name, coordinates and number of lines 
    when 'p' is called */
    i = 0;
    j = 0;
    d = 0;
    g = 0;
    while (p > 0 && i < numParagens) {
        printf("%s: %16.12f %16.12f %d\n", nomeParagem[i].nome, 
        coordenadas[i].latitude, coordenadas[i].longitude, countIC[i]);
        i++;
    }

    NoSuchLine(numLFlag);
    if (noP > 0) {
    NoSuchStopL(vPchar, noP); }
    if (p12Erro > 0) {
        LinkError();
    }
    if (cdNeg > 0) {
        printf("negative cost or duration.\n");
    }

    /* lists stop names with "c" + name command */
    i = 0;
    while (a > 0 && i < a) {
        b = numCarLigs[i];
        d = 0;
        while (d < numPlig[b]) {
            if (d == (numPlig[b]-1)) {
                printf("%s", nomeCarLig[b].pars[d].paragens);
            }
            else {
                printf("%s, ", nomeCarLig[b].pars[d].paragens); 
            }
            d++;
        }
        printf("\n");
        i++;
    }

    /* lists stop with "inverso" command */
    i = 0;
    while (k > 0 && i < k) {
        b = invCar[i];
        g = 1;
        d = numPlig[b];
        while ((d-g) >= 0) {
            while ((d-g) != 0) {
                printf("%s, ", nomeCarLig[b].pars[d-g].paragens);
                g++;
            }
            if ((d-g) == 0) {
                printf("%s", nomeCarLig[b].pars[0].paragens);
                break;
            }
        }
        printf("\n");
        i++;
    }
                                                                                
    /* lists lines when "c" is called */
    numCar = 0;
    while (c > 0 && numCar < numECnome) {
        i = numPlig[numCar];
        printf("%s %s %s %d %.02f %.02f\n", 
        nomeCarLig[numCar].pars[numCar].carLig, 
        nomeCarLig[numCar].pars[i-i].paragens, 
        nomeCarLig[numCar].pars[i-1].paragens, 
        numPlig[numCar], cost[numCar], 
        duration[numCar]);
        numCar++;
    } 

    /* lists lines without any connections */
    i = 0;
    g = 0;
    while (c > 0 && i < numECnome) {
        if (strcmp(nomeCarLig[i].pars[i].carLig, numCnoP[g]) == 0) {
            strcpy(tempNumCa, numCnoP[g+1]);
                while (g < f) {
                    strcpy(numCnoP[g], tempNumCa);
                    strcpy(tempNumCa, numCnoP[g+1]);
                    g++;
                }
            f--;
            break;
        }
        while (strcmp(nomeCarLig[i].pars[i].carLig, numCnoP[g]) != 0 
        && g < f) {
            g++; 
        }
        i++;
    }

    i = 0;
    while (c > 0 && i < f) {
        printf("%s 0 0.00 0.00\n", numCnoP[i]);
        i++;
    }
    
    /* lists stop and line names with "interligacao" commands */
    i = 0;
    j = 0;
    while (i < numPP && numI > 0) {
        if (countIC[i] <= 1) {
            i++;
            continue;
        }
        printf("%s %d", paragemm[i], countIC[i]);
        j = 0;
        while (j < countIC[i] && i < numPP) {
            if (j == 0) {
                printf(": %s", paragenss[i].parsVef[j].parVef); 
            }
            else {
                printf(" %s", paragenss[i].parsVef[j].parVef); 
            }
            j++;
        }
        printf("\n");
        i++;
    }
    return 0;
}
