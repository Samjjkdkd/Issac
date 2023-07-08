/**********  游戏配置数据 **********/
#define GAME_WIDTH  2048  //宽度
#define GAME_HEIGHT 1200  //高度
#define START_WIDTH  2048  //宽度
#define START_HEIGHT 1200  //高度
#define GAME_TITLE "以撒 v4.1.2" //标题
#define GAME_ICON  ":/res/app.ico"
#define GAME_RATE  10
/**********  地图配置数据 **********/
#define MAP_PATH  ":/res/Blue_Womb.png" //地图图片路径
#define START_MAP_PATH ":/res/startbg.jpg"
#define MAP_SCROLL_SPEED 2  //地图滚动速度
#define MAP_RESIZE_WIDTH 2048
#define MAP_RESIZE_HEIGHT 1200
/**********  飞机配置数据 **********/
#define HERO_PATH ":/res/Entity_887.0.0.1.png"
#define RESIZE_RADIUS 128
#define ROTATE_SENSITIVITY 2
#define I_SHOW_SPEED 4.0f
#define I_GOT_SPRINT 8.0f //这个值为绝对值（小速度）
#define I_SHOOT_SPEED 10.0f//子弹射速(个/100帧)
#define I_SPEED_BURST 0.5 //提升比例（大速度）
#define MAX_HEALTH 200.0f
#define SKILL_INTERVAL 200
#define SPRINT_INTERVAL 50
#define BURST_INTERVAL 1000
#define ASHWAB_INTERVAL 2000
#define SKILL_DEGREE 70
#define SKILL_BULLET_NUM 30
#define CHARGE_MAX 20
#define CHARGE2_MAX 20
#define MAX_STAMINA 1000
#define SPRINT_COST 200
#define SPRINT_TIME 5
#define BOOST_TIME 8
#define BURST_TIME 500
#define ASHWAB_TIME 446
/**********  子弹配置数据 **********/
#define BULLET_PATH ":/res/bullet_11.png"   //子弹图片路径
#define SKILL_BULLET_PATH  ":/res/bullet_12.png"
#define BULLET_SPEED 7  //子弹移动速度
#define BULLET_NUM 120   //弹匣中子弹总数
#define BULLET_MAX 1000   //子弹上限
#define BULLET_INTERVAL 8 //发射子弹时间间隔
/**********  敌机配置数据 **********/
#define ENEMY_PATH_1  ":/res/Entity_10.0.0.png"  //敌机资源图片
#define ENEMY_PATH_2  ":/res/Entity_11.0.0.png"  //敌机资源图片
#define ENEMY_PATH_3_1  ":/res/Entity_14.0.1.png"  //敌机资源图片
#define ENEMY_PATH_3_2  ":/res/Entity_14.0.2.png"  //敌机资源图片
#define ENEMY_PATH_4  ":/res/Entity_867.0.0.png"  //敌机资源图片
#define RESIZE_WIDTH_1 128 //敌方统一大小
#define RESIZE_HEIGHT_1 128 //敌方统一大小
#define RESIZE_WIDTH_2 80 //敌方统一大小
#define RESIZE_HEIGHT_2 80 //敌方统一大小
#define RESIZE_WIDTH_3 80 //敌方统一大小
#define RESIZE_HEIGHT_3 80 //敌方统一大小
#define RESIZE_WIDTH_4 300
#define RESIZE_HEIGHT_4 300

#define ENEMY4_TRACK_TRANSPARANT 0.3

//奖励相关
#define ENEMY_ENERGY_1 10.0f
#define ENEMY_ENERGY_2 3.0f
#define ENEMY_ENERGY_3 5.0f
#define ENEMY_SCORE_1 3
#define ENEMY_SCORE_2 1
#define ENEMY_SCORE_3 1

//动图播放
#define ENEMY_3_INTERVAL 7
#define ENEMY_3_MAX 2

#define ENEMY_SPEED_1 1.5  //敌机移动速度
#define ENEMY_SPEED_2 2  //敌机移动速度
#define ENEMY_SPEED_3 2.0  //敌机移动速度
#define ENEMY_SPEED_4 16.0
#define ENEMY_HP_1 3 //敌方血量
#define ENEMY_HP_2 1
#define ENEMY_HP_3 1
#define ENEMY_HP_4 999
#define ENEMY_NUM   50  //敌机总数量
#define ENEMY_MAX   500  //敌机最大数量
#define ENEMY_INTERVAL  5  //敌机出场时间间隔

#define BOMB_PATH  ":/res/blood-%1.png"   //爆炸资源图片
#define BOMB_NUM  30     //爆炸数量
#define BOMB_MAX  5      //爆炸图片最大索引
#define BOMB_INTERVAL 10   //爆炸切图时间间隔

#define BLOOD_PATH_1 ":/res/bloodtrail-1.png" //血迹图片
#define BLOOD_PATH_2 ":/res/bloodtrail-4.png" //血迹图片

#define BLOOD_NUM 500
#define BLOOD_MAX 8
#define BLOOD_INTERVAL 100
#define RESIZE_BLOOD_WIDTH 200
#define RESIZE_BLOOD_HEIGHT 200

#define SOUND_BGM_PATH ":/res/sound_fx/WildFire.wav"
#define START_BGM_PATH ":/res/sound_fx/StarRail.wav"
#define SOUND_BOOM_PATH ":/res/sound_fx/bomb.wav"
#define RESULT_BGM_PATH ":/res/sound_fx/PANIC!.wav"
#define WELCOME_BGM_PATH ":/res/sound_fx/Shooting_Stars.wav"

/***********终结技资源***************/
#define Z_PATH_PREFFIX ":/res/z_effect/z-"
#define Z_SOUND_PATH ":/res/sound_fx/z.wav"
/***********能量相关****************/
#define ENERGY_PATH ":/res/energy.png"
#define ENERGY_PATH_1 ":/res/energy_1.png"
#define ENERGY_LASTING 500
#define ENERGY_BLING 50
#define ENERGY_MAX 50
#define ENERGY_POSSIBILITY 0.10
/***********界面位置相关*************/
#define SKILL_ICON_MARGIN_X 450
#define SKILL_ICON_MARGIN_Y 200
#define SKILL_ICON_SIZE 100
#define BURST_ICON_MARGIN_X 330
#define BURST_ICON_MARGIN_Y 200
#define BURST_ICON_SIZE 100
#define ASHWAB_ICON_MARGIN_X 210
#define ASHWAB_ICON_MARGIN_Y 200
#define ASHWAB_ICON_SIZE 100


#define Pi 3.1415926

