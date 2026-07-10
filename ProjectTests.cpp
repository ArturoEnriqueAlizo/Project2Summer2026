#include "Performance.h"
#include "Recommendation.h"
#include "Watchlist.h"
#include <iostream>
#include <vector>
using namespace std;

Movie makeMovie(int id, const string& genres, double rating,
                int expirationDays, double score)
{
    Movie movie;
    movie.movieId = id;
    movie.title = "Test Movie";
    movie.genres = genres;
    movie.averageRating = rating;
    movie.daysUntilExpiration = expirationDays;
    movie.recommendationScore = score;
    return movie;
}

bool check(bool condition, const string& testName)
{
    cout << (condition ? "PASS: " : "FAIL: ") << testName << endl;
    return condition;
}

int main()
{
    bool allPassed = true;

    Movie comedy = makeMovie(1, "Action|Comedy", 4.0, 5, 0);
    allPassed &= check(genreMatches(comedy, {"comedy"}),
                       "genre matching ignores case");
    allPassed &= check(genreMatches(comedy, {"Action", "Comedy"}),
                       "multiple genres must all match");
    allPassed &= check(!genreMatches(comedy, {"Horror"}),
                       "nonexistent genre does not match");
    allPassed &= check(getExpirationBonus(comedy) == 26,
                       "gradual expiration bonus");

    vector<Movie> movies = {
        makeMovie(1, "Comedy", 4.0, 10, 80),
        makeMovie(2, "Drama", 5.0, 20, 100),
        makeMovie(3, "Action", 3.0, 5, 60),
        makeMovie(4, "Comedy", 5.0, 2, 100)
    };

    vector<Movie> heapMovies = movies;
    vector<Movie> mergeMovies = movies;
    heapSort(heapMovies);
    mergeMovies = mergeSort(mergeMovies);

    allPassed &= check(verifySortResults(heapMovies, mergeMovies),
                       "Heap Sort and Merge Sort outputs agree");

    PerformanceResult result = compareSorts(movies, 3);
    allPassed &= check(result.outputsMatch && result.dataSize == 4,
                       "performance comparison verifies its output");

    vector<Movie> watchlist;
    allPassed &= check(addToWatchlist(watchlist, movies[0]),
                       "movie can be added to watchlist");
    allPassed &= check(!addToWatchlist(watchlist, movies[0]),
                       "duplicate watchlist movie is rejected");
    allPassed &= check(removeFromWatchlist(watchlist, movies[0].title),
                       "movie can be removed from watchlist");
    allPassed &= check(watchlist.empty(),
                       "watchlist is empty after removal");

    return allPassed ? 0 : 1;
}
