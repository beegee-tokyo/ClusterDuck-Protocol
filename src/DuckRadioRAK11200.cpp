

#include "include/DuckRadio.h"

#if defined(_VARIANT_RAK11200_)
#include "include/DuckUtils.h"
#include <SX126x-Arduino.h>

/*
 * set LoraWan_RGB to 1,the RGB active in loraWan
 * RGB blue means sending;
 * RGB green means received done;
 */
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

#define RF_FREQUENCY CDPCFG_RF_LORA_FREQ_HZ

#define TX_OUTPUT_POWER CDPCFG_RF_LORA_TXPOW
#define RX_TIMEOUT_VALUE 1000
#define TX_TIMEOUT_VALUE 3000
// [0: 125 kHz, 1: 250 kHz, 2: 500 kHz, 3: Reserved]
#define LORA_BANDWIDTH 0

// [7: SF7, ... 12: SF12]
#define LORA_SPREADING_FACTOR 7

// [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
#define LORA_CODINGRATE 3
#define LORA_PREAMBLE_LENGTH 8 // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 0  // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false
#define LORA_CRC_ON true

// DuckRadio* DuckRadio::instance = NULL;

DuckRadio::DuckRadio() {}

static RadioEvents_t RadioEvents;

static void OnLoraTxDone(void) {
  loginfo("TX done");

  Radio.Sleep();
  // turnOnRGB(0x002000, 300);
  // turnOnRGB(0x000000, 0);
}

static void OnLoraTxTimeout(void) {
  loginfo("TX timeout");
  Radio.Sleep();
  // turnOnRGB(0x3000000,300);
  // turnOnRGB(0x000000, 0);
}

static void OnLoraRxDone(uint8_t* payload, uint16_t size, int16_t rssi,
                         int8_t snr) {
  loginfo("RX Done");
  Radio.Sleep();
  logdbg("Received Hex:");
  for (int i = 0; i < size; i++) {
    logdbg(*payload++, HEX);
  }
  logdbg_f("\nRSSI:%d, SNR:%d, Size:%d\r\n", rssi, snr, size);
}

static void OnLoraRxTimeout(void) {
  loginfo("RX Timeout");
  Radio.Sleep();
}

static void OnLoraRxError(void) {
  Radio.Sleep();
  loginfo("RX error");
}

// DuckRadio* DuckRadio::getInstance() {
//   return (instance == NULL) ? new DuckRadio : instance;
// }

int DuckRadio::setupRadio(LoraConfigParams config) {
	pinMode(LED_GREEN, OUTPUT);
	pinMode(LED_BLUE, OUTPUT);
	digitalWrite(LED_GREEN, LOW);
	digitalWrite(LED_BLUE, LOW);
	lora_rak13300_init();
	RadioEvents.TxDone = OnLoraTxDone;
	RadioEvents.TxTimeout = OnLoraTxTimeout;
	Radio.Init(&RadioEvents);

	Radio.SetChannel(RF_FREQUENCY);
	Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
					  LORA_SPREADING_FACTOR, LORA_CODINGRATE,
					  LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
					  LORA_CRC_ON, 0, 0, LORA_IQ_INVERSION_ON, TX_TIMEOUT_VALUE);

	// Set Radio RX configuration
	Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
					  LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
					  LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
					  0, true, 0, 0, LORA_IQ_INVERSION_ON, true);

	Radio.SetPublicNetwork(false);
	// Radio.SetSyncWord(LORA_MAC_PRIVATE_SYNCWORD);
	Radio.Sleep();
	return DUCK_ERR_NONE;
}

int DuckRadio::sendData(byte* data, int length) {
  // turnOnRGB(0, 300);
  loginfo("Sending packet: len: " + String(length));
  Radio.Send(data, length);
  return DUCK_ERR_NONE;
}

int DuckRadio::sendData(std::vector<byte> data) {
  // turnOnRGB(0x0000F0, 0);
  loginfo("Sending packet: len: " + String(data.size()));
  Radio.Send(data.data(), data.size());
  return DUCK_ERR_NONE;
}

int DuckRadio::relayPacket(DuckPacket* packet) {
  loginfo("Relaying packet: len: " + packet->getBuffer().size());
  Radio.Send(packet->getBuffer().data(), packet->getBuffer().size());
  return DUCK_ERR_NONE;
}

int DuckRadio::startReceive() { return DUCK_ERR_NOT_SUPPORTED; }
int DuckRadio::readReceivedData(std::vector<byte>* packetBytes) {
  return DUCK_ERR_NOT_SUPPORTED;
}

int DuckRadio::getRSSI() { return Radio.Rssi(MODEM_LORA); }

// TODO: implement this
int DuckRadio::ping() { return DUCK_ERR_NOT_SUPPORTED; }

int DuckRadio::standBy() {
  Radio.Standby();
  return DUCK_ERR_NONE;
}

void DuckRadio::processRadioIrq() { Radio.IrqProcess(); }

void DuckRadio::setChannel(int channelNum)
{
	logdbg("Setting Channel to : ");
	logdbg(channelNum);

	switch (channelNum)
	{
	case 2:
		loginfo("Set channel: " + String(channelNum));
		Radio.SetChannel((uint32_t)(CHANNEL_2 * 1000000)); // 914.0 * 1000 = 914000.0 * 1000 = 914.000.000
		Radio.Rx(0);
		break;
	case 3:
		loginfo("Set channel: " + String(channelNum));
		Radio.SetChannel((uint32_t)(CHANNEL_3 * 1000000));
		Radio.Rx(0);
		break;
	case 4:
		loginfo("Set channel: " + String(channelNum));
		Radio.SetChannel((uint32_t)(CHANNEL_4 * 1000000));
		Radio.Rx(0);
		break;
	case 5:
		loginfo("Set channel: " + String(channelNum));
		Radio.SetChannel((uint32_t)(CHANNEL_5 * 1000000));
		Radio.Rx(0);
		break;
	case 6:
		loginfo("Set channel: " + String(channelNum));
		Radio.SetChannel((uint32_t)(CHANNEL_6 * 1000000));
		Radio.Rx(0);
		break;
	case 1:
	default:
		loginfo("Set channel: " + String(channelNum));
		Radio.SetChannel((uint32_t)(CHANNEL_1 * 1000000));
		Radio.Rx(0);
		break;
	}

		Radio.Rx(0);
		channel = channelNum;
		loginfo("Channel Set");
}

// IMPORTANT: this function MUST be 'void' type and MUST NOT have any arguments!
void DuckRadio::onInterrupt(void)
{
	// Interrupt handling is within SX126x-Arduino library
	// interruptFlags = lora.getIRQFlags();
}

int DuckRadio::startTransmitData(byte *data, int length)
{
	int err = DUCK_ERR_NONE;
	loginfo("TX data");
	logdbg(" -> " + duckutils::convertToHex(data, length));
	logdbg(" -> length: " + String(length));

	if (length > 256)
	{
		return DUCKLORA_ERR_MSG_TOO_LARGE;
	}
	long t1 = millis();
	/// \todo SX126x-Arduino does this in the background and is not waiting for the transmission to end
	Radio.Send(data, length);

	return DUCK_ERR_NONE;
}

#endif