#include "Recommendation.h"
#include <iostream>
#include <cctype>
using namespace std;

bool genreMatches(Movie movie, vector<string> favoriteGenres)
{
    string genres = movie.genres;

    for (int i = 0; i < genres.size(); i++)
    {
        genres[i] = tolower(genres[i]);
    }

    for (string genre : favoriteGenres)
    {
        for (int j = 0; j < genre.size(); j++)
        {
            genre[j] = tolower(genre[j]);
        }

        if (genres.find(genre) == -1)
        {
            return false;
        }
    }

    return true;
}

double getExpirationBonus(Movie movie)
{
    double bonus = 0;

    if (movie.daysUntilExpiration <= 30)
    {
        bonus = 31 - movie.daysUntilExpiration;
    }

    return bonus;
}

double calculateScore(Movie movie, vector<string> favoriteGenres)
{
    double score = 0;

    double ratingPoints =
        movie.averageRating * 20;

    double genrePoints = 0;

    if (genreMatches(movie, favoriteGenres))
    {
        genrePoints = 25;
    }

    double expirationPoints =
        getExpirationBonus(movie);

    score =
        ratingPoints +
        genrePoints +
        expirationPoints;

    return score;
}

vector<Movie> getRecommendations(vector<Movie> movies, vector<string> favoriteGenres)
{
    vector<Movie> recommendations;

    for (int i = 0; i < movies.size(); i++)
    {
        movies[i].recommendationScore = calculateScore(movies[i], favoriteGenres);

        if (genreMatches(movies[i], favoriteGenres))
        {
            recommendations.push_back(movies[i]);
        }
    }

    return recommendations;
}

vector<Movie> getLeavingSoonMovies(vector<Movie> movies)
{
    vector<Movie> leavingSoon;

    for (int i = 0; i < movies.size(); i++)
    {
        if (movies[i].daysUntilExpiration <= 7)
        {
            leavingSoon.push_back(movies[i]);
        }
    }
    // rank urgent movies by recommendation score before displaying them
    return mergeSort(leavingSoon);
}

void heapify(vector<Movie>& movies, int size, int index)
{
    int largest = index;

    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size)
    {
        if (movies[left].recommendationScore > movies[largest].recommendationScore)
        {
            largest = left;
        }
    }

    if (right < size)
    {
        if (movies[right].recommendationScore > movies[largest].recommendationScore)
        {
            largest = right;
        }
    }

    if (largest != index)
    {
        Movie temp = movies[index];
        movies[index] = movies[largest];
        movies[largest] = temp;

        heapify(movies, size, largest);
    }
}

void heapSort(vector<Movie>& movies)
{
    int size = movies.size();

    // build the heap
    for (int i = size / 2 - 1; i >= 0; i--)
    {
        heapify(movies, size, i);
    }

    // sort the heap
    for (int i = size - 1; i > 0; i--)
    {
        Movie temp = movies[0];
        movies[0] = movies[i];
        movies[i] = temp;

        heapify(movies, i, 0);
    }
}

// merge two descending score sequences into one sorted result
vector<Movie> mergeMovieVectors(vector<Movie> leftMovies, vector<Movie> rightMovies) {
    vector<Movie> sortedMovies;
    int leftIndex = 0;
    int rightIndex = 0;
    while (leftMovies.size() > leftIndex && rightMovies.size() > rightIndex) {
        if (leftMovies[leftIndex].recommendationScore > rightMovies[rightIndex].recommendationScore) {
            sortedMovies.push_back(leftMovies[leftIndex]);
            leftIndex++;
        }
        else {
            sortedMovies.push_back(rightMovies[rightIndex]);
            rightIndex++;
        }
    }
    vector<Movie> remainingLeft(leftMovies.begin() + leftIndex, leftMovies.end());
    vector<Movie> remainingRight(rightMovies.begin() + rightIndex, rightMovies.end());
    for (auto& movie : remainingLeft) {
        sortedMovies.push_back(movie);
    }
    for (auto& movie : remainingRight) {
        sortedMovies.push_back(movie);
    }
    return sortedMovies;
}
vector<Movie> mergeSort(vector<Movie>& movies) {
    if (movies.size() <= 1) {
        return movies;
    }
    int middle = movies.size() / 2;
    vector<Movie> leftMovies(movies.begin(), movies.begin() + middle);
    vector<Movie> rightMovies(movies.begin() + middle, movies.end());

    vector<Movie> sortedLeft = mergeSort(leftMovies);
    vector<Movie> sortedRight = mergeSort(rightMovies);

    return mergeMovieVectors(sortedLeft, sortedRight);

}



