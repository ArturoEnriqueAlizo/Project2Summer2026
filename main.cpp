#include <iostream>
#include <vector>

#include "Movie.h"
#include "DataLoader.h"

using namespace std;

int main()
{
    vector<Movie> movies;

    movies = loadMovies("../Data/movies.csv",
                        "../Data/ratings.csv");

    cout << endl;
    cout << "Total movies loaded: " << movies.size() << endl;
    cout << endl;

    if (movies.size() == 0)
    {
        cout << "No movies loaded." << endl;
        return 0;
    }

    cout << "First movie loaded:" << endl;
    cout << "ID: " << movies[0].movieId << endl;
    cout << "Title: " << movies[0].title << endl;
    cout << "Genres: " << movies[0].genres << endl;
    cout << "Average rating: " << movies[0].averageRating << endl;
    cout << "Days until expiration: " << movies[0].daysUntilExpiration << endl;

    return 0;
}