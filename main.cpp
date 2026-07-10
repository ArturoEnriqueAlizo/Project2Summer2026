#include <iostream>
#include <vector>
#include <string>
#include "Movie.h"
#include "DataLoader.h"
#include "Recommendation.h"

using namespace std;

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

    vector<string> favoriteGenres;

    string genre;

    cout << "Enter your favorite genre: ";
    cin >> genre;

    favoriteGenres.push_back(genre);

    vector<Movie> recommendations;

    recommendations = getRecommendations(movies, favoriteGenres);
    if (recommendations.size() == 0) {
        cout << "No recommendations found! Maybe try a different genre?" << endl;
        return 0;
    }

    heapSort(recommendations);
    vector<Movie> mergedRecs = mergeSort(recommendations);

    cout << endl;
    cout << "Heapsort Implementation"<<endl;
    cout<<endl;
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

    leavingSoon = getLeavingSoonMovies(recommendations);

    cout << endl;
    cout << "Movies Leaving Soon" << endl;
    cout << "-------------------" << endl;
    cout << endl;

    count = 0;

    for (int i = 0; i < leavingSoon.size(); i++)
    {
        cout << leavingSoon[i].title << endl;
        cout << "Days Until Expiration: " << leavingSoon[i].daysUntilExpiration << endl;
        cout << "Reccomendation Score: " << leavingSoon[i].recommendationScore << endl;
        cout << endl;

        count++;

        if (count == 10)
        {
            break;
        }
    }

    string movieTitle;
    string answer;
    while (movieTitle == "") {
        cout << "Search for a film? (Y/N)";
        cin >> answer;
        if (answer == "Y") {
            cout<<"Enter a movie title to search: "<<endl;
            cin>>movieTitle;
        }
        else {
            cout<<"Understood! Hope you liked these reccomendations! :)"<<endl;
            break;
        }
    }

    return 0;
}