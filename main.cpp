#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Movie.h"
#include "DataLoader.h"
#include "Recommendation.h"
using namespace std;

Movie searchinForAFilm(vector<Movie>& movies, string searchTitle) {
    if (movies.empty()) {
        Movie temp;
        temp.title = "FAIL";
        return temp;
    }

    if (movies[0].title == searchTitle) {
        return movies[0];
    }
    if (movies.size() == 1 && movies[0].title != searchTitle) {
        Movie temp;
        temp.title = "FAIL";
        return temp;
    }
    int merginsearch = movies.size()/2;
    vector <Movie> filmleft(movies.begin(), movies.begin() + merginsearch);
    vector <Movie> filmright(movies.begin() + merginsearch, movies.end());

    Movie leftsearch = searchinForAFilm(filmleft, searchTitle);
    if (leftsearch.title !="FAIL") {
        return leftsearch;
    }
    Movie rightsearch = searchinForAFilm(filmright, searchTitle);
    if (rightsearch.title !="FAIL") {
        return rightsearch;
    }
    Movie temp;
    temp.title = "FAIL";
    return temp;

}

int main()
{
    vector<Movie> movies;

    movies = loadMovies("Data/movies.csv",
                        "Data/ratings.csv");

    if (movies.size() == 0)
    {
        cout << "No movies loaded." << endl;
        return 0;
    }

    cout << "Movies loaded successfully!" << endl;
    cout << "Total movies: " << movies.size() << endl;
    cout << endl;

    string menuChoice = "1";

    while (menuChoice != "3")
    {
        if (menuChoice == "1")
        {
            vector<string> favoriteGenres;

            string genre;

            cout << "Enter your favorite genre(s) (use space to separate genres): ";
            getline(cin, genre);

            stringstream stringy(genre);
            string gene;

            while (getline(stringy, gene, ' '))
            {
                if (gene != "")
                {
                    favoriteGenres.push_back(gene);
                }
            }

            vector<Movie> recommendations;

            recommendations = getRecommendations(movies, favoriteGenres);

            if (recommendations.size() == 0)
            {
                cout << "No recommendations found! Maybe try a different genre?" << endl;
            }
            else
            {
                heapSort(recommendations);
                vector<Movie> mergedRecs = mergeSort(recommendations);

                // Ok so I set it up so that heapsort is still used for the main recommendation sorting, and I set it so that
                // mergesort is used for the set that suggests movies leaving soon.
                // This ensures that the algorithms outlined in the initial proposal are still valid.
                // However, this can be changed if needed.
                cout << endl;
                cout << "Top Recommended Movies" << endl;
                cout << "----------------------" << endl;
                cout << endl;

                int count = 0;

                for (int i = recommendations.size() - 1; i >= 0; i--)
                {
                    cout << recommendations[i].title << endl;
                    cout << "Genres: " << recommendations[i].genres << endl;
                    cout << "Rating: " << recommendations[i].averageRating << endl;
                    cout << "Days Until Expiration: " << recommendations[i].daysUntilExpiration << endl;
                    cout << "Recommendation Score: " << recommendations[i].recommendationScore << endl;
                    cout << endl;

                    count++;

                    if (count == 10)
                    {
                        break;
                    }
                }

                vector<Movie> leavingSoon;

                leavingSoon = getLeavingSoonMovies(mergedRecs);

                cout << endl;
                cout << "Movies Leaving Soon" << endl;
                cout << "-------------------" << endl;
                cout << endl;

                count = 0;

                for (int i = 0; i < leavingSoon.size(); i++)
                {
                    cout << leavingSoon[i].title << endl;
                    cout << "Days Until Expiration: " << leavingSoon[i].daysUntilExpiration << endl;
                    cout << "Genres: " << leavingSoon[i].genres << endl;
                    cout << "Recommendation Score: " << leavingSoon[i].recommendationScore << endl;
                    cout << endl;

                    count++;

                    if (count == 10)
                    {
                        break;
                    }
                }
            }
        }
        else if (menuChoice == "2")
        {
            string movieTitle;

            cout << "Enter a movie title to search." << endl;
            cout << "Please use the format: Title (year)" << endl;
            cout << "Movie title: ";

            getline(cin, movieTitle);

            Movie finder = searchinForAFilm(movies, movieTitle);

            if (finder.title == "FAIL")
            {
                cout << "Sorry! We couldn't find that movie! :(" << endl;
            }
            else
            {
                cout << endl;
                cout << finder.title << endl;
                cout << "Genres: " << finder.genres << endl;
                cout << "Rating: " << finder.averageRating << endl;
                cout << "Days Until Expiration: " << finder.daysUntilExpiration << endl;
                cout << endl;
            }
        }
        else if (menuChoice != "3")
        {
            cout << "Please enter 1, 2, or 3." << endl;
        }

        cout << endl;
        cout << "What do you want to do?" << endl;
        cout << "1. Choose another genre" << endl;
        cout << "2. Search for a specific movie" << endl;
        cout << "3. Exit" << endl;
        cout << "Choice: ";

        getline(cin, menuChoice);

        cout << endl;
    }

    cout << "Understood! Hope you liked these recommendations! :)" << endl;

    return 0;
}