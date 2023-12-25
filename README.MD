## IoT - IoT Water Usage Monitoring

Aplikasi IoT untuk menghitung aliran air dan mengirimkan hasil perhitungan data ke backend



### Prerequisites

1. Arduino IDE
2. ESP8266
3. Waterflow Rate Sensor YF-S201



### Design

![Design IoT](https://github.com/riskikukuh/esp8266-iot-water-usage-monitoring/blob/master/images/design.png?raw=true)

#### Keterangan

1. Pin Positif Sensor YF-S201 dihubungkan ke pin VV ( 5 Volt )
2. Pin Negatif Sensor YF-S201 dihubungkan GND ( Ground )
3. Pin Output Sensor YF-S201 dihubungkan ke pin D2 ESP8266
4. Pin Negatif LED Hijau dihubungkan ke GND ( Ground )
5. Pin Positif LED Hijau dihubungkan ke pin D4 ESP8266
6. Pin Negatif LED Biru dihubungkan ke GND ( Ground )
7. Pin Positif LED Biru dihubungkan ke pin D3 ESP8266
8. Pin GND Nodemcu ESP8266 dihubungkan ke jalur Breadboard
9. Tempat aliran air untuk dihitung Sensor YF-S201 
10. Nodemcu ESP8266
11. Breadboard sebagai penghubung kabel jumper



### Installation

1. Atur konfigurasi WiFi pada source code dengan SSID dan Password yang akan digunakan
2. Atur `HOST` server pada source code, dengan IP / Host server anda
3. Sambungkan Kabel sensor YF-S201 sesuai dengan design
4. Sambungkan adapter microusb dengan ESP8266
5. LED Hijau hidup artinya, IoT berhasil tersambung dengan WiFi
6. LED Biru hidup artinya, ESP8266 berhasil mendeteksi aliran air



### Prototype

![Prototype](https://github.com/riskikukuh/esp8266-iot-water-usage-monitoring/blob/master/images/prototype-with-numbering.jpg?raw=true)


#### Keterangan

1. Air sebagai sumber air disimpan pada wadah / tandon yang terbuat dari botol
2. Tersambung dengan Sensor Waterflow Rate YF-S201 yang juga tersambung ke ESP8266
3. Pada ujung alat, terpasang keran air untuk membuka aliran air