#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <limits>
#include <cstdlib>

// Structure to represent a city
struct City {
    int id;
    int x;
    int y;
};

// Calculate Euclidean distance between two cities, rounded to the nearest integer
int euclidean_distance(const City& c1, const City& c2) {
    return round(sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2)));
}

// Read input file and parse city data
std::vector<City> read_input(const std::string& file_path) {
    std::ifstream infile(file_path);
    std::vector<City> cities;
    int id, x, y;
    while (infile >> id >> x >> y) {
        cities.push_back({id, x, y});
    }
    return cities;
}

// Compute distance matrix
std::vector<std::vector<int>> compute_distance_matrix(const std::vector<City>& cities) {
    int n = cities.size();
    std::vector<std::vector<int>> distance_matrix(n, std::vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                distance_matrix[i][j] = euclidean_distance(cities[i], cities[j]);
            } else {
                distance_matrix[i][j] = 0;
            }
        }
    }
    return distance_matrix;
}

// Heuristic 2-TSP solver
std::pair<std::vector<int>, std::vector<int>> solve_2tsp(const std::vector<std::vector<int>>& distance_matrix) {
    int n = distance_matrix.size();
    std::vector<int> tour1, tour2;
    std::set<int> visited;
    
    // Randomly select two starting points for the two salesmen
    int start1 = rand() % n;
    int start2;
    do {
        start2 = rand() % n;
    } while (start2 == start1);
    
    tour1.push_back(start1);
    tour2.push_back(start2);
    visited.insert(start1);
    visited.insert(start2);
    
    // Greedy approach to construct tours
    for (int i = 0; i < n - 2; ++i) {
        int next_city1 = -1, next_city2 = -1;
        int min_dist1 = std::numeric_limits<int>::max();
        int min_dist2 = std::numeric_limits<int>::max();
        
        for (int j = 0; j < n; ++j) {
            if (visited.find(j) == visited.end()) {
                int last_city1 = tour1.back();
                int last_city2 = tour2.back();
                int dist1 = distance_matrix[last_city1][j];
                int dist2 = distance_matrix[last_city2][j];
                
                if (dist1 < min_dist1) {
                    min_dist1 = dist1;
                    next_city1 = j;
                }
                if (dist2 < min_dist2) {
                    min_dist2 = dist2;
                    next_city2 = j;
                }
            }
        }
        
        if (min_dist1 < min_dist2) {
            tour1.push_back(next_city1);
            visited.insert(next_city1);
        } else {
            tour2.push_back(next_city2);
            visited.insert(next_city2);
        }
    }
    
    // Assign remaining unvisited city
    for (int i = 0; i < n; ++i) {
        if (visited.find(i) == visited.end()) {
            if (tour1.size() < tour2.size()) {
                tour1.push_back(i);
            } else {
                tour2.push_back(i);
            }
        }
    }
    
    return {tour1, tour2};
}

// Calculate total distance of a tour
int calculate_tour_distance(const std::vector<int>& tour, const std::vector<std::vector<int>>& distance_matrix) {
    int total_distance = 0;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        total_distance += distance_matrix[tour[i]][tour[i + 1]];
    }
    total_distance += distance_matrix[tour.back()][tour[0]];  // return to start
    return total_distance;
}

// Write output to file
void write_output(const std::string& file_path, const std::vector<int>& tour1, const std::vector<int>& tour2,
                  int dist1, int dist2) {
    std::ofstream outfile(file_path);
    outfile << (dist1 + dist2) << "\n";
    outfile << dist1 << " " << tour1.size() << "\n";
    for (int city : tour1) {
        outfile << city << "\n";
    }
    outfile << "\n";
    outfile << dist2 << " " << tour2.size() << "\n";
    for (int city : tour2) {
        outfile << city << "\n";
    }
    outfile << "\n";
}

int main() {
    std::string input_file = "inputs/test-input-1.txt";  // Adjust the input file path
    std::string output_file = "outputs/test-output-1.txt";
    
    std::vector<City> cities = read_input(input_file);
    std::vector<std::vector<int>> distance_matrix = compute_distance_matrix(cities);
    auto [tour1, tour2] = solve_2tsp(distance_matrix);
    int dist1 = calculate_tour_distance(tour1, distance_matrix);
    int dist2 = calculate_tour_distance(tour2, distance_matrix);
    write_output(output_file, tour1, tour2, dist1, dist2);
    
    return 0;
}
