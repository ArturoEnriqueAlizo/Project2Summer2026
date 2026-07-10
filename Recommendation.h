#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H
#include <vector>
#include <string>
#include "Movie.h"
using namespace std;

// Returns true if the movie has one of the user's favorite genres
bool genreMatches(Movie movie, vector<string> favoriteGenres);

// Returns bonus points based on how soon the movie expires
int getExpirationBonus(Movie movie);

// Calculates the final recommendation score
double calculateScore(Movie movie, vector<string> favoriteGenres);

// Returns only the recommended movies
vector<Movie> getRecommendations(vector<Movie> movies, vector<string> favoriteGenres);

vector<Movie> getLeavingSoonMovies(vector<Movie> movies);

// Sorts the recommended movies using Heap Sort
void heapSort(vector<Movie>& movies);

vector<Movie> mergeSort(vector<Movie>& movies);

// need to combine heapsort with leaving soon. In curr state, leaving soon does not consider reccomendation score.
// well, that was a simple fix in main.

#endif