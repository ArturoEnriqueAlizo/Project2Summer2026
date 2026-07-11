#include <iostream>
#include <vector>
#include <string>

#include "Movie.h"

#ifndef PROJECT2SUMMER2026FINAL_VARIOUSSEARCHES_H
#define PROJECT2SUMMER2026FINAL_VARIOUSSEARCHES_H

#endif //PROJECT2SUMMER2026FINAL_VARIOUSSEARCHES_H

// Mergesort but by rating instead of reccomendation score, used exclusively to sort searches.
vector<Movie> mergeSortByRatingNotReccomendation(vector<Movie>& movies);

// Search by title
Movie searchForMovie(const vector<Movie>& movies, const string& searchTitle);

// Searches for films by genre, returns top 10 matches sorted by rating.
vector<Movie> searchByGenre(const vector<Movie>& movies, string searchGenre);

// Search by rating, returns vector of top 10 rated films in any category.
// ex: search for 5-star films, returns top 10 between 5 and 4-star ratings.
vector<Movie> searchByRating(const vector<Movie>& movies, string searchGenre);