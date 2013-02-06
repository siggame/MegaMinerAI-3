import com.sun.jna.Library;
import com.sun.jna.Pointer;
import com.sun.jna.Native;

public interface Client extends Library {
    Client INSTANCE = (Client)Native.loadLibrary("client", Client.class);
    int open_server_connection(String host, String port);

    boolean login(int socket, String username, String password);
    void createGame();
    void joinGame(int id);

    void endTurn();
    void getStatus();


    //commands
    boolean humanMove(Pointer unit, int x, int y);
    boolean humanAttack(Pointer unit, Pointer target);
    boolean humanAttackCrate(Pointer unit, Pointer target);
    boolean humanAttackWall(Pointer unit, Pointer target);
    boolean humanAttackGround(Pointer unit, int x, int y);
    boolean humanGrab(Pointer unit, Pointer target);
    boolean humanGive(Pointer unit, Pointer target, int quantity);
    boolean humanThrowWeapon(Pointer unit, int x, int y);
    boolean humanThrowAmmo(Pointer unit, int x, int y, int quantity);
    boolean humanBuild(Pointer unit, int x, int y);
    boolean humanCallWeapon(Pointer weapon, int x, int y);
    boolean humanCallAmmo(int x, int y);
    boolean humanSpawn(Pointer weapon, int x, int y);

    boolean zombieMove(Pointer unit);
    boolean zombieAttack(Pointer unit, Pointer target);
    boolean zombieAttackWall(Pointer unit, Pointer target);
    boolean zombieAttackGround(Pointer unit, int x, int y);
    boolean zombieEat(Pointer unit, Pointer target);
    boolean zombieTurn(Pointer unit, int direction);
    boolean zombieSpawn(Pointer zone, int facing);

    //accessors
    int getTurnNumber();
    float getAirstrikesReady();
    int getZombiesReady();
    int getHumansReady();
    int getZombieCap();

    Pointer getWalls(int num);
    int getWallCount();

    Pointer getCrates(int num);
    int getCrateCount();

    Pointer getWeapons(int num);
    int getWeaponCount();

    Pointer getHumans(int num);
    int getHumanCount();

    Pointer getZombies(int num);
    int getZombieCount();

    Pointer getAirstrikes(int num);
    int getAirstrikeCount();

    Pointer getSpawnzones(int num);
    int getSpawnzoneCount();
    boolean isHuman();


    int networkLoop(int socket, boolean practice);

    //getters
    int wallGetID(Pointer ptr);

    int wallGetX(Pointer ptr);

    int wallGetY(Pointer ptr);

    int wallGetHp(Pointer ptr);

    int wallGetMaxHp(Pointer ptr);

    int crateGetID(Pointer ptr);

    int crateGetX(Pointer ptr);

    int crateGetY(Pointer ptr);

    int crateGetHp(Pointer ptr);

    int crateGetMaxHp(Pointer ptr);

    int crateGetContent(Pointer ptr);

    int weaponGetID(Pointer ptr);

    String weaponGetName(Pointer ptr);

    int weaponGetRange(Pointer ptr);

    int weaponGetAmmo(Pointer ptr);

    float weaponGetDelay(Pointer ptr);

    int weaponGetDamage(Pointer ptr);

    int weaponGetVerticalSplashDamage(Pointer ptr);

    int weaponGetHorizontalSplashDamage(Pointer ptr);

    int weaponGetHorizontalSplashRadius(Pointer ptr);

    boolean weaponGetCanSpawnWith(Pointer ptr);

    boolean weaponGetCanCall(Pointer ptr);

    int humanGetID(Pointer ptr);

    int humanGetX(Pointer ptr);

    int humanGetY(Pointer ptr);

    int humanGetHp(Pointer ptr);

    int humanGetMaxHp(Pointer ptr);

    int humanGetMoves(Pointer ptr);

    int humanGetMaxMoves(Pointer ptr);

    int humanGetWeaponID(Pointer ptr);

    int humanGetSightRadius(Pointer ptr);

    int humanGetOwnerID(Pointer ptr);

    float humanGetIdleTime(Pointer ptr);

    int humanGetAmmo(Pointer ptr);

    int humanGetSpecialty(Pointer ptr);

    int zombieGetID(Pointer ptr);

    int zombieGetX(Pointer ptr);

    int zombieGetY(Pointer ptr);

    int zombieGetHp(Pointer ptr);

    int zombieGetMaxHp(Pointer ptr);

    int zombieGetMoves(Pointer ptr);

    int zombieGetMaxMoves(Pointer ptr);

    int zombieGetWeaponID(Pointer ptr);

    int zombieGetSightRadius(Pointer ptr);

    int zombieGetOwnerID(Pointer ptr);

    float zombieGetIdleTime(Pointer ptr);

    int zombieGetFacing(Pointer ptr);

    float zombieGetSmell(Pointer ptr);

    int airstrikeGetID(Pointer ptr);

    int airstrikeGetX(Pointer ptr);

    int airstrikeGetY(Pointer ptr);

    int airstrikeGetDelay(Pointer ptr);

    int airstrikeGetItemID(Pointer ptr);

    int spawnzoneGetID(Pointer ptr);

    int spawnzoneGetX1(Pointer ptr);

    int spawnzoneGetY1(Pointer ptr);

    int spawnzoneGetX2(Pointer ptr);

    int spawnzoneGetY2(Pointer ptr);

    int getMapSize();

    String getHumanTeamName();

    String getZombieTeamName();

    float getHumanTime();

    float getZombieTime();
}
