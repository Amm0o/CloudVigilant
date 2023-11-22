## Components
Data is sent from a client (which could be your `ProcessLister` application) to the backend server, and then from the backend server to the frontend.
Classic client-server architecture with a frontend interface.

### Client to Backend Connection

1. **Client Agent (ProcessLister App)**:
   - The client collects process data (like CPU usage, process name, etc.).
   - Periodically (every second), the client sends this data to the backend. This is  done using an HTTP POST request.
   - The client can use a library like `libcurl` (for C++).

2. **Backend Server**:
   - Receives data from the client via POST requests.
   - Processes or stores this data as needed. This could involve saving to a database, performing some calculations, or preparing the data for the frontend.
   - The backend will be implemented in **BUN**

### Backend to Frontend Connection

1. **Backend Server**:
   - Provides an API endpoint for the frontend to fetch the processed data.
   - This endpoint will be polled by the frontend or, for real-time updates, use WebSockets or Server-Sent Events (SSE).

2. **Frontend Application**:
   - Makes requests to the backend to fetch the data. This could be a simple fetch request if polling, or setting up a WebSocket/SSE connection for real-time updates.
   - Displays the data from the backend in a user-friendly format(Graphics). The frontend will be built in Svelte/React

### Real-time Updates Consideration

- If real-time updates are critical (e.g., the frontend needs to reflect changes in process data almost immediately), WebSockets or Server-Sent Events are preferable. These allow the server to push updates to the frontend without the need for constant polling.
- For less time-sensitive updates, regular AJAX polling (where the frontend periodically makes HTTP requests to the backend for new data) could be sufficient and simpler to implement.

### Security and Performance

- Ensure secure data transmission by using HTTPS for all requests.
- Authenticate requests from the client to the backend.
- Consider the frequency of data updates and the performance implications. Sending data every second can be resource-intensive, so optimize both the client and server implementations for handling frequent requests.

### Example Flow

1. **Data Collection**: The client (ProcessLister app) gathers process data.
2. **Data Transmission**: Every second, the client sends this data to the backend server via a POST request.
3. **Data Processing**: The backend server processes/stores this data and makes it available via an API endpoint.
4. **Data Retrieval**: The frontend periodically requests this data from the backend (or receives it via a WebSocket/SSE connection) and updates the UI accordingly.

This architecture allows for a clear separation of concerns: the client focuses on data collection, the backend on data processing and storage, and the frontend on data presentation.