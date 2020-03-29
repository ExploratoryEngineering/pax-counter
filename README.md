# PAX Counter

Tested on ESP-WROOM-32D

## Wiring (ESP):
* VCC   3.3V
* EN    to VCC via 10k pull-up
* IO0   to GND via boot switch (enable programming mode by grounding IO0 before power-on)
* GND
* IO16    to EE-NBIoT-01 RXD
* IO17    to EE-NBIoT-01 TXD
* RXD0    a) to FTDI TX and b) to GND via 10k pull-down
* TXD0    to FTDI RX

1) Use common ground for ESP-WROOM-32D, EE-NBIoT-01 and FTDI.
2) Leave FTDI VCC pin un connected
