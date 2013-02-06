import os
import time
import sys
import dircache
from stat import *

def trimFolder(folder, logLimit, ext):
    oldest = int(sys.maxint)
    files = dircache.listdir(folder)
    oldestLog = ""
    logCount = 0
    for log in files:
        if (log.endswith(ext) and (not log.startswith("."))):
            logCount += 1
            if (os.stat(folder + "/" + log)[ST_MTIME] < oldest):
                oldest = os.stat(folder + "/" + log)[ST_MTIME]
                oldestLog = log
    
    if (len(oldestLog) > 0 and logCount > logLimit):
        try:
            os.remove(folder + "/" + oldestLog)
            print "Removing " + folder + "/" + oldestLog
        except OSError:
            print folder + "/" + oldestLog + " could not be removed at this time."


folderLimits = [["played", 2, ".gamelog"],
                ["databaase", 3, ""]]
while (True):
    for info in folderLimits:
        trimFolder(info[0], info[1], info[2])
    time.sleep(10) 

