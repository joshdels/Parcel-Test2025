Collaboration Project of Arduino Hardware of SwiftDropper repository

Instructions:

    A. Getting Files Ready
        1. Extract from Main and Place then in you folder --> User/Documents/Arduino
            a. Parcel Parcel_Swift
            b. Server
        2. Install libraries inside ARDUINO IDE for TFT Screen
            a. Adafruit GFX Libraries
            b. Adafruit TouchScreen
            b. MCUFRIEND_kbv 
        3. Add ESP 8266 board by adding it this to the preferences --> http://arduino.esp8266.com/stable/package_esp8266com_index.json

    B. Uploading Sketches to ATMEGA + ESP 8266 Developer Board
        B.1 For ESP 8266 Board
            1. Select Board ESP 8266
            2. Modify Sketch Server.ino inside Server Folder
                a. ssd = YOUR WIFI SSD NAME
                b. password = YOUR WIFI PASSWORD
                c. serverUrl = YOUR SERVER URL
            3. Switch Mode --> SW5, SW6, SW7 ON, the rest are OFF to upload USB port to Board
            4. Switch Mode --> SW5, SW6 ON the rest are OFF to view the fetch data in JSON format
        B.2 For ATMEGA Board
            1. Select Board ATMEGA 2560
            2. Modify Sketch Parcel_Swift Ino
                a. ceiling_limit --> for bin limit to full in cm
                b. courierPasscode --> for correct bin entry to open collecting bin
            2. Switch Mode --> SW3, SW4, the rest are OFF to upload USB port to Board
            3. Upload the Sketch
            4. Switch Mode --> SW1, SW2, Sw3, SW4 to interact USD <-> ATMEGA <-> ESP 8266




