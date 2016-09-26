import subprocess

class PythonBasePlugin:

    def __init__(self, sendMessage):
	self.sendMessage = sendMessage
	text = '"init"'
        print(text+"\n\n")
        subprocess.call('espeak '+ text, shell=True)
	pass

    def name(self):
        return "pythonbaseplugin"

    def getTitle(self):
        return "Python Base Plugin"

    def getVersion(self):
        return "0.1"

    def getDescription(self):
        return "Plugin running Python code"

    def doWork(self):
	text = '"doWork"'
        #self.sendMessage(text+"\n\n")
        subprocess.call('espeak '+ text, shell=True)
        
    def setup(self):
	text = '"setup"'
        print(text+"\n\n")
        subprocess.call('espeak '+ text, shell=True)

    def communicate(self, message):
        return message;

    def loadConfig(self, json):
	text = json
        print(text+"\n\n")
        subprocess.call('espeak '+ text, shell=True)

    def getConfig(self):
        return "{}"

    def receiveMessage(self, message):
        print(message)
