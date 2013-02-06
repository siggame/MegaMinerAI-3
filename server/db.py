#!/usr/bin/python
"""
Database backend
"""
from __future__ import with_statement
import shelve
import threading
import cPickle
import random

DB_NAME = "mstaigamedb"
KEY = 5
class Database():
    """
    probably slow as balls, but easily transportable database representation.
    """

    def __init__(self):
        """create if it isn't there, and open for writing"""
        self.write_lock = threading.Lock()

    def write_game(self, key, game):
        with self.write_lock:
            file('databaase/%s' % (key,), 'w').write(game.statuses)

    def write_more_game(self, key, game):
        with self.write_lock:
            file('databaase/%s' % (key,), 'a').write(game)

    def lookup_game(self, key):
        try:
            return file('databaase/%s' % (key,), 'r').read()
        except:
            raise

    def write_new_game(self, game):
        global KEY
        KEY += random.randint(1,4)
        self.write_game(`KEY`, game)
        return `KEY`

    def load_games(self):
        """
        don't know if this will work
        """
        return self.d

    def close(self):
        self.d.close()


class Game:
    """
    stores game players, all the turn statuses, and the winner.
    we'll store this in the database above. 
    """
    DB = None

    @staticmethod
    def initializeDB():
        if not(Game.DB):
            Game.DB = Database()

    @staticmethod
    def closeDB():
        if Game.DB:
            Game.DB.close()
            Game.DB = None

    def __init__(self, players):
        Game.initializeDB()
        self.players = players #should be a tuple
        self.statuses = ""
        self.id = Game.DB.write_new_game(self)
        print self.id

    def append_status(self, status):
        self.statuses = self.statuses + (status)
        Game.DB.write_more_game(self.id, status)

    def declare_winner(self, player):
        self.winner = player
        Game.DB.write_game(self.id, self)
        #Game.DB.close()

    def __str__(self):
        return str(self.dict)


if __name__ == "__main__":
    Game.initializeDB()
    g = Game(('1', '2'))
    g.append_status("lol")
    g.declare_winner('1')

    print Game.DB.load_games()
    Game.closeDB()
