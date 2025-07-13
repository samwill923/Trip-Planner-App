#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
using namespace std;

int n;

struct TravelDetails {
    int roadDistance;
    string roadTime;
    int taxiFare;
    int airDistance;
    int flightCost;
    string trainTime;
    int sleeperFare;
    int acFare;
    int totalFare = 0;
    int totalTime = 0;
    // For tracking user's transportation choice
    string transportMode = "road"; // Default: road, options: road, train-sleeper, train-ac, flight
};

// Updated Graph class
class Graph {
public:
    unordered_map<string, vector<pair<string, TravelDetails>>> adjList;

    void addEdge(const string& city1, const string& city2, const TravelDetails& details) {
        adjList[city1].push_back({city2, details});
        adjList[city2].push_back({city1, details});
    }

    void displayGraph() {
        for (const auto& city : adjList) {
            cout << city.first << " ->" << endl;
    
            if (city.second.empty()) {
                cout << "  No connections" << endl;
            } else {
                for (const auto& neighbor : city.second) {
                    const TravelDetails& d = neighbor.second;
                    cout << "  To: " << neighbor.first << endl;
                    cout << "    Road Distance: " << d.roadDistance << " km" << endl;
                    cout << "    Road Time: " << d.roadTime << endl;
                    cout << "    Taxi Fare: Rs. " << d.taxiFare << endl;
                    cout << "    Air Distance: " << d.airDistance << " km" << endl;
                    cout << "    Flight Cost: Rs. " << d.flightCost << endl;
                    cout << "    Train Time: " << d.trainTime << endl;
                    cout << "    Sleeper Fare: Rs. " << d.sleeperFare << endl;
                    cout << "    AC Fare: Rs. " << d.acFare << endl;
                    cout << "    Transport Mode: " << d.transportMode << endl;
                }
            }
            cout << endl;
        }
    }
};


class Details_activities {
    public:
        // Each entry: city name + activity details (type, name, address, latitude, longitude, price, time)
        vector<pair<string, vector<string>>> activities;
        
        void addEdge(const string& city, const vector<string>& details) {
            if (details.size() != 7) {
                cerr << "Error: Expected 7 details for activity in '" << city << "'." << endl;
                return;
            }
            activities.push_back({city, details});
        }
        
        void printGraph() const {
            if (activities.empty()) {
                cout << "No activities found.\n";
                return;
            }
            
            string currentCity = "";
            for (vector<pair<string, vector<string>>>::const_iterator it = activities.begin(); it != activities.end(); ++it) {
                const pair<string, vector<string>>& entry = *it;
                if (entry.first != currentCity) {
                    currentCity = entry.first;
                    cout << "\nCity: " << currentCity << endl;
                }
                const vector<string>& details = entry.second;
                cout << "  Name: " << details[0] << "\t";
                cout << "  Type: " << details[1] << "\t";
                cout << "  Price: " << details[5] << "\t";
                cout << "  Time: " << details[6] << "\t";
                cout << endl;
            }
        }
        // In Details_activities or a related helper:
     void displayActivityGraph(const Graph& g) {
    for (const auto& city : g.adjList) {
        cout << city.first << " ->" << endl;
        if (city.second.empty()) {
            cout << "  No connections" << endl;
        } else {
            for (const auto& neighbor : city.second) {
                const TravelDetails& d = neighbor.second;
                cout << "  To: " << neighbor.first << endl;
                cout << "    Road Distance: " << d.roadDistance << " km" << endl;
                cout << "    Road Time: " << d.roadTime << endl;
                cout << "    Taxi Fare: Rs. " << d.taxiFare << endl;
                // Do NOT print train/flight fields here
            }
        }
        cout << endl;
    }
}

        
    };


//function to get cities from user
vector<string> getUserCities() {
    cout << "Enter the number of cities you want to visit: ";
    cin >> n;

    vector<string> cities;
    string city;
    cout << "Enter source city: ";
    cin >> city;
    transform(city.begin(), city.end(), city.begin(), ::toupper);
    cities.push_back(city);

    for (int i = 1; i <= n; ++i) {
        cout << "Enter city " << i << ": ";
        cin >> city;
        transform(city.begin(), city.end(), city.begin(), ::toupper);
        cities.push_back(city);
    }
    return cities;
}

string convertToDecimalHour(const string& input) {
    int hours = 0, minutes = 0;
    stringstream ss(input);
    string token;

    while (ss >> token) {
        if (isdigit(token[0])) {
            int value = stoi(token);
            string nextToken;
            if (ss >> nextToken) {
                if (nextToken.find("hour") != string::npos)
                    hours = value;
                else if (nextToken.find("min") != string::npos)
                    minutes = value;
            }
        }
    }

    double total = hours + (minutes / 60.0);
    
    stringstream result;
    result << fixed << setprecision(2) << total;
    return result.str();
}

// Convert decimal hours to a human-readable format
string formatTime(double hours) {
    int hrs = static_cast<int>(hours);
    int mins = static_cast<int>((hours - hrs) * 60);
    return to_string(hrs) + " hours " + to_string(mins) + " minutes";
}

// Calculate flight time based on distance
string calculateFlightTime(int distance) {
    double hours = distance / 600.0; // Average flight speed of 700 kmph
    return formatTime(hours);
}

//load cities data from .csv file
bool loadDataFromCSV(Graph &g, const string& filename, const vector<string>& userCities, vector<string>& foundCities) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return false;
    }

    string line;
    getline(file, line); // skip header

    unordered_set<string> citySet(userCities.begin(), userCities.end());
    set<pair<string, string>> addedEdges;

    while (getline(file, line)) {
        if (line.empty() || count(line.begin(), line.end(), ',') < 9) continue;

        stringstream ss(line);
        string city1, city2, roadDistStr, roadTime, taxiFareStr, airDistStr, flightCostStr, trainTime, sleeperFareStr, acFareStr;
        
        getline(ss, city1, ',');
        getline(ss, city2, ',');
        getline(ss, roadDistStr, ',');
        getline(ss, roadTime, ',');
        getline(ss, taxiFareStr, ',');
        getline(ss, airDistStr, ',');
        getline(ss, flightCostStr, ',');
        getline(ss, trainTime, ',');
        getline(ss, sleeperFareStr, ',');
        getline(ss, acFareStr, ',');

        // Capitalize cities
        transform(city1.begin(), city1.end(), city1.begin(), ::toupper);
        transform(city2.begin(), city2.end(), city2.begin(), ::toupper);

        if (citySet.count(city1) == 0 || citySet.count(city2) == 0) continue;

        try {
            TravelDetails details;
            details.roadDistance = stoi(roadDistStr);
            details.roadTime = roadTime;
            details.taxiFare = stoi(taxiFareStr);
            details.airDistance = stoi(airDistStr);
            details.flightCost = stoi(flightCostStr);
            details.trainTime = trainTime;
            details.sleeperFare = stoi(sleeperFareStr);
            details.acFare = stoi(acFareStr);
            details.transportMode = "road"; // Default mode

            if (find(foundCities.begin(), foundCities.end(), city1) == foundCities.end())
                foundCities.push_back(city1);
            if (find(foundCities.begin(), foundCities.end(), city2) == foundCities.end())
                foundCities.push_back(city2);

            string a = city1 < city2 ? city1 : city2;
            string b = city1 < city2 ? city2 : city1;
            if (addedEdges.insert({a, b}).second) {
                g.addEdge(city1, city2, details);
            }

        } catch (...) {
            cerr << "Invalid numeric data in line: " << line << endl;
            continue;
        }
    }

    file.close();
    return true;
}


//To load from activity.csv
void loadActivitiesFromCSV(Details_activities& g, const string& filename, const string& cities) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open activities file.\n";
        return;
    }

    string line, name, type, city, address, lat, lon, price, time;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        if (line.empty() || count(line.begin(), line.end(), ',') < 7) continue;

        stringstream ss(line);
        getline(ss, name, ',');
        getline(ss, type, ',');
        getline(ss, city, ',');
        getline(ss, address, ',');
        getline(ss, lat, ',');
        getline(ss, lon, ',');
        getline(ss, price, ',');
        getline(ss, time, ',');
        
        // Convert city name to uppercase for uniformity
        transform(city.begin(), city.end(), city.begin(), ::toupper);
        transform(type.begin(), type.end(), type.begin(), ::toupper);
        time = convertToDecimalHour(time);

        if (city == cities) {
            g.addEdge(city, {name, type, address, lat, lon, price, time});
        }
    }

    file.close();
}

void tspBacktrack(
    const unordered_map<string, vector<pair<string, TravelDetails>>> &adjList,
    const vector<string> &cities,
    vector<bool> &visited, 
    int currentCityIndex, 
    int countVisited, 
    int currentCost, 
    int &minCost,
    vector<int> &currentPath,
    vector<int> &bestPath) {

    int n = cities.size();

    if (countVisited == n) {
        for (const auto& neighbor : adjList.at(cities[currentCityIndex])) {
            if (neighbor.first == cities[0]) {
                int totalCost = currentCost + neighbor.second.roadDistance;
                if (totalCost < minCost) {
                    minCost = totalCost;
                    bestPath = currentPath;
                    bestPath.push_back(0); // return to start
                }
                return;
            }
        }
        return;
    }

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            bool hasEdge = false;
            int edgeCost = 0;

            for (const auto& neighbor : adjList.at(cities[currentCityIndex])) {
                if (neighbor.first == cities[i]) {
                    hasEdge = true;
                    edgeCost = neighbor.second.roadDistance;
                    break;
                }
            }

            if (!hasEdge) continue;

            visited[i] = true;
            currentPath.push_back(i);
            tspBacktrack(adjList, cities, visited, i, countVisited + 1, currentCost + edgeCost, minCost, currentPath, bestPath);
            visited[i] = false;
            currentPath.pop_back();
        }
    }
}

pair<vector<string>, int> runTSPBacktracking(Graph& g, const vector<string>& cities) {
    int n = cities.size();
    vector<string> tspOrder;

    if (n == 0) {
        cout << "No cities to plan a trip.\n";
        return {tspOrder, -1};
    }

    vector<bool> visited(n, false);
    visited[0] = true;
    vector<int> currentPath = {0};
    vector<int> bestPath;
    int minCost = INT_MAX;

    function<void(int, int, int)> tspBacktrack = [&](int currentIndex, int countVisited, int currentCost) {
        if (countVisited == n) {
            for (const auto& neighbor : g.adjList[cities[currentIndex]]) {
                if (neighbor.first == cities[0]) {
                    int totalCost = currentCost + neighbor.second.roadDistance;
                    if (totalCost < minCost) {
                        minCost = totalCost;
                        bestPath = currentPath;
                        bestPath.push_back(0); // back to start
                    }
                    return;
                }
            }
            return;
        }

        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                int roadDist = -1;
                for (const auto& neighbor : g.adjList[cities[currentIndex]]) {
                    if (neighbor.first == cities[i]) {
                        roadDist = neighbor.second.roadDistance;
                        break;
                    }
                }
                if (roadDist == -1) continue;

                visited[i] = true;
                currentPath.push_back(i);
                tspBacktrack(i, countVisited + 1, currentCost + roadDist);
                visited[i] = false;
                currentPath.pop_back();
            }
        }
    };

    tspBacktrack(0, 1, 0);

    if (minCost != INT_MAX) {
        for (int idx : bestPath) {
            tspOrder.push_back(cities[idx]);
        }
        return {tspOrder, minCost};
    } else {
        return {{}, -1};
    }
}


void displayTSPPath(const pair<vector<string>, int>& tspPath, Graph& g) {
    const vector<string>& path = tspPath.first;
    int totalDistance = 0;
    int totalFare = 0;
    double totalTravelTime = 0.0;

    if (path.empty()) {
        cout << "No valid tour exists connecting all cities." << endl;
        return;
    }

    cout << "\n=== DETAILED TRIP ITINERARY ===" << endl;
    cout << "Optimal route: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i != path.size() - 1) cout << " -> ";
    }
    cout << endl << endl;

    // Display segment-by-segment details
    cout << "Trip segments with details:" << endl;
    cout << setw(25) << left << "From" << setw(25) << "To" << setw(15) << "Distance" 
         << setw(20) << "Mode" << setw(15) << "Cost" << setw(20) << "Travel Time" << endl;
    cout << string(115, '-') << endl;

    for (size_t i = 0; i < path.size() - 1; ++i) {
        const string& from = path[i];
        const string& to = path[i + 1];
        
        // Find the travel details for this segment
        TravelDetails details;
        bool found = false;
        
        // First check direct direction
        for (const auto& neighbor : g.adjList[from]) {
            if (neighbor.first == to) {
                details = neighbor.second;
                found = true;
                break;
            }
        }
        
        // If not found, check reverse direction
        if (!found) {
            for (const auto& neighbor : g.adjList[to]) {
                if (neighbor.first == from) {
                    details = neighbor.second;
                    found = true;
                    break;
                }
            }
        }
        
        if (!found) {
            cout << "  Error: No connection found between " << from << " and " << to << endl;
            continue;
        }
        
        // Use the appropriate cost, distance and time based on transport mode
        int cost = 0;
        string time = "";
        int distance = 0;
        
        if (details.transportMode == "road") {
            cost = details.taxiFare;
            time = details.roadTime;
            distance = details.roadDistance;
        } else if (details.transportMode == "train-sleeper") {
            cost = details.sleeperFare;
            time = details.trainTime;
            distance = details.roadDistance;  // Using road distance for train
        } else if (details.transportMode == "train-ac") {
            cost = details.acFare;
            time = details.trainTime;
            distance = details.roadDistance;  // Using road distance for train
        } else if (details.transportMode == "flight") {
            cost = details.flightCost;
            time = calculateFlightTime(details.airDistance);
            distance = details.airDistance;
        } else {
            // Fallback for unknown mode (shouldn't happen)
            cout << "  Warning: Unknown transport mode '" << details.transportMode << "' for " 
                 << from << " to " << to << ". Using road data." << endl;
            cost = details.taxiFare;
            time = details.roadTime;
            distance = details.roadDistance;
        }
        
        totalDistance += distance;
        totalFare += cost;
        
        // Parse time and add to total
        double timeInHours = 0.0;
        
        if (details.transportMode == "flight") {
            // For flights, we calculate time from distance
            timeInHours = static_cast<double>(details.airDistance) / 600.0;
        } else if (details.transportMode.find("train") != string::npos) {
            // Parse train time
            stringstream ss(details.trainTime);
            int hours = 0, minutes = 0;
            string token;
            bool foundHours = false, foundMinutes = false;
            
            // Handle different train time formats
            if (details.trainTime.find("day") != string::npos) {
                int days = 0;
                if (sscanf(details.trainTime.c_str(), "%d day %d hours", &days, &hours) >= 1) {
                    timeInHours = (days * 24) + hours;
                    foundHours = true;
                }
            }
            
            if (!foundHours) {
                while (ss >> token) {
                    if (isdigit(token[0])) {
                        int value = stoi(token);
                        string nextToken;
                        if (ss >> nextToken) {
                            if (nextToken.find("hour") != string::npos || nextToken.find("hr") != string::npos) {
                                hours = value;
                                foundHours = true;
                            } else if (nextToken.find("min") != string::npos) {
                                minutes = value;
                                foundMinutes = true;
                            }
                        }
                    }
                }
                
                if (foundHours || foundMinutes) {
                    timeInHours = hours + (minutes / 60.0);
                } else {
                    timeInHours = stod(details.trainTime); // Fallback to direct conversion
                }
            }
        } else {
            // Parse road time
            stringstream ss(time);
            int hours = 0, minutes = 0;
            string token;
            bool foundHours = false, foundMinutes = false;
            
            // Handle "day" format
            if (time.find("day") != string::npos) {
                int days = 0;
                if (sscanf(time.c_str(), "%d day %d hours", &days, &hours) >= 1) {
                    timeInHours = (days * 24) + hours;
                    foundHours = true;
                }
            }
            
            if (!foundHours) {
                while (ss >> token) {
                    if (isdigit(token[0])) {
                        int value = stoi(token);
                        string nextToken;
                        if (ss >> nextToken) {
                            if (nextToken.find("hour") != string::npos || nextToken.find("hr") != string::npos) {
                                hours = value;
                                foundHours = true;
                            } else if (nextToken.find("min") != string::npos) {
                                minutes = value;
                                foundMinutes = true;
                            }
                        }
                    }
                }
                
                if (foundHours || foundMinutes) {
                    timeInHours = hours + (minutes / 60.0);
                } else {
                    // Fallback
                    timeInHours = 0;
                    cout << "  Warning: Could not parse time format for " << from << " to " << to << endl;
                }
            }
        }
        
        totalTravelTime += timeInHours;
        
        cout << setw(25) << left << from << setw(25) << to << setw(15) << distance 
             << setw(20) << details.transportMode << setw(15) << cost << setw(20) << time << endl;
    }
    
    cout << string(115, '-') << endl;
    cout << "Total travel distance: " << totalDistance << " km" << endl;
    cout << "Total travel cost: Rs. " << totalFare << endl;
    cout << "Total travel time: " << formatTime(totalTravelTime) << endl;
}

// Function to allow user to choose transportation mode for each segment
void chooseTransportModes(Graph& g, const vector<string>& path) {
    cout << "\n=== TRANSPORTATION OPTIONS ===" << endl;
    cout << "You can customize how you travel between cities." << endl;
    
    for (size_t i = 0; i < path.size() - 1; ++i) {
        const string& from = path[i];
        const string& to = path[i + 1];
        
        // Find travel details for both directions
        bool fromToFound = false;
        bool toFromFound = false;
        TravelDetails* fromToDetails = nullptr;
        TravelDetails* toFromDetails = nullptr;
        
        // Search for from->to connection
        for (auto& neighbor : g.adjList[from]) {
            if (neighbor.first == to) {
                fromToDetails = &neighbor.second;
                fromToFound = true;
                break;
            }
        }
        
        // Search for to->from connection
        for (auto& neighbor : g.adjList[to]) {
            if (neighbor.first == from) {
                toFromDetails = &neighbor.second;
                toFromFound = true;
                break;
            }
        }
        
        if (!fromToFound && !toFromFound) {
            cout << "Error: No connection found between " << from << " and " << to << endl;
            continue;
        }
        
        cout << "\nFor travel from " << from << " to " << to << ":" << endl;
        
        // Display details - using fromToDetails if available, otherwise toFromDetails
        TravelDetails* displayDetails = fromToFound ? fromToDetails : toFromDetails;
        
        cout << "1. Road - Distance: " << displayDetails->roadDistance << " km, Time: " << displayDetails->roadTime 
             << ", Cost: Rs. " << displayDetails->taxiFare << endl;
        cout << "2. Train - Time: " << displayDetails->trainTime << endl;
        cout << "   a. Sleeper Class - Cost: Rs. " << displayDetails->sleeperFare << endl;
        cout << "   b. AC Class - Cost: Rs. " << displayDetails->acFare << endl;
        cout << "3. Flight - Distance: " << displayDetails->airDistance << " km, Time: " 
             << calculateFlightTime(displayDetails->airDistance) << ", Cost: Rs. " << displayDetails->flightCost << endl;
        
        int choice;
        cout << "Choose transportation mode (1-3): ";
        cin >> choice;
        
        string newMode;
        int newFare = 0;
        
        switch (choice) {
            case 1:
                newMode = "road";
                newFare = displayDetails->taxiFare;
                break;
        
            case 2: {
                char trainChoice;
                cout << "Choose train class (a-Sleeper, b-AC): ";
                cin >> trainChoice;
                if (trainChoice == 'a' || trainChoice == 'A') {
                    newMode = "train-sleeper";
                    newFare = displayDetails->sleeperFare;
                } else {
                    newMode = "train-ac";
                    newFare = displayDetails->acFare;
                }
                break;
            }
        
            case 3:
                newMode = "flight";
                newFare = displayDetails->flightCost;
                break;
        
            default:
                cout << "Invalid choice. Defaulting to road transportation." << endl;
                newMode = "road";
                newFare = displayDetails->taxiFare;
        }
        
        // Update both directions
        if (fromToFound) {
            fromToDetails->transportMode = newMode;
            fromToDetails->totalFare = newFare;
        }
        
        
        
        cout << "Selected " << newMode << " for " << from << " to " << to << endl;
    }
    
    // After all changes, verify the updates
    cout << "\nVerifying transportation mode updates..." << endl;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        const string& from = path[i];
        const string& to = path[i + 1];
        
        bool found = false;
        for (const auto& neighbor : g.adjList[from]) {
            if (neighbor.first == to) {
                cout << "Route " << from << " to " << to << " is set to: " << neighbor.second.transportMode << endl;
                found = true;
                break;
            }
        }
        
        if (!found) {
            // Check reverse direction
            for (const auto& neighbor : g.adjList[to]) {
                if (neighbor.first == from) {
                    cout << "Route " << from << " to " << to << " is set to: " << neighbor.second.transportMode 
                         << " (using reverse direction)" << endl;
                    found = true;
                    break;
                }
            }
        }
        
        if (!found) {
            cout << "Warning: Could not verify route " << from << " to " << to << endl;
        }
    }
}

double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    // Haversine formula to calculate the great-circle distance between two points
    const double R = 6371.0; // Earth radius in kilometers
    
    // Convert degrees to radians
    lat1 *= M_PI / 180.0;
    lon1 *= M_PI / 180.0;
    lat2 *= M_PI / 180.0;
    lon2 *= M_PI / 180.0;
    
    // Haversine formula
    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;
    double a = pow(sin(dlat/2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon/2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    double distance = R * c;
    
    return distance; // Distance in kilometers
}
    
void displayActivityGraph(const Graph& g) {//only for intracity
    for (const auto& city : g.adjList) {
        cout << city.first << " ->" << endl;
        if (city.second.empty()) {
            cout << "  No connections" << endl;
        } else {
            for (const auto& neighbor : city.second) {
                const TravelDetails& d = neighbor.second;
                cout << "  To: " << neighbor.first << endl;
                cout << "    Road Distance: " << d.roadDistance << " km" << endl;
                cout << "    Road Time: " << d.roadTime << endl;
                cout << "    Taxi Fare: Rs. " << d.taxiFare << endl;
            }
        }
        cout << endl;
    }
}

void getOptimalActivities(const Details_activities& cityActivitiesData, const string& targetCity,
                          double maxBudget, double maxTime, Graph& resultGraph, Details_activities& Selected) {
    // Filter activities for the given city
    vector<pair<string, vector<string>>> cityActivities;
    for (const auto& entry : cityActivitiesData.activities) {
        if (entry.first == targetCity) {
            cityActivities.push_back(entry);
        }
    }

    if (cityActivities.empty()) {
        cerr << "No activities found for city: " << targetCity << endl;
        return;
    }

    // Prepare activity indices sorted by time/cost ratio (greedy)
    vector<pair<double, int>> valueRatios;  // (time/cost ratio, index)
    for (size_t i = 0; i < cityActivities.size(); ++i) {
        double cost = stod(cityActivities[i].second[5]);
        double time = stod(cityActivities[i].second[6]);

        double ratio = (cost > 0) ? time / cost : 0;
        valueRatios.emplace_back(ratio, i);
    }

    sort(valueRatios.begin(), valueRatios.end(),
         [](const auto& a, const auto& b) {
             return a.first > b.first;
         });

    // Select activities under constraints
    vector<int> selectedIndices;
    double totalCost = 0, totalTime = 0;
    for (const auto& pair : valueRatios) {
        double ratio = pair.first;
        int index = pair.second;
        double cost = stod(cityActivities[index].second[5]);
        double time = stod(cityActivities[index].second[6]);

        if (totalCost + cost <= maxBudget && totalTime + time <= maxTime) {
            selectedIndices.push_back(index);
            totalCost += cost;
            totalTime += time;

            Selected.activities.push_back(cityActivities[index]);
        }
    }

    // Build activity connection graph
    for (size_t i = 0; i < selectedIndices.size(); ++i) {
        for (size_t j = i + 1; j < selectedIndices.size(); ++j) {
            const auto& activity1 = cityActivities[selectedIndices[i]].second;
            const auto& activity2 = cityActivities[selectedIndices[j]].second;

            string name1 = activity1[0], name2 = activity2[0];
            double lat1 = stod(activity1[3]), lon1 = stod(activity1[4]);
            double lat2 = stod(activity2[3]), lon2 = stod(activity2[4]);

            double distance = calculateDistance(lat1, lon1, lat2, lon2);
            double travelTimeHours = distance / 40.0;
            double taxiFare = 50.0 + 20.0 * distance;

            TravelDetails details;
            details.roadDistance = static_cast<int>(distance);
            int hours = static_cast<int>(travelTimeHours);
            int minutes = static_cast<int>((travelTimeHours - hours) * 60);
            details.roadTime = to_string(hours) + " hrs " + to_string(minutes) + " mins";
            details.taxiFare = static_cast<int>(taxiFare);

            resultGraph.addEdge(name1, name2, details);
        }
    }
    cout<<endl<<"Total Cost: Rs. " << totalCost << endl << "Total Time: " << totalTime << " hours" << endl;
}

    int getFare(const string& from, const string& to, const string& mode, 
        const unordered_map<string, vector<pair<string, pair<string, pair<int, int>>>>>& travelData) 
    {
    if (travelData.find(from) != travelData.end()) {
    const vector<pair<string, pair<string, pair<int, int>>>>& routes = travelData.at(from);
    for (int i = 0; i < routes.size(); ++i) {
        if (routes[i].first == to && routes[i].second.first == mode) {
            return routes[i].second.second.first; // fare
        }
    }
    }
    return -1; // not found
    }

    int getDuration(const string& from, const string& to, const string& mode, 
            const unordered_map<string, vector<pair<string, pair<string, pair<int, int>>>>>& travelData) {
    if (travelData.find(from) != travelData.end()) {
    const vector<pair<string, pair<string, pair<int, int>>>>& routes = travelData.at(from);
    for (int i = 0; i < routes.size(); ++i) {
        if (routes[i].first == to && routes[i].second.first == mode) {
            return routes[i].second.second.second; // duration
        }
    }
    }
    return -1; // not found
    }

    

int main(int argc, char* argv[]) {
    cout << "\t\t\tWELCOME TO HAPPY TRIP PLANNER" << endl;
    cout << "\t\tCreate your time and budget friendly trip" << endl<<endl;

    Graph g;
    string filename = "city_travel_data.csv";
    vector<string> cities;
    vector<string> foundCities;
    unordered_set<string> foundSet;
    char choice;

    //when user enters a city with incorrect spelling or a city not available in trip planner.
    do {
        cities.clear();
        cities = getUserCities();
        foundCities.clear();

        if (!loadDataFromCSV(g, filename, cities, foundCities)) {
            cerr << "Failed to load city data.\n";
            return 1;
        }

        bool allFound = true;
        foundSet.clear();
        foundSet.insert(foundCities.begin(), foundCities.end());
        for (const string& city : cities) {
            if (!foundSet.count(city)) {
                cout << "City \"" << city << "\" not available in our plan or incorrect spelling.\n";
                allFound = false;
            } else if (!g.adjList.count(city)) {
                cout << "Note: \"" << city << "\" is not connected to any other selected city — skipping.\n";
            }
        }

        if (!allFound) {
            cout << "Do you want to enter different cities instead? (Y/N): ";
            cin >> choice;
            choice = toupper(choice);
        } else {
            break;
        }

    } while (choice == 'Y' || choice=='y');

    cout<<endl<<"\t\t=========================================="<<endl<<endl;
    cout<<"Calculating best optimal path from city to city" << endl;
    pair<vector<string>, int> bestPath = runTSPBacktracking(g, foundCities);
    if (bestPath.first.empty() || bestPath.second == -1) {
        cout << "No valid TSP tour could be found for the given cities." << endl;
        return 1;
    }

    // First display based on default road transport
    displayTSPPath(bestPath, g);

    cout<<endl<<"\t\t==========================================="<<endl<<endl;

    
    // Ask if user wants to customize transport modes
    cout << "\nDo you want to customize transportation modes between cities? (Y/N): ";
    cin >> choice;
    if (choice == 'Y' || choice == 'y') {
        chooseTransportModes(g, bestPath.first);
        // Re-display with updated transport choices
        cout << "\nUpdated itinerary with your transportation choices:" << endl;
        displayTSPPath(bestPath, g);
    }
    
    cout<<endl<<"\t\t==========================================="<<endl<<endl;

    cout<<"Do you want to see distances of every possible path from every city? (Y/N): ";
    cin>>choice;
    if (choice=='Y' || choice=='y') {
        cout<<endl<<"Displaying all the possible paths from every city"<<endl;
        g.displayGraph();
        cout<<endl;
    }
    cout<<endl;

    //////////////////////////////////////////////////////////////////////////////////
   
    vector<Details_activities> activitiesPerCity(cities.size());
    vector<Details_activities> Selected_activitiesPerCity(cities.size());
    Graph Activities[n];

    ///////////////////////////////////////////////////////////////////////////////////


    cout<<"Do you want to continue to build plan for all the activities you can do in a city? (Y/N): ";
    cin>>choice;
    if(choice=='Y' || choice=='y'){
        for (size_t i = 1; i < cities.size(); ++i) {
            loadActivitiesFromCSV(activitiesPerCity[i-1], "activity.csv", cities[i]);
        }
        cout<<endl<<endl;
     
        //budget and time constraint
        vector<double>maxbudget(cities.size()-1);
        vector<double>maxTime(cities.size()-1);
        cout<<"Enter maximum budget for every city (in Rs.)"<<endl;
        for(int i=0;i<cities.size()-1;i++){
            cout<<"Enter budget for "<<cities[i+1]<<" : ";
            cin>>maxbudget[i];
        }

        cout<<endl;
        cout<<"Enter maximum time allocated for every city(in hours)"<<endl;
        for(int i=0;i<cities.size()-1;i++){
            cout<<"Enter Maximum time for "<<cities[i+1]<<" : ";
            cin>>maxTime[i];
        }
        cout<<"\t\t============================================================"<<endl<<endl;
        cout << "\n\nGenerating optimal activity plans for each city..." << endl;
    
       // Skip cities[0] as it's the source city
       for (size_t i = 1; i < cities.size(); i++) {
        const string& city = cities[i];
        if (foundSet.count(city)) { // Only process cities found in our database
            cout << "\n==== Optimal Activities Plan for " << city << " ====" << endl;
            cout << "Budget: Rs." << maxbudget[i-1] << ", Time: " << maxTime[i-1] << " hours" << endl;
            
            getOptimalActivities(activitiesPerCity[i-1], city, maxbudget[i - 1], maxTime[i - 1], Activities[i-1],Selected_activitiesPerCity[i-1]);

            if (!Selected_activitiesPerCity[i-1].activities.empty()) {
                cout << "Selected activities :" << endl;
                Selected_activitiesPerCity[i-1].printGraph();
                cout<<endl;
                
            } else {
                cout << "No activities could be planned within the given constraints." << endl;
            }
        }
    }
    cout<<endl<<"\t\t============================================"<<endl<<endl;

    cout<<"Displaying the Shortest complete path to do all the activities in a city : "<<endl<<endl;
    //running tsp on activities
    for(int i=1;i<=n;i++){
        vector<string>temp_act;
        for(const auto& pair : Selected_activitiesPerCity[i-1].activities){
            temp_act.push_back(pair.second[0]);
        }
        cout<<"Path for activities in "<<cities[i]<<endl;
        pair<vector<string>,int>activity_tsp=runTSPBacktracking(Activities[i-1],temp_act);
        displayTSPPath(activity_tsp, Activities[i-1]);
        cout<<endl;
    } 
}else{
    goto end_;
}

    cout<<"Want to see all paths for all activities in all cities?(Y/N) : ";
    cin>>choice;
    if (choice=='Y' || choice=='y' ){
        for(int i=1;i<=n;i++){
            cout << "For: " << cities[i] << endl;
            displayActivityGraph(Activities[i-1]);
            cout << endl;
        }
    }


cout<<"Display all activities that you can do in all cities? (Y/N) ";
cin>>choice;
if (choice=='Y' || choice=='y' ){
    for(int i=1;i<=n;i++){
        activitiesPerCity[i-1].printGraph();
    }
}

end_:
cout << "\nThank you for using Happy Trip Planner!" << endl;
return 0;
}