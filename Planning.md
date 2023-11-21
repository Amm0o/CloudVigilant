# Client Side

## Local Components

### Performance gatherer
* Collect CPU, Memory for all processes running on the device --> More KPIs to be added in the future
* Store local logs, small ones rotated every time they reach like 5mbs.
* Made in c++ ideally with no 3rd party requisites
* Send data in real time using the HTTP Library to the backend. 

### HttpClient --> Library
* The performance gatherer will collect the data and use a HttpClient(Fully built in c++) to send the data to the backend

### WatchDog
* Additional service that will listen for call from the backend to start the performance gatherer.
* Simple program that will rely on the HttpClient library to listen to on/off signals from the backend to turn the performance gatherer on/off.


# CloudClient

## Backend
* Built in bun, will take the data from the performance gatherer and pass it to the front end.
* Will have multiple API endpoints for each KPI.
* **API security** to be defined.

## Front End
* Built in React/Svelte.
* Will get data from the backend in real time.
* Give the ability to display graphs for resource consumption per process. 
