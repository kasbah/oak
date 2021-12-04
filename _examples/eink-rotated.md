---
layout: example
title: E-Ink rotated
description: Display lat-long and Haversine distance
references:
  - name: Arduino Waveshare E-paper 1in54 version 1
    url: https://github.com/waveshare/e-Paper/tree/master/Arduino/epd1in54
  - name: Arduino Waveshare E-paper 1in54 version 2
    url: https://github.com/waveshare/e-Paper/tree/master/Arduino/epd1in54_V2
difficulty: medium
features:
  - e-ink
  - e-paper
  - waveshare
  - display
prerequisites:
  - name: E-Ink hello world
    url: ./eink
images:
  prototype: eink-rotated-prototype.jpg
  console: eink-rotated-console.png
steps:
  - step: Plug in the Oak PCB to the computer
  - step: Ensure the PCB can be detected with <code>ls -al /dev/cu.usbmodem</code> and <code>arduino-cli board list</code>
    image: print-hello-check.png
  - step: Run <code>make</code> to upload the code
  - step: Start the serial monitor to view the console logs
---

Display rotated information on the E-Ink as an example to display the various information such as latitude, longitude, Haversine distance and last timestamp.

This example uses Version 1 of WaveShare 1.54inch E-Ink module.