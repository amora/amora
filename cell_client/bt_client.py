import socket
import appuifw
import e32

class bt_client:
    def connect(self):
        self.sock = socket.socket(socket.AF_BT, socket.SOCK_STREAM)
        addr,services = socket.bt_discover()
        print "Discovered: %s, %s"%(addr,services)
        if len(services) > 0:
            choices = services.keys()
            choices.sort()
            choice = appuifw.popup_menu([unicode(services[x]) + ": " + x
                                       for x in choices], u'Choose port:')
            port = services[choices[choice]]
        else:
            port = services[services.keys()[0]]
        address = (addr,port)
        print "Connecting to " + str(address) + "...",
        self.sock.connect(address)
        print "OK."
#     def readline(self):
#         line=[]
#         self.sock.recv(100)
#         line.append(ch)
#         while 1:
#             ch=self.sock.recv(100)
#             if(ch=='\n'):
#                 break
#             line.append(ch)
#        return ''.join(line)
    def write_line(self, command):
        self.sock.send(command + '\n');
    def close(self):
        self.sock.close()



