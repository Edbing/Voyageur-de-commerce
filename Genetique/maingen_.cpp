// Import des bibliothèques

#include <vector>
#include <string>
#include <limits>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

using std::string;
using namespace std;

//définition des constantes
#define nb 4  // nombre de villes
#define noms ABCDE // Nom des villes
#define init 0 // ville de départ
#define taillepop 15 // taille de la population
#define chancesurvie 20

// On définit la structure chemin qui contient les noms des villes et la fitness
struct chemin {
  string composition;
  double fitness;
  double longueurtotale;
  int nbmuta;
};

// On crée une fonction qui génère un nombre aléatoire car
int nb_aleatoire(int nbini, int nbfin){
  int nomb = nbini - nbfin;
  int nbal = nbini + rand() % nomb; // On prend le module car rand() génère un nombre entre 0 et 32767
  return nbal;
}

// on crée une fonction qui vérifie si une ville a déjà été visitée

bool visitecheck(string s, char ch)
{
  for (int i=0; i<s.size();i++){
    if (s[i]==ch){
      return true;
    }
  }
  return false;
}

// On créé la fonction mutation
string mutation(struct chemin chemin)
{
  while (true)
  {
    int al1 = nb_aleatoire(1,nb); // à voir changer encore en focntion de l'utilisation plus tard
    int al2 = nb_aleatoire(1,nb);
    if (al1 != al2) {
      char temp =chemin.composition[al1];
      chemin.composition[al1] = chemin.composition[al2];
      chemin.composition[al2] = temp;
      chemin.nbmuta++;
      break;
    }
  }
  return chemin.composition;
}

// On créé une composition

string generation_composition(struct chemin chemin)
{
  string compo = "0";
  while (true) {
    if (compo.size() == nb)
    {
      compo+=compo[0];
      break;
    }
    int temp = nb_aleatoire(1,nb);
    if (!visitecheck(compo,(char)(temp+48)))
      compo += (char)(temp+48);
  }
  //cout<<compo<<endl;
  chemin.composition = compo;
  //cout<<chemin.composition<<endl;
  return chemin.composition;
}

// On définie ici une fonction qui calcule la fitness

double calfit(struct chemin chemin, double distances[nb][nb])
{
  /*double distances[nb][nb] = { { 0, 2, 12, 12, 5 },
					{ 2, 0, 4, 8, 12 },
					{ 12, 4, 0, 3, 3 },
					{ 12, 8, 3, 0, 10 },
					{ 5, 12, 3, 10, 0 } };*/
  double f =0;
  for (int i = 0; i < chemin.composition.size() -1; i++){
    f += distances[chemin.composition[i] -48][chemin.composition[i + 1]- 48];
  }
  chemin.fitness = 1/f;
  return chemin.fitness;
}

double callong(struct chemin chemin, double distances[nb][nb])
{
  /*double distances[nb][nb] = { { 0, 2, 12, 12, 5 },
					{ 2, 0, 4, 8, 12 },
					{ 12, 4, 0, 3, 3 },
					{ 12, 8, 3, 0, 10 },
					{ 5, 12, 3, 10, 0 } };*/
  double f =0;
  for (int i = 0; i < chemin.composition.size() -1; i++){
    f += distances[chemin.composition[i] -48][chemin.composition[i + 1]- 48];
  }
  chemin.longueurtotale = f;
  return chemin.longueurtotale;
}

// Il nous faut aussi créer un comparateur de chemins

bool comparateur(struct chemin chemin1, struct chemin chemin2)
{
  return chemin1.fitness > chemin2.fitness;
}

// Finalement, on crée la fonction complète

void geneTSP (double distances[nb][nb])
{
  int gen = 1; // indice de la génération
  int gen_nb =10; // nombre d'itérations de la génération

  vector<struct chemin> population;
  struct chemin tempo;

  for (int i = 0; i < taillepop; i++)
  {
    tempo.composition = generation_composition(tempo);
    tempo.fitness = calfit(tempo,distances);
    tempo.longueurtotale = callong(tempo,distances);
    population.push_back(tempo);
  }
  cout << "\nInitial population: " << endl
  << "compo fitness longueur  \n";
  for (int i = 0; i < taillepop; i++)
    cout << population[i].composition << " "
      << population[i].fitness << " " << population[i].longueurtotale<<endl;
  cout << "\n";

  bool found = false;
  while(gen <= gen_nb) {
    sort(population.begin(),population.end(),comparateur);
    vector<struct chemin> new_pop;

    for (int i = 0; i < taillepop; i++) {
      struct chemin chem1 = population[i];


      while(true) {
        struct chemin new_chem;
        new_chem.composition = mutation(chem1);
        new_chem.fitness = calfit(chem1,distances);
        new_chem.longueurtotale = callong(chem1,distances);


        if (new_chem.fitness >= population[i].fitness) {
          new_pop.push_back(new_chem);

          break;
        }
        else {
          double proba = (chancesurvie)*new_chem.fitness;

          if (proba > 0.6) {
            new_pop.push_back(new_chem);
            break;
          }
          break;
        }
      }
    }
    population = new_pop;
    sort(population.begin(),population.end(),comparateur);
    cout<<"\nGénération "<< gen << "\n";
    cout<<"compo fitness longueur \n";

    for (int i = 0; i < taillepop; i++)
      cout << population[i].composition << " "
        << population[i].fitness << " " << population[i].longueurtotale<<endl;

    gen++;
  }
  //break;
}



int main()
{
  double distances[nb][nb] = { { -1, 10, 15, 20},
                            { 10, -1, 35, 25},
                            { 15, 35, -1, 30},
                            { 20, 25, 30, -1} };

  geneTSP(distances);
}
