import numpy
from matplotlib import pyplot
from prcRadio import PrcRadio

radio = PrcRadio('COM6')
if (not(radio.connect())):
    print('Echec de la connexion')
else:
    sampleNb = 100                  # Nombre d'échantillons
    radio.sendInt(sampleNb)
    samplePer = 10                  # Période d'échantillonnage, en ms
    radio.sendInt(samplePer)
    data = numpy.zeros((sampleNb,1), dtype='int16')
    for a in range(sampleNb):
        data[a] = radio.getInt()
    radio.disconnect()

    endTime = sampleNb*samplePer/1000    #Durée de l'acquisition, en s
    time = numpy.linspace(0,endTime,sampleNb)
    pyplot.plot(time, data)
    pyplot.show()
