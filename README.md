# Arduino-Based Gate Entry Security System

## 📖 Project Overview
This project implements a fully functional Arduino-powered security camera and gate control system. It automates user access through motion detection, remote monitoring, and secure identification using an IR remote.
---

## 🚀 Features
- 📸 **Motion Detection with TTL Serial Camera**  
  Detects motion and captures images, storing them on an SD card.
  
- 📟 **Real-Time Notifications via LCD Display**  
  Displays system status messages such as *Motion Detected*, *Provide Code*, and *Permission Granted/Denied*.

- 🌐 **TODO: Remote Monitoring with ThingSpeak**  
  Notifies the moderator when motion is detected and checks their availability status through a web-connected interface.

- 🎮 **User Authentication via IR Remote**  
  Uses an IR receiver to accept user ID codes. Access is granted or denied based on predefined authorized codes.

- ⚙️ **Automated Gate Control**  
  Controls a DC motor using a relay to physically open and close the gate based on access permissions.

- 📈 **User Tracking and Logging**  
  Maintains a count of users inside the facility and logs events through serial communication.

---

## 📋 Workflow
1. Motion detected → Capture image → Store on SD card.
2. TODO: Notify moderator via ThingSpeak → Display *Moderator Notified* on LCD.
3. TODO: Check moderator status:
   - **Busy (`0`)**: Display *Admin Busy*.
   - **Available (`1`)**: Prompt for user code via IR remote.
4. Validate user code:
   - **Authorized**: Display *Permission Granted* → Open gate → Log event → Update user count.
   - **Unauthorized**: Display *Permission Denied*.
5. LCD Messages Include:
   - *Motion Detected*
   - *Provide Code*
   - *Name Associated with Code*
   - *Permission Granted* / *Permission Denied*
   - *Welcome [User]*

---

## 📦 Hardware Components
- TTL Serial Camera
- Arduino UNO (or compatible)
- 1602 LCD Display with I2C or Shift Register
- SD Card Module
- Relay Module (SRD-05VDC-SL-C)
- DC Motor
- IR Receiver and Remote Control
- Internet Connectivity for ThingSpeak Integration

---

## 📚 Documentation
- ✅ Final report (APA 7th Edition)
- ✅ Detailed algorithm and flowchart
- ✅ Annotated source code with comments
- ✅ Project schematics and wiring diagrams
- ❌ Functional ThingSpeak web integration

---

## 📌 Notes
This project was developed as part of a university course on IoT Microcontrollers. It demonstrates real-world embedded systems design, integrating hardware control, cloud communication, and secure access management.

---

Feel free to explore the code and contribute to future enhancements! ⭐
