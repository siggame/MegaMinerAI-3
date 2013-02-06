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
"""
    Stephen Mues : I am 90% sure this is just an old relic from previous
                   games.  I don't think it does anything.
"""

from Server import *
from Filter import *

class EchoFilter(Filter):
    def readOut(self, data):
        print data
        self.writeOut(data)

if __name__ == "__main__":
    server = TCPServer(2100, PacketizerFilter(), CompressionFilter(), EchoFilter())
    server.run()
