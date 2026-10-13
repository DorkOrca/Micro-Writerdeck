#include <Arduino.h>
// Display library for ST7789
#include <TFT_eSPI.h>
#include <SPI.h>
// BLE for Bluetooth 5.0 keyboard
#include <NimBLEDevice.h>

TFT_eSPI tft = TFT_eSPI();


// put function declarations here:

int initializeNimBLE() {
  // Set up BLE
  NimBLEDevice::init("");
  return 0;
}

int readNimBLE(NimBLEClient *pClient, const NimBLEAdvertisedDevice *device) {

  if (pClient->connect(&device)) {
    // Success!
    tft.fillScreen(TFT_BLACK);
    tft.drawString("Connected to device!", 5, 5);
  } else {
    // Failed to connect...
    tft.fillScreen(TFT_BLACK);
    tft.drawString("Found a device,", 5, 5);
    tft.drawString("but failed to connect...", 5, 25);
  }
  return 0;
}

int scanNimBLE() {
  tft.fillScreen(TFT_BLACK);
  tft.drawString("Searching for a device...", 5, 5);
  NimBLEScan *pScan = NimBLEDevice::getScan();
  NimBLEScanResults results = pScan->getResults(10 * 1000);

  // 0x1812 is the standard uuid for the BLE HID service:
  // https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/Assigned_Numbers/out/en/Assigned_Numbers.pdf
  NimBLEUUID serviceUuid = NimBLEUUID::fromString("0x1812");

  // Iterate through all devices, identifying if they advertise the desired service
  for (int i = 0; i < results.getCount(); i++) {
    const NimBLEAdvertisedDevice *device = results.getDevice(i);

    if (device->isAdvertisingService(serviceUuid)) {
      NimBLEClient *pClient = NimBLEDevice::createClient();
      readNimBLE(pClient, device);
      return 0; // Exit after finding first device
    }
  }
  tft.fillScreen(TFT_BLACK);
  tft.drawString("No devices found.", 5, 5);
  tft.drawString("Searching again in 3 secs.", 5, 25);
  delay(3000);
  return 0;
}

void setup() {
  // Set up display
  tft.begin();
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

  initializeNimBLE();
  
}

void loop() {
  tft.fillScreen(TFT_BLACK);
  scanNimBLE();
  delay(1000);
}