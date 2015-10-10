// LIOC.ino (LinkIt ONE Camera)

/*
	Drv card basic file example

	created   Nov 2010
	by David A. Mellis
	modified 9 Apr 2012
	by Tom Igoe


	port to LinkIt ONE
	by Loovee
	2014-10-12

	This example code is in the public domain.
 
 
	The circuit:
	 * SD card attached to SPI bus as follows:
	 ** MOSI - pin 11
	 ** MISO - pin 12
	 ** CLK - pin 13
	 ** CHIP_SELECT - pin 4

	modified 10 October 2015
	by George Yeo

	Examples:
	
	Arduino - SD - CardInfo
	Arduino - hardware - arduino - mtk - libraries - LStorage - examples - listfiles
	Arduino - hardware - arduino - mtk - libraries - LStorage - examples - Files
	Arduino - hardware - arduino - mtk - libraries - LStorage - examples - ReadWrite


*/
 



// include libraries:
#include <LTask.h>
#include <LFlash.h>
#include <LSD.h>
#include <LStorage.h>


#define Drv LFlash          // use Internal 10M Flash
// #define Drv LSD           // use SD card


LFile fileRoot;
LFile fileMyFile;


// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
//const int CHIP_SELECT = 4;


// the setup function runs once when you press reset or power the board
void setup()
{
	char	szExampleFile[255] = {0x00};
	
	strcpy(szExampleFile, "example.txt");
	
	// initialize digital pin 13 as an output.
	//pinMode(13, OUTPUT);

	// Open serial communications and wait for port to open:
    Serial.begin(9600);

    while(!Serial.available());         // input any thing to start


	Serial.print("\nInitializing internal 10M flash...");
    // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
    // Note that even if it's not used as the CS pin, the hardware SS pin
    // (10 on most Arduino boards, 53 on the Mega) must be left as an output
    // or the Drv library functions will not work.
    pinMode(10, OUTPUT);     // change this to 53 on a mega


	// we'll use the initialization code from the utility libraries
	// since we're just testing if the card is working!
	//if (!card.init(SPI_HALF_SPEED, CHIP_SELECT))
	if (!Drv.begin())
	{
		Serial.println("initialization failed. Things to check:");
		return;
	}
	
	Serial.println("initialization done.");
	
	
	fileRoot = Drv.open("/");

    printDirectory(fileRoot, 0);

    Serial.println("done!");
	
	
	if (Drv.exists(szExampleFile)) {
        Serial.println("example.txt exists.");
    }
    else {
        Serial.println("example.txt doesn't exist.");
    }

    // open a new file and immediately close it:
    Serial.println("Creating example.txt...");
	
	// open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    fileMyFile = Drv.open(szExampleFile, FILE_WRITE);
    
	if(fileMyFile)
	{
		fileMyFile.seek(0);
        // read from the file until there's nothing else in it:
        while (fileMyFile.available()) {            
            Serial.write(fileMyFile.read());
        }
		
		Serial.print("Writing ...");
        fileMyFile.println("testing 1, 2, 3.");
		
		fileMyFile.close();
	}
	else
		Serial.println("Failed in opening file");

    // Check to see if the file exists:
    if (Drv.exists(szExampleFile)) {
        Serial.println("example.txt exists.");
    }
    else {
        Serial.println("example.txt doesn't exist.");
    }

    // delete the file:
    Serial.println("Removing example.txt...");
    Drv.remove(szExampleFile);

    if (Drv.exists(szExampleFile)) {
        Serial.println("example.txt exists.");
    }
    else {
        Serial.println("example.txt doesn't exist.");
    }
}

// main of arduino
void loop()
{
	/*
	digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(1000);              // wait for a second
	digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
	delay(1000);              // wait for a second
	*/
}

void printDirectory(LFile dir, int numTabs)
{
    while(true)
	{
		LFile entry =  dir.openNextFile();
		
        if (! entry) {
            // no more files
            break;
        }
		
        for (uint8_t i=0; i<numTabs; i++) {
            Serial.print('\t');
        }
		
        Serial.print(entry.name());
		
        if (entry.isDirectory()) {
            Serial.println("/");
            printDirectory(entry, numTabs+1);
        } else {
            // files have sizes, directories do not
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
		
        entry.close();
    }
}

