import struct

class DataParser:
    '''parses bytes from sensor and saves the values in a SensorStream; acc_unit and gy_unit
    are the quantization steps for accelerometer and gyroscope respectively'''
    def __init__(self, acc_unit=100, gy_unit=100):
        self.data_split = range(6)
        self.acc_unit = acc_unit
        self.gy_unit = gy_unit

    def parse_data(self, buffer, sensor):
        for i in range(6):
            self.data_split[i] = buffer[2*i:2*(i+1)]
            
        sensor.setValues(x=struct.unpack_from("!h",self.data_split[0])[0]/self.acc_unit, 
                         y=struct.unpack_from("!h",self.data_split[1])[0]/self.acc_unit, 
                         z=struct.unpack_from("!h",self.data_split[2])[0]/self.acc_unit, 
                         gX=struct.unpack_from("!h",self.data_split[3])[0]/self.gy_unit, 
                         gY=struct.unpack_from("!h",self.data_split[4])[0]/self.gy_unit, 
                         gZ=struct.unpack_from("!h",self.data_split[5])[0]/self.gy_unit
                         )