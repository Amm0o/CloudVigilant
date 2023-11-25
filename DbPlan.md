### Database Design Plan

#### Overview
The database will store and manage all data related to processes, devices, tenants, and users. It will support multi-tenancy and ensure that users can only access data for devices within their tenant.

---

#### Tables and Relationships

**1. Tenants Table**
- Stores information about each tenant (e.g., companies or distinct entities).
- Fields: TenantID (Primary Key), TenantName, TenantDescription, etc.

**2. Users Table**
- Stores information about users.
- Fields: UserID (Primary Key), TenantID (Foreign Key), Username, PasswordHash, Email, Role, etc.
- Relationship: Each user belongs to one tenant.

**3. Devices Table**
- Stores information about devices on which the ProcessLister app is installed.
- Fields: DeviceID (Primary Key), TenantID (Foreign Key), DeviceName, DeviceDescription, RegistrationDate, LastActiveDate, etc.
- Relationship: Each device belongs to one tenant.

**4. Processes Table**
- Stores the data reported by the ProcessLister client app.
- Fields: ProcessID (Primary Key), DeviceID (Foreign Key), ProcessName, ProcessCommand, CPUPercentage, Timestamp, etc.
- Relationship: Each process record is associated with a device.

**5. Device_Process_Link Table** (Optional, if many-to-many relationship needed)
- Manages a many-to-many relationship between devices and processes if a process can belong to multiple devices or vice versa.
- Fields: DeviceID (Foreign Key), ProcessID (Foreign Key)
- Relationship: Links devices to processes.

---

#### Data Flow and Correlation

**Client-Side to Backend**
- The client-side ProcessLister app collects process data and sends it to the backend, along with the DeviceID and TenantID from the onboarding script.
- The backend uses DeviceID to insert records into the Processes Table.
- The backend uses TenantID to validate that the DeviceID belongs to the correct tenant before inserting the record.

**Backend Processing**
- The backend authenticates user requests using JWT, extracts the UserID, and verifies the TenantID from the Users Table.
- The backend retrieves device data from the Devices Table based on the TenantID to ensure users can only see devices belonging to their tenant.
- The backend queries the Processes Table for process data related to the user’s devices and prepares it for the frontend.

**Frontend Display**
- The frontend sends requests to the backend with the JWT.
- The backend validates the JWT and retrieves the corresponding tenant's data.
- The frontend receives the data and displays it to the user, ensuring that users only see data for their associated devices.

---

#### Security and Maintenance

**Access Control**
- Row-level security policies will be implemented to restrict data access based on TenantID.
- All queries will be constructed to require TenantID as a condition to prevent cross-tenant data access.

**Data Retention Policy**
- Scheduled jobs will delete records from the Processes Table older than 30 days.
- Audit logs will be maintained for user activities and device registrations for security monitoring.

---

This database plan ensures data integrity, supports multi-tenancy, and provides a clear structure for data flow from the client-side through the backend to the frontend. It accommodates the separation of concerns, allowing for efficient data management and retrieval based on user and tenant permissions.