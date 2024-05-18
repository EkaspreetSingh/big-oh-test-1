#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

class Person {
public:
    string name;
    string id;
};

class User : public Person {
public:
    User(string name, string userId) {
        this->name = name;
        this->id = userId;
    }

    bool operator==(const User &other) const {
        return id == other.id;
    }
};

class HotelAgent : public Person {
public:
    HotelAgent(string name, string userId) {
        this->name = name;
        this->id = userId;
    }

    bool operator==(const HotelAgent &other) const {
        return id == other.id;
    }
};

// Custom hash function for User
struct UserHash {
    size_t operator()(const User& user) const {
        return hash<string>()(user.id);
    }
};

// Custom equality function for User
struct UserEqual {
    bool operator()(const User& lhs, const User& rhs) const {
        return lhs.id == rhs.id;
    }
};

// Custom hash function for HotelAgent
struct HotelAgentHash {
    size_t operator()(const HotelAgent& agent) const {
        return hash<string>()(agent.id);
    }
};

// Custom equality function for HotelAgent
struct HotelAgentEqual {
    bool operator()(const HotelAgent& lhs, const HotelAgent& rhs) const {
        return lhs.id == rhs.id;
    }
};

class Hotel {
protected:
    string name;
    string location;
    int rating;
    vector<string> reviews;
    int totalRooms;
    int occupiedRooms;
    vector<string> facilities;
    vector<User> users;

public:
    Hotel(string name, string location, int totalRooms, vector<string> facilities)
        : name(name), location(location), totalRooms(totalRooms), occupiedRooms(0), facilities(facilities), rating(0) {}

    string getName() { return name; }
    string getLocation() { return location; }
    int getAvailableRooms() { return totalRooms - occupiedRooms; }
    vector<string> getFacilities() { return facilities; }

    void setNumberOfRooms(int n) { totalRooms = n; }
    void setReview(string review) { reviews.push_back(review); }
    void setRating(int rating1) {
        rating = ((rating * reviews.size()) + rating1) / (reviews.size() + 1);
    }
    void addFacility(string facility) { facilities.push_back(facility); }

    void viewHotel() {
        cout << "Hotel name: " << name << endl;
        cout << "Hotel location: " << location << endl;
        cout << "Number of available rooms: " << (totalRooms - occupiedRooms) << endl;
        cout << "Rating: " << rating << " stars\n";
        cout << "Reviews: " << endl;
        for (const auto& review : reviews) {
            cout << review << endl;
        }
        cout << "Facilities: " << endl;
        for (const auto& facility : facilities) {
            cout << facility << endl;
        }
    }

    void addUser(User user, int rooms) {
        users.push_back(user);
        occupiedRooms += rooms;
    }

    void removeUser(User user, int rooms) {
        auto it = remove(users.begin(), users.end(), user);
        if (it != users.end()) {
            users.erase(it, users.end());
            occupiedRooms -= rooms;
        }
    }

    bool operator==(const Hotel &other) const {
        return name == other.name && location == other.location;
    }
};

class Admin : public Person {
private:
    string adminKey;

public:
    vector<Hotel> hotels;
    unordered_set<User, UserHash, UserEqual> users;
    unordered_set<HotelAgent, HotelAgentHash, HotelAgentEqual> hotel_agents;

    Admin(string name, string adminKey) {
        this->name = name;
        this->adminKey = adminKey;
    }

    bool addUser(User user) {
        if (users.find(user) != users.end()) {
            cout << "User is registered already" << endl;
            return false;
        }
        users.insert(user);
        return true;
    }

    bool addHotelAgent(HotelAgent hotelAgent) {
        if (hotel_agents.find(hotelAgent) != hotel_agents.end()) {
            cout << "Hotel agent is registered already" << endl;
            return false;
        }
        hotel_agents.insert(hotelAgent);
        return true;
    }
};

class AdminManagement {
public:
    AdminManagement() {}

    void updateRooms(Hotel& hotel, int n) {
        hotel.setNumberOfRooms(n);
    }

    void addFacility(Hotel& hotel, string facility) {
        hotel.addFacility(facility);
    }

    void addHotel(Admin& admin, Hotel hotel) {
        admin.hotels.push_back(hotel);
    }

    void removeHotel(Admin& admin, Hotel hotel) {
        admin.hotels.erase(remove(admin.hotels.begin(), admin.hotels.end(), hotel), admin.hotels.end());
    }
};

class HotelAgentManagement {
public:
    void updateRooms(Hotel& hotel, int n) {
        hotel.setNumberOfRooms(n);
    }

    void addFacility(Hotel& hotel, string facility) {
        hotel.addFacility(facility);
    }
};

class UserManagement {
public:
    void bookHotel(User& user, Hotel* hotel, int rooms) {
        if (hotel->getAvailableRooms() < rooms) {
            cout << "Hotel does not have enough rooms" << endl;
            return;
        }
        hotel->addUser(user, rooms);
    }

    void checkOut(User& user, Hotel* hotel, int rooms) {
        hotel->removeUser(user, rooms);
    }
};

class Menu {
public:
    void mainMenu() {
        cout << "-----------Main Menu:-----------" << endl;
        cout << "0. Exit" << endl;
        cout << "1. Admin Operations" << endl;
        cout << "2. User Operations" << endl;
        cout << "3. Hotel Agent Operations" << endl;
    }

    void adminMenu() {
        cout << "-----------Admin Operations Menu:-----------" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "1. Add New Hotel" << endl;
        cout << "2. Remove a Hotel" << endl;
        cout << "3. Add facilities to a Hotel" << endl;
        cout << "4. Set number of rooms for a Hotel" << endl;
    }

    void userMenu() {
        cout << "-----------User Operations Menu:-----------" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "1. Book a Hotel" << endl;
        cout << "2. Checkout" << endl;
        cout << "3. Add a rating and review to hotel" << endl;
    }

    void hotelAgentMenu() {
        cout << "-----------Hotel Agent Operations Menu:-----------" << endl;
        cout << "0. Back to Main Menu" << endl;
        cout << "1. Update rooms for a Hotel" << endl;
        cout << "2. Add a facility to a Hotel" << endl;
    }
};

int main() {
    AdminManagement adminManagement;
    HotelAgentManagement hotelAgentManagement;
    Admin admin("Eren Yeager", "admin123");
    Menu menu;

    while (true) {
        menu.mainMenu();
        int choice;
        cin >> choice;
        if (choice == 0) break;
        else if (choice == 1) {
            while (true) {
                menu.adminMenu();
                int adminChoice;
                cin >> adminChoice;
                if (adminChoice == 0) break;
                else if (adminChoice == 1) {
                    string name, location;
                    int totalRooms;
                    vector<string> facilities;
                    cout << "Enter hotel name: ";
                    cin >> name;
                    cout << "Enter hotel location: ";
                    cin >> location;
                    cout << "Enter number of rooms: ";
                    cin >> totalRooms;
                    cout << "Enter facilities (type 'done' to finish): ";
                    string facility;
                    while (true) {
                        cin >> facility;
                        if (facility == "done") break;
                        facilities.push_back(facility);
                    }
                    Hotel hotel(name, location, totalRooms, facilities);
                    adminManagement.addHotel(admin, hotel);
                } else if (adminChoice == 2) {
                    string name;
                    cout << "Enter hotel name to remove: ";
                    cin >> name;
                    auto it = find_if(admin.hotels.begin(), admin.hotels.end(), [&name](Hotel& h) { return h.getName() == name; });
                    if (it != admin.hotels.end()) {
                        adminManagement.removeHotel(admin, *it);
                    } else {
                        cout << "Hotel not found." << endl;
                    }
                } else if (adminChoice == 3) {
                    string name, facility;
                    cout << "Enter hotel name to add facility: ";
                    cin >> name;
                    cout << "Enter facility: ";
                    cin >> facility;
                    auto it = find_if(admin.hotels.begin(), admin.hotels.end(), [&name](Hotel& h) { return h.getName() == name; });
                    if (it != admin.hotels.end()) {
                        adminManagement.addFacility(*it, facility);
                    } else {
                        cout << "Hotel not found." << endl;
                    }
                } else if (adminChoice == 4) {
                    string name;
                    int rooms;
                    cout << "Enter hotel name to update rooms: ";
                    cin >> name;
                    cout << "Enter number of rooms: ";
                    cin >> rooms;
                    auto it = find_if(admin.hotels.begin(), admin.hotels.end(), [&name](Hotel& h) { return h.getName() == name; });
                    if (it != admin.hotels.end()) {
                        adminManagement.updateRooms(*it, rooms);
                    } else {
                        cout << "Hotel not found." << endl;
                    }
                }
            }
        } else if (choice == 2) {
            cout << "-----------User login/register:-----------" << endl;
            string name, userId;
            cout << "Enter name: ";
            cin >> name;
            cout << "Enter user id: ";
            cin >> userId;
            User user(name, userId);
            if (admin.addUser(user)) {
                cout << "User login success" << endl;
                UserManagement userManagement;
                while (true) {
                    menu.userMenu();
                    int userChoice;
                    cin >> userChoice;
                    if (userChoice == 0) break;
                    else if (userChoice == 1) {
                        string hotelName;
                        int rooms;
                        cout << "Enter hotel name: ";
                        cin >> hotelName;
                        cout << "Enter number of rooms to book: ";
                        cin >> rooms;
                        auto it = find_if(admin.hotels.begin(), admin.hotels.end(), [&hotelName](Hotel& h) { return h.getName() == hotelName; });
                        if (it != admin.hotels.end()) {
                            userManagement.bookHotel(user, &(*it), rooms);
                        } else {
                            cout << "Hotel not found." << endl;
                        }
                    } else if (userChoice == 2) {
                        string hotelName;
                        int rooms;
                        cout << "Enter hotel name: ";
                        cin >> hotelName;
                        cout << "Enter number of rooms to checkout: ";
                        cin >> rooms;
                        auto it = find_if(admin.hotels.begin(), admin.hotels.end(), [&hotelName](Hotel& h) { return h.getName() == hotelName; });
                        if (it != admin.hotels.end()) {
                            userManagement.checkOut(user, &(*it), rooms);
                        } else {
                            cout << "Hotel not found." << endl;
                        }
                    } else if (userChoice == 3) {
                        string hotelName, review;
                        int rating;
                        cout << "Enter hotel name to review: ";
                        cin >> hotelName;
                        cout << "Enter rating (1-5): ";
                        cin >> rating;
                        cout << "Enter review: ";
                        cin.ignore();
                        getline(cin, review);
                        auto it = find_if(admin.hotels.begin(), admin.hotels.end(), [&hotelName](Hotel& h) { return h.getName() == hotelName; });
                        if (it != admin.hotels.end()) {
                            it->setRating(rating);
                            it->setReview(review);
                        } else {
                            cout << "Hotel not found." << endl;
                        }
                    }
                }
            }
        } else if (choice == 3) {
            cout << "-----------Hotel Agent login/register:-----------" << endl;
            string name, agentId;
            cout << "Enter name: ";
            cin >> name;
            cout << "Enter agent id: ";
            cin >> agentId;
            HotelAgent hotelAgent(name, agentId);
            if (admin.addHotelAgent(hotelAgent)) {
                cout << "Hotel Agent login success" << endl;
                while (true) {
                    menu.hotelAgentMenu();
                    int agentChoice;
                    cin >> agentChoice;
                    if (agentChoice == 0) break;
                    else if (agentChoice == 1) {
                        string hotelName;
                        int rooms;
                        cout << "Enter hotel name to update rooms: ";
                        cin >> hotelName;
                        cout << "Enter number of rooms: ";
                        cin >> rooms;
                        auto it = find_if(admin.hotels.begin(), admin.hotels.end(), [&hotelName](Hotel& h) { return h.getName() == hotelName; });
                        if (it != admin.hotels.end()) {
                            hotelAgentManagement.updateRooms(*it, rooms);
                        } else {
                            cout << "Hotel not found." << endl;
                        }
                    } else if (agentChoice == 2) {
                        string hotelName, facility;
                        cout << "Enter hotel name to add facility: ";
                        cin >> hotelName;
                        cout << "Enter facility: ";
                        cin >> facility;
                        auto it = find_if(admin.hotels.begin(), admin.hotels.end(), [&hotelName](Hotel& h) { return h.getName() == hotelName; });
                        if (it != admin.hotels.end()) {
                            hotelAgentManagement.addFacility(*it, facility);
                        } else {
                            cout << "Hotel not found." << endl;
                        }
                    }
                }
            }
        }
    }

    return 0;
}