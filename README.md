# Memento

Memento is a console-based movie recommendation and expiration alert system. It uses MovieLens ratings and genre data to calculate a recommendation score, display highly ranked movies, identify movies that are leaving soon, and search for a title. The program also compares two sorting algorithms implemented from scratch: Heap Sort and Merge Sort.

## Team

- Angelina Coutsoukes (`MyDogIsCool4`)
- Arturo Alizo (`ArturoEnriqueAlizo`)
- Samuel Calle (`SamCalle`)

## Features

- Recommendations based on one or more favorite genres
- Scores combining average rating genre preference and expiration urgency
- A top ten recommendation list sorted with Heap Sort
- A leaving-soon list sorted with Merge Sort
- Exact title movie search
- Heap Sort versus Merge Sort performance comparison
- Repeated benchmark trials at several dataset sizes
- Automated verification that both sorting algorithms produce equivalent results
- Input validation for genre and Y/N prompts

## Requirements

- CMake 4.0 or newer
- A compiler with C++20 support
  - Windows: Visual Studio 2022/2026 Community or Build Tools with **Desktop development with C++**

