# Application Architecture Plan

#### Overview
This plan describes the secure flow of data from client applications (ProcessLister) to the backend server, and then to the frontend interface. It includes device onboarding, user authentication via OAuth 2.0 with JWT for security, data storage with SQL, and real-time updates to the frontend.

---

### Client-Backend Interaction

**Client Onboarding**
- Generate an onboarding script through the backend for each device, embedding **`tenantID`** and **`deviceID`**.
- The script configures the client device, enabling it to send data to the backend.

**Client Agent (ProcessLister)**
- Periodically collect process data: process name, command, and CPU usage.
- Authenticate with OAuth 2.0 provider to receive JWT.
- Send process data to backend via HTTPS POST, including JWT for user authentication and device identifiers for data association.

---

### Backend Server Responsibilities

**Device Registration and Association**
- Link `tenantID` and `deviceID` with the user's account based on the JWT during the initial device registration.
- Validate the JWT to ensure data is stored correctly per tenant and device.

**Data Processing and Storage**
- Process incoming data and store it in an SQL database.
- Implement data retention policies to automatically purge data older than 30 days.

---

### Backend-Frontend Communication

**Secured API Endpoints**
- Provide API endpoints for data retrieval, requiring JWT for access.
- Use WebSockets or SSE for real-time data streaming, ensuring the frontend receives immediate updates.

**Frontend Application (Svelte)**
- Offer a login interface for user authentication; upon login, obtain JWT.
- Fetch and display process data using JWT to access the backend APIs.
- Present data with interactive graphics and real-time capabilities.

---

### Security and Data Management

**Encryption and Protocols**
- Use HTTPS for all data transmissions to ensure security in transit.
- Encrypt sensitive data at rest within the SQL database.

**Access Control and Validation**
- Validate JWT on each request to enforce access control.
- Ensure users can only access data associated with their tenant.

**Performance Considerations**
- Optimize data transmission frequency to balance system load.
- Implement rate limiting to protect against DDoS attacks and system abuse.

---

#### Implementation Flow

1. **Onboarding**: Use backend-generated scripts to onboard devices with unique identifiers.
2. **Registration**: Devices authenticate and register with the backend, establishing their tenant association.
3. **Data Handling**: Collect and transmit data securely, processing and storing it with tenant and device context.
4. **Database Management**: Apply retention policies for data lifecycle management.
5. **User Interaction**: Authenticate users, retrieve and present data through the frontend, ensuring a secure and user-friendly experience.

---