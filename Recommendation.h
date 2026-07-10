#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H
#include <vector>
#include <string>
#include "Movie.h"
using namespace std;

// returns true if the movie has one of the users favorite genres
bool genreMatches(Movie movie, vector<string> favoriteGenres);

// returns bonus points based on how soon the movie expires
double getExpirationBonus(Movie movie);

// calculates the final recommendation score
double calculateScore(Movie movie, vector<string> favoriteGenres);

// returns only the recommended movies
vector<Movie> getRecommendations(vector<Movie> movies, vector<string> favoriteGenres);

vector<Movie> getLeavingSoonMovies(vector<Movie> movies);

// sorts the recommended movies using heap sort
void heapSort(vector<Movie>& movies);

// sorts the recommended movies using merge sort
vector<Movie> mergeSort(vector<Movie>& movies);

#endif
