---
layout: example
title: LoRa duplex node B
description: LoRa transmit and receive from destination address approximately at the same time
references:
  - name: Arduino LoRa duplex example code
    url: https://github.com/sandeepmistry/arduino-LoRa/blob/master/examples/LoRaDuplex/LoRaDuplex.ino
prerequisites:
  - name: LoRa Duplex A
    url: ./lora-duplex-a
difficulty: medium
features:
  - LoRa
  - duplex
  - p2p
images:
  prototype: lora-tx-prototype.jpg
  console: lora-duplex-console.png
steps:
  - step: Prepare <a href="./lora-duplex-a">LoRa duplex A</a> to send and receive at the same time
  - step: Plug in the Oak PCB to the computer
  - step: Ensure the PCB can be detected with <code>ls -al /dev/cu.usbmodem</code> and <code>arduino-cli board list</code>
    image: print-hello-check.png
  - step: Run <code>make</code> to upload the code
---