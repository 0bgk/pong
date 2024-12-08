Technical documentation for the scripts and required updates to the `Makefile`:

---

### **Executable Scripts**

1. **Compile the Project**:
   ```bash
   make
   ```

2. **Run the Game**:
   ```bash
   make run
   ```

3. **Clean Generated Files**:
   ```bash
   make clean
   ```

---

### **Required Updates to the `Makefile`**

1. **Library and Include Paths**:
   - **INCLUDE_DIR**: Update the path to Raylib headers based on your system configuration.
   - **LIB_DIR**: Update the path to Raylib libraries based on your system configuration.

   **Example**:
   ```make
   INCLUDE_DIR = /path/to/raylib/include
   LIB_DIR = /path/to/raylib/lib
   ```

   Ensure these paths are correctly set to where Raylib is installed on your system.

---

These adjustments will ensure the project can be compiled and executed on any device.