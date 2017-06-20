#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Created on Sun Feb 19 22:19:08 2017

@author: M. Lapucci, F. Vittorini
@title:  cilent_connection
"""

# Import some stuff
import socket
import time
import matplotlib.pyplot as plt
from sensor_stream import SensorStream
from data_parser import DataParser
         
def main():
     
    threshold = 60
    second_threshold = 10
    stream = SensorStream()
    CONTINUOUS_INCREMENT = False
          
    TCP_IP = '192.168.43.222'
    TCP_PORT = 80
    BUFFER_SIZE = 1024
    sleep_time = 0.05
     
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((TCP_IP, TCP_PORT))
      
    parser = DataParser(acc_unit=100, gy_unit=128)
    
    mov = 0
    rotating = False
     
    while True:
         
        raw_bytestream = s.recv(BUFFER_SIZE)
        parser.parse_data(raw_bytestream, stream)
        
        x,y,z,gX,gY,gZ = stream.getValues()

		print x,y,z,gX,gY,gZ
        
           
        time.sleep(sleep_time)
         
    s.close()
     
if __name__ == "__main__":
    main()
