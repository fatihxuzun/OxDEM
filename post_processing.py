from pyevtk.hl import pointsToVTK
from pyevtk.vtk import VtkGroup
import numpy as np
import os, glob

def readData(d):
    for i in range(len(d)):
        d[i] = d[i].split('\n')[0].split('\t')
        for j in range(len(d[i])):
            d[i][j] = float(d[i][j])
    return d

p = open('simulation_output.txt', 'r')
p = p.readlines()
p = readData(p)

P = int((len(p[0]) - 1) / 11) # -1 is for time column

totalTimeSteps = len(p)

tf = VtkGroup("./time_frame")

for i in range(totalTimeSteps):
    x = []; y = []; z = []; r = []; Vx = []; Vy = []; Vz = []; mass = []; a_x = []; a_y = []; a_z = []
    for j in range(P):
        x.append(p[i][j * 11 + 0 + 1])
        y.append(p[i][j * 11 + 1 + 1])
        z.append(p[i][j * 11 + 2 + 1])
        r.append(p[i][j * 11 + 3 + 1])
        mass.append(p[i][j * 11 + 4 + 1])
        Vx.append(p[i][j * 11 + 5 + 1])
        Vy.append(p[i][j * 11 + 6 + 1])
        Vz.append(p[i][j * 11 + 7 + 1])
        a_x.append(p[i][j * 11 + 8 + 1])
        a_y.append(p[i][j * 11 + 9 + 1])
        a_z.append(p[i][j * 11 + 10 + 1])
    x = np.array(x)
    y = np.array(y)
    z = np.array(z)
    r = np.array(r)
    mass = np.array(mass)
    Vx = np.array(Vx)
    Vy = np.array(Vy)
    Vz = np.array(Vz)
    a_x = np.array(a_x)
    a_y = np.array(a_y)
    a_z = np.array(a_z)
    pointsToVTK("time_frame/tf_" + str(i), x, y, z, data={"radius": r, "Vx": Vx, "Vy": Vy, "Vz": Vz, "mass": mass, "a_x": a_x, "a_y": a_y, "a_z": a_z})

for i in range(totalTimeSteps):
    tf.addFile(filepath = "time_frame/tf_" + str(i) + ".vtu", sim_time = i)
tf.save()