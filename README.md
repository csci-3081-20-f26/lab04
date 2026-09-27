# Lab 08 - Web Services and the Proxy Pattern

In this lab, your goal is to create a program that caches calls to a web service using the Proxy Pattern.

$\color{blue}{\textsf{You will need to get \textbf{Milestones 1 and 2} checked off by a TA during lab for full credit.}}$

## What You Will Learn

1. How to call a web service to get information using Swagger and C++.
2. Understanding how a facade is used to simplify complex systems.
3. How to query json objects for information returned from a REST web service.
4. How to use the std::vector<T> class for dynamic arrays.
5. How to get real-time transit information about buses and trains.


### What's in lab04

- Makefile:
  - This makefile is provided to automate the building of your project.  You should not need to edit this file.

- WebService.h / WebService.cpp
  - These files contains the code for simplified web service calls.  You can create a web service by passing in the base url for the service (```WebService webService("https://my-api.com")```).  You can call specific methods by using ```webService.get("/path/to/method")```.  You should not need to edit this file.

- TransitService.h / TransitService.cpp
  - These files contain the following classes:
    * ```TransitService``` - Implements the TransitService interface and calls the Metro Transit web service directly.

- main.cpp:
  - This file contains the main application that handles the core logic of the program.  You may edit this if you need.

### What You Will Edit

- main.cpp
  - You will edit parts of this file, specifically the ```realTimeInfo(...)``` function for **Milestone 3**.

- TransitService.h
  - You will shouldn't need to edit this file.

- TransitService.cpp
  - You will need to edit this file for **Milestone 2** and **Milestone 3**.

### Goal

<hr>
  **The primary goal of this lab is to become familiar with calling web services and using the proxy pattern to remove the expensive overhead of repeating api calls and restricting access to data.**
<hr>

### Getting Started

Navigate to lab04 in one of the development environments (VOLE / CSE Labs, SSH).

You can run the program with the following commands using the provided `Makefile`.

```bash
% cd lab04
% make
% ./transit_service
```

When you run, you will see the following output:

```bash
Options:
-------------------
0: Exit
1: List Agencies
2: List Routes
3: Real-Time Info

Enter a selection: 
```

Enter `1` and you should see the following output, which is a call to the MetroTransitAPI (https://svc.metrotransit.org/swagger/index.html):

```bash
Enter a selection: 1

[ Web Service Call: https://svc.metrotransit.org/nextrip/agencies ]
0: Metro Transit
2: Met Council
3: Minnesota Valley
4: Maple Grove
5: Plymouth
6: SouthWest Transit
10: Airport (MAC)
11: University of Minnesota
16: St Cloud Metro Bus

<Press enter to continue>
```

In this assignment, you will implement options 2 (Milestone 2) and 3 (Milestone 3).  You will also get rid of the `Web Service Call` for repeated calls (Milestone 4).   In Milestones 5 and 6, you will implement two other proxy classes that restrict data access.

You are now ready to begin the Lab!

### Milestone 1 - Swagger

Swagger is a useful interface for accessing and trying out existing REST APIs.  The first milestone for this lab entails exploring the Metro Transit NextTrip API.  Navigate to the following URL:

* [Metro Transit - NextTrip API](https://svc.metrotransit.org/swagger/index.html)

Investigate the various API calls.  Click on “GET /nextrip/agencies”.  What is the id for the agency for the “University of Minnesota”.  Most likely this represents buses like the Campus Connector.

Use the Swagger interface to find a stop for a place of interest ([Twin Cities Metropolitan Area Transit System Map](https://www.metrotransit.org/Data/Sites/1/media/pdfs/system-map.pdf)) finding a route, direction and place_code.  The following three API calls should help:

* ```/nextrip/routes```
* ```/nextrip/directions/{route}```
* ```/nextrip/stops/{route}/{direction}```

___
For **Milestone 1** you should be able to understand and make web service calls using the swagger.  You should be able to execute calls to find the places of interest in the Metro Transit system.
___

### Milestone 2 - List Routes

For this milestone, you will use your c++ program to list the routes in our system.  This is option two in our program:

```bash
Options:
-------------------
0: Exit
1: List Agencies
2: List Routes
3: Real-Time Info

Enter a selection: 2
```

To get this to work, you will need to call the correct web services similar to the ```MetroTransitAPI::getAgencies()``` method.  It will be a different web service call and you will need to get the data using a json object returned from the web service.

The json object can be accessed several ways (similar to lists and dictionaries in python):
* As an array: `json[5]`
* As a dictionary object: `json["route_id"]`
* As a int: `json["agency_id"].get<int>()`
* As a float: `json["agency_id"].get<float>()`
* As a string: `json["agency_name"].get<std::string>()`
* As debug output: `std::cout << json << std::endl;`
* As a hierarchical object (combining the above): `json[2]["departures"][0]["date"].get<std::string>()`

[MetroTransitAPI::getAgencies()](TransitService.cpp#L6) shows an example of how to call the web service and how access the json object.

Once you implement the ```MetroTransitAPI::getRoutes()``` method, option 2 should work.

___
For **Milestone 2** your output should look something like the following:
```bash
Enter a selection: 2

[ Web Service Call: https://svc.metrotransit.org/nextrip/routes ]
901: METRO Blue Line
902: METRO Green Line
905: METRO Gold Line
904: METRO Orange Line
425: Orange Link
903: METRO Red Line
921: METRO A Line
922: METRO B Line
923: METRO C Line
924: METRO D Line
906: Airport Shuttle
2: Route 2
3: Route 3
4: Route 4
...
```
___

### Milestone 3 - Get Real-Time Info

For this milestone, you will use your c++ program to list the routes in our system.  This is option three in our program:

```bash
Options:
-------------------
0: Exit
1: List Agencies
2: List Routes
3: Real-Time Info

Enter a selection: 3
```

To do this, you will implement the rest of the MetroTransitAPI methods in [TransitService.cpp](TransitService.cpp#L33).  You will also need to modify the `realTimeInfo(...)` method in the [main.cpp](main.cpp#L69).

___
For **Milestone 3** your output should look similar to the following (It is okay if `[ Web Service Call: ... ]` is part of the output:
```bash
Enter a selection: 3

Enter a route: 14

Available Directions:
-----------------------
0: Southbound
1: Northbound

Enter a direction: 1

Available Stops:
-----------------------
RBTC: Robbinsdale Transit Center
36NB: Noble Ave and 36th Ave
...

Enter a stop: 36NB

Nobel Ave and 36th Ave
-----------------------
Latitude: 45.020797
Longitude: -93.338913
Next Departure: 12 Min
```
___

### Implementing a Proxy

The second part of this lab (milestones 4-6) involve implementing the proxy pattern.  Proxies control access to an underlying object of the same type.  In this case, we can make method calls to a real object through a proxy object that controls how the real object is accessed.  In this lab we have three different examples:

1. A proxy that caches calls to a web service for data that doesn't change.  This reduces the number of repeated calls to a web service to reduce traffic and improve performance (```CachedTransitServiceProxy``` - Milestone 4).
2. A proxy that restricts access to certain types of data.  For example, we might want to restrict our web service access to only the "Metro Transit" and "University of Minnesota" agencies.  Perhaps don't want users to get access to other transit services without paying an additional fee or unless a certain authentication status is met (```TransitAgencyFilter``` - Milestone 5).
3. A proxy that uses your GPS location to unlock certain location based services.  In other words, based on where you are, you can have additional access to stop and real-time information (```TransitGpsLocaitonProxy``` - Milestone 6).

These proxies can be nested, changing the underlying access of the MetroTransitAPI.  When nesting proxies, we can get more flexible behavior as follows:

```c++
ITransitService* service = new MetroTransitAPI();
service = new CachedTransitServiceProxy(service);
service = new TransitAgencyFilter(service, {0, 11});
service = new TransitGpsLocationProxy(service, 44.973746, 93.235586, 0.0001);
```

This set of proxies, enables a service that caches calls the Metro Transit API, restricts the user to only "Metro Transit" or the "University of Minnesota", and only knows about the routes in those agencies that are near "Washington Ave & Coffman Union". Below is a UML of the proposed system described above:

<img src="proxy_design.png" alt="" />

### Milestone 4 - Use a proxy to cache repeated calls

For milestone 4, you will implement the `CachedTransitServiceProxy` in [TransitServiceProxy.h](TransitServiceProxy.h) and [TransitServiceProxy.cpp](TransitServiceProxy.cpp).

Your goal is to save any repeated calls that do not contain live info (e.g. next bus times) in arrays or maps to save the data if they are called again.

For example, in the `getRoutes()` method, you could check to see if routes have already been downloaded.  If they have, use the downloaded routes.  If they haven't, get the routes from the real service, save them, and return them.

___
For **Milestone 4** ensure that repeated calls are cached (e.g. `[Web Service Call: ... ]` is only called once for options 1, 2, and 3 - except the real-time data):
```bash
...

Enter a selection: 1

[ Web Service Call: https://svc.metrotransit.org/nextrip/agencies ]
0: Metro Transit
2: Met Council
...

Enter a selection: 1

0: Metro Transit
2: Met Council
...

Enter a selection: 2

[ Web Service Call: https://svc.metrotransit.org/nextrip/routes ]
901: METRO Blue Line
902: METRO Green Line
905: METRO Gold Line
...

Enter a selection: 2

901: METRO Blue Line
902: METRO Green Line
905: METRO Gold Line
...
```
___


### Milestone 5 - Use a proxy to restrict access to agencies

For milestone 5, you will implement the `TransitAgencyFilter` in [TransitServiceProxy.h](TransitServiceProxy.h) and [TransitServiceProxy.cpp](TransitServiceProxy.cpp).

Your goal is to only allow queries for routes that are available for specific agencies.  For example, route 789 is part of Maple Grove Transit (agency 4).  You should not be able to have access to any information (including real-time data) about route 789.  However, since route 2 and 14 are part of Metro Transit, you should be able to get information about these routes. 

___
For **Milestone 5** ensure that you can only get route information for Metro Transit and the University of Minnesota:
```bash
...

Enter a selection: 1

[ Web Service Call: https://svc.metrotransit.org/nextrip/agencies ]
0: Metro Transit
11: University of Minnesota


Enter a selection: 2

[ Web Service Call: https://svc.metrotransit.org/nextrip/routes ]
901: METRO Blue Line
902: METRO Green Line
905: METRO Gold Line
904: METRO Orange Line
903: METRO Red Line
921: METRO A Line
922: METRO B Line
923: METRO C Line
924: METRO D Line
2: Route 2
3: Route 3
...
14: Route 14
...
121: Route 121
...
766: Route 766
768: Route 768
824: Route 824
...


Enter a selection: 3

Enter a route: 787
Sorry that is not a valid route, 
Enter a route: 14

[ Web Service Call: ... ]
Available Directions:
-----------------------
0: Southbound
1: Northbound

...


```
___

### Milestone 6 - Use a proxy to restrict access based on GPS location

For milestone 6, you will implement the `TransitGpsLocationProxy` in [TransitServiceProxy.h](TransitServiceProxy.h) and [TransitServiceProxy.cpp](TransitServiceProxy.cpp).

Your goal is to only allow queries for routes and agencies that are at a nearby stops based on a longitude and latitude.  You can read in [stops.csv](stops.csv), (the data was downloaded from the [Metro Transit Site](https://svc.metrotransit.org/), to calculate the Euclidean Distance between each stop.  Only look at stops that are close enough (distance is less than the provided tolerance).  You can call https://svc.metrotransit.org/nextrip/{stop_id} to get information about the routes that are available for each stop.  Use these routes to determine which agencies are available and which routes and directions are available.  It's possible that a stop may only service one direction.

___
For **Milestone 6** ensure that you can only get route information for Metro Transit and the University of Minnesota (using the previous proxy) and are also close by (e.g. Washington Ave & Coffman Union" defined by 44.973746, 93.235586):
```bash
...

Enter a selection: 1

[ Web Service Call: https://svc.metrotransit.org/nextrip/agencies ]
0: Metro Transit
11: University of Minnesota


Enter a selection: 2

[ Web Service Call: https://svc.metrotransit.org/nextrip/routes ]
2: Route 2
121: Route 121
122: Route 122


Enter a selection: 3

Enter a route: 787
Sorry that is not a valid route, 
Enter a route: 14
Sorry that is not a valid route, 
Enter a route: 2

[ Web Service Call: ... ]
Available Directions:
-----------------------
0: Eastbound
1: Westbound
...


```

## Gradescope Submission

After creating your pull request, submit your work to the Gradescope assignment **"Lab 8: Web Services and Proxy"**:

1. From the root of your Iteration 1 project directory, create a compressed archive of your project:
    ```bash
    cd <path_to_your_lab_dir>
    tar -czf lab08_submission.tar.gz --exclude='.git' .
    ```
2. Upload `lab08_submission.tar.gz` to the Gradescope assignment.

The autograder will check:
- For the correctness of your code.

Once your submission is graded on Gradescope, you are done with the lab.
