
class SensorStream:
    '''keeps the last values sent by the sensor'''
    
    def __init__(self, x=0, y=0, z=0, gX=0, gY=0, gZ=0, window_size=20, memory_size = 500):
        self.x = x
        self.y = y
        self.z = z
        self.gX = gX
        self.gY = gY
        self.gZ = gZ
        self.past = [x,y,z,gX,gY,gZ]
        self.window_index = 0
        self.window_size = window_size
        self.gZ_window = [0] * self.window_size
        self.memory_size = memory_size
        self.memory = [0] * self.memory_size
        
    def setValues(self, x=0, y=0, z=0, gX=0, gY=0, gZ=0):
        self.past[0] = self.x
        self.x = x
        self.past[1] = self.y
        self.y = y
        self.past[2] = self.z
        self.z = z
        self.past[3] = self.gX
        self.gX = gX
        self.past[4] = self.gY
        self.gY = gY
        self.past[5] = self.gZ
        self.gZ = gZ
        self.gZ_window[self.window_index] = gZ
        self.window_index = (self.window_index + 1)%self.window_size
        
    def getValues(self):
        return self.x, self.y, self.z, self.gX, self.gY, self.gZ
    
    def getWindowAvg(self):
        return sum(self.gZ_window) / float(len(self.gZ_window))
    
    def resetWindow(self, new_window_size):
        self.window_index = 0
        self.window_size = new_window_size
        self.gZ_window = [0] * self.window_size
                         
    def setMemoryCell(self, i):
        self.memory[i] = self.gZ
        
    def getVariations(self):
        delta_x = self.x-self.past[0]
        delta_y = self.y-self.past[1]
        delta_z = self.z-self.past[2]
        delta_gX = self.gX-self.past[3]
        delta_gY = self.gY-self.past[4]
        delta_gZ = self.gZ-self.past[5]
        return delta_x, delta_y, delta_z, delta_gX, delta_gY, delta_gZ