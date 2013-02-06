import os

from ctypes import *

try:
  if os.name == 'posix':
    library = CDLL("../libclient.so")
  elif os.name == 'nt':
    library = CDLL("../client.dll")
  else:
    raise Exception("Unrecognized OS: "+os.name)
except OSError:
  raise Exception("It looks like you didn't builh libclient. Run 'make' and try again.")

#Networking commands

library.open_server_connection.argtypes = [c_char_p, c_char_p]
library.restype = c_int

library.login.argtypes = [c_int, c_char_p, c_char_p]
library.login.restype = c_int

library.createGame.restype = None
library.createGame.argtypes = []

library.joinGame.restype = None
library.joinGame.argtypes = [c_int]

library.endTurn.restype = None
library.endTurn.argtypes = []

library.getStatus.restype = None
library.getStatus.argtypes = []

library.humanMove.restype = c_int
library.humanMove.argtypes = [c_void_p, c_int, c_int]

library.humanAttack.restype = c_int
library.humanAttack.argtypes = [c_void_p, c_void_p]

library.humanAttackCrate.restype = c_int
library.humanAttackCrate.argtypes = [c_void_p, c_void_p]

library.humanAttackWall.restype = c_int
library.humanAttackWall.argtypes = [c_void_p, c_void_p]

library.humanAttackGround.restype = c_int
library.humanAttackGround.argtypes = [c_void_p, c_int, c_int]

library.humanGrab.restype = c_int
library.humanGrab.argtypes = [c_void_p, c_void_p]

library.humanGive.restype = c_int
library.humanGive.argtypes = [c_void_p, c_void_p, c_int]

library.humanThrowWeapon.restype = c_int
library.humanThrowWeapon.argtypes = [c_void_p, c_int, c_int]

library.humanThrowAmmo.restype = c_int
library.humanThrowAmmo.argtypes = [c_void_p, c_int, c_int, c_int]

library.humanBuild.restype = c_int
library.humanBuild.argtypes = [c_void_p, c_int, c_int]

library.humanCallWeapon.restype = c_int
library.humanCallWeapon.argtypes = [c_void_p, c_int, c_int]

library.humanCallAmmo.restype = c_int
library.humanCallAmmo.argtypes = [c_int, c_int]

library.humanSpawn.restype = c_int
library.humanSpawn.argtypes = [c_void_p, c_int, c_int]

library.zombieMove.restype = c_int
library.zombieMove.argtypes = [c_void_p]

library.zombieAttack.restype = c_int
library.zombieAttack.argtypes = [c_void_p, c_void_p]

library.zombieAttackWall.restype = c_int
library.zombieAttackWall.argtypes = [c_void_p, c_void_p]

library.zombieAttackGround.restype = c_int
library.zombieAttackGround.argtypes = [c_void_p, c_int, c_int]

library.zombieEat.restype = c_int
library.zombieEat.argtypes = [c_void_p, c_void_p]

library.zombieTurn.restype = c_int
library.zombieTurn.argtypes = [c_void_p, c_int]

library.zombieSpawn.restype = c_int
library.zombieSpawn.argtypes = [c_void_p, c_int]

library.getTurnNumber.restype = c_int
library.getTurnNumber.argtypes = []

library.getAirstrikesReady.restype = c_float
library.getAirstrikesReady.argtypes = []

library.getZombiesReady.restype = c_int
library.getZombiesReady.argtypes = []

library.getHumansReady.restype = c_int
library.getHumansReady.argtypes = []

library.getZombieCap.restype = c_int
library.getZombieCap.argtypes = []

library.getWalls.restype = c_void_p
library.getWalls.argtypes = [c_int]

library.getWallCount.restype = c_int
library.getWallCount.argtypes = []

library.getCrates.restype = c_void_p
library.getCrates.argtypes = [c_int]

library.getCrateCount.restype = c_int
library.getCrateCount.argtypes = []

library.getWeapons.restype = c_void_p
library.getWeapons.argtypes = [c_int]

library.getWeaponCount.restype = c_int
library.getWeaponCount.argtypes = []

library.getHumans.restype = c_void_p
library.getHumans.argtypes = [c_int]

library.getHumanCount.restype = c_int
library.getHumanCount.argtypes = []

library.getZombies.restype = c_void_p
library.getZombies.argtypes = [c_int]

library.getZombieCount.restype = c_int
library.getZombieCount.argtypes = []

library.getAirstrikes.restype = c_void_p
library.getAirstrikes.argtypes = [c_int]

library.getAirstrikeCount.restype = c_int
library.getAirstrikeCount.argtypes = []

library.getSpawnzones.restype = c_void_p
library.getSpawnzones.argtypes = [c_int]

library.getSpawnzoneCount.restype = c_int
library.getSpawnzoneCount.argtypes = []

library.isHuman.restype = c_int
library.isHuman.argtypes = []

library.networkLoop.restype = c_int
library.networkLoop.argtypes = [c_int, c_int]

library.wallGetID.restype = c_int
library.wallGetID.argtypes = [c_void_p]

library.wallGetX.restype = c_int
library.wallGetX.argtypes = [c_void_p]

library.wallGetY.restype = c_int
library.wallGetY.argtypes = [c_void_p]

library.wallGetHp.restype = c_int
library.wallGetHp.argtypes = [c_void_p]

library.wallGetMaxHp.restype = c_int
library.wallGetMaxHp.argtypes = [c_void_p]

library.crateGetID.restype = c_int
library.crateGetID.argtypes = [c_void_p]

library.crateGetX.restype = c_int
library.crateGetX.argtypes = [c_void_p]

library.crateGetY.restype = c_int
library.crateGetY.argtypes = [c_void_p]

library.crateGetHp.restype = c_int
library.crateGetHp.argtypes = [c_void_p]

library.crateGetMaxHp.restype = c_int
library.crateGetMaxHp.argtypes = [c_void_p]

library.crateGetContent.restype = c_int
library.crateGetContent.argtypes = [c_void_p]

library.weaponGetID.restype = c_int
library.weaponGetID.argtypes = [c_void_p]

library.weaponGetName.restype = c_char_p
library.weaponGetName.argtypes = [c_void_p]

library.weaponGetRange.restype = c_int
library.weaponGetRange.argtypes = [c_void_p]

library.weaponGetAmmo.restype = c_int
library.weaponGetAmmo.argtypes = [c_void_p]

library.weaponGetDelay.restype = c_float
library.weaponGetDelay.argtypes = [c_void_p]

library.weaponGetDamage.restype = c_int
library.weaponGetDamage.argtypes = [c_void_p]

library.weaponGetVerticalSplashDamage.restype = c_int
library.weaponGetVerticalSplashDamage.argtypes = [c_void_p]

library.weaponGetHorizontalSplashDamage.restype = c_int
library.weaponGetHorizontalSplashDamage.argtypes = [c_void_p]

library.weaponGetHorizontalSplashRadius.restype = c_int
library.weaponGetHorizontalSplashRadius.argtypes = [c_void_p]

library.weaponGetCanSpawnWith.restype = c_int
library.weaponGetCanSpawnWith.argtypes = [c_void_p]

library.weaponGetCanCall.restype = c_int
library.weaponGetCanCall.argtypes = [c_void_p]


library.humanGetID.restype = c_int
library.humanGetID.argtypes = [c_void_p]

library.humanGetX.restype = c_int
library.humanGetX.argtypes = [c_void_p]

library.humanGetY.restype = c_int
library.humanGetY.argtypes = [c_void_p]

library.humanGetHp.restype = c_int
library.humanGetHp.argtypes = [c_void_p]

library.humanGetMaxHp.restype = c_int
library.humanGetMaxHp.argtypes = [c_void_p]

library.humanGetMoves.restype = c_int
library.humanGetMoves.argtypes = [c_void_p]

library.humanGetMaxMoves.restype = c_int
library.humanGetMaxMoves.argtypes = [c_void_p]

library.humanGetWeaponID.restype = c_int
library.humanGetWeaponID.argtypes = [c_void_p]

library.humanGetSightRadius.restype = c_int
library.humanGetSightRadius.argtypes = [c_void_p]

library.humanGetOwnerID.restype = c_int
library.humanGetOwnerID.argtypes = [c_void_p]

library.humanGetIdleTime.restype = c_float
library.humanGetIdleTime.argtypes = [c_void_p]

library.humanGetAmmo.restype = c_int
library.humanGetAmmo.argtypes = [c_void_p]

library.humanGetSpecialty.restype = c_int
library.humanGetSpecialty.argtypes = [c_void_p]

library.zombieGetID.restype = c_int
library.zombieGetID.argtypes = [c_void_p]

library.zombieGetX.restype = c_int
library.zombieGetX.argtypes = [c_void_p]

library.zombieGetY.restype = c_int
library.zombieGetY.argtypes = [c_void_p]

library.zombieGetHp.restype = c_int
library.zombieGetHp.argtypes = [c_void_p]

library.zombieGetMaxHp.restype = c_int
library.zombieGetMaxHp.argtypes = [c_void_p]

library.zombieGetMoves.restype = c_int
library.zombieGetMoves.argtypes = [c_void_p]

library.zombieGetMaxMoves.restype = c_int
library.zombieGetMaxMoves.argtypes = [c_void_p]

library.zombieGetWeaponID.restype = c_int
library.zombieGetWeaponID.argtypes = [c_void_p]

library.zombieGetSightRadius.restype = c_int
library.zombieGetSightRadius.argtypes = [c_void_p]

library.zombieGetOwnerID.restype = c_int
library.zombieGetOwnerID.argtypes = [c_void_p]

library.zombieGetIdleTime.restype = c_float
library.zombieGetIdleTime.argtypes = [c_void_p]

library.zombieGetFacing.restype = c_int
library.zombieGetFacing.argtypes = [c_void_p]

library.zombieGetSmell.restype = c_float
library.zombieGetSmell.argtypes = [c_void_p]

library.airstrikeGetID.restype = c_int
library.airstrikeGetID.argtypes = [c_void_p]

library.airstrikeGetX.restype = c_int
library.airstrikeGetX.argtypes = [c_void_p]

library.airstrikeGetY.restype = c_int
library.airstrikeGetY.argtypes = [c_void_p]

library.airstrikeGetDelay.restype = c_int
library.airstrikeGetDelay.argtypes = [c_void_p]

library.airstrikeGetItemID.restype = c_int
library.airstrikeGetItemID.argtypes = [c_void_p]

library.spawnzoneGetID.restype = c_int
library.spawnzoneGetID.argtypes = [c_void_p]

library.spawnzoneGetX1.restype = c_int
library.spawnzoneGetX1.argtypes = [c_void_p]

library.spawnzoneGetY1.restype = c_int
library.spawnzoneGetY1.argtypes = [c_void_p]

library.spawnzoneGetX2.restype = c_int
library.spawnzoneGetX2.argtypes = [c_void_p]

library.spawnzoneGetY2.restype = c_int
library.spawnzoneGetY2.argtypes = [c_void_p]
