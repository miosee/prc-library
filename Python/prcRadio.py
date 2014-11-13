import serial
from struct import unpack, pack


class PrcRadio(object):
    "Objet permettant de communiquer avec la radio BlueTooth du PRC"
    
    def __init__(self, port='COM3'):
        "constructeur de la classe, initialise les champs"
        self.connected = False                  # flag indiquant si on est connecté au PRC ou pas
        self.success = True                     # flag indiquant si la dernière méthode appelée a réussi sa tâche, utile surtout pour les lectures
        self.serialPort = serial.Serial()       # objet pour le port série
        self.serialPort.baudrate = 115200       # le débit est de 115200 bits
        self.serialPort.rtscts = True           # on utilise le handshake hardware du port série
        self.serialPort.timeout = 1             # Les méthodes read et write auront un time out de 1s
        self.serialPort.port = port             # Désigne le port COM à ouvrir
        
    def connect(self):
        "tente de se connecter au PRC, renvoie le flag de réussite"
        if (self.connected):
            self.success = True                 # si on est déjà connecté, on n'a rien à faire et on considère qu'on a réussi
        else:        
            try:
                self.serialPort.open()          # on essaie d'ouvrir le port
                self.connected = True           # en cas de réussite, on met à jour le flag de connexion
                self.success = True             # et celui de réussite
            except:
                self.success = False            # si l'ouverture a échoué, on ne met à jour que le flag de réussite
        return self.success                     # on renvoie le flag de réussite
    
    def disconnect(self):
        "déconnecte le PC du PRC, ne renvoie rien (tjs réussi)"
        self.serialPort.close()                 # on se déconnecte (cette action réussit tjs)
        self.connected = False                  # on met à jour le flag de connexion
        self.success = True                     # et celui de réussite
        
    def getInt(self):
        "Essaye de lire un 'int16' émis par le PRC. Abandonne après le ""time out"" défini (1s par défaut)"
        try:
            tmp = self.serialPort.read(2)       # lit 2 octets (tmp de type 'Bytes')
            tmp = unpack('h',tmp)               # convertit le 'Bytes' en l'entier correspondant (tmp devient un tuple de int)
            tmp = tmp[0]                        # extrait l'entier du tuple (tmp devient un int)
            self.success = True                 # on met à jour le flag de réussite
        except:
            tmp = 0                             # en cas d'échec, on renvoie la valeur 0
            self.success = False                # on met à jour le flag de réussite
        return tmp

    def getString(self):
        "Essaye de lire une chaine de caractère émise par le PRC. Abandonne après le ""time out"" défini (1s par défaut)"
        "La chaine doit etre terminee par le caractere '\n' (=10 en code ASCII)"
        tmp = ''
        myStr = str()
        while tmp!='\n':
            try:
                tmp = self.serialPort.read(1)       # lit 2 octets (tmp de type 'Bytes')
                tmp = tmp.decode("utf-8")
                myStr = myStr + tmp     # convertit le 'Bytes' en l'entier correspondant (tmp devient un tuple de int)
                self.success = True                 # on met à jour le flag de réussite
            except:
                #myStr = ''                          # en cas d'échec, on renvoie la valeur 0
                self.success = False                # on met à jour le flag de réussite
        return myStr

    def getFloat(self):
        "essaye de lire un 'float32' émis par le PRC. Abandonne après le ""time out"" défini (1s par défaut)"
        try:
            tmp = self.serialPort.read(4)       # lit 4 octets (tmp de type 'Bytes')
            tmp = unpack('f',tmp)               # convertit le 'Bytes' en le flottant correspondant (tmp devient un tuple de float)
            tmp = tmp[0]                        # extrait le flottant du tuple (tmp devient un float)
            self.success = True                 # on met à jour le flag de réussite
        except:
            tmp = 0                             # en cas d'échec, on renvoie la valeur 0
            self.success = False                # on met à jour le flag de réussite
        return tmp

    def sendInt(self, data):
        "essaye d'envoyer un 'int16' au PRC. Abandonne après le ""time out"" défini (1s par défaut)"
        try:
            tmp = int(data)                     # transforme la donnée en entier (au cas où)
            tmp = pack('h', tmp)                # transforme la donnée en un 'Bytes'
            self.serialPort.write(tmp)          # écrit la donnée sur le port série
            self.success = True                 # met à jour le flag de réussite
        except:
            self.success = False                # on met à jour le flag de réussite
        return self.success
    
    def sendFloat(self, data):
        "essaye d'envoyer un 'float' au PRC. Abandonne après le ""time out"" défini (1s par défaut)"
        try:
            tmp = pack('f', data)               # on transforme la donnée en un 'Bytes'
            self.serialPort.write(tmp)          # on l'écrit sur le port série
            self.success = True                 # on met à jour le flag de réussite
        except:
            self.success = False                # on met à jour le flag de réussite
        return self.success

    def isSuccessful(self):
        "renvoie le flag de réussite de la dernière action"
        return self.success
    
    def isConnected(self):
        "renvoie l'état de la connexion avec le PRC"
        return self.connected
