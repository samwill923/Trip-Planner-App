# Trip Planner

Trip Planner is a C++ application that generates optimized travel itineraries based on user-defined budget, time constraints, and destination preferences. It provides personalized routes, transportation suggestions, and activity recommendations using graph-based algorithms and greedy heuristics.

## Features

- Generate optimal multi-city travel routes using backtracking-based TSP algorithm
- Support for multiple transportation modes: road, train (sleeper/AC), and flight
- User-defined preferences for cost and time per trip segment
- Activity suggestions in each city based on available budget and time
- Greedy selection of activities to maximize value within constraints
- Visualization of travel paths and detailed itinerary breakdown

## Technologies Used

- C++ STL for data structures and algorithms
- File handling for reading `.csv` data (travel details and activities)
- Graph algorithms (Travelling Salesman Problem)
- Greedy algorithm for activity selection
- Terminal-based user interaction

## Input Files

- `city_travel_data.csv`: Contains distances, fares, and times between cities for different modes of transport
- `activity.csv`: Lists available activities in each city along with details such as price, time required, and coordinates

## How It Works

1. User inputs a list of cities they wish to visit.
2. The system loads city data and verifies availability.
3. A TSP algorithm computes the most efficient route covering all cities.
4. Users optionally select transportation modes between cities.
5. For each city, the system recommends a set of activities within the given budget and time.
6. A separate TSP is run to minimize intra-city travel for activities.

## Getting Started

1. Compile the code using a C++17-compliant compiler:

   ```bash
   g++ -std=c++17 Trip\ Planner_Final.cpp -o trip_planner
   Ensure the following files are present in the same directory:

2. Ensure these files are present in the same directory :
   o city_travel_data.csv
   o activity.csv

3. Run the executable:

   ```bash
   ./trip_planner
   
4. Follow on-screen prompts to enter cities, choose transportation modes, set constraints, and view plans.

## OUTPUT SCREEN 
<img width="921" height="607" alt="image" src="https://github.com/user-attachments/assets/d6e6f3d5-40e8-4900-8d37-6962b173a93a" />
<img width="802" height="669" alt="image" src="https://github.com/user-attachments/assets/6b71ae68-30e3-4ac9-a0a6-33c3f77b5e29" />
<img width="777" height="297" alt="image" src="https://github.com/user-attachments/assets/b31252d0-776f-49c7-b8ce-409766a5721c" />

## Limitations
- Assumes data consistency in .csv files
- TSP implementation may not scale well for a large number of cities or activities
- Command-line only interface

## Credits :
College : Jaypee Institute of Information Technology
Course : B.Tech CSE
Year : 2025
