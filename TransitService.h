#ifndef TRANSIT_SERVICE_H_
#define TRANSIT_SERVICE_H_

#include <vector>
#include <string>

struct Route {
    std::string id;
    std::string name;
    int agencyId;
};

struct Agency {
    int id;
    std::string name;
};

struct Direction {
    int id;
    std::string name;
};

struct Stop {
    std::string placeCode;
    std::string description;
};

struct StopDetail {
    float longitude;
    float latitude;
    std::string nextDepartureText;
};

class ITransitService {
public:
    virtual ~ITransitService() {}
    virtual std::vector<Route> GetRoutes() = 0;
    virtual std::vector<Agency> GetAgencies() = 0;
    virtual std::vector<Direction> GetDirections(const std::string& routeId) = 0;
    virtual std::vector<Stop> GetStops(const std::string& routeId, int direction) = 0;
    virtual std::vector<StopDetail> GetStopDetail(const std::string& routeId, int direction, const std::string& placeCode) = 0;
};

#include "WebService.h"

class MetroTransitAPI : public ITransitService {
public:
    MetroTransitAPI() : ws("https://svc.metrotransit.org") {}
    std::vector<Agency> GetAgencies();
    std::vector<Route> GetRoutes();
    std::vector<Direction> GetDirections(const std::string& routeId);
    std::vector<Stop> GetStops(const std::string& routeId, int direction);
    std::vector<StopDetail> GetStopDetail(const std::string& routeId, int direction, const std::string& placeCode);

private:
    WebService ws;
};

#endif