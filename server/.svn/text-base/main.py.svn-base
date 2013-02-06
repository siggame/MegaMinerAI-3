# -*- coding: utf-8 -*-
"""

   Copyright (C) 2008 by Steven Wallace
   snwallace@gmail.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the
    Free Software Foundation, Inc.,
    59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 """
from __future__ import with_statement
from networking.Server import *
from networking.Filter import *
from config import getUserInfo

import traceback

import lexpr.sexpr as sexpr
import threading
import sys
import time
import getopt
import threading

LOGIN_CONFIG = 'config/login.cfg'

ID = 1

class LogicFilter(Filter):
    ID = 0
    def _init(self, *args):
        self.user = None
        self.screenName = None
        self.password = None
        self.hash = None
        self.game = None
        self.type = None
        self.hasMap = None
        self.ID = str(LogicFilter.ID)
        LogicFilter.ID += 1
        self.run = True

    def _begin(self):
        if not self.initialized.isSet():
            self.thread = threading.Thread(None, self.logic)
            self.thread.start()
            Filter._begin(self)

    def logic(self):
        #override this if you need logic
        pass

    def writeSExpr(self, list):
        self.writeOut(sexpr.sexpr2str(list))

    def _readOut(self, data):
        try:
            self.readSExpr(sexpr.str2sexpr(data))
        except ValueError:
            self.writeSExpr(['malformed-message', data])

    def disconnect(self):
        self.run = False
        if self.game:
            pass
            #self.readSExpr("(leave-game)")

    def readSExpr(self, expression):
        for i in expression:
            self.evalStatement(i)

    def evalStatement(self, expression):
        #print expression
        if type(expression) != list:
            self.writeSExpr(['invalid-expression', expression])
            return False
        try:
            statements[expression[0]](self, expression)
        except (KeyError, IndexError), e:
            print e
            self.writeSExpr(['malformed-statement', expression])

    def login(self, user, password):
        registered = False
        output = ""
        userInfo = getUserInfo(user, LOGIN_CONFIG)

        if not (userInfo is None):
            if userInfo['password'] == password:
                registered = True
                self.user = user
                self.password = password
                self.screenName = userInfo['screenName']

        if not registered:
            output = "user    :" + user + '\n'
            output += "password:" + password + '\n\n'
            outFile = open("badLogin.dat", "a")
            outFile.write(output)
            outFile.close()

        return registered

    def logout(self):
        self.user = None
        self.password = None
        self.hash = None
        return True

class MasterFilter(LogicFilter):
    Connections = {}

    def _init(self):
        LogicFilter._init(self)
        print self.ID
        MasterFilter.Connections[self.ID] = self

    def logic(self):
        while self.run:
            time.sleep(5)

    def disconnect(self):
        if self.ID:
            del MasterFilter.Connections[self.ID]
            self.ID = None
        LogicFilter.disconnect(self)

class SlaveFilter(LogicFilter):
    def _init(self):
        self.logged_in = False
        LogicFilter._init(self)

    def logic(self):
        self.writeSExpr(['login', 'slave', '12345'])
        while self.run:
            self.writeSExpr(['ping'])
            time.sleep(1)

def runMaster():
    try:
        print "Running master."
        master = TCPServer(19000,  PacketizerFilter, CompressionFilter, MasterFilter)
        master.run()
    except Exception, exception:
        print "runMaster - Unexpected error:", exception
        sys.exit(1)
    sys.exit(0)

def runSlave(master = ("127.0.0.1", 19000)):
    try:
        print "Running slave."
        slave = SlaveTCPServer(master, 19001,  PacketizerFilter, CompressionFilter, SlaveFilter)
        slave.run()
    except Exception, exception:
        print "runSlave - Unexpected error:", exception
        sys.exit(1)

def main():
    opts = None
    #Todo: Implement a config file.
    try:
        opts = getopt.getopt(sys.argv[1:], "m")[0]
    except getopt.GetoptError, err:
        print str(err)
        sys.exit(2)
    try:
        runMaster() if opts else runSlave()
    except KeyboardInterrupt, e:
        print ""
        sys.exit()

if __name__ == "__main__":
    from lexpr.Statements import statements
    from lexpr.ServerStatements import statements as ss
    statements.update(ss)
    main()

