#include "Performance.h"
#include "Recommendation.h"
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <unordered_map>
using namespace std;

bool verifySortResults(const vector<Movie>& heapResult,
                       const vector<Movie>& mergeResult)
{
    if (heapResult.size() != mergeResult.size())
    {
        return false;
    }

    unordered_map<int, int> movieCounts;

    for (const Movie& movie : heapResult)
    {
        movieCounts[movie.movieId]++;
    }

    for (const Movie& movie : mergeResult)
    {
        movieCounts[movie.movieId]--;
    }

    for (const auto& entry : movieCounts)
    {
        if (entry.second != 0)
        {
            return false;
        }
    }

    for (int i = 0; i < heapResult.size(); i++)
    {
        // Heap Sort is ascending so compare it backward with descending Merge Sort
        int heapIndex = static_cast<int>(heapResult.size()) - 1 - i;
        if (abs(heapResult[heapIndex].recommendationScore
                - mergeResult[i].recommendationScore) > 0.000001)
        {
            return false;
        }
    }

    return true;
}

PerformanceResult compareSorts(const vector<Movie>& movies, int trials)
{

    PerformanceResult result;
    result.dataSize = static_cast<int>(movies.size());
    result.heapAverageMicroseconds = 0;
    result.mergeAverageMicroseconds = 0;
    result.outputsMatch = true;

    if (movies.empty() || trials <= 0)
    {
        result.outputsMatch = false;
        return result;
    }

    double heapTotal = 0;
    double mergeTotal = 0;

    for (int trial = 0; trial < trials; trial++){
    
        // Copy before timing so both algorithms receive identical input.
        vector<Movie> heapMovies = movies;
        vector<Movie> mergeMovies = movies;

        auto heapStart = chrono::steady_clock::now();
        heapSort(heapMovies);
        auto heapEnd = chrono::steady_clock::now();

        auto mergeStart = chrono::steady_clock::now();
        mergeMovies = mergeSort(mergeMovies);
        auto mergeEnd = chrono::steady_clock::now();

        heapTotal += chrono::duration<double, micro>(heapEnd - heapStart).count();
        mergeTotal += chrono::duration<double, micro>(mergeEnd - mergeStart).count();
        result.outputsMatch = result.outputsMatch
                              && verifySortResults(heapMovies, mergeMovies);
    }

    result.heapAverageMicroseconds = heapTotal / trials;
    result.mergeAverageMicroseconds = mergeTotal / trials;
    return result;
}

void runPerformanceTests(const vector<Movie>& movies, int trials)
{
    const vector<int> requestedSizes = {100, 500, 1000, 2500, 5000, 10000};
    vector<int> testSizes;

    for (int size : requestedSizes)
    {
        if (size <= movies.size())
        {
            testSizes.push_back(size);
        }
    }

    if (testSizes.empty() || testSizes.back() != movies.size())
    {
        testSizes.push_back(static_cast<int>(movies.size()));
    }
    cout << endl;
    cout << "Heap Sort vs. Merge Sort Performance" << endl;
    cout << "-------------------------------------------------------" << endl;
    cout << left << setw(12) << "Movies"
         << setw(15) << "Heap (us)"
         << setw(15) << "Merge (us)"
         << "Match" << endl;

    for (int size : testSizes)
    {
        vector<Movie> sample(movies.begin(), movies.begin() + size);
        PerformanceResult result = compareSorts(sample, trials);

        cout << left << setw(12) << result.dataSize
             << setw(15) << fixed << setprecision(2)
             << result.heapAverageMicroseconds
             << setw(15) << result.mergeAverageMicroseconds
             << (result.outputsMatch ? "PASS" : "FAIL") << endl;
    }
}
