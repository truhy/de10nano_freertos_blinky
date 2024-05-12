# FreeRTOS LED blink sample demo for the Terasic DE10-Nano development board

## Overview

Demonstrates FreeRTOS tasks to read an input key and blink a LED using the GPIO controller of the processor system (HPS) on the DE10-Nano development board.

In brief, this program creates some tasks that runs preemptively to blink an LED and also read an input key.  Both are wired to the processing side and labelled as HPS_KEY and HPS_LED within the DE10-Nano user manual.

The program starts with the LED blinking, holding the input key stops the blinking but keeps the LED on, releasing the key resumes the blinking.

## Guide

You can find the guide on my website:
[https://truhy.co.uk](https://truhy.co.uk)
