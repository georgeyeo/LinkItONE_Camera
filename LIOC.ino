// LIOC.ino (LinkIt ONE Camera)

/*
	The circuit:
	 * SD card attached to SPI bus as follows:
	 ** MOSI - pin 11
	 ** MISO - pin 12
	 ** CLK - pin 13
	 ** CHIP_SELECT - pin 4

	modified 23 September 2015
	by George Yeo

	Examples:
	
	Arduino - SD - CardInfo
	Arduino - SD - listfiles
	Arduino - SD - Files
	Arduino - SD - 


  */
 



// include the SD library:
#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

File fileRoot;
File fileMyFile;


// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int CHIP_SELECT = 4;


// the setup function runs once when you press reset or power the board
void setup() {
	// initialize digital pin 13 as an output.
	//pinMode(13, OUTPUT);

	// Open serial communications and wait for port to open:
	Serial.begin(9600);
	
	while (!Serial) {
		; // wait for serial port to connect. Needed for Leonardo only
	}


	Serial.print("\nInitializing SD card...");
	// On the Ethernet Shield, CS is pin 4. It's set as an output by default.
	// Note that even if it's not used as the CS pin, the hardware SS pin
	// (10 on most Arduino boards, 53 on the Mega) must be left as an output
	// or the SD library functions will not work.
	pinMode(10, OUTPUT);     // change this to 53 on a mega


	// we'll use the initialization code from the utility libraries
	// since we're just testing if the card is working!
	//if (!card.init(SPI_HALF_SPEED, CHIP_SELECT))
	if (!SD.begin(CHIP_SELECT))
	{
		Serial.println("initialization failed. Things to check:");
		Serial.println("* is a card is inserted?");
		Serial.println("* Is your wiring correct?");
		Serial.println("* did you change the chipSelect pin to match your shield or module?");
		return;
	}
	
	Serial.println("Wiring is correct and a card is present.");
	
	// print the type of card
	Serial.print("\nCard type: ");
	
	switch (card.type())
	{
	case SD_CARD_TYPE_SD1:
		Serial.println("SD1");
		break;
	case SD_CARD_TYPE_SD2:
		Serial.println("SD2");
		break;
	case SD_CARD_TYPE_SDHC:
		Serial.println("SDHC");
		break;
	default:
		Serial.println("Unknown");
	}

	// Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
	if (!volume.init(card)) {
		Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
		return;
	}


	// print the type and size of the first FAT-type volume
	uint32_t volumesize;
	Serial.print("\nVolume type is FAT");
	Serial.println(volume.fatType(), DEC);
	Serial.println();

	volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
	volumesize *= volume.clusterCount();       // we'll have a lot of clusters
	volumesize *= 512;                            // SD card blocks are always 512 bytes
	Serial.print("Volume size (bytes): ");
	Serial.println(volumesize);
	Serial.print("Volume size (Kbytes): ");
	volumesize /= 1024;
	Serial.println(volumesize);
	Serial.print("Volume size (Mbytes): ");
	volumesize /= 1024;
	Serial.println(volumesize);


	Serial.println("\nFiles found on the card (name, date and size in bytes): ");
	root.openRoot(volume);

	// list all files in the card with date and size
	root.ls(LS_R | LS_DATE | LS_SIZE);
	
	
	// List files
	fileRoot = SD.open("/");

	printDirectory(fileRoot, 0);

	Serial.println("List files done!");


	// Open a new file and immediately close it:
	Serial.println("Creating example.txt...");
	
	fileMyFile = SD.open("example.txt", FILE_WRITE);
	fileMyFile.close();

	// Check to see if the file exists:
	if (SD.exists("example.txt")) {
		Serial.println("example.txt exists.");
	}
	else {
		Serial.println("example.txt doesn't exist.");
	}

	
	// Delete the file:
	Serial.println("Removing example.txt...");
	SD.remove("example.txt");

	if (SD.exists("example.txt")) {
		Serial.println("example.txt exists.");
	}
	else {
		Serial.println("example.txt doesn't exist.");
	}
}

// the loop function runs over and over again forever
void loop() {
	//digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
	//delay(1000);              // wait for a second
	//digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
	//delay(1000);              // wait for a second
}


void printDirectory(File dir, int numTabs)
{
	while(true)
	{
		File entry =  dir.openNextFile();
		
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
