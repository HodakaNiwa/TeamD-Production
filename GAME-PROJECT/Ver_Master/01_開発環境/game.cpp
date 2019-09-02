//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "map.h"
#include "modelcreate.h"
#include "camera.h"
#include "cameraManager.h"
#include "textureManager.h"
#include "fileLoader.h"
#include "fileSaver.h"
#include "functionlib.h"
#include "input.h"
#include "fade.h"
#include "UI.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "respawn.h"
#include "item.h"
#include "itemCylinder.h"
#include "river.h"
#include "boxCollider.h"
#include "scene2D.h"
#include "number.h"
#include "server.h"
#include "sound.h"
#include "charaselect.h"
#include "effectManager.h"
#include "characterManager.h"
#include "playerManager.h"
#include "blossoms.h"
#include "hinaarare.h"
#include "mist.h"
#include "title.h"
#include "result.h"
#include "debugproc.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define GAME_DEBUG      // 宣言時デバッグコマンド適用
#define GAME_MAPEVENT   // 宣言時マップイベント適用

// マップ更新時に消されるオブジェクト(判定用のマクロなのでここに追加)
#define GAME_DELETE_OBJECT (pScene->GetObjType() == CScene::OBJTYPE_ITEM || pScene->GetObjType() == CScene::OBJTYPE_BULLET || pScene->GetObjType() == CScene::OBJTYPE_PARTICLE || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER || pScene->GetObjType() == CScene::OBJTYPE_EMITTER || pScene->GetObjType() == CScene::OBJTYPE_BLOSSOMS || pScene->GetObjType() == CScene::OBJTYPE_ITEMCYLINDER)

#ifdef GAME_DEBUG
#define GAME_DEBUG_STARTMAP                          (CGame::MAPTYPE_HINAMATSURI)
#endif

#define GAME_SYSTEM_FILENAME                         "data/TEXT/MODE/game.ini"    // 初期化に使用するシステムファイル名
#define GAME_STAGEDISP_TIME                          (120)                        // ステージ番号表示状態になってから通常状態に切り替わるまでの時間
#define GAME_RESULT_TIMING                           (60)                         // 終了状態になってからリザルト状態に切り替わるまでの時間
#define GAME_NEXTMODE_TIMING                         (180)                        // リザルト状態になってから次のモードに切り替わるまでの時間
#define GAME_NEXTSCORE_TIMING                        (30)                         // ゲーム内リザルトで次のスコアを出すまでの時間
#define GAME_SCOREUP_TIMING                          (10)                         // ゲーム内リザルトでスコアアップさせるまでの時間
#define GAME_GAMEOVER_TIMING                         (180)                        // ゲームオーバー状態になってから次のモードに切り替わるまでの時間
#define GAME_GAMEOVER_POSUP                          (8.0f)                       // ゲームオーバーロゴをどれくらい上げるか
#define GAME_GAMEOVER_POSYMAX                        (10.0f)                      // ゲームオーバーポリゴンのY座標最大値(ポリゴンの高さに加えて判定)
#define GAME_NUMBER_INTERVAL_CUT                     (0.8f)                       // 数字の間隔の倍率(ポリゴンの幅をこれで乗算する)
#define GAME_PLAYER_STOCK_INI                        (3)                          // プレイヤーの残機数初期値
#define GAME_PLAYER_RESPAWN_TIMING                   (180)                        // プレイヤーがリスポーンするまでの時間
#define GAME_MAPSTAGE_MAX                            (3)                          // ステージの種類ごとの数
#define GAME_ITEM_SCORE                              (500)                        // アイテムのスコアの量
#define GAME_BONUS_SCORE                             (1000)                       // ボーナススコアの量
#define GAME_STAGEPOLYGON_PRIORITY                   (8)                          // ステージポリゴンの描画優先順位
#define GAME_PLAYER_SPAWN_EFFECT_IDX                 (11)                         // プレイヤーがリスポーンするときのエフェクト
#define GAME_MAPEVENT_RANDOM                         (600)                        // マップイベントを起こすランダム評価値
#define GAME_SE_FANFARE_IDX                          (5)                          // ゲームでステージが始まるときの音番号
#define GAME_SE_PLAYER_RESPAWN_IDX                   (17)                         // プレイヤーがリスポーンした時の音番号
#define GAME_SE_PAUSE_OPEN_IDX                       (11)                         // ポーズ画面を開いたときの音番号
#define GAME_SE_GAMERESULT_SCOREUP_IDX               (13)                         // ゲームリザルトでスコアを加算したときの音番号
#define GAME_SE_BONUS_IDX                            (14)                         // ボーナスを付与した時の音番号


// アイテムイベント用マクロ
#define GAME_ITEMEVENT_GRENADESHAKE_TIME             (10)                         // グレネードを取った時にカメラをどれくらい揺らし続けるか
#define GAME_ITEMEVENT_GRENADESHAKE_VALUE            (40.0f)                      // グレネードを取った時にカメラをどれくらい揺らすか
#define GAME_ITEMEVENT_GRENADESHAKE_CUT              (3.0f)                       // カメラの揺れをどれくらい減らしていくか
#define GAME_ITEMEVENT_ENEMYSTOPTIME                 (300)                        // 敵をどれくらい止めておくか
#define GAME_ITEMEVENT_SE_STAR                       (19)                         // スターアイテムイベントの音番号
#define GAME_ITEMEVENT_SE_GRENADE                    (20)                         // グレネードアイテムイベントの音番号
#define GAME_ITEMEVENT_SE_1UP                        (21)                         // 1UPアイテムイベントの音番号
#define GAME_ITEMEVENT_SE_SCOOP                      (22)                         // スコップアイテムイベントの音番号
#define GAME_ITEMEVENT_SE_CLOCK                      (23)                         // 時計アイテムイベントの音番号
#define GAME_ITEMEVENT_SE_HELMET                     (24)                         // ヘルメットアイテムイベントの音番号

// ステージ背景ポリゴン初期化用
#define GAME_STAGEBG_POS_INI                         (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_STAGEBG_COL_INI                         (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))
#define GAME_STAGEBG_WIDTH_INI                       (SCREEN_WIDTH / 2.0f)
#define GAME_STAGEBG_HEIGHT_INI                      (SCREEN_HEIGHT / 2.0f)

// ステージロゴポリゴン初期化用
#define GAME_STAGELOGO_POS_INI                       (D3DXVECTOR3(560.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_STAGELOGO_COL_INI                       (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))
#define GAME_STAGELOGO_WIDTH_INI                     (170.0f)
#define GAME_STAGELOGO_HEIGHT_INI                    (60.0f)

// ステージ番号表示用ポリゴン初期化用
#define GAME_STAGENUMBER_POS_INI                     (D3DXVECTOR3(830.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_STAGENUMBER_COL_INI                     (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_STAGENUMBER_WIDTH_INI                   (40.0f)
#define GAME_STAGENUMBER_HEIGHT_INI                  (40.0f)

// ゲームオーバーロゴポリゴン初期化用
#define GAME_GAMEOVERLOGO_POS_INI                    (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 900.0f, 0.0f))
#define GAME_GAMEOVERLOGO_COL_INI                    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMEOVERLOGO_WIDTH_INI                  (80.0f)
#define GAME_GAMEOVERLOGO_HEIGHT_INI                 (80.0f)
#define GAME_GAMEOVERLOGO_PRIORITY                   (8)

// マップイベント用
#define GAME_MAPEVENT_MAX                            (3)
#define GAME_MAPEVENT_STARTTIME                      (200)
#define GAME_MAPEVENT_SOUNDIDX                       (25)
#define GAME_MAPEVENT_STARTLOGO_WIDTH                (300.0f)
#define GAME_MAPEVENT_STARTLOGO_HEIGHT               (90.0f)
#define GAME_MAPEVENT_STARTLOGO_POS                  (D3DXVECTOR3(SCREEN_WIDTH + 100.0f, GAME_MAPEVENT_STARTLOGO_HEIGHT - 30.0f, 0.0f))
#define GAME_MAPEVENT_STARTLOGO_COL                  (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_MAPEVENT_STARTLOGO_PRIORITY             (6)
#define GAME_MAPEVENT_STARTLOGO_TEXIDX               (29)
#define GAME_MAPEVENT_STARTLOGO_TEXU_SPLIT           (2)
#define GAME_MAPEVENT_STARTLOGO_TEXV_SPLIT           (3)
#define GAME_MAPEVENT_STARTLOGO_MOVE                 (-9.0f)

// 桜の花びらデータ初期化用
#define GAME_BLOSSOMS_POS_INI                        (D3DXVECTOR3(SCREEN_WIDTH - UI_BG_WIDTH_INI + 100.0f, -100.0f, 0.0f))
#define GAME_BLOSSOMS_COL_INI                        (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_BLOSSOMS_TIME_INI                       (300)
#define GAME_BLOSSOMS_LAP_INI                        (3)
#define GAME_BLOSSOMS_MOVEX_MAX_INI                  (15)
#define GAME_BLOSSOMS_MOVEX_MIN_INI                  (5)
#define GAME_BLOSSOMS_MOVEY_MAX_INI                  (10)
#define GAME_BLOSSOMS_MOVEY_MIN_INI                  (2)
#define GAME_BLOSSOMS_WIDTH_MAX_INI                  (20)
#define GAME_BLOSSOMS_WIDTH_MIN_INI                  (10)
#define GAME_BLOSSOMS_HEIGHT_MAX_INI                 (20)
#define GAME_BLOSSOMS_HEIGHT_MIN_INI                 (10)
#define GAME_BLOSSOMS_ANGLESPEED_MAX_INI             (5)
#define GAME_BLOSSOMS_ANGLESPEED_MIN_INI             (0)
#define GAME_BLOSSOMS_ROTSPEED_MAX_INI               (5)
#define GAME_BLOSSOMS_ROTSPEED_MIN_INI               (0)
#define GAME_BLOSSOMS_BGMIDX                         (28)

// アイテムドロップイベント用
#define GAME_DROPITEM_POS_Y                          (1200.0f)
#define GAME_DROPITEM_CYLINDER_MOVE_Y                (-14.0f)
#define GAME_DROPITEM_ITEM_MOVE_Y                    (-10.0f)
#define GAME_DROPITEM_CYLINDER_COL                   (D3DXCOLOR(0.3f,0.3f,1.0f,1.0f))
#define GAME_DROPITEM_CYLINDER_HEIGHT                (3600.0f)
#define GAME_DROPITEM_CYLINDER_RADIUS                (50.0f)
#define GAME_DROPITEM_CYLINDER_XBLOCK                (20)
#define GAME_DROPITEM_CYLINDER_YBLOCK                (1)
#define GAME_DROPITEM_CYLINDER_PRIORITY              (7)
#define GAME_DROPITEM_EFFECT_IDX                     (14)
#define GAME_DROPITEM_EFFECT_APPEAR                  (7)
#define GAME_DROPITEM_CYLINDER_SE_IDX                (29)
#define GAME_DROPITEM_ITEM_SE_IDX                    (30)

// ひなあられデータ初期化用
#define GAME_HINAARARE_POSY                          (800.0f)
#define GAME_HINAARARE_MODELIDX                      (0)
#define GAME_HINAARARE_TIME                          (300)
#define GAME_HINAARARE_APPEAR                        (60)
#define GAME_HINAARARE_CAMERA_SHAKE                  (3.0f)
#define GAME_HINAARARE_MODEL_NUM                     (4)
#define GAME_HINAARARE_BGMIDX                        (31)

// ゲーム内リザルト用背景ポリゴン初期化用
#define GAME_GAMERESULT_BG_POS_INI                   (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_GAMERESULT_BG_COL_INI                   (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))
#define GAME_GAMERESULT_BG_WIDTH_INI                 (SCREEN_WIDTH / 2.0f)
#define GAME_GAMERESULT_BG_HEIGHT_INI                (SCREEN_HEIGHT / 2.0f)
#define GAME_GAMERESULT_BG_PRIORITY                  (8)

// ゲーム内リザルト用ハイスコアロゴポリゴン初期化用
#define GAME_GAMERESULT_HIGHSCORELOGO_POS_INI        (D3DXVECTOR3(500.0f, 25.0f, 0.0f))
#define GAME_GAMERESULT_HIGHSCORELOGO_COL_INI        (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define GAME_GAMERESULT_HIGHSCORELOGO_WIDTH_INI      (100.0f)
#define GAME_GAMERESULT_HIGHSCORELOGO_HEIGHT_INI     (25.0f)
#define GAME_GAMERESULT_HIGHSCORELOGO_PRIORITY       (8)
#define GAME_GAMERESULT_HIGHSCORELOGO_TEXIDX         (30)

// ゲーム内リザルト用ハイスコア数字ポリゴン初期化用
#define GAME_GAMERESULT_HIGHSCORENUMBER_POS_INI      (D3DXVECTOR3(830.0f, 25.0f, 0.0f))
#define GAME_GAMERESULT_HIGHSCORENUMBER_COL_INI      (D3DXCOLOR(1.0f,0.73f,0.31f,1.0f))
#define GAME_GAMERESULT_HIGHSCORENUMBER_WIDTH_INI    (15.0f)
#define GAME_GAMERESULT_HIGHSCORENUMBER_HEIGHT_INI   (15.0f)
#define GAME_GAMERESULT_HIGHSCORENUMBER_INTERVAL_INI (D3DXVECTOR3(-GAME_GAMERESULT_HIGHSCORENUMBER_WIDTH_INI - 14.0f, 0.0f, 0.0f))
#define GAME_GAMERESULT_HIGHSCORENUMBER_TEXIDX       (28)
#define GAME_GAMERESULT_HIGHSCORENUMBER_PRIORITY     (8)

// ゲーム内リザルト用ステージロゴポリゴン初期化用
#define GAME_GAMERESULT_STAGELOGO_POS_INI            (D3DXVECTOR3(620.0f, 85.0f, 0.0f))
#define GAME_GAMERESULT_STAGELOGO_COL_INI            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_STAGELOGO_WIDTH_INI          (100.0f)
#define GAME_GAMERESULT_STAGELOGO_HEIGHT_INI         (25.0f)
#define GAME_GAMERESULT_STAGELOGO_PRIORITY           (8)
#define GAME_GAMERESULT_STAGELOGO_TEXIDX             (1)

// ゲーム内リザルト用ステージ番号ポリゴン初期化用
#define GAME_GAMERESULT_STAGENUMBER_POS_INI          (D3DXVECTOR3(740.0f, 85.0f, 0.0f))
#define GAME_GAMERESULT_STAGENUMBER_COL_INI          (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_STAGENUMBER_WIDTH_INI        (20.0f)
#define GAME_GAMERESULT_STAGENUMBER_HEIGHT_INI       (20.0f)
#define GAME_GAMERESULT_STAGENUMBER_PRIORITY         (8)

// ゲーム内リザルト用プレイヤーロゴポリゴン初期化用
#define GAME_GAMERESULT_PLAYERLOGO_POS_INI           (D3DXVECTOR3(SCREEN_WIDTH / 4.0f, 135.0f, 0.0f))
#define GAME_GAMERESULT_PLAYERLOGO_COL_INI           (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define GAME_GAMERESULT_PLAYERLOGO_WIDTH_INI         (120.0f)
#define GAME_GAMERESULT_PLAYERLOGO_HEIGHT_INI        (17.0f)
#define GAME_GAMERESULT_PLAYERLOGO_PRIORITY          (8)
#define GAME_GAMERESULT_PLAYERLOGO_TEXIDX            (13)

// ゲーム内リザルト用プレイヤースコアポリゴン初期化用
#define GAME_GAMERESULT_PLAYERNUMBER_POS_INI         (D3DXVECTOR3(380.0f, 175.0f, 0.0f))
#define GAME_GAMERESULT_PLAYERNUMBER_COL_INI         (D3DXCOLOR(1.0f,0.73f,0.31f,1.0f))
#define GAME_GAMERESULT_PLAYERNUMBER_WIDTH_INI       (15.0f)
#define GAME_GAMERESULT_PLAYERNUMBER_HEIGHT_INI      (15.0f)
#define GAME_GAMERESULT_PLAYERNUMBER_INTERVAL_INI    (D3DXVECTOR3(-GAME_GAMERESULT_PLAYERNUMBER_WIDTH_INI - 14.0f, 0.0f, 0.0f))
#define GAME_GAMERESULT_PLAYERNUMBER_PRIORITY        (8)

// ゲーム内リザルト用敵矢印ポリゴン初期化用
#define GAME_GAMERESULT_ENEMY_INTERVAL_INI           (80.0f)
#define GAME_GAMERESULT_ENEMYARROW_POS_INI           (D3DXVECTOR3(SCREEN_WIDTH / 2.0f - 50.0f, 250.0f, 0.0f))
#define GAME_GAMERESULT_ENEMYARROW_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_ENEMYARROW_WIDTH_INI         (15.0f)
#define GAME_GAMERESULT_ENEMYARROW_HEIGHT_INI        (15.0f)
#define GAME_GAMERESULT_ENEMYARROW_INTERVAL_INI      (D3DXVECTOR3(50.0f, GAME_GAMERESULT_ENEMY_INTERVAL_INI, 0.0f))
#define GAME_GAMERESULT_ENEMYARROW_TEXIDX            (9)
#define GAME_GAMERESULT_ENEMYARROW_PRIORITY          (8)

// ゲーム内リザルト用敵ポイントロゴポリゴン初期化用
#define GAME_GAMERESULT_ENEMYPOINTLOGO_POS_INI       (D3DXVECTOR3(430.0f, 250.0f, 0.0f))
#define GAME_GAMERESULT_ENEMYPOINTLOGO_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_ENEMYPOINTLOGO_WIDTH_INI     (30.0f)
#define GAME_GAMERESULT_ENEMYPOINTLOGO_HEIGHT_INI    (20.0f)
#define GAME_GAMERESULT_ENEMYPOINTLOGO_INTERVAL_INI  (D3DXVECTOR3(210.0f, GAME_GAMERESULT_ENEMY_INTERVAL_INI, 0.0f))
#define GAME_GAMERESULT_ENEMYPOINTLOGO_TEXIDX        (11)
#define GAME_GAMERESULT_ENEMYPOINTLOGO_PRIORITY      (8)

// ゲーム内リザルト用敵アイコンポリゴン初期化用
#define GAME_GAMERESULT_ENEMYICON_POS_INI            (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 250.0f, 0.0f))
#define GAME_GAMERESULT_ENEMYICON_COL_INI            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_ENEMYICON_WIDTH_INI          (20.0f)
#define GAME_GAMERESULT_ENEMYICON_HEIGHT_INI         (20.0f)
#define GAME_GAMERESULT_ENEMYICON_INTERVAL_INI       (D3DXVECTOR3(0.0f, GAME_GAMERESULT_ENEMY_INTERVAL_INI, 0.0f))
#define GAME_GAMERESULT_ENEMYICON_PRIORITY           (8)
#define GAME_GAMERESULT_ENEMYICON_TEXIDX             (15)

// ゲーム内リザルト用敵スコアポリゴンポリゴン初期化用
#define GAME_GAMERESULT_ENEMYSCORE_POS_INI           (D3DXVECTOR3(330.0f, 250.0f, 0.0f))
#define GAME_GAMERESULT_ENEMYSCORE_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_ENEMYSCORE_WIDTH_INI         (15.0f)
#define GAME_GAMERESULT_ENEMYSCORE_HEIGHT_INI        (15.0f)
#define GAME_GAMERESULT_ENEMYSCORE_INTERVAL_INI      (D3DXVECTOR3(-30.0f, GAME_GAMERESULT_ENEMY_INTERVAL_INI, 0.0f))
#define GAME_GAMERESULT_ENEMYSCORE_PRIORITY          (8)

// ゲーム内リザルト用敵撃破数ポリゴンポリゴン初期化用
#define GAME_GAMERESULT_ENEMYBREAK_POS_INI           (D3DXVECTOR3(540.0f, 250.0f, 0.0f))
#define GAME_GAMERESULT_ENEMYBREAK_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_ENEMYBREAK_WIDTH_INI         (15.0f)
#define GAME_GAMERESULT_ENEMYBREAK_HEIGHT_INI        (15.0f)
#define GAME_GAMERESULT_ENEMYBREAK_INTERVAL_INI      (D3DXVECTOR3(-30.0f, GAME_GAMERESULT_ENEMY_INTERVAL_INI, 0.0f))
#define GAME_GAMERESULT_ENEMYBREAK_PRIORITY          (8)

// ゲーム内リザルト用トータルロゴポリゴン初期化用
#define GAME_GAMERESULT_TOTALLOGO_POS_INI            (D3DXVECTOR3(370.0f, 580.0f, 0.0f))
#define GAME_GAMERESULT_TOTALLOGO_COL_INI            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_TOTALLOGO_WIDTH_INI          (80.0f)
#define GAME_GAMERESULT_TOTALLOGO_HEIGHT_INI         (15.0f)
#define GAME_GAMERESULT_TOTALLOGO_PRIORITY           (8)
#define GAME_GAMERESULT_TOTALLOGO_TEXIDX             (12)

// ゲーム内リザルト用トータルラインポリゴン初期化用
#define GAME_GAMERESULT_TOTALLINE_POS_INI            (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 545.0f, 0.0f))
#define GAME_GAMERESULT_TOTALLINE_COL_INI            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_TOTALLINE_WIDTH_INI          (180.0f)
#define GAME_GAMERESULT_TOTALLINE_HEIGHT_INI         (3.2f)
#define GAME_GAMERESULT_TOTALLINE_PRIORITY           (8)

// ゲーム内リザルト用トータルスコアポリゴン初期化用
#define GAME_GAMERESULT_TOTALSCORE_POS_INI           (D3DXVECTOR3(540.0f, 580.0f, 0.0f))
#define GAME_GAMERESULT_TOTALSCORE_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_TOTALSCORE_WIDTH_INI         (15.0f)
#define GAME_GAMERESULT_TOTALSCORE_HEIGHT_INI        (15.0f)
#define GAME_GAMERESULT_TOTALSCORE_INTERVAL_INI      (D3DXVECTOR3(-30.0f, 0.0f, 0.0f))
#define GAME_GAMERESULT_TOTALSCORE_PRIORITY          (8)

// ゲーム内リザルト用ボーナススコアロゴポリゴン初期化用
#define GAME_GAMERESULT_BONUS_ADDPOS                 (SCREEN_WIDTH / 2.0f)
#define GAME_GAMERESULT_BONUSSCORELOGO_POS_INI       (D3DXVECTOR3(240.0f, 630.0f, 0.0f))
#define GAME_GAMERESULT_BONUSSCORELOGO_COL_INI       (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define GAME_GAMERESULT_BONUSSCORELOGO_WIDTH_INI     (120.0f)
#define GAME_GAMERESULT_BONUSSCORELOGO_HEIGHT_INI    (17.0f)
#define GAME_GAMERESULT_BONUSSCORELOGO_TEXIDX        (25)
#define GAME_GAMERESULT_BONUSSCORELOGO_PRIORITY      (8)

// ゲーム内リザルト用ボーナスポイントロゴポリゴン初期化用
#define GAME_GAMERESULT_BONUSPOINTLOGO_POS_INI       (D3DXVECTOR3(430.0f, 680.0f, 0.0f))
#define GAME_GAMERESULT_BONUSPOINTLOGO_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_BONUSPOINTLOGO_WIDTH_INI     (30.0f)
#define GAME_GAMERESULT_BONUSPOINTLOGO_HEIGHT_INI    (20.0f)
#define GAME_GAMERESULT_BONUSPOINTLOGO_TEXIDX        (11)
#define GAME_GAMERESULT_BONUSPOINTLOGO_PRIORITY      (8)

// ゲーム内リザルト用ボーナススコアロゴポリゴン初期化用
#define GAME_GAMERESULT_BONUSSCORE_POS_INI           (D3DXVECTOR3(320.0f, 680.0f, 0.0f))
#define GAME_GAMERESULT_BONUSSCORE_COL_INI           (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_GAMERESULT_BONUSSCORE_WIDTH_INI         (15.0f)
#define GAME_GAMERESULT_BONUSSCORE_HEIGHT_INI        (15.0f)
#define GAME_GAMERESULT_BONUSSCORE_INTERVAL_INI      (D3DXVECTOR3(-30.0f, 0.0f, 0.0f))
#define GAME_GAMERESULT_BONUSSCORE_PRIORITY          (8)

// ポーズしたプレイヤー番号表示用ポリゴン初期化用
#define GAME_PAUSE_PLAYERNUMBER_POS_INI              (D3DXVECTOR3(120.0f, 60.0f, 0.0f))
#define GAME_PAUSE_PLAYERNUMBER_COL_INI              (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_PAUSE_PLAYERNUMBER_WIDTH_INI            (120.0f)
#define GAME_PAUSE_PLAYERNUMBER_HEIGHT_INI           (60.0f)
#define GAME_PAUSE_PLAYERNUMBER_PRIORITY             (8)
#define GAME_PAUSE_PLAYERNUMBER_TEXIDX               (26)

// ポーズしてますポリゴン初期化用
#define GAME_PAUSE_NOT_POS_INI                       (D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_PAUSE_NOT_COL_INI                       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_PAUSE_NOT_WIDTH_INI                     (120.0f)
#define GAME_PAUSE_NOT_HEIGHT_INI                    (60.0f)
#define GAME_PAUSE_NOT_PRIORITY                      (8)
#define GAME_PAUSE_NOT_TEXIDX                        (28)

// ポーズしてます黒背景ポリゴン初期化用
#define GAME_PAUSE_NOT_BLACKBG_COL_INI               (D3DXCOLOR(0.0f,0.0f,0.0f,0.5f))
#define GAME_PAUSE_NOT_BLACKBG_PRIORITY              (8)


// クリーム靄ポリゴン初期化用
#define GAME_CREAMMIST_POS_INI                       (D3DXVECTOR3(SCREEN_WIDTH / 2.0f - UI_BG_WIDTH_INI, SCREEN_HEIGHT / 2.0f, 0.0f))
#define GAME_CREAMMIST_COL_INI                       (D3DXCOLOR(1.0f,1.0f,1.0f,0.3f))
#define GAME_CREAMMIST_WIDTH_INI                     (SCREEN_WIDTH / 2.0f)
#define GAME_CREAMMIST_HEIGHT_INI                    (SCREEN_HEIGHT / 2.0f)
#define GAME_CREAMMIST_ALPHA_UP                      (0.3f)
#define GAME_CREAMMIST_ALPHA_CUT                     (0.02f)
#define GAME_CREAMMIST_ALPHA_MAX                     (0.8f)
#define GAME_CREAMMIST_APPAER_TIME                   (360)
#define GAME_CREAMMIST_PRIORITY                      (5)
#define GAME_CREAMMIST_TEXIDX                        (31)

// 値読み込み用のパス
// テクスチャ用
#define NUM_TEXTURE "NUM_TEXTURE = "
#define TEXTURE_FILENAME "TEXTURE_FILENAME = "

// モデル用
#define NUM_MODEL "NUM_MODEL = "
#define MODEL_FILENAME "MODEL_FILENAME = "

// マップ用
#define NUM_MAP "NUM_MAP = "

// アイテムイベント用
#define ITEMEVENT_FILENAME "ITEMEVENT_FILENAME = "
#define SHAKESET "SHAKESET"
#define END_SHAKESET "END_SHAKESET"
#define TIME "TIME = "
#define VALUE "VALUE = "
#define CUT "CUT = "
#define ENEMY_STOP "ENEMY_STOP = "

// エフェクト用
#define EFFECT_FILENAME "EFFECT_FILENAME = "

// 弾用
#define BULLET_MODEL_IDX "BULLET_MODEL_IDX = "

// アイテム用
#define ITEM_MODEL_IDX "ITEM_MODEL_IDX = "

// プレイヤーデータ用
#define PLAYERDATA_FILENAME "PLAYERDATA_FILENAME = "
#define PLAYER_STOCK_INI "PLAYER_STOCK_INI = "
#define PLAYER_STOCK_MAX "PLAYER_STOCK_MAX = "

// 敵データ用
#define ENEMY_FILENAME "ENEMY_FILENAME = "

// 背景ポリゴン用
#define BGPOLYGONSET "BGPOLYGONSET"
#define END_BGPOLYGONSET "END_BGPOLYGONSET"
#define TEX_IDX "TEX_IDX = "
#define POS "POS = "
#define COL "COL = "
#define WIDTH "WIDTH = "
#define HEIGHT "HEIGHT = "

// 数字テクスチャ用
#define NUMBER_TEX_IDX "NUMBER_TEX_IDX = "

// ステージ背景ポリゴン用
#define STAGEBGSET "STAGEBGSET"
#define END_STAGEBGSET "END_STAGEBGSET"

// ステージロゴポリゴン用
#define STAGELOGOSET "STAGELOGOSET"
#define END_STAGELOGOSET "END_STAGELOGOSET"

// ステージ番号ポリゴン用
#define STAGENUMBERSET "STAGENUMBERSET"
#define END_STAGENUMBERSET "END_STAGENUMBERSET"

// ゲームオーバーロゴポリゴン用
#define GAMEOVERPOLYGONSET "GAMEOVERPOLYGONSET"
#define END_GAMEOVERPOLYGONSET "END_GAMEOVERPOLYGONSET"

// ひな祭りイベント用
#define HINAMATSURI_FILENAME "HINAMATSURI_FILENAME = "
#define EVENT_RANDOM "EVENT_RANDOM = "

// 桜の花びらデータ用
#define CHERRYBLOSSOMSSET "CHERRYBLOSSOMSSET"
#define END_CHERRYBLOSSOMSSET "END_CHERRYBLOSSOMSSET"
#define TIME "TIME = "
#define LAP "LAP = "
#define MOVEX "MOVEX = "
#define MOVEY "MOVEY = "
#define ANGLESPEED "ANGLESPEED = "
#define ROTSPEED "ROTSPEED = "

// ひなあられデータ用
#define HINAARARESET "HINAARARESET"
#define END_HINAARARESET "END_HINAARARESET"
#define MODEL_IDX "MODEL_IDX = "
#define APPEAR "APPEAR = "

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// ゲームのコンストラクタ
//=============================================================================
CGame::CGame()
{

}

//=============================================================================
// ゲームのデストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ゲームの生成処理
//=============================================================================
CGame *CGame::Create()
{
	CGame *pGame = NULL;
	if (pGame == NULL)
	{
		pGame = new CGame;
		if (pGame != NULL)
		{
			pGame->Init();
		}
	}
	return pGame;
}

//=============================================================================
// ゲームの初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
	// 変数を初期化する
	ClearVariable();

	// マップの番号を設定する
#ifdef GAME_DEBUG
	CCharaSelect::SetStageType(GAME_DEBUG_STARTMAP);
#endif
	m_nMapIdx = CCharaSelect::GetStageType() * GAME_MAPSTAGE_MAX;

	// システムの初期化
	LoadSystem();

	// 残機数を設定
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_nPlayerStock[nCntPlayer] = m_nPlayerStockIni;
	}

	// カメラの生成
	CreateCamera();

	// UIを生成
	CreateUI();

	// プレイヤーの生成
	CreatePlayer();

	// ステージ番号の桁数を計算する
	CalcStageDigits();

	// マップの生成
	CreateMap();

	// ファンファーレを再生
	CManager::GetSound()->PlaySound(GAME_SE_FANFARE_IDX);

	// ローカル1Pプレイならばもう一人のプレイヤーを消しておく
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{
		if (m_pPlayer[MAX_NUM_PLAYER - 1] != NULL)
		{
			m_pPlayer[MAX_NUM_PLAYER - 1]->Uninit();
			m_pPlayer[MAX_NUM_PLAYER - 1] = NULL;
		}
	}

	return S_OK;
}

//=============================================================================
// ゲームの終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// 各種ポインタの開放
	ReleaseUI();
	ReleasePlayer();
	ReleasePlayerManager();
	ReleaseEnemyManager();
	ReleaseGameOverLogo();
	ReleaseStageBg();
	ReleaseStageLogo();
	ReleaseStageNumber();
	ReleaseGameResult();
	ReleasePause();
	ReleaseNotPause();

	// 読み込むマップのファイル名のポインタを開放
	ReleaseMapFilePointer();

	// 共通の終了処理
	CBasemode::Uninit();

	// 全てのオブジェクト開放
	CScene::ReleaseAll();

	if (m_State != STATE_END_RETRY)
	{// やり直しの終了状態でない
		// クライアントを開放する
		CManager::ReleaseClient();
	}

	// ハイスコアを更新したかどうかチェック
	int nHighScore = CTitle::GetHighScore();
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_nScore[nCntPlayer] > nHighScore)
		{// 獲得スコアがハイスコアを上回った
			CTitle::SetHighScore(m_nScore[nCntPlayer]);
			CResult::SetHighScore();
		}
	}

	// BGMを止める
	CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX);
	StopBGM();
}

//=============================================================================
// ゲームの更新処理
//=============================================================================
void CGame::Update(void)
{
	// カメラの更新処理
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->Update();
	}

	// 敵を動かせるかチェック
	m_nEnemyMoveCounter--;
	if (m_nEnemyMoveCounter <= 0)
	{
		m_nEnemyMoveCounter = 0;
		m_bEnemyMove = true;
	}

	// 状態によって処理わけ
	switch (m_State)
	{
	case STATE_STAGE_SELECT:
		StageSelectUpdate();
		CScene::DeathCheck();
		break;
	case STATE_STAGE_DISP:
		StageDispUpdate();
		CScene::DeathCheck();
		break;
	case STATE_NORMAL:
		NormalUpdate();
		CScene::UpdateAll();
		break;
	case STATE_PAUSE:
		PauseUpdate();
		break;
	case STATE_GAMEOVER:
		GameOverUpdate();
		CScene::UpdateAll();
		break;
	case STATE_RESULT:
		ResultUpdate();
		break;
	case STATE_CHANGE_MAP:
		ChangeMapUpdate();
		CScene::UpdateAll();
		break;
	case STATE_PREV_MAP:
		ChangePrevMap();
		CScene::DeathCheck();
		break;
	case STATE_NEXT_MAP:
		ChangeNextMap();
		CScene::DeathCheck();
		break;
	case STATE_END:
		EndUpdate();
		CScene::UpdateAll();
		break;
	case STATE_END_RETRY:
		EndRetryUpdate();
		break;
	case STATE_END_QUIT:
		EndQuitUpdate();
		break;
	}

#ifdef GAME_MAPEVENT
	// マップの状態によって処理わけ
	switch (CCharaSelect::GetStageType())
	{
	case MAPTYPE_HINAMATSURI:  // ひな祭り
		MapEvent_Hinamatsuri();
		break;
	}
#endif

	// プレイヤーをリスポーンさせるかチェック
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{// ローカル1Pプレイならば
		CheckPlayerResSpawn(0);
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL2P)
	{// ローカル2Pプレイならば
		for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			CheckPlayerResSpawn(nCntPlayer);
		}
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// オンライン2Pプレイならば
		CheckPlayerResSpawn(CManager::GetClient()->GetClientId());
	}

	// 敵を生成するかチェックする
	int nIdxClient = 0;
	if (CManager::GetClient() != NULL)
	{
		nIdxClient = CManager::GetClient()->GetClientId();
	}
	if (nIdxClient == 0 && m_State != STATE_STAGE_SELECT && m_State != STATE_STAGE_DISP && m_State != STATE_PAUSE)
	CheckEnemySpawn(m_nGameCounter);

	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		// サーバーへ送るデータを設定する
		SetDataToServer();

		// クライアントの更新処理
		CManager::ClientUpdate();

		// サーバーから送られたデータを設定する
		GetDataFromServer();
	}

	// データクリア
	m_bDeletePlayerFlag = false;
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
	m_nNumDeleteItem = 0;
	strcpy(m_aDeleteItem, "\0");
	m_bHitBulletFlag = false;

	CDebugProc::Print(1, "ゲーム画面\n");
	if (CManager::GetClient() != NULL)
	{
		CDebugProc::Print(1, "クライアント番号      : %d\n", CManager::GetClient()->GetClientId());
	}
	CDebugProc::Print(1, "プレイヤー1の種類番号 : %d\n", CCharaSelect::GetPlayerNumber(0));
	CDebugProc::Print(1, "プレイヤー2の種類番号 : %d\n\n", CCharaSelect::GetPlayerNumber(1));
	CDebugProc::Print(1, "現在のステージ番号    : %d\n", m_nStageIdx);
	CDebugProc::Print(1, "ゲームカウンター      : %d\n", m_nGameCounter);
	CDebugProc::Print(1, "現在生成された敵の数  : %d\n", m_nSpawnEnemyCount);
	CDebugProc::Print(1, "残りの敵の数          : %d\n", m_nNumEnemy);
	CDebugProc::Print(1, "プレイヤー1の残機数   : %d\n", m_nPlayerStock[0]);
	CDebugProc::Print(1, "プレイヤー2の残機数   : %d\n", m_nPlayerStock[1]);
	CDebugProc::Print(1, "リスポーンカウンター  : %d\n", m_nPlayerRespawnCounter);
	CDebugProc::Print(1, "プレイヤー1の弾の数   : %d\n", CBulletPlayer::GetNumAll_0());
	CDebugProc::Print(1, "プレイヤー2の弾の数   : %d\n", CBulletPlayer::GetNumAll_1());
	CDebugProc::Print(1, "敵の弾の数            : %d\n", CBulletEnemy::GetNumAll());
	CDebugProc::Print(1, "ブロックの数          : %d\n", CBlock::GetNumAll());
	CDebugProc::Print(1, "スポーンした敵の数    : %d\n", CEnemy::GetSpawnCounter());
}

//=============================================================================
// ゲームの描画処理
//=============================================================================
void CGame::Draw(void)
{
	// カメラの設定
	if (GetCameraManager() != NULL)
	{
		GetCameraManager()->BindCamera();
	}

	// シーンクラスの描画処理
	CScene::DrawAll();
}


//*****************************************************************************
//
// サーバーの更新用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームの消すブロックのデータを設定
//=============================================================================
void CGame::DeleteBlock(const int nIdx)
{
	m_nNumDeleteBlock++;
	char aData[64] = "\0";
	sprintf(aData, "%d", nIdx);
	strcat(m_aDeleteBlock, aData);
	strcat(m_aDeleteBlock, " ");
}

//=============================================================================
// ゲームの消す敵のデータを設定
//=============================================================================
void CGame::DeleteEnemy(const int nIdx)
{
	m_nNumDeleteEnemy++;
	char aData[64] = "\0";
	sprintf(aData, "%d", nIdx);
	strcat(m_aDeleteEnemy, aData);
	strcat(m_aDeleteEnemy, " ");
}

//=============================================================================
// ゲームの消すアイテムのデータを設定
//=============================================================================
void CGame::DeleteItem(const int nIdx)
{
	m_nNumDeleteItem++;
	char aData[64] = "\0";
	sprintf(aData, "%d", nIdx);
	strcat(m_aDeleteItem, aData);
	strcat(m_aDeleteItem, " ");
}

//=============================================================================
// ゲームのプレイヤーを空にする処理
//=============================================================================
void CGame::DeletePlayer(CPlayer *pPlayer, const int nIdx)
{
	SetPlayer(pPlayer, nIdx);
	if (m_nPlayerStock[nIdx] <= 0)
	{
		SetState(STATE_GAMEOVER);
	}

	// ホストじゃなかったら
	if (CManager::GetClient() == NULL)return;
	if (CManager::GetClient()->GetClientId() == 0 && nIdx != 0)
	{
		m_bDeletePlayerFlag = true;
	}
}

//=============================================================================
// ゲームの相手プレイヤーの弾が当たったかどうか設定する
//=============================================================================
void CGame::HitBullet(void)
{
	m_bHitBulletFlag = true;
}

//=============================================================================
// ゲームのサーバーに送るデータを設定する処理
//=============================================================================
void CGame::SetDataToServer(void)
{
	if (CManager::GetClient() == NULL || CManager::GetClient()->GetConnected() == false)return;

	// プレイヤーのデータを設定
	SetDataToServerFromPlayer();

	if (CManager::GetClient()->GetClientId() == 0)
	{// 自分がホストなら
	    // ゲームのデータを設定
		SetDataToServerFromGame();

		// 敵のデータを設定
		SetDataToServerFromEnemy();

		// 敵の弾のデータを設定
		SetDataToServerFromEnemyBullet();

		// 消すブロックのデータを設定
		SetDataToServerFromDeleteBlock();

		// アイテムのデータを設定
		SetDataToServerFromItem();

		// 敵のスポーン数を設定
		SetDataToServerFromSpawnEnemyCount();

		// マップイベントのデータを設定
		SetDataToServerFromMapEvent();
	}

	// プレイヤーの弾情報を設定
	SetDataToServerFromPlayerBullet();

	if (CManager::GetClient()->GetClientId() != 0)
	{// 自分がホストでないなら
		// 消すブロックのデータを設定
		SetDataToServerFromDeleteBlock();

		// 消す敵のデータを設定
		SetDataToServerFromDeleteEnemy();

		// 自身の状態を設定
		SetDataToServerFromClientState();

		// 消すアイテムのデータを設定
		SetDataToServerFromDeleteItem();
	}

	// 倒した敵の数を設定
	SetDataToServerFromBreakEnemy();

	// 弾に当たったかどうか設定
	SetDataToServerFromHitBullet();
}

//=============================================================================
// ゲームのサーバーに送るプレイヤーデータを設定する処理
//=============================================================================
void CGame::SetDataToServerFromPlayer(void)
{
	// プレイヤーの人数を設定
	if (m_pPlayer[CManager::GetClient()->GetClientId()] == NULL)
	{
		CManager::GetClient()->Print("0");
		CManager::GetClient()->Print(" ");
		if (CManager::GetClient()->GetClientId() == 0)
		{
			CManager::GetClient()->Print("%d", (int)m_bDeletePlayerFlag);
			CManager::GetClient()->Print(" ");
		}
		return;
	}
	else if(m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
	{
		CManager::GetClient()->Print("1");
		CManager::GetClient()->Print(" ");

		// プレイヤーの座標を設定
		D3DXVECTOR3 PlayerPos = m_pPlayer[CManager::GetClient()->GetClientId()]->GetPos();
		CManager::GetClient()->Print("%.1f %.1f %.1f", PlayerPos.x, PlayerPos.y, PlayerPos.z);
		CManager::GetClient()->Print(" ");

		// プレイヤーの向きを設定
		D3DXVECTOR3 PlayerRot = m_pPlayer[CManager::GetClient()->GetClientId()]->GetRot();
		CManager::GetClient()->Print("%.1f %.1f %.1f", PlayerRot.x, PlayerRot.y, PlayerRot.z);
		CManager::GetClient()->Print(" ");

		// プレイヤーの状態を設定
		int nPlayerState = m_pPlayer[CManager::GetClient()->GetClientId()]->GetState();
		CManager::GetClient()->Print("%d", nPlayerState);
		CManager::GetClient()->Print(" ");
	}

	// ホストだったら
	if (CManager::GetClient()->GetClientId() == 0)
	{
		CManager::GetClient()->Print("%d", (int)m_bDeletePlayerFlag);
		CManager::GetClient()->Print(" ");
	}
}

//=============================================================================
// ゲームのサーバーに送るゲームデータを設定する処理
//=============================================================================
void CGame::SetDataToServerFromGame(void)
{
	// 状態を設定
	CManager::GetClient()->Print("%d", m_State);
	CManager::GetClient()->Print(" ");

	// マップの番号を設定
	CManager::GetClient()->Print("%d", m_nMapIdx);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// ゲームのサーバーに送る敵データを設定する処理
//=============================================================================
void CGame::SetDataToServerFromEnemy(void)
{
	int nNumEnemy = 0;
	char aData[64] = "\0";
	char aEnemyData[MAX_SERVER_DATA] = "\0";

	// 敵のデータを設定
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// 敵クラスだった
				strcat(aEnemyData, GetDataToEnemy((CEnemy*)pScene, aData));
				strcpy(aData, "\0");
				nNumEnemy++;
			}
			pScene = pSceneNext;
		}
	}

	// 敵の数を設定
	CManager::GetClient()->Print("%d", nNumEnemy);
	CManager::GetClient()->Print(" ");

	// 敵のデータを設定
	CManager::GetClient()->Print("%s", aEnemyData);
}

//=============================================================================
// 敵のデータを文字列に変換する処理
//=============================================================================
char *CGame::GetDataToEnemy(CEnemy *pEnemy, char *pStr)
{
	char aData[64] = "\0";

	// 敵の座標を設定
	D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
	sprintf(aData, "%.1f %.1f %.1f", EnemyPos.x, EnemyPos.y, EnemyPos.z);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// 敵の向きを設定
	D3DXVECTOR3 EnemyRot = pEnemy->GetRot();
	sprintf(aData, "%.1f %.1f %.1f", EnemyRot.x, EnemyRot.y, EnemyRot.z);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// 敵の種類
	int nEnemyType = pEnemy->GetType();
	sprintf(aData, "%d", nEnemyType);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// 敵の番号
	int nEnemyIdx = pEnemy->GetIdx();
	sprintf(aData, "%d", nEnemyIdx);
	strcat(pStr, aData);
	strcat(pStr, " ");

	return pStr;
}

//=============================================================================
// ゲームのサーバーに送るプレイヤーの弾データを設定する処理
//=============================================================================
void CGame::SetDataToServerFromPlayerBullet(void)
{
	int nNumBullet = 0;
	char aData[64] = "\0";
	char aBulletData[MAX_SERVER_DATA] = "\0";

	// 現在のプレイヤーの弾の数を設定
	if (CManager::GetClient()->GetClientId() == 0)
	{
		nNumBullet = CBulletPlayer::GetNumAll_0();
	}
	else
	{
		nNumBullet = CBulletPlayer::GetNumAll_1();
	}

	// 弾の数を設定
	CManager::GetClient()->Print("%d", nNumBullet);
	CManager::GetClient()->Print(" ");
	if (nNumBullet < 1) return;

	// 弾のデータを設定する
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// 弾クラスだった
				strcat(aBulletData, GetDataToPlayerBullet((CBullet*)pScene, aData));
				strcpy(aData, "\0");
			}
			pScene = pSceneNext;
		}
	}

	// 弾のデータを設定
	CManager::GetClient()->Print("%s", aBulletData);
}

//=============================================================================
// プレイヤーの弾のデータを文字列に変換する処理
//=============================================================================
char *CGame::GetDataToPlayerBullet(CBullet *pBullet, char *pStr)
{
	if (pBullet->GetType() == CManager::GetClient()->GetClientId())
	{// 自分のプレイヤーが撃った弾の情報を書き込む
		char aData[64] = "\0";

		// 弾の座標を設定
		D3DXVECTOR3 BulletPos = pBullet->GetPos();
		sprintf(aData, "%.1f %.1f %.1f", BulletPos.x, BulletPos.y, BulletPos.z);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// 弾の番号を設定
		int nBulletIdx = pBullet->GetIdx();
		sprintf(aData, "%d", nBulletIdx);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// 弾の種類を設定
		int nBulletType = pBullet->GetType();
		sprintf(aData, "%d", nBulletType);
		strcat(pStr, aData);
		strcat(pStr, " ");
	}

	return pStr;
}

//=============================================================================
// ゲームのサーバーに送る敵の弾データを設定する処理
//=============================================================================
void CGame::SetDataToServerFromEnemyBullet(void)
{
	int nNumBullet = 0;
	char aData[64] = "\0";
	char aBulletData[MAX_SERVER_DATA] = "\0";

	// 敵の弾の数を設定
	nNumBullet = CBulletEnemy::GetNumAll();

	// 弾の数を設定
	CManager::GetClient()->Print("%d", nNumBullet);
	CManager::GetClient()->Print(" ");
	if (nNumBullet < 1) return;

	// 弾のデータを設定する
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// 弾クラスだった
				strcat(aBulletData, GetDataToEnemyBullet((CBullet*)pScene, aData));
				strcpy(aData, "\0");
			}
			pScene = pSceneNext;
		}
	}

	// 弾のデータを設定
	CManager::GetClient()->Print("%s", aBulletData);
}

//=============================================================================
// 敵の弾のデータを文字列に変換する処理
//=============================================================================
char *CGame::GetDataToEnemyBullet(CBullet *pBullet, char *pStr)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{// 敵の弾データを設定する
		char aData[64] = "\0";

		// 弾の座標を設定
		D3DXVECTOR3 BulletPos = pBullet->GetPos();
		sprintf(aData, "%.1f %.1f %.1f", BulletPos.x, BulletPos.y, BulletPos.z);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// 弾の番号を設定
		int nBulletIdx = pBullet->GetIdx();
		sprintf(aData, "%d", nBulletIdx);
		strcat(pStr, aData);
		strcat(pStr, " ");

		// 弾の種類を設定
		int nBulletType = pBullet->GetType();
		sprintf(aData, "%d", nBulletType);
		strcat(pStr, aData);
		strcat(pStr, " ");
	}

	return pStr;
}

//=============================================================================
// ゲームのサーバーに送るアイテムデータを設定する処理
//=============================================================================
void CGame::SetDataToServerFromItem(void)
{
	int nNumItem = 0;
	char aData[64] = "\0";
	char aItemData[MAX_SERVER_DATA] = "\0";

	// アイテムの数を設定
	nNumItem = CItem::GetNumAll();

	// アイテムの数を設定
	CManager::GetClient()->Print("%d", nNumItem);
	CManager::GetClient()->Print(" ");
	if (nNumItem < 1) return;

	// アイテムのデータを設定する
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ITEM)
			{// アイテムクラスだった
				strcat(aItemData, GetDataToItem((CItem*)pScene, aData));
				strcpy(aData, "\0");
			}
			pScene = pSceneNext;
		}
	}

	// アイテムのデータを設定
	CManager::GetClient()->Print("%s", aItemData);
}

//=============================================================================
// ゲームのアイテムのデータを文字列に変換する処理
//=============================================================================
char *CGame::GetDataToItem(CItem *pItem, char *pStr)
{
	char aData[64] = "\0";

	// アイテムの座標を設定
	D3DXVECTOR3 ItemPos = pItem->GetPos();
	sprintf(aData, "%.1f %.1f %.1f", ItemPos.x, ItemPos.y, ItemPos.z);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// アイテムの番号を設定
	int nItemIdx = pItem->GetIdx();
	sprintf(aData, "%d", nItemIdx);
	strcat(pStr, aData);
	strcat(pStr, " ");

	// アイテムの種類を設定
	int nItemType = pItem->GetType();
	sprintf(aData, "%d", nItemType);
	strcat(pStr, aData);
	strcat(pStr, " ");

	return pStr;
}

//=============================================================================
// ゲームのサーバーに送る消すブロックのデータを設定する処理
//=============================================================================
void CGame::SetDataToServerFromDeleteBlock(void)
{
	CManager::GetClient()->Print("%d", m_nNumDeleteBlock);
	CManager::GetClient()->Print(" ");
	if (m_nNumDeleteBlock > 0)
	{
		CManager::GetClient()->Print("%s", m_aDeleteBlock);
		CManager::GetClient()->Print(" ");
	}
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
}

//=============================================================================
// ゲームのサーバーに送る消す敵のデータを設定する処理
//=============================================================================
void CGame::SetDataToServerFromDeleteEnemy(void)
{
	CManager::GetClient()->Print("%d", m_nNumDeleteEnemy);
	CManager::GetClient()->Print(" ");
	if (m_nNumDeleteEnemy > 0)
	{
		CManager::GetClient()->Print("%s", m_aDeleteEnemy);
		CManager::GetClient()->Print(" ");
	}
}

//=============================================================================
// ゲームのサーバーに送る敵の数を設定する処理
//=============================================================================
void CGame::SetDataToServerFromSpawnEnemyCount(void)
{
	CManager::GetClient()->Print("%d", m_nSpawnEnemyCount);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// ゲームのサーバーに送る状態を設定する処理
//=============================================================================
void CGame::SetDataToServerFromClientState(void)
{
	// 状態を設定
	CManager::GetClient()->Print("%d", m_State);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// ゲームのサーバーに消すアイテムのデータを設定する処理
//=============================================================================
void CGame::SetDataToServerFromDeleteItem(void)
{
	CManager::GetClient()->Print("%d", m_nNumDeleteItem);
	CManager::GetClient()->Print(" ");
	if (m_nNumDeleteItem > 0)
	{
		CManager::GetClient()->Print("%s", m_aDeleteItem);
		CManager::GetClient()->Print(" ");
	}
}

//=============================================================================
// ゲームのサーバーに送る倒した敵の数を設定する処理
//=============================================================================
void CGame::SetDataToServerFromBreakEnemy(void)
{
	// 番号を設定
	int nIdxClient = 0;
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		nIdxClient = pClient->GetClientId();
	}

	for (int nCntType = 0; nCntType < CEnemy::TYPE_MAX; nCntType++)
	{
		CManager::GetClient()->Print("%d", m_nNumBreakEnemy[nIdxClient][nCntType]);
		CManager::GetClient()->Print(" ");
	}

	CManager::GetClient()->Print("%d", m_nScore[nIdxClient]);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// ゲームのサーバーに送る倒した敵の数を設定する処理
//=============================================================================
void CGame::SetDataToServerFromMapEvent(void)
{
	int nHinaEvent = m_HinaEvent;
	CManager::GetClient()->Print("%d", nHinaEvent);
	CManager::GetClient()->Print(" ");
	CManager::GetClient()->Print("%d", m_nNextEvent);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// ゲームのサーバーに送る倒した敵の数を設定する処理
//=============================================================================
void CGame::SetDataToServerFromHitBullet(void)
{
	CManager::GetClient()->Print("%d", (int)m_bHitBulletFlag);
	CManager::GetClient()->Print(" ");
}

//=============================================================================
// ゲームのサーバーから送られたデータを設定する処理
//=============================================================================
void CGame::GetDataFromServer(void)
{
	// 送られたメッセージを取得
	if (CManager::GetClient() == NULL)return;
	char *pStr = CManager::GetClient()->GetReceiveMessage();
	if (pStr == NULL || CManager::GetClient()->GetConnected() == false || *pStr == *"???")
	{// メッセージ受信に失敗 || サーバーに接続できていない
		if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL && m_State == STATE_NORMAL)
		{// プレイヤーが生きている
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->Uninit();
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] = NULL;
			m_nPlayerStock[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]--;
		}
		return;
	}

	// プレイヤーのデータを設定
	pStr = SetDataToPlayerFromServer(pStr);

	if (CManager::GetClient()->GetClientId() != 0)
	{// ホストじゃなかったら
	    // ゲームの状態を設定
		pStr = SetDataToGameFromServer(pStr);

		// 敵のデータを設定
		pStr = SetDataToEnemyFromServer(pStr);

		// 敵の弾のデータを設定
		pStr = SetDataToEnemyBulletFromServer(pStr);

		// 消すブロックのデータを設定
		pStr = SetDataToDeleteBlock(pStr);

		// アイテムのデータを設定
		pStr = SetDataToItemFromServer(pStr);

		// 敵の数のデータを設定
		pStr = SetDataToSpawnEnemyCount(pStr);

		// マップイベントのデータを設定
		pStr = SetDataToMapEvent(pStr);
	}

	// プレイヤーの弾のデータを設定
	pStr = SetDataToPlayerBulletFromServer(pStr);

	if (CManager::GetClient()->GetClientId() == 0)
	{// ホストだったら
		// 消すブロックのデータを設定
		pStr = SetDataToDeleteBlock(pStr);

		// 消す敵のデータを設定
		pStr = SetDataToDeleteEnemy(pStr);

		// 相手の状態を設定
		pStr = SetDataToClientState(pStr);

		// 消すアイテムのデータを設定
		pStr = SetDataToDeleteItem(pStr);
	}

	// 敵を倒した数を設定
	pStr = SetDataToBreakEnemy(pStr);

	// 弾がヒットしたかどうか設定
	pStr = SetDataToHitBullet(pStr);

	// 死亡フラグチェック
	CScene::DeathCheck();
}

//=============================================================================
// ゲームのサーバーから送られたデータをプレイヤーに設定する処理
//=============================================================================
char *CGame::SetDataToPlayerFromServer(char *pStr)
{
	int nWord = 0;
	int nNumPlayer = 0;

	// プレイヤーの人数を読み取る
	nNumPlayer = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumPlayer == 0)
	{// プレイヤーが存在しない
		if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL && m_State == STATE_NORMAL)
		{// プレイヤーが生きている
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetDeathEffect();
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->Uninit();
			m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] = NULL;
			m_nPlayerStock[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]--;
			if (m_pUI != NULL)
			{
				m_pUI->ReCreatePlayerStock(&m_nPlayerStock[0]);
			}
		}
	}
	else if(nNumPlayer == 1)
	{// プレイヤーが存在する
		CMap *pMap = GetMap();
		bool bPlayerSpawn = false;
		if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] == NULL && pMap != NULL)
		{// メモリが確保されていない
			int nNumber = (CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER;
			m_pPlayer[nNumber] = m_pPlayerManager[nNumber]->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nNumber);
			bPlayerSpawn = true;
		}
		if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL)
		{// メモリが確保できている
			// プレイヤーの座標を読み取る
			float fGetPlayerPosX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerPosZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// プレイヤーの向きを読み取る
			float fGetPlayerRotX = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotY = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			float fGetPlayerRotZ = CFunctionLib::ReadFloat(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// プレイヤーの状態を読み取る
			int nGetPlayerState = CFunctionLib::ReadInt(pStr, "");
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;

			// プレイヤーに値を設定
			if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER] != NULL)
			{
				m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetPos(D3DXVECTOR3(fGetPlayerPosX, fGetPlayerPosY, fGetPlayerPosZ));
				m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetRot(D3DXVECTOR3(fGetPlayerRotX, fGetPlayerRotY, fGetPlayerRotZ));
				if (m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->GetState() != nGetPlayerState)
				{
					m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]->SetState((CPlayer::STATE)nGetPlayerState);
				}
			}

			// プレイヤーが生成されたばっかりならエフェクトを出す
			CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
			if (pEffectManager != NULL && bPlayerSpawn == true)
			{
				D3DXVECTOR3 EffectPos = D3DXVECTOR3(fGetPlayerPosX, fGetPlayerPosY, fGetPlayerPosZ);
				pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, GAME_PLAYER_SPAWN_EFFECT_IDX);
				pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, GAME_PLAYER_SPAWN_EFFECT_IDX + 1);
			}
		}
	}


	// ホストじゃなかったら
	if (CManager::GetClient()->GetClientId() != 0)
	{
		bool bDeletePlayerFlag = CFunctionLib::ReadBool(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		if (bDeletePlayerFlag == true && m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
		{
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetDeathEffect();
			m_pPlayer[CManager::GetClient()->GetClientId()]->Uninit();
			m_pPlayer[CManager::GetClient()->GetClientId()] = NULL;
		}
	}

	return pStr;
}

//=============================================================================
// ゲームのサーバーから送られたデータをゲームに設定する処理
//=============================================================================
char *CGame::SetDataToGameFromServer(char *pStr)
{
	int nGameState = CFunctionLib::ReadInt(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	int nMapIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 状態を設定
	m_StateOld = m_State;
	m_State = (STATE)nGameState;
	if (m_State != STATE_END && m_State != STATE_GAMEOVER && m_State != STATE_RESULT)
	{// 終了状態でもゲームオーバー状態でもゲーム内リザルト状態でもない
		m_nStateCounter = 0;
	}

	// マップの番号を設定
	m_nMapIdx = nMapIdx;

	// 状態によって処理わけ
	if (m_State == STATE_STAGE_DISP && GetMap() == NULL)
	{
		CreateMap();
		CManager::GetSound()->PlaySound(GAME_SE_FANFARE_IDX);
	}
	else if (m_State == STATE_STAGE_SELECT && GetMap() != NULL)
	{
		ReleaseMap();
		DeleteGameObject();
	}
	else if (m_State == STATE_STAGE_DISP && m_StateOld == STATE_NORMAL)
	{
		ReleaseMap();
		DeleteGameObject();
	}
	else if (m_State == STATE_NORMAL && m_StateOld == STATE_STAGE_DISP)
	{
		ReleaseStageBg();
		ReleaseStageLogo();
		ReleaseStageNumber();
		SetPlayerPosToSpawn();
		CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX);
	}
	else if (m_State == STATE_STAGE_DISP || m_State == STATE_STAGE_SELECT)
	{
		ReleaseStageNumber();
		CalcStageDigits();
		CreateStageNumber();
	}
	else if (m_State == STATE_RESULT && m_StateOld == STATE_END)
	{
		CreateGameResult();
		CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX);
	}
	else if (m_State == STATE_PAUSE && m_StateOld == STATE_NORMAL)
	{
		if (m_bPauseOpen == true)
		{
			m_bPauseOpen = false;
			m_State = STATE_NORMAL;
			return pStr;
		}
		m_bPauseOpen = true;
		CreateNotPause();
		CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX, true);
	}
	else if (m_State == STATE_NORMAL && m_StateOld == STATE_PAUSE)
	{
		if (m_pPause != NULL)
		{
			m_State = STATE_PAUSE;
			return pStr;
		}

		m_bPauseOpen = false;
		ReleaseNotPause();
		CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX, true);
	}
	else if (m_State == STATE_END_RETRY && m_StateOld == STATE_PAUSE
		|| m_State == STATE_END_QUIT && m_StateOld == STATE_PAUSE)
	{
		ReleaseNotPause();
	}

	// やり直しバグ回避用
	if (m_StateOld == STATE_END_RETRY && m_State != STATE_END_RETRY)
	{
		m_State = STATE_END_RETRY;
	}

	return pStr;
}

//=============================================================================
// ゲームのサーバーから送られたデータを敵に設定する処理
//=============================================================================
char *CGame::SetDataToEnemyFromServer(char *pStr)
{
	int nWord = 0;
	int nNumEnemy = 0;

	// 生成されている敵の数を読み取る
	nNumEnemy = CFunctionLib::ReadInt(pStr, "");
	m_nNumEnemy = nNumEnemy;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 数合わせ
	ReleaseEnemy(nNumEnemy);
	if (nNumEnemy == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY && nNumEnemy > 0)
			{// 敵クラスだった
				pStr = SetDataToEnemy((CEnemy*)pScene, pStr);
				nNumEnemy--;
			}
			pScene = pSceneNext;
		}
	}

	// 足りない敵は新たに生成しデータを設定
	for (int nCntEnemy = 0; nCntEnemy < nNumEnemy; nCntEnemy++)
	{
		pStr = SetDataToCreateEnemy(pStr);
	}

	return pStr;
}

//=============================================================================
// 文字列をデータに変換し敵に設定する
//=============================================================================
char *CGame::SetDataToEnemy(CEnemy *pEnemy, char *pStr)
{
	int nWord = 0;
	float fEnemyPosX = 0.0f;
	float fEnemyPosY = 0.0f;
	float fEnemyPosZ = 0.0f;
	float fEnemyRotX = 0.0f;
	float fEnemyRotY = 0.0f;
	float fEnemyRotZ = 0.0f;
	int nEnemyType = 0;
	int nEnemyIdx = 0;

	// 敵の座標を読み取る
	fEnemyPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵の向きを読み取る
	fEnemyRotX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵の種類番号
	nEnemyType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵の番号
	nEnemyIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵の種類を取得し違うなら作り替える
	int nEnemyTypeOld = pEnemy->GetType();
	if (nEnemyType != nEnemyTypeOld)
	{
		// 破棄する
		pEnemy->Uninit();
		pEnemy = NULL;
		CScene::DeathCheck();

		// 生成しなおす
		CEnemy_ListData *pEnemyData = GetMap()->GetEnemyListData(nEnemyIdx);
		D3DXVECTOR3 pos = D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ);
		D3DXVECTOR3 rot = D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ);
		pEnemy = m_pEnemyManager[nEnemyType]->SetEnemy(pos, rot, nEnemyType);
		if (pEnemy == NULL && pEnemyData != NULL)return pStr;
		if (pEnemyData->GetItem() == true)
		{
			pEnemy->SetItemData(pEnemyData->GetItemType());
		}
		pEnemy->SetAIData(pEnemyData->GetAI_BulletEva(), pEnemyData->GetAI_MassEva(), pEnemyData->GetAI_DownEva());
	}
	else
	{// 同じ種類の敵である
		// 敵にデータを設定
		pEnemy->SetPos(D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ));
		pEnemy->SetRot(D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ));
		pEnemy->SetIdx(nEnemyIdx);
	}

	return pStr;
}

//=============================================================================
// 文字列をデータに変換し生成した敵に設定する
//=============================================================================
char *CGame::SetDataToCreateEnemy(char *pStr)
{
	int nWord = 0;
	float fEnemyPosX = 0.0f;
	float fEnemyPosY = 0.0f;
	float fEnemyPosZ = 0.0f;
	float fEnemyRotX = 0.0f;
	float fEnemyRotY = 0.0f;
	float fEnemyRotZ = 0.0f;
	int nEnemyType = 0;
	int nEnemyIdx = 0;

	// 敵の座標を読み取る
	fEnemyPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵の向きを読み取る
	fEnemyRotX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fEnemyRotZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵の種類番号
	nEnemyType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵の番号
	nEnemyIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 敵を生成する(同時に必要なデータを設定)
	CEnemy_ListData *pEnemyData = GetMap()->GetEnemyListData(nEnemyIdx);
	D3DXVECTOR3 pos = D3DXVECTOR3(fEnemyPosX, fEnemyPosY, fEnemyPosZ);
	D3DXVECTOR3 rot = D3DXVECTOR3(fEnemyRotX, fEnemyRotY, fEnemyRotZ);
	CEnemy *pEnemy = m_pEnemyManager[nEnemyType]->SetEnemy(pos, rot, nEnemyType);

	if (pEnemy == NULL)return pStr;
	// 敵のデータを設定する
	if (pEnemyData->GetItem() == true)
	{
		pEnemy->SetItemData(pEnemyData->GetItemType());
	}
	pEnemy->SetAIData(pEnemyData->GetAI_BulletEva(), pEnemyData->GetAI_MassEva(), pEnemyData->GetAI_DownEva());
	pEnemy->SetIdx(nEnemyIdx);

	return pStr;
}

//=============================================================================
// プレイヤーの弾の数を合わせる処理
//=============================================================================
void CGame::ReleaseEnemy(int nNumEnemy)
{
	// 現在の敵の数を設定
	int nAllEnemy = 0;
	nAllEnemy = CEnemy::GetNumAll();
	if (nAllEnemy < nNumEnemy)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY && nAllEnemy > nNumEnemy)
			{// 弾クラスだった
				ReleaseCheckEnemy((CEnemy*)pScene, &nAllEnemy);
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();
}

//=============================================================================
// 敵を破棄するかチェックする処理
//=============================================================================
void CGame::ReleaseCheckEnemy(CEnemy *pEnemy, int *pNumEnemy)
{
	pEnemy->Uninit();
	pEnemy = NULL;
	*pNumEnemy = *pNumEnemy - 1;
}

//=============================================================================
// ゲームのサーバーから送られたデータをプレイヤーの弾に設定する処理
//=============================================================================
char *CGame::SetDataToPlayerBulletFromServer(char *pStr)
{
	int nWord = 0;
	int nNumBullet = 0;

	// 生成されている弾の数を読み取る
	nNumBullet = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 数合わせ
	ReleasePlayerBullet(nNumBullet);
	if (nNumBullet == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET)
			{// 弾クラスだった
				pStr = SetDataToPlayerBullet((CBullet*)pScene, pStr, &nNumBullet);
			}
			pScene = pSceneNext;
		}
	}

	// 足りない弾は新たに生成しデータを設定
	for (int nCntBullet = 0; nCntBullet < nNumBullet; nCntBullet++)
	{
		pStr = SetDataToCreatePlayerBullet(pStr);
	}

	return pStr;
}

//=============================================================================
// プレイヤーの弾の数を合わせる処理
//=============================================================================
void CGame::ReleasePlayerBullet(int nNumBullet)
{
	// 現在のプレイヤーの弾の数を設定
	int nAllBullet = 0;
	if (CManager::GetClient()->GetClientId() == 0)
	{
		nAllBullet = CBulletPlayer::GetNumAll_1();
	}
	else
	{
		nAllBullet = CBulletPlayer::GetNumAll_0();
	}
	if (nAllBullet < nNumBullet)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nAllBullet > nNumBullet)
			{// 弾クラスだった
				ReleaseCheckPlayerBullet((CBullet*)pScene, &nAllBullet);
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();
}

//=============================================================================
// プレイヤーの弾を破棄するかチェックする処理
//=============================================================================
void CGame::ReleaseCheckPlayerBullet(CBullet *pBullet, int *pNumBullet)
{
	if (pBullet->GetType() != CBullet::TYPE_ENEMY &&
		pBullet->GetType() != CManager::GetClient()->GetClientId())
	{
		pBullet->Uninit();
		pBullet = NULL;
		*pNumBullet = *pNumBullet - 1;
	}
}

//=============================================================================
// 文字列をデータに変換しプレイヤーの弾に設定する
//=============================================================================
char *CGame::SetDataToPlayerBullet(CBullet *pBullet, char *pStr, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY ||
		pBullet->GetType() == CManager::GetClient()->GetClientId()) return pStr;

	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// 弾の座標を読み取る
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾の番号を読み取る
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾の種類を読み取る
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// プレイヤーの弾にデータを設定
	pBullet->SetPos(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ));
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);
	*pNumBullet = *pNumBullet - 1;

	return pStr;
}

//=============================================================================
// 文字列をデータに変換し生成したプレイヤーの弾に設定する
//=============================================================================
char *CGame::SetDataToCreatePlayerBullet(char *pStr)
{
	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// 弾の座標を読み取る
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾の番号を読み取る
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾の種類を読み取る
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾を生成する(同時に必要なデータを設定)
	CBulletPlayer *pBullet = CBulletPlayer::Create(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ), INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3,
		(CBullet::TYPE)((CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER), m_pPlayer[(CManager::GetClient()->GetClientId() + 1) % MAX_NUM_PLAYER]);
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);

	return pStr;
}

//=============================================================================
// ゲームのサーバーから送られたデータを敵の弾に設定する処理
//=============================================================================
char *CGame::SetDataToEnemyBulletFromServer(char *pStr)
{
	int nWord = 0;
	int nNumBullet = 0;

	// 生成されている弾の数を読み取る
	nNumBullet = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 数合わせ
	ReleaseEnemyBullet(nNumBullet);
	if (nNumBullet == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nNumBullet > 0)
			{// 弾クラスだった
				pStr = SetDataToEnemyBullet((CBullet*)pScene, pStr, &nNumBullet);
			}
			pScene = pSceneNext;
		}
	}

	// 足りない弾は新たに生成しデータを設定
	for (int nCntBullet = 0; nCntBullet < nNumBullet; nCntBullet++)
	{
		pStr = SetDataToCreateEnemyBullet(pStr);
	}

	return pStr;
}

//=============================================================================
// 敵の弾の数を合わせる処理
//=============================================================================
void CGame::ReleaseEnemyBullet(int nNumBullet)
{
	// 現在の敵の弾の数を設定
	int nAllBullet = 0;
	nAllBullet = CBulletEnemy::GetNumAll();
	if (nAllBullet < nNumBullet)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BULLET && nAllBullet > nNumBullet)
			{// 弾クラスだった
				ReleaseCheckEnemyBullet((CBullet*)pScene, &nAllBullet);
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();
}

//=============================================================================
// 敵の弾を破棄するかチェックする処理
//=============================================================================
void CGame::ReleaseCheckEnemyBullet(CBullet *pBullet, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{
		pBullet->Uninit();
		pBullet = NULL;
		*pNumBullet = *pNumBullet - 1;
	}
}

//=============================================================================
// 文字列をデータに変換し弾に設定する
//=============================================================================
char *CGame::SetDataToEnemyBullet(CBullet *pBullet, char *pStr, int *pNumBullet)
{
	if (pBullet->GetType() == CBullet::TYPE_ENEMY)
	{
		int nWord = 0;
		float fBulletPosX = 0.0f;
		float fBulletPosY = 0.0f;
		float fBulletPosZ = 0.0f;
		int nBulletIdx = 0;
		int nBulletType = 0;

		// 弾の座標を読み取る
		fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// 弾の番号を読み取る
		nBulletIdx = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// 弾の種類を読み取る
		nBulletType = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;

		// 弾にデータを設定
		pBullet->SetPos(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ));
		pBullet->SetIdx(nBulletIdx);
		pBullet->SetType((CBullet::TYPE)nBulletType);
		*pNumBullet = *pNumBullet - 1;
	}

	return pStr;
}

//=============================================================================
// 文字列をデータに変換し生成した敵の弾に設定する
//=============================================================================
char *CGame::SetDataToCreateEnemyBullet(char *pStr)
{
	int nWord = 0;
	float fBulletPosX = 0.0f;
	float fBulletPosY = 0.0f;
	float fBulletPosZ = 0.0f;
	int nBulletIdx = 0;
	int nBulletType = 0;

	// 弾の座標を読み取る
	fBulletPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fBulletPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾の番号を読み取る
	nBulletIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾の種類を読み取る
	nBulletType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 弾を生成する(同時に必要なデータを設定)
	CBulletEnemy *pBullet = CBulletEnemy::Create(D3DXVECTOR3(fBulletPosX, fBulletPosY, fBulletPosZ), INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, NULL);
	pBullet->SetIdx(nBulletIdx);
	pBullet->SetType((CBullet::TYPE)nBulletType);

	return pStr;
}

//=============================================================================
// ゲームのサーバーから送られたデータをアイテムに設定する処理
//=============================================================================
char *CGame::SetDataToItemFromServer(char *pStr)
{
	int nWord = 0;
	int nNumItem = 0;

	// 生成されている弾の数を読み取る
	nNumItem = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 数合わせ
	ReleaseItem(nNumItem);
	if (nNumItem == 0) return pStr;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ITEM && nNumItem > 0)
			{// アイテムクラスだった
				pStr = SetDataToItem((CItem*)pScene, pStr, &nNumItem);
			}
			pScene = pSceneNext;
		}
	}

	// 足りないアイテムは新たに生成しデータを設定
	for (int nCntItem = 0; nCntItem < nNumItem; nCntItem++)
	{
		pStr = SetDataToCreateItem(pStr);
	}

	return pStr;
}

//=============================================================================
// 文字列をデータに変換しアイテムに設定する
//=============================================================================
char *CGame::SetDataToItem(CItem *pItem, char *pStr, int *pNumItem)
{
	int nWord = 0;
	float fItemPosX = 0.0f;
	float fItemPosY = 0.0f;
	float fItemPosZ = 0.0f;
	int nItemIdx = 0;
	int nItemType = 0;

	// アイテムの座標を読み取る
	fItemPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fItemPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fItemPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// アイテムの番号
	nItemIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// アイテムの種類番号
	nItemType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// アイテムの種類を取得し違うなら作り替える
	int nItemTypeOld = pItem->GetType();
	if (nItemType != nItemTypeOld)
	{
		// 破棄する
		pItem->Uninit();
		pItem = NULL;
		CScene::DeathCheck();

		// 生成しなおす
		pItem = CreateItem(D3DXVECTOR3(fItemPosX, fItemPosY, fItemPosZ), INITIALIZE_D3DXVECTOR3, nItemType);
		pItem->SetIdx(nItemIdx);
	}
	else
	{// 同じ種類のアイテムである
	    // アイテムにデータを設定
		pItem->SetPos(D3DXVECTOR3(fItemPosX, fItemPosY, fItemPosZ));
		pItem->SetIdx(nItemIdx);
	}
	*pNumItem = *pNumItem - 1;

	return pStr;
}

//=============================================================================
// 文字列をデータに変換し生成したアイテムに設定する
//=============================================================================
char *CGame::SetDataToCreateItem(char *pStr)
{
	int nWord = 0;
	float fItemPosX = 0.0f;
	float fItemPosY = 0.0f;
	float fItemPosZ = 0.0f;
	int nItemIdx = 0;
	int nItemType = 0;

	// アイテムの座標を読み取る
	fItemPosX = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fItemPosY = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	fItemPosZ = CFunctionLib::ReadFloat(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// アイテムの番号を読み取る
	nItemIdx = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// アイテムの種類を読み取る
	nItemType = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// アイテムを生成する(同時に必要なデータを設定)
	CItem *pItem = CreateItem(D3DXVECTOR3(fItemPosX, fItemPosY, fItemPosZ), INITIALIZE_D3DXVECTOR3, nItemType);
	if (pItem != NULL)
	{
		pItem->SetIdx(nItemIdx);
	}

	return pStr;
}

//=============================================================================
// アイテムの数を合わせる処理
//=============================================================================
void CGame::ReleaseItem(int nNumItem)
{
	// 現在のアイテムの数を設定
	int nAllItem = 0;
	nAllItem = CItem::GetNumAll();
	if (nAllItem < nNumItem)return;

	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ITEM && nAllItem > nNumItem)
			{// アイテムクラスだった
				ReleaseCheckItem((CItem*)pScene, &nAllItem);
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();
}

//=============================================================================
// アイテムを破棄するかチェックする処理
//=============================================================================
void CGame::ReleaseCheckItem(CItem *pItem, int *pNumItem)
{
	pItem->Uninit();
	pItem = NULL;
	*pNumItem = *pNumItem - 1;
}

//=============================================================================
// ゲームの消すブロックのデータを設定
//=============================================================================
char *CGame::SetDataToDeleteBlock(char *pStr)
{
	int nWord = 0;
	int nNumDeleteBlock = 0;
	int *pDeleteIdx = NULL;

	// 消すブロックの数を読み取る
	nNumDeleteBlock = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumDeleteBlock <= 0) return pStr;

	// 消す分だけメモリを確保
	pDeleteIdx = new int[nNumDeleteBlock];
	for (int nCnt = 0; nCnt < nNumDeleteBlock; nCnt++)
	{
		pDeleteIdx[nCnt] = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
	}

	// 消す数だけブロックを開放する
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_BLOCK && m_State == STATE_NORMAL)
			{// ブロッククラスだった
				ReleaseCheckBlock((CBlock*)pScene, pDeleteIdx, &nNumDeleteBlock);
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();

	// メモリの開放
	if (pDeleteIdx != NULL)
	{
		delete[] pDeleteIdx;
		pDeleteIdx = NULL;
	}

	return pStr;
}

//=============================================================================
// ゲームのブロックを破棄するかチェックする処理
//=============================================================================
void CGame::ReleaseCheckBlock(CBlock *pBlock, int *pDeleteIdx, int *nNumDeleteBlock)
{
	int nIdx = pBlock->GetIdx();
	for (int nCntDeleteNum = 0; nCntDeleteNum < *nNumDeleteBlock; nCntDeleteNum++)
	{// 消す数分繰り返し
		if (nIdx == pDeleteIdx[nCntDeleteNum] && pBlock != NULL)
		{// 番号一致
			pBlock->Uninit();
			pBlock = NULL;
		}
	}
}

//=============================================================================
// ゲームの消す敵のデータを設定
//=============================================================================
char *CGame::SetDataToDeleteEnemy(char *pStr)
{
	int nWord = 0;
	int nNumDeleteEnemy = 0;
	int nCntDelete = 0;
	int *pDeleteIdx = NULL;

	// 消す敵の数を読み取る
	nNumDeleteEnemy = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumDeleteEnemy <= 0) return pStr;

	// 消す分だけメモリを確保
	pDeleteIdx = new int[nNumDeleteEnemy];
	for (int nCnt = 0; nCnt < nNumDeleteEnemy; nCnt++)
	{
		pDeleteIdx[nCnt] = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
	}

	// 消す数だけ敵を開放する
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY && m_State == STATE_NORMAL)
			{// 敵クラスだった
				ReleaseCheckDeleteEnemy((CEnemy*)pScene, pDeleteIdx, &nNumDeleteEnemy);
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();

	// メモリの開放
	if (pDeleteIdx != NULL)
	{
		delete[] pDeleteIdx;
		pDeleteIdx = NULL;
	}

	return pStr;
}

//=============================================================================
// ゲームの敵を破棄するかチェックする処理
//=============================================================================
void CGame::ReleaseCheckDeleteEnemy(CEnemy *pEnemy, int *pDeleteIdx, int *nNumDeleteEnemy)
{
	int nIdx = pEnemy->GetIdx();
	for (int nCntDeleteNum = 0; nCntDeleteNum < *nNumDeleteEnemy; nCntDeleteNum++)
	{// 消す数分繰り返し
		if (nIdx == pDeleteIdx[nCntDeleteNum] && pEnemy != NULL)
		{// 番号一致
			pEnemy->SetDeathEffect();
			pEnemy->Uninit();
			pEnemy = NULL;
		}
	}
}

//=============================================================================
// ゲームの消すアイテムのデータを設定
//=============================================================================
char *CGame::SetDataToDeleteItem(char *pStr)
{
	int nWord = 0;
	int nNumDeleteItem = 0;
	int nCntDelete = 0;
	int *pDeleteIdx = NULL;

	// 消す敵の数を読み取る
	nNumDeleteItem = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	if (nNumDeleteItem <= 0) return pStr;

	// 消す分だけメモリを確保
	pDeleteIdx = new int[nNumDeleteItem];
	for (int nCnt = 0; nCnt < nNumDeleteItem; nCnt++)
	{
		pDeleteIdx[nCnt] = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
	}

	// 消す数だけ敵を開放する
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ITEM && m_State == STATE_NORMAL)
			{// アイテムクラスだった
				ReleaseCheckDeleteItem((CItem*)pScene, pDeleteIdx, &nNumDeleteItem);
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();

	// メモリの開放
	if (pDeleteIdx != NULL)
	{
		delete[] pDeleteIdx;
		pDeleteIdx = NULL;
	}

	return pStr;
}

//=============================================================================
// ゲームのアイテムを破棄するかチェックする処理
//=============================================================================
void CGame::ReleaseCheckDeleteItem(CItem *pItem, int *pDeleteIdx, int *nNumDeleteItem)
{
	int nIdx = pItem->GetIdx();
	for (int nCntDeleteNum = 0; nCntDeleteNum < *nNumDeleteItem; nCntDeleteNum++)
	{// 消す数分繰り返し
		if (nIdx == pDeleteIdx[nCntDeleteNum] && pItem != NULL)
		{// 番号一致
			pItem->Uninit();
			pItem = NULL;
		}
	}
}

//=============================================================================
// ゲームの相手の状態を取得する処理
//=============================================================================
char *CGame::SetDataToClientState(char *pStr)
{
	// 状態を読み取る
	int nGameState = CFunctionLib::ReadInt(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// 状態によってポーズするか設定
	if (nGameState == STATE_PAUSE && m_State == STATE_NORMAL && m_bPauseOpen == false)
	{
		m_State = STATE_PAUSE;
		CreateNotPause();
		CManager::GetSound()->PlaySound(GAME_SE_PAUSE_OPEN_IDX);
		CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX, true);
		m_bPauseOpen = true;
	}
	else if (nGameState == STATE_NORMAL && m_State == STATE_PAUSE && m_bPauseOpen == true)
	{
		if (m_pPause != NULL)
		{
			return pStr;
		}

		m_State = STATE_NORMAL;
		ReleaseNotPause();
		CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX, true);
		m_bPauseOpen = false;
	}
	else if (nGameState == STATE_END_RETRY)
	{
		m_State = STATE_END_RETRY;
		ReleaseNotPause();
	}
	else if (nGameState == STATE_END_QUIT)
	{
		m_State = STATE_END_QUIT;
		ReleaseNotPause();
	}

	return pStr;
}

//=============================================================================
// ゲームの相手の倒した敵の数を取得する処理
//=============================================================================
char *CGame::SetDataToBreakEnemy(char *pStr)
{
	// 番号を設定
	int nIdxClient = 0;
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		nIdxClient = pClient->GetClientId();
	}

	// 敵を倒した数を取得
	int nNumBreak = 0;
	int nWord = 0;
	for (int nCntType = 0; nCntType < CEnemy::TYPE_MAX; nCntType++)
	{
		nNumBreak = CFunctionLib::ReadInt(pStr, "");
		nWord = CFunctionLib::PopString(pStr, "");
		pStr += nWord;
		m_nNumBreakEnemy[(nIdxClient + 1) % MAX_NUM_PLAYER][nCntType] = nNumBreak;
	}

	// スコアを取得
	m_nScore[(nIdxClient + 1) % MAX_NUM_PLAYER] = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	return pStr;
}

//=============================================================================
// ゲームの敵のスポーン数を取得する処理
//=============================================================================
char *CGame::SetDataToSpawnEnemyCount(char *pStr)
{
	// スポーン数を読み取る
	int nSpawn = CFunctionLib::ReadInt(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	if (nSpawn > m_nSpawnEnemyCount)
	{
		m_nSpawnEnemyCount = nSpawn;

		// UIを整える
		CMap *pMap = GetMap();
		if (m_pUI != NULL && pMap != NULL)
		{
			m_pUI->ReCreateEnemyIcon(pMap->GetNumEnemyListData() - nSpawn);
		}
	}

	return pStr;
}

//=============================================================================
// ゲームのマップイベントを取得する処理
//=============================================================================
char *CGame::SetDataToMapEvent(char *pStr)
{
	// マップイベントを読み取る
	int nHinaEvent = CFunctionLib::ReadInt(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;
	int nNextEvent = CFunctionLib::ReadInt(pStr, "");
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	// イベントの種類によって処理わけ
	int nHinaEventOld = m_HinaEvent;
	m_HinaEvent = (HINAEVENT)nHinaEvent;
	m_nNextEvent = nNextEvent;

	if (nHinaEventOld == HINAEVENT_NORMAL && m_HinaEvent == HINAEVENT_EVENTSTART)
	{
		CreateEventStartLogo();
	}
	else if (nHinaEventOld == HINAEVENT_EVENTSTART && m_HinaEvent == HINAEVENT_CHERRYBLOSSOMS)
	{
		CManager::GetSound()->PlaySound(GAME_BLOSSOMS_BGMIDX);
		ReleaseEventStartLogo();
	}
	else if (nHinaEventOld == HINAEVENT_CHERRYBLOSSOMS && m_HinaEvent == HINAEVENT_NORMAL)
	{
		CManager::GetSound()->StopSound(GAME_BLOSSOMS_BGMIDX);
	}
	else if (nHinaEventOld == HINAEVENT_EVENTSTART && m_HinaEvent == HINAEVENT_DROP_HINAARARE)
	{
		CManager::GetSound()->PlaySound(GAME_HINAARARE_BGMIDX);
		ReleaseEventStartLogo();
	}
	else if (nHinaEventOld == HINAEVENT_DROP_HINAARARE && m_HinaEvent == HINAEVENT_NORMAL)
	{
		CManager::GetSound()->StopSound(GAME_HINAARARE_BGMIDX);
	}

	return pStr;
}

//=============================================================================
// ゲームの相手プレイヤーの弾に当たったかどうか取得する
//=============================================================================
char *CGame::SetDataToHitBullet(char *pStr)
{
	// 当たったかどうか読み取る
	m_bHitBulletFlag = CFunctionLib::ReadBool(pStr, "");
	int nWord = 0;
	nWord = CFunctionLib::PopString(pStr, "");
	pStr += nWord;

	int nIdxClient = 0;
	CClient *pClient = CManager::GetClient();
	if (pClient != NULL)
	{
		nIdxClient = pClient->GetClientId();
	}

	if (m_bHitBulletFlag == true && m_pPlayer[(nIdxClient + 1) % MAX_NUM_PLAYER] != NULL)
	{// 弾に当たっていた
		m_pPlayer[(nIdxClient + 1) % MAX_NUM_PLAYER]->SetState(CPlayer::STATE_STOP);
		m_pPlayer[(nIdxClient + 1) % MAX_NUM_PLAYER]->SetStateCounter(0);
	}

	return pStr;
}


//*****************************************************************************
//
// 状態による処理分け用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームのステージセレクト状態の更新処理
//=============================================================================
void CGame::StageSelectUpdate(void)
{
	// 背景生成
	if (m_pStageBg == NULL)
	{
		CreateStageBg();
	}

	// ステージロゴ生成
	if (m_pStageLogo == NULL)
	{
		CreateStageLogo();
	}

	// ステージ番号生成
	if (m_pStageNumber == NULL)
	{
		CreateStageNumber();
	}


	int nIdxClient = 0;
	CClient *pClient = CManager::GetClient();
	if(pClient != NULL)
	{
		nIdxClient = pClient->GetClientId();
		if (nIdxClient != 0)
		{
			ReleaseStageNumber();
			CalcStageDigits();
			CreateStageNumber();
		}
	}

	CInputKeyboard *pKey = CManager::GetKeyboard();
	if (pKey == NULL || CManager::GetClient()->GetClientId() != 0) return;

	if (pKey->GetTrigger(DIK_W) == true || pKey->GetRepeat(DIK_W) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_0) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_0) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_16) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_16) == true)
	{// 上方向の入力がされた
		if (m_nStageIdx < GAME_MAPSTAGE_MAX - 1)
		{
			m_nStageIdx++;
			m_nMapIdx++;
			ReleaseStageNumber();
			CalcStageDigits();
			CreateStageNumber();
		}
	}
	else if (pKey->GetTrigger(DIK_S) == true || pKey->GetRepeat(DIK_S) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_1) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_1) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_17) == true ||
		CManager::GetXInput()->GetRepeat(0, CXInput::XIJS_BUTTON_17) == true)
	{// 下方向の入力がされた
		if (m_nStageIdx > 0)
		{
			m_nStageIdx--;
			m_nMapIdx--;
			ReleaseStageNumber();
			CalcStageDigits();
			CreateStageNumber();
		}
	}
	else if (pKey->GetTrigger(DIK_RETURN) == true ||
		CManager::GetXInput()->GetTrigger(0, CXInput::XIJS_BUTTON_11) == true)
	{// 決定ボタンが押された
		SetState(STATE_STAGE_DISP);
		ReleaseMap();
		DeleteGameObject();
		CreateMap();
		m_nGameCounter = 0;
	}
}

//=============================================================================
// ゲームのステージ番号を表示する状態の更新処理
//=============================================================================
void CGame::StageDispUpdate(void)
{
	// ゲーム内リザルトに必要なものを破棄する
	ReleaseGameResult();

	// 背景生成
	if (m_pStageBg == NULL)
	{
		CreateStageBg();
	}

	// ステージロゴ生成
	if (m_pStageLogo == NULL)
	{
		CreateStageLogo();
	}

	// ステージ番号生成
	if (m_pStageNumber == NULL)
	{
		CreateStageNumber();
	}

	// カウンター加算
	m_nStateCounter++;
	if (m_nStateCounter % GAME_STAGEDISP_TIME == 0)
	{// ステージ番号を出してからある程度たった
		SetState(STATE_NORMAL);
		ReleaseStageBg();
		ReleaseStageLogo();
		ReleaseStageNumber();
		CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX);
	}
}

//=============================================================================
// ゲームの通常状態の更新処理
//=============================================================================
void CGame::NormalUpdate(void)
{
	// クリーム靄チェック処理
	MistCheck();

	m_bPauseOpen = false;

	CInputKeyboard *pKey = CManager::GetKeyboard();
	CXInput *pXInput = CManager::GetXInput();
	if (pKey == NULL) return;

	// ポーズON/OFF切り替え
	if (pKey->GetTrigger(DIK_P) == true)
	{// キーボード判定
		m_State = STATE_PAUSE;
		CreatePause();
		m_bPauseOpen = true;
		CManager::GetSound()->PlaySound(GAME_SE_PAUSE_OPEN_IDX);
		CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX, true);
		CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX, true);
		CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX + 1, true);
		CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX + 2, true);
		CManager::GetSound()->StopSound(GAME_BLOSSOMS_BGMIDX);
		CManager::GetSound()->StopSound(GAME_HINAARARE_BGMIDX);
	}
	else if (pXInput != NULL)
	{// コントローラー判定
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{
			if (pXInput->GetTrigger(nCntPlayer, CXInput::XIJS_BUTTON_4) == true)
			{
				m_State = STATE_PAUSE;
				m_bPauseOpen = true;
				CreatePause(nCntPlayer);
				CManager::GetSound()->PlaySound(GAME_SE_PAUSE_OPEN_IDX);
				CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX, true);
				CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX, true);
				CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX + 1, true);
				CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX + 2, true);
				CManager::GetSound()->StopSound(GAME_BLOSSOMS_BGMIDX);
				CManager::GetSound()->StopSound(GAME_HINAARARE_BGMIDX);
			}
		}
	}

#ifdef GAME_DEBUG  // デバッグコマンド適用時
	// ステージセレクトデバッグ
	int nIdxClient = 0;
	if (CManager::GetClient() != NULL)
	{
		nIdxClient = CManager::GetClient()->GetClientId();
	}
	if (nIdxClient == 0)
	{
		if (pKey->GetTrigger(DIK_1) == true && pKey->GetPress(DIK_LSHIFT) == true)
		{
			SetState(STATE_PREV_MAP);
			StopBGM();
		}
		else if (pKey->GetTrigger(DIK_2) == true && pKey->GetPress(DIK_LSHIFT) == true)
		{
			SetState(STATE_NEXT_MAP);
			StopBGM();
		}
		else if (pKey->GetTrigger(DIK_3) == true && pKey->GetPress(DIK_LSHIFT) == true)
		{
			SetState(STATE_STAGE_SELECT);
			StopBGM();
		}
	}

	// 状態チェックデバッグ
	if (pKey->GetTrigger(DIK_4) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		SetState(STATE_GAMEOVER);
	}
	else if (pKey->GetTrigger(DIK_5) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		SetState(STATE_END);
	}

	// プレイヤーを倒すデバッグ
	if (pKey->GetTrigger(DIK_6) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		if (m_pPlayer[nIdxClient] != NULL)
		{
			m_pPlayer[nIdxClient]->SetDeathEffect();
			m_pPlayer[nIdxClient]->Uninit();
			m_pPlayer[nIdxClient] = NULL;
		}
	}


	// アイテムイベントデバッグ
	if (pKey->GetTrigger(DIK_F1) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Star(0);
	}
	else if (pKey->GetTrigger(DIK_F2) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Star(1);
	}
	else if (pKey->GetTrigger(DIK_F3) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Grenade(0);
	}
	else if (pKey->GetTrigger(DIK_F4) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_1Up(0);
	}
	else if (pKey->GetTrigger(DIK_F5) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_1Up(1);
	}
	else if (pKey->GetTrigger(DIK_F6) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Scoop(0);
	}
	else if (pKey->GetTrigger(DIK_F7) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Clock(0);
	}
	else if (pKey->GetTrigger(DIK_F8) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Helmet(0);
	}
	else if (pKey->GetTrigger(DIK_F9) == true && pKey->GetPress(DIK_LSHIFT) == true)
	{
		ItemEvent_Helmet(1);
	}

	// ステージイベントデバッグ
	if (CManager::GetKeyboard()->GetTrigger(DIK_1) == true && CManager::GetKeyboard()->GetPress(DIK_RSHIFT) == true)
	{
		m_nNextEvent = HINAEVENT_CHERRYBLOSSOMS;
		m_HinaEvent = HINAEVENT_EVENTSTART;
		m_nEventCounter = 0;
		m_nNotEventCounter = 0;
		CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + (m_nNextEvent - 1));
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_2) == true && CManager::GetKeyboard()->GetPress(DIK_RSHIFT) == true)
	{
		m_nNextEvent = HINAEVENT_DROP_ITEM;
		m_HinaEvent = HINAEVENT_EVENTSTART;
		m_nEventCounter = 0;
		m_nNotEventCounter = 0;
		CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + (m_nNextEvent - 1));
	}
	else if (CManager::GetKeyboard()->GetTrigger(DIK_3) == true && CManager::GetKeyboard()->GetPress(DIK_RSHIFT) == true)
	{
		m_nNextEvent = HINAEVENT_DROP_HINAARARE;
		m_HinaEvent = HINAEVENT_EVENTSTART;
		m_nEventCounter = 0;
		m_nNotEventCounter = 0;
		CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + (m_nNextEvent - 1));
	}

	CDebugProc::Print(1, "デバッグコマンド\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [1]キー        : 前のステージにする\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [2]キー        : 次のステージにする\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [3]キー        : ステージセレクト状態にする\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [4]キー        : ゲームオーバー状態にする\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [5]キー        : 終了状態にする\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [6]キー        : プレイヤーを倒して残機を減らす\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [F1]～[F2]キー : スターアイテムイベント\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [F3]キー       : グレネードアイテムイベント\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [F4]～[F5]キー : 1Upアイテムイベント\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [F6]キー       : スコップアイテムイベント\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [F7]キー       : 時計アイテムイベント\n");
	CDebugProc::Print(1, "[LSHIFT]キー + [F8]～[F9]キー : ヘルメットアイテムイベント\n");
	CDebugProc::Print(1, "[RSHIFT]キー + [1]キー        : 桜吹雪マップイベント開始\n");
	CDebugProc::Print(1, "[RSHIFT]キー + [2]キー        : アイテム投下マップイベント開始\n");
	CDebugProc::Print(1, "[RSHIFT]キー + [3]キー        : ひなあられ投下マップイベント開始\n\n");

#endif

	CDebugProc::Print(1, "通常状態\n");
}

//=============================================================================
// ゲームのポーズ状態の更新処理
//=============================================================================
void CGame::PauseUpdate(void)
{
	CInputKeyboard *pKey = CManager::GetKeyboard();
	CXInput *pXInput = CManager::GetXInput();
	if (pKey == NULL && pXInput == NULL) return;

	// ポーズの更新
	if (m_pPause != NULL)
	{
		m_pPause->Update();
	}

	// ポーズON/OFF切り替え
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{// 1人プレイならば
		if (pKey->GetTrigger(DIK_P) == true || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_4))
		{
			m_State = STATE_NORMAL;
			ReleasePause();
			CScene::DeathCheck();
			CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX, true);

			// マップイベント待機状態ならば次のイベントによって処理分けする
			if (m_HinaEvent == HINAEVENT_EVENTSTART)
			{
				switch (m_nNextEvent)
				{
				case HINAEVENT_CHERRYBLOSSOMS:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX, true);
					break;
				case HINAEVENT_DROP_ITEM:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + 1, true);
					break;
				case HINAEVENT_DROP_HINAARARE:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + 2, true);
					break;
				}
			}

			// マップイベント中だったら再生しなおす
			switch (m_HinaEvent)
			{
			case HINAEVENT_CHERRYBLOSSOMS:
				CManager::GetSound()->PlaySound(GAME_BLOSSOMS_BGMIDX);
				break;
			case HINAEVENT_DROP_ITEM:
				break;
			case HINAEVENT_DROP_HINAARARE:
				CManager::GetSound()->PlaySound(GAME_HINAARARE_BGMIDX);
				break;
			}
		}
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL2P)
	{// ローカル2人プレイならば
		// どちらのプレイヤーがポーズを開いたのか取得
		int nOpneId = 0;
		if (m_pPause != NULL)
		{
			nOpneId = m_pPause->GetOpenId();
		}

		// ポーズ解除判定
		if (pKey->GetTrigger(DIK_P) == true || pXInput->GetTrigger(nOpneId, CXInput::XIJS_BUTTON_4))
		{
			m_State = STATE_NORMAL;
			ReleasePause();
			CScene::DeathCheck();
			CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX, true);

			// マップイベント待機状態ならば次のイベントによって処理分けする
			if (m_HinaEvent == HINAEVENT_EVENTSTART)
			{
				switch (m_nNextEvent)
				{
				case HINAEVENT_CHERRYBLOSSOMS:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX, true);
					break;
				case HINAEVENT_DROP_ITEM:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + 1, true);
					break;
				case HINAEVENT_DROP_HINAARARE:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + 2, true);
					break;
				}
			}

			// マップイベント中だったら再生しなおす
			switch (m_HinaEvent)
			{
			case HINAEVENT_CHERRYBLOSSOMS:
				CManager::GetSound()->PlaySound(GAME_BLOSSOMS_BGMIDX);
				break;
			case HINAEVENT_DROP_ITEM:
				break;
			case HINAEVENT_DROP_HINAARARE:
				CManager::GetSound()->PlaySound(GAME_HINAARARE_BGMIDX);
				break;
			}
		}
	}
	else if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// オンライン2人プレイならば
		// ポーズを生成していなければ処理しない
		if (m_pPause == NULL)
		{
			return;
		}

		// ポーズ解除判定
		if (pKey->GetTrigger(DIK_P) == true || pXInput->GetTrigger(0, CXInput::XIJS_BUTTON_4))
		{
			m_State = STATE_NORMAL;
			ReleasePause();
			CScene::DeathCheck();
			CManager::GetSound()->PlaySound(GAME_BGM_HINAMAP_IDX, true);

			// マップイベント待機状態ならば次のイベントによって処理分けする
			if (m_HinaEvent == HINAEVENT_EVENTSTART)
			{
				switch (m_nNextEvent)
				{
				case HINAEVENT_CHERRYBLOSSOMS:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX, true);
					break;
				case HINAEVENT_DROP_ITEM:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + 1, true);
					break;
				case HINAEVENT_DROP_HINAARARE:
					CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + 2, true);
					break;
				}
			}

			// マップイベント中だったら再生しなおす
			switch (m_HinaEvent)
			{
			case HINAEVENT_CHERRYBLOSSOMS:
				CManager::GetSound()->PlaySound(GAME_BLOSSOMS_BGMIDX);
				break;
			case HINAEVENT_DROP_ITEM:
				break;
			case HINAEVENT_DROP_HINAARARE:
				CManager::GetSound()->PlaySound(GAME_HINAARARE_BGMIDX);
				break;
			}
		}
	}
}

//=============================================================================
// ゲームのゲームオーバー状態の更新処理
//=============================================================================
void CGame::GameOverUpdate(void)
{
	// ゲームオーバーのロゴを出す
	if (m_pGameOverLogo == NULL)
	{
		CreateGameOverLogo();
		return;
	}

	// BGMを止める
	StopBGM();

	// ゲームオーバーロゴポリゴンを上げる
	GameOverLogoUp();

	// カウンター加算
	m_nStateCounter++;
	if (m_nStateCounter % GAME_GAMEOVER_TIMING == 0)
	{
		CFade *pFade = CManager::GetFade();
		if (pFade == NULL) return;
		if (pFade->GetFade() != CFade::FADE_NONE) return;

		// 次のモードに設定
		pFade->SetFade(CManager::MODE_TITLE);
	}

	CDebugProc::Print(1, "ゲームオーバー状態\n");
}

//=============================================================================
// ゲームのゲームオーバーロゴを上にあげる処理
//=============================================================================
void CGame::GameOverLogoUp(void)
{
	if (m_pGameOverLogo != NULL)
	{
		D3DXVECTOR3 GameOverLogoPos = m_pGameOverLogo->GetPos();
		GameOverLogoPos.y -= GAME_GAMEOVER_POSUP;
		if (GameOverLogoPos.y <= m_pGameOverLogo->GetHeight() + GAME_GAMEOVER_POSYMAX)
		{
			GameOverLogoPos.y += GAME_GAMEOVER_POSUP;
		}

		m_pGameOverLogo->SetPos(GameOverLogoPos);
		m_pGameOverLogo->SetVtxBuffPos();
	}
}

//=============================================================================
// ゲームのリザルト表示状態の更新処理
//=============================================================================
void CGame::ResultUpdate(void)
{
	CDebugProc::Print(1, "ゲーム内リザルト状態\n");
	CDebugProc::Print(1, "%d\n", m_nStateCounter);

	// 音を止めておく
	CManager::GetSound()->StopSound(GAME_BLOSSOMS_BGMIDX);
	CManager::GetSound()->StopSound(GAME_HINAARARE_BGMIDX);

	// 進行度によって処理わけ
	switch (m_GameResultAddvance)
	{
	case GAMERESULT_NONE:
		ResultUpdate_None();
		break;
	case GAMERESULT_NORMALENEMY:
		ResultUpdate_Normal();
		break;
	case GAMERESULT_ARMOREENEMY:
		ResultUpdate_Armore();
		break;
	case GAMERESULT_FASTENEMY:
		ResultUpdate_Fast();
		break;
	case GAMERESULT_HEAVYENEMY:
		ResultUpdate_Heavy();
		break;
	case GAMERESULT_TORTAL:
		ResultUpdate_Tortal();
		break;
	}
}

//=============================================================================
// ゲームの何もしないときのゲーム内リザルト更新処理
//=============================================================================
void CGame::ResultUpdate_None(void)
{
	m_nStateCounter++;
	if (m_nStateCounter % GAME_NEXTSCORE_TIMING == 0)
	{// 次のスコアを出すタイミングになった
		// 進行度を加算
		m_GameResultAddvance = (GAMERESULT)(m_GameResultAddvance + 1);

		// 敵のスコアを生成
		CreateGameResult_EnemyScore_Number();

		// カウンターリセット
		m_nStateCounter = 0;
		m_bNextGameResult = false;

		// 音を鳴らす
		CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
	}
}

//=============================================================================
// ゲームの通常のタンクスコア進行度のゲーム内リザルト更新処理
//=============================================================================
void CGame::ResultUpdate_Normal(void)
{
	int nNumBreak[MAX_NUM_PLAYER] = {};
	bool bNextResult[MAX_NUM_PLAYER] = { false, false };

	// カウンター加算
	m_nStateCounter++;
	if (m_nStateCounter % GAME_SCOREUP_TIMING == 0)
	{// スコアをアップさせるタイミングだった
		for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
		{
			if (m_apNumBreakEnemy[nCnt][CEnemy::TYPE_NORMAL] != NULL)
			{// 敵が生成されている
			    // 現在の数字を取得
				nNumBreak[nCnt] = m_apNumBreakEnemy[nCnt][CEnemy::TYPE_NORMAL]->GetNumber();

				if (nNumBreak[nCnt] < m_nNumBreakEnemy[nCnt][CEnemy::TYPE_NORMAL])
				{// 現在の数字が倒した数より小さい
					// 次のモードに行かないようにフラグを消す
					m_bNextGameResult = false;

					// 数字を増やす
					nNumBreak[nCnt]++;

					// 表示する数字を設定
					m_apNumBreakEnemy[nCnt][CEnemy::TYPE_NORMAL]->SetNumber(nNumBreak[nCnt]);

					// カウンターリセット
					m_nStateCounter = 0;

					// 音を鳴らす
					CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
				}
				else
				{// 現在の数字が倒した数になっている
					bNextResult[nCnt] = true;
				}
			}
			else
			{// 生成されてなければ次の更新へ行ける状態に
				bNextResult[nCnt] = true;
			}

			// 表示するスコアも変える
			if (m_apEnemyScore[nCnt][CEnemy::TYPE_NORMAL] != NULL)
			{
				// 表示する数字を設定
				m_apEnemyScore[nCnt][CEnemy::TYPE_NORMAL]->SetNumber(nNumBreak[nCnt] * ENEMY_SCORE_NORMAL);
			}
		}
	}

	// まだスコアを増やしているならこの先は処理しない
	if (bNextResult[0] == true && bNextResult[MAX_NUM_PLAYER - 1] == true)
	{
		m_bNextGameResult = true;
	}
	if (m_bNextGameResult == false) { return; }

	// カウンター加算
	if (m_nStateCounter % GAME_NEXTSCORE_TIMING == 0)
	{// 次のスコアを出すタイミングになった
	    // 進行度を加算
		m_GameResultAddvance = (GAMERESULT)(m_GameResultAddvance + 1);

		// 敵スコアを生成
		CreateGameResult_EnemyScore_Number();

		// カウンターリセット
		m_nStateCounter = 0;

		// 音を鳴らす
		CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
	}
}

//=============================================================================
// ゲームの装甲車タンクスコア進行度のゲーム内リザルト更新処理
//=============================================================================
void CGame::ResultUpdate_Armore(void)
{
	int nNumBreak[MAX_NUM_PLAYER] = {};
	bool bNextResult[MAX_NUM_PLAYER] = { false, false };

	// カウンター加算
	m_nStateCounter++;
	if (m_nStateCounter % GAME_SCOREUP_TIMING == 0)
	{// スコアをアップさせるタイミングだった
		for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
		{
			if (m_apNumBreakEnemy[nCnt][CEnemy::TYPE_ARMORE] != NULL)
			{// 敵が生成されている
			    // 現在の数字を取得
				nNumBreak[nCnt] = m_apNumBreakEnemy[nCnt][CEnemy::TYPE_ARMORE]->GetNumber();

				if (nNumBreak[nCnt] < m_nNumBreakEnemy[nCnt][CEnemy::TYPE_ARMORE])
				{// 現在の数字が倒した数より小さい
				    // 次のモードに行かないようにフラグを消す
					m_bNextGameResult = false;

					// 数字を増やす
					nNumBreak[nCnt]++;

					// 表示する数字を設定
					m_apNumBreakEnemy[nCnt][CEnemy::TYPE_ARMORE]->SetNumber(nNumBreak[nCnt]);

					// カウンターリセット
					m_nStateCounter = 0;

					// 音を鳴らす
					CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
				}
				else
				{// 現在の数字が倒した数になっている
					bNextResult[nCnt] = true;
				}
			}
			else
			{// 生成されてなければ次の更新へ行ける状態に
				bNextResult[nCnt] = true;
			}

			// 表示するスコアも変える
			if (m_apEnemyScore[nCnt][CEnemy::TYPE_ARMORE] != NULL)
			{
				// 表示する数字を設定
				m_apEnemyScore[nCnt][CEnemy::TYPE_ARMORE]->SetNumber(nNumBreak[nCnt] * ENEMY_SCORE_ARMORE);
			}
		}
	}

	// まだスコアを増やしているならこの先は処理しない
	if (bNextResult[0] == true && bNextResult[MAX_NUM_PLAYER - 1] == true)
	{
		m_bNextGameResult = true;
	}
	if (m_bNextGameResult == false) { return; }

	if (m_nStateCounter % GAME_NEXTSCORE_TIMING == 0)
	{// 次のスコアを出すタイミングになった
	    // 進行度を加算
		m_GameResultAddvance = (GAMERESULT)(m_GameResultAddvance + 1);

		// 敵スコアを生成
		CreateGameResult_EnemyScore_Number();

		// カウンターリセット
		m_nStateCounter = 0;

		// 音を鳴らす
		CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
	}
}

//=============================================================================
// ゲームの速射砲タンクスコア進行度のゲーム内リザルト更新処理
//=============================================================================
void CGame::ResultUpdate_Fast(void)
{
	int nNumBreak[MAX_NUM_PLAYER] = {};
	bool bNextResult[MAX_NUM_PLAYER] = { false, false };

	// カウンター加算
	m_nStateCounter++;
	if (m_nStateCounter % GAME_SCOREUP_TIMING == 0)
	{// スコアをアップさせるタイミングだった
		for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
		{
			if (m_apNumBreakEnemy[nCnt][CEnemy::TYPE_FAST] != NULL)
			{// 敵が生成されている
			    // 現在の数字を取得
				nNumBreak[nCnt] = m_apNumBreakEnemy[nCnt][CEnemy::TYPE_FAST]->GetNumber();

				if (nNumBreak[nCnt] < m_nNumBreakEnemy[nCnt][CEnemy::TYPE_FAST])
				{// 現在の数字が倒した数より小さい
				    // 次のモードに行かないようにフラグを消す
					m_bNextGameResult = false;

					// 数字を増やす
					nNumBreak[nCnt]++;

					// 表示する数字を設定
					m_apNumBreakEnemy[nCnt][CEnemy::TYPE_FAST]->SetNumber(nNumBreak[nCnt]);

					// カウンターリセット
					m_nStateCounter = 0;

					// 音を鳴らす
					CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
				}
				else
				{// 現在の数字が倒した数になっている
					bNextResult[nCnt] = true;
				}
			}
			else
			{// 生成されてなければ次の更新へ行ける状態に
				bNextResult[nCnt] = true;
			}

			// 表示するスコアも変える
			if (m_apEnemyScore[nCnt][CEnemy::TYPE_FAST] != NULL)
			{
				// 表示する数字を設定
				m_apEnemyScore[nCnt][CEnemy::TYPE_FAST]->SetNumber(nNumBreak[nCnt] * ENEMY_SCORE_FAST);
			}
		}
	}

	// まだスコアを増やしているならこの先は処理しない
	if (bNextResult[0] == true && bNextResult[MAX_NUM_PLAYER - 1] == true)
	{
		m_bNextGameResult = true;
	}
	if (m_bNextGameResult == false) { return; }

	if (m_nStateCounter % GAME_NEXTSCORE_TIMING == 0)
	{// 次のスコアを出すタイミングになった
	    // 進行度を加算
		m_GameResultAddvance = (GAMERESULT)(m_GameResultAddvance + 1);

		// 敵スコアを生成
		CreateGameResult_EnemyScore_Number();

		// カウンターリセット
		m_nStateCounter = 0;

		// 音を鳴らす
		CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
	}
}

//=============================================================================
// ゲームのヘビータンクスコア進行度のゲーム内リザルト更新処理
//=============================================================================
void CGame::ResultUpdate_Heavy(void)
{
	int nNumBreak[MAX_NUM_PLAYER] = {};
	bool bNextResult[MAX_NUM_PLAYER] = { false, false };

	// カウンター加算
	m_nStateCounter++;
	if (m_nStateCounter % GAME_SCOREUP_TIMING == 0)
	{// スコアをアップさせるタイミングだった
		for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
		{
			if (m_apNumBreakEnemy[nCnt][CEnemy::TYPE_HEAVY] != NULL)
			{// 敵が生成されている
			    // 現在の数字を取得
				nNumBreak[nCnt] = m_apNumBreakEnemy[nCnt][CEnemy::TYPE_HEAVY]->GetNumber();

				if (nNumBreak[nCnt] < m_nNumBreakEnemy[nCnt][CEnemy::TYPE_HEAVY])
				{// 現在の数字が倒した数より小さい
			   	    // 次のモードに行かないようにフラグを消す
					m_bNextGameResult = false;

					// 数字を増やす
					nNumBreak[nCnt]++;

					// 表示する数字を設定
					m_apNumBreakEnemy[nCnt][CEnemy::TYPE_HEAVY]->SetNumber(nNumBreak[nCnt]);

					// カウンターリセット
					m_nStateCounter = 0;

					// 音を鳴らす
					CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);
				}
				else
				{// 現在の数字が倒した数になっている
					bNextResult[nCnt] = true;
				}
			}
			else
			{// 生成されてなければ次の更新へ行ける状態に
				bNextResult[nCnt] = true;
			}

			// 表示するスコアも変える
			if (m_apEnemyScore[nCnt][CEnemy::TYPE_HEAVY] != NULL)
			{
				// 表示する数字を設定
				m_apEnemyScore[nCnt][CEnemy::TYPE_HEAVY]->SetNumber(nNumBreak[nCnt] * ENEMY_SCORE_HEAVY);
			}
		}
	}

	// まだスコアを増やしているならこの先は処理しない
	if (bNextResult[0] == true && bNextResult[MAX_NUM_PLAYER - 1] == true)
	{
		m_bNextGameResult = true;
	}
	if (m_bNextGameResult == false) { return; }


	if (m_nStateCounter % GAME_NEXTSCORE_TIMING == 0)
	{// 次のスコアを出すタイミングになった
	    // 進行度を加算
		m_GameResultAddvance = (GAMERESULT)(m_GameResultAddvance + 1);

		// トータルスコアを生成
		CreateGameResult_TortalScore_Number();

		// 音を鳴らす
		CManager::GetSound()->PlaySound(GAME_SE_GAMERESULT_SCOREUP_IDX);

		// カウンターリセット
		m_nStateCounter = 0;
	}
}

//=============================================================================
// ゲームのトータルスコア進行度のゲーム内リザルト更新処理
//=============================================================================
void CGame::ResultUpdate_Tortal(void)
{
	// カウンター加算
	m_nStateCounter++;
	if (m_nStateCounter % GAME_NEXTSCORE_TIMING == 0 && CTitle::GetGameMode() != CTitle::GAMEMODE_LOCAL1P &&
		m_bAddBonus == false)
	{// ボーナスを付与していない状態で付与するタイミングになった
		CreateGameResult_BonusScore();
	}

	if (m_nStateCounter % GAME_NEXTMODE_TIMING == 0)
	{
		if (m_nStageIdx >= GAME_MAPSTAGE_MAX - 1)
		{// これ以上ステージがない
			CFade *pFade = CManager::GetFade();
			if (pFade == NULL) return;
			if (pFade->GetFade() != CFade::FADE_NONE) return;

			// 次のモードに設定
			pFade->SetFade(CManager::MODE_RESULT);
		}
		else
		{// まだ次のマップが存在する
		    // クライアント番号を取得
			int nIdxClient = 0;
			if (CManager::GetClient() != NULL)
			{
				nIdxClient = CManager::GetClient()->GetClientId();
			}
			if (nIdxClient == 0)
			{// ホストだった
				SetState(STATE_CHANGE_MAP);
			}
		}
	}
}

//=============================================================================
// ゲームのマップを変える状態の更新処理
//=============================================================================
void CGame::ChangeMapUpdate(void)
{
	// クライアント番号を取得
	int nIdxClient = 0;
	if (CManager::GetClient() != NULL)
	{
		nIdxClient = CManager::GetClient()->GetClientId();
	}
	if (nIdxClient == 0)
	{// ホストだったら
		m_nMapIdx++;
		m_nStageIdx++;
		CalcStageDigits();
		ReleaseMap();
		DeleteGameObject();
		CreateMap();
		m_nGameCounter = 0;
		SetState(STATE_STAGE_DISP);
		CManager::GetSound()->PlaySound(GAME_SE_FANFARE_IDX);
	}
	else
	{// ホストじゃなかったら
		m_nMapIdx++;
		m_nStageIdx++;
		CalcStageDigits();
		ReleaseMap();
		DeleteGameObject();
		CreateMap();
		m_nGameCounter = 0;
		SetState(STATE_STAGE_DISP);
		CManager::GetSound()->PlaySound(GAME_SE_FANFARE_IDX);
	}

	// データクリア
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
}

//=============================================================================
// ゲームの終了状態の更新処理
//=============================================================================
void CGame::EndUpdate(void)
{
	CDebugProc::Print(1, "終了状態\n");

	// カウンター加算
	m_nStateCounter++;
	if (m_nStateCounter % GAME_RESULT_TIMING == 0)
	{// ある程度時間がたった
		SetState(STATE_RESULT);
		CreateGameResult();

		// 音楽を停止
		StopBGM();
	}
}

//=============================================================================
// ゲームの終了状態(RETRY)の更新処理
//=============================================================================
void CGame::EndRetryUpdate(void)
{
	CDebugProc::Print(1, "ポーズからゲームをやり直す状態\n");

	CFade *pFade = CManager::GetFade();
	if (pFade == NULL) { return; }

	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		pFade->SetFade(CManager::MODE_GAME);

		// 音楽を停止
		StopBGM();
	}
}

//=============================================================================
// ゲームの終了状態(QUIT)の更新処理
//=============================================================================
void CGame::EndQuitUpdate(void)
{
	CDebugProc::Print(1, "ポーズからタイトルに戻る状態\n");

	CFade *pFade = CManager::GetFade();
	if (pFade == NULL) { return; }

	if (pFade->GetFade() == CFade::FADE_NONE)
	{
		pFade->SetFade(CManager::MODE_TITLE);

		// 音楽を停止
		StopBGM();
	}
}

//=============================================================================
// ゲームのBGMを停止させる処理
//=============================================================================
void CGame::StopBGM(void)
{
	CManager::GetSound()->StopSound(GAME_BGM_HINAMAP_IDX);
	CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX, true);
	CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX + 1, true);
	CManager::GetSound()->StopSound(GAME_MAPEVENT_SOUNDIDX + 2, true);
	CManager::GetSound()->StopSound(GAME_BLOSSOMS_BGMIDX);
	CManager::GetSound()->StopSound(GAME_HINAARARE_BGMIDX);
}

//=============================================================================
// ゲームのクリーム靄を消すかどうか判定する処理
//=============================================================================
void CGame::MistCheck(void)
{
	// クリーム靄が生成されていなければ処理終了
	if (m_pCreamMist == NULL) { return; }

	// カウンターを減らす
	m_nMistCounter++;
	if (m_nMistCounter >= GAME_CREAMMIST_APPAER_TIME)
	{// クリーム靄が出てから一定時間たった
		D3DXCOLOR col = m_pCreamMist->GetCol();
		col.a -= GAME_CREAMMIST_ALPHA_CUT;
		m_pCreamMist->SetCol(col);
		m_pCreamMist->SetVtxBuffCol();
		if (col.a <= 0.0f)
		{
			ReleaseCreamMist();
		}
	}
}



//*****************************************************************************
//
// アイテムイベント処理用関数
// Auther : Hodaka Niwa & Jukiya Hayakawa
//
//*****************************************************************************
//=============================================================================
// ゲームのスターアイテムのイベント処理
//=============================================================================
void CGame::ItemEvent_Star(int nPlayerNumber)
{
	// アビリティ変更
	if (m_pPlayer[nPlayerNumber] != NULL)
	{
		m_pPlayer[nPlayerNumber]->SwitchAbility();
	}

	// スコアを加算する
	m_nScore[nPlayerNumber] += GAME_ITEM_SCORE;

	// 音を鳴らす
	CManager::GetSound()->PlaySound(GAME_ITEMEVENT_SE_STAR);
}

//=============================================================================
// ゲームのグレネードアイテムのイベント処理
//=============================================================================
void CGame::ItemEvent_Grenade(int nPlayerNumber)
{
	// 今生成されている敵を消す
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
			{// 敵だった
				((CEnemy*)pScene)->SetDeathEffect();
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}

	// カメラを揺らす
	ItemEvent_Granade_Shake();

	// スコアを加算する
	m_nScore[nPlayerNumber] += GAME_ITEM_SCORE;

	// 音を鳴らす
	CManager::GetSound()->PlaySound(GAME_ITEMEVENT_SE_GRENADE);
}

//=============================================================================
// ゲームのグレネードを取った時にカメラを揺らす処理
//=============================================================================
void CGame::ItemEvent_Granade_Shake(void)
{
	// カメラを揺らす状態にする
	if (GetCameraManager() == NULL) { return; }
	CCamera *pCamera = GetCameraManager()->GetCamera();
	if (pCamera == NULL) { return; }

	if (pCamera->GetType() == CCamera::TYPE_GAME)
	{// ゲームで使用しているカメラである
		CGameCamera *pGameCamera = (CGameCamera*)pCamera;
		pGameCamera->StartShake(m_ItemEventData.nShakeTime, m_ItemEventData.fShake, m_ItemEventData.fShakeCut);
	}
}

//=============================================================================
// ゲームの1Upアイテムのイベント処理
//=============================================================================
void CGame::ItemEvent_1Up(int nPlayerNumber)
{
	// プレイヤーの残機数を増やす
	if (m_nPlayerStock[nPlayerNumber] < m_nPlayerStockMax)
	{// 残機の最大値より少ない
		m_nPlayerStock[nPlayerNumber]++;

		// UIの数字を増やす
		if (m_pUI != NULL)
		{
			m_pUI->ReCreatePlayerStock(&m_nPlayerStock[0]);
		}
	}

	// スコアを加算する
	m_nScore[nPlayerNumber] += GAME_ITEM_SCORE;

	// 音を鳴らす
	CManager::GetSound()->PlaySound(GAME_ITEMEVENT_SE_1UP);
}

//=============================================================================
// ゲームのスコップアイテムのイベント処理
//=============================================================================
void CGame::ItemEvent_Scoop(int nPlayerNumber)
{
	// 司令部の周りに壊せないブロックを置く

	// スコアを加算する
	m_nScore[nPlayerNumber] += GAME_ITEM_SCORE;

	// 音を鳴らす
	CManager::GetSound()->PlaySound(GAME_ITEMEVENT_SE_SCOOP);
}

//=============================================================================
// ゲームの時計アイテムのイベント処理
//=============================================================================
void CGame::ItemEvent_Clock(int nPlayerNumber)
{
	// 敵が動けない状態にする
	m_bEnemyMove = false;
	m_nEnemyMoveCounter = m_ItemEventData.nStop;

	// スコアを加算する
	m_nScore[nPlayerNumber] += GAME_ITEM_SCORE;

	// 音を鳴らす
	CManager::GetSound()->PlaySound(GAME_ITEMEVENT_SE_CLOCK);
}

//=============================================================================
// ゲームのヘルメットアイテムのイベント処理
//=============================================================================
void CGame::ItemEvent_Helmet(int nPlayerNumber)
{
	// スコアを加算する
	m_nScore[nPlayerNumber] += GAME_ITEM_SCORE;

	// 音を鳴らす
	CManager::GetSound()->PlaySound(GAME_ITEMEVENT_SE_HELMET);
}


//*****************************************************************************
//
// スコア加算実行用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームの通常のタンクを倒した数を加算する処理
//=============================================================================
void CGame::ScoreUp_NormalEnemy(int nPlayerIdx)
{
	m_nNumBreakEnemy[nPlayerIdx][CEnemy::TYPE_NORMAL]++;
}

//=============================================================================
// ゲームの装甲車タンクを倒した数を加算する処理
//=============================================================================
void CGame::ScoreUp_ArmoreEnemy(int nPlayerIdx)
{
	m_nNumBreakEnemy[nPlayerIdx][CEnemy::TYPE_ARMORE]++;
}

//=============================================================================
// ゲームの速射砲タンクを倒した数を加算する処理
//=============================================================================
void CGame::ScoreUp_FastEnemy(int nPlayerIdx)
{
	m_nNumBreakEnemy[nPlayerIdx][CEnemy::TYPE_FAST]++;
}

//=============================================================================
// ゲームのヘビータンクを倒した数を加算する処理
//=============================================================================
void CGame::ScoreUp_HeavyEnemy(int nPlayerIdx)
{
	m_nNumBreakEnemy[nPlayerIdx][CEnemy::TYPE_HEAVY]++;
}



//*****************************************************************************
//
// マップイベント用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームのひな祭りのマップイベント処理
//=============================================================================
void CGame::MapEvent_Hinamatsuri(void)
{
	CDebugProc::Print(1, "ひな祭りイベント\n");

	// ポーズ中なら更新しない
	if (m_State == STATE_PAUSE || m_State == STATE_END_RETRY || m_State == STATE_END_QUIT) { return; }

	// 現在のイベント状態によって処理わけ
	switch (m_HinaEvent)
	{
	case HINAEVENT_NORMAL:
		MapEvent_Hinamatsuri_Normal();
		break;
	case HINAEVENT_CHERRYBLOSSOMS:
		MapEvent_Hinamatsuri_CherryBlossoms();
		break;
	case HINAEVENT_DROP_ITEM:
		MapEvent_Hinamatsuri_Drop_Item();
		break;
	case HINAEVENT_DROP_HINAARARE:
		MapEvent_Hinamatsuri_Drop_Hinaarare();
		break;
	case HINAEVENT_EVENTSTART:
		MapEvent_Hinamatsuri_EventStart();
		break;
	}
}

//=============================================================================
// ゲームのひな祭りの通常のマップイベント処理
//=============================================================================
void CGame::MapEvent_Hinamatsuri_Normal(void)
{
	CDebugProc::Print(1, "通常イベント\n");

	// オンラインプレイの場合ホストじゃないなら処理しない
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		CClient *pClient = CManager::GetClient();
		if (pClient != NULL)
		{
			if (pClient->GetClientId() != 0) { return; }
		}
	}

	// 通常時でなければ処理しない
	if (m_State != STATE_NORMAL) { return; }

	// イベントカウンター加算
	m_nEventCounter++;
	m_nNotEventCounter++;
	if ((m_nRandomEventEva - m_nNotEventCounter) <= 0 || m_nEventCounter % (m_nRandomEventEva - m_nNotEventCounter) == 0)
	{// ランダムでイベントを引き起こす
		// カウンター初期化
		m_nNotEventCounter = 0;
		m_nEventCounter = 0;

		// 引き起こすイベントをランダムで決定
		m_nNextEvent = (HINAEVENT)(rand() % GAME_MAPEVENT_MAX) + 1;

		// マップイベント開始状態に設定
		m_HinaEvent = HINAEVENT_EVENTSTART;

		// 音を鳴らす
		CManager::GetSound()->PlaySound(GAME_MAPEVENT_SOUNDIDX + (m_nNextEvent - 1));
	}
}

//=============================================================================
// ゲームのひな祭りの桜が舞い散るマップイベント処理
//=============================================================================
void CGame::MapEvent_Hinamatsuri_CherryBlossoms(void)
{
	CDebugProc::Print(1, "桜舞い散るイベント\n");
	if (m_State != STATE_NORMAL) { return; }

	// 桜の花びら生成
	for (int nCnt = 0; nCnt < m_CherryBlossomsData.nLap; nCnt++)
	{
		CreateBlossoms();
	}

	// イベントカウンター進行
	m_nEventCounter++;

	// ホストではないなら処理しない
	int nIdxClient = 0;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		CClient *pClient = CManager::GetClient();
		if (pClient != NULL)
		{
			nIdxClient = pClient->GetClientId();
		}
	}
	if (nIdxClient != 0) { return; }

	if (m_nEventCounter >= m_CherryBlossomsData.nTime)
	{// カウンターが一定値になった
		m_nEventCounter = 0;
		m_HinaEvent = HINAEVENT_NORMAL;

		// 音を止める
		CManager::GetSound()->StopSound(GAME_BLOSSOMS_BGMIDX);
	}
}

//=============================================================================
// ゲームのひな祭りのアイテムを投下するマップイベント処理
//=============================================================================
void CGame::MapEvent_Hinamatsuri_Drop_Item(void)
{
	CDebugProc::Print(1, "アイテムドロップイベント\n");
	if (m_State != STATE_NORMAL) { return; }

	// ホストではないなら処理しない
	int nIdxClient = 0;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		CClient *pClient = CManager::GetClient();
		if (pClient != NULL)
		{
			nIdxClient = pClient->GetClientId();
		}
	}
	if (nIdxClient != 0) { return; }

	// 円筒の生成
	if (m_pItemCylinder == NULL)
	{
		// アイテムを出す位置をマス目からランダムで計算
		int nCntCheck = 0;
		do
		{// 川がある場所以外になったら抜ける
			int nMassX = rand() % MASS_BLOCK_X;
			int nMassZ = rand() % MASS_BLOCK_Z;
			m_ItemDropPos.x = (nMassX * MASS_SIZE_X) + MASS_SIZE_X_HALF - (MASS_BLOCK_X * MASS_SIZE_X_HALF);
			m_ItemDropPos.z = (nMassZ * MASS_SIZE_Z) + MASS_SIZE_Z_HALF - (MASS_BLOCK_Z * MASS_SIZE_Z_HALF);
			m_ItemDropPos.y = GAME_DROPITEM_POS_Y;
			nCntCheck++;
			if (nCntCheck == 3000)
			{// 3000回チェックしてだめなら抜ける(ストップバグ防止)
				break;
			}
		} while (MapEvent_Hinamatsuri_Drop_Item_CheckPos() == true);

		// 円筒生成
		m_pItemCylinder = CItemCylinder::Create(m_ItemDropPos, INITIALIZE_D3DXVECTOR3,
			GAME_DROPITEM_CYLINDER_COL, GAME_DROPITEM_CYLINDER_HEIGHT, GAME_DROPITEM_CYLINDER_RADIUS,
			GAME_DROPITEM_CYLINDER_XBLOCK, GAME_DROPITEM_CYLINDER_YBLOCK, GAME_DROPITEM_CYLINDER_PRIORITY);

		// 音を鳴らす
		CManager::GetSound()->PlaySound(GAME_DROPITEM_CYLINDER_SE_IDX);

		// 処理をここで終了させる
		return;
	}

	// 円筒を移動させる
	D3DXVECTOR3 CylinderPos = m_pItemCylinder->GetPos();
	CylinderPos.y += GAME_DROPITEM_CYLINDER_MOVE_Y;
	m_pItemCylinder->SetPos(CylinderPos);
	if (CylinderPos.y > 0.0f)
	{
		return;
	}
	m_pItemCylinder->SetPos(D3DXVECTOR3(CylinderPos.x, 0.0f, CylinderPos.z));

	// アイテムを生成する
	if (m_pItem == NULL)
	{
		// アイテムを出す種類を設定
		int nItemType = rand() % CItem::TYPE_MAX;

		// アイテム生成
		m_pItem = CreateItem(m_ItemDropPos, INITIALIZE_D3DXVECTOR3, (CItem::TYPE)nItemType);

		// 音を鳴らす
		CManager::GetSound()->PlaySound(GAME_DROPITEM_ITEM_SE_IDX);
	}

	// アイテムの座標を取得
	if (m_pItem == NULL)
	{
		m_nEventCounter = 0;
		m_HinaEvent = HINAEVENT_NORMAL;
		return;
	}
	D3DXVECTOR3 ItemPos = m_pItem->GetPos();

	// アイテムを下に移動させる
	ItemPos.y += GAME_DROPITEM_ITEM_MOVE_Y;

	// エフェクト管轄クラスを取得
	m_nEventCounter++;
	if (m_nEventCounter % GAME_DROPITEM_EFFECT_APPEAR == 0)
	{
		CEffectManager *pEffectManager = GetEffectManager();
		if (pEffectManager == NULL) { return; }

		// エフェクトを生成
		pEffectManager->SetEffect(ItemPos + D3DXVECTOR3(0.0f, 30.0f, 0.0f),
			INITIALIZE_D3DXVECTOR3, GAME_DROPITEM_EFFECT_IDX);
	}

	// アイテムが地面についていなければ処理終了
	if (ItemPos.y > 0.0f)
	{
		m_pItem->SetPos(ItemPos);
		return;
	}

	// アイテムの座標を地面に戻す
	ItemPos.y = 0.0f;

	// アイテムの座標を設定
	m_pItem->SetPos(ItemPos);

	// シリンダーに状態を設定
	m_pItemCylinder->SetState(CItemCylinder::STATE_DEATH);
	m_pItemCylinder = NULL;

	// ここまで処理が通ったら通常マップイベント状態に戻す
	m_pItem = NULL;
	m_nEventCounter = 0;
	m_HinaEvent = HINAEVENT_NORMAL;
}


//=============================================================================
// ゲームのアイテムを投下する位置に川がないか判定する処理
//=============================================================================
bool CGame::MapEvent_Hinamatsuri_Drop_Item_CheckPos(void)
{
	bool bCol = false;

	CScene *pScene = NULL;               // オブジェクトのポインタ
	CScene *pSceneNext = NULL;           // 次のオブジェクトのポインタ

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// NULLになるまで繰り返す
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_RIVER)
			{// 川だったら
				bCol = MapEvent_Hinamatsuri_Drop_Item_CheckRiver((CRiver*)pScene);
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}

	return bCol;
}

//=============================================================================
// ゲームのアイテムを投下する位置に川がないか判定する処理
//=============================================================================
bool CGame::MapEvent_Hinamatsuri_Drop_Item_CheckRiver(CRiver *pRiver)
{
	if (pRiver != NULL)
	{
		CBoxCollider *pBoxCollider = pRiver->GetBoxCollider();
		if (pBoxCollider != NULL)
		{
			if (pBoxCollider->Collision(&m_ItemDropPos, &m_ItemDropPos, &INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, NULL) == true)
			{
				return true;
			}
		}
	}

	return false;
}

//=============================================================================
// ゲームのひな祭りのひなあられを投下するマップイベント処理
//=============================================================================
void CGame::MapEvent_Hinamatsuri_Drop_Hinaarare(void)
{
	CDebugProc::Print(1, "ひなあられドロップイベント\n");
	if (m_State != STATE_NORMAL) { return; }

	// ひなあられ投下
	int nAppear = m_HinarareData.nAppear;
	if (nAppear <= 0 || m_nEventCounter % nAppear == 0)
	{
		CreateHinaarare();
	}

	// カメラを揺らす状態にする
	if (GetCameraManager() == NULL) { return; }
	CCamera *pCamera = GetCameraManager()->GetCamera();
	if (pCamera == NULL) { return; }

	if (pCamera->GetType() == CCamera::TYPE_GAME)
	{// ゲームで使用しているカメラである
		CGameCamera *pGameCamera = (CGameCamera*)pCamera;
		pGameCamera->StartShake(1, GAME_HINAARARE_CAMERA_SHAKE, GAME_HINAARARE_CAMERA_SHAKE);
	}

	// イベントカウンター進行
	m_nEventCounter++;

	// ホストではないなら処理しない
	int nIdxClient = 0;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		CClient *pClient = CManager::GetClient();
		if (pClient != NULL)
		{
			nIdxClient = pClient->GetClientId();
		}
	}
	if (nIdxClient != 0) { return; }

	if (m_nEventCounter >= m_HinarareData.nTime)
	{// カウンターが一定値になった
		m_nEventCounter = 0;
		m_HinaEvent = HINAEVENT_NORMAL;

		// 音を止める
		CManager::GetSound()->StopSound(GAME_HINAARARE_BGMIDX);
	}
}

//=============================================================================
// ゲームのマップイベント開始時の処理
//=============================================================================
void CGame::MapEvent_Hinamatsuri_EventStart(void)
{
	CDebugProc::Print(1, "マップイベント開始\n");

	// ポリゴンを生成する
	CreateEventStartLogo();

	// 生成されていたら横に動かす
	D3DXVECTOR3 logoPos = m_pEventStartLogo->GetPos();
	logoPos.x += GAME_MAPEVENT_STARTLOGO_MOVE;
	m_pEventStartLogo->SetPos(logoPos);
	m_pEventStartLogo->SetVtxBuffPos();

	// ホストではないなら処理しない
	int nIdxClient = 0;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{
		CClient *pClient = CManager::GetClient();
		if (pClient != NULL)
		{
			nIdxClient = pClient->GetClientId();
		}
	}
	if (nIdxClient != 0) { return; }

	// イベントカウンター進行
	m_nEventCounter++;
	if (m_nEventCounter >= GAME_MAPEVENT_STARTTIME)
	{// カウンターが一定値になった
		m_nEventCounter = 0;
		m_HinaEvent = (HINAEVENT)m_nNextEvent;

		// イベント開始ロゴを開放
		ReleaseEventStartLogo();

		// 鳴らす音を開始するイベントよって分ける
		switch (m_HinaEvent)
		{
		case HINAEVENT_CHERRYBLOSSOMS:
			CManager::GetSound()->PlaySound(GAME_BLOSSOMS_BGMIDX);
			break;
		case HINAEVENT_DROP_ITEM:
			break;
		case HINAEVENT_DROP_HINAARARE:
			CManager::GetSound()->PlaySound(GAME_HINAARARE_BGMIDX);
			break;
		}
	}
}


//*****************************************************************************
//
// ゲーム内スポーン処理用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームのプレイヤーをリスポーン位置にずらす処理
//=============================================================================
void CGame::SetPlayerPosToSpawn(void)
{
	// プレイヤーのリスポーン位置を取得
	D3DXVECTOR3 PlayerPos = INITIALIZE_D3DXVECTOR3;
	CMap *pMap = GetMap();
	if (pMap == NULL) { return; }

	// プレイヤーの位置をリスポーン位置に設定
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_ONLINE2P)
	{// オンラインプレイならば
		PlayerPos = pMap->GetPlayerRespawn(CManager::GetClient()->GetClientId())->GetPos();
		if (m_pPlayer[CManager::GetClient()->GetClientId()] != NULL)
		{
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetPos(PlayerPos);
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
			m_pPlayer[CManager::GetClient()->GetClientId()]->SetNowRotInfo(CCharacter::ROT_UP);
		}
	}
	else
	{// ローカルプレイならば
		for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
		{// プレイヤーの人数分繰り返し
			PlayerPos = pMap->GetPlayerRespawn(nCntPlayer)->GetPos();
			if (m_pPlayer[nCntPlayer] != NULL)
			{
				m_pPlayer[nCntPlayer]->SetPos(PlayerPos);
				m_pPlayer[nCntPlayer]->SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
				m_pPlayer[nCntPlayer]->SetNowRotInfo(CCharacter::ROT_UP);
			}
		}
	}
}

//=============================================================================
// ゲームのプレイヤーをリスポーンさせるかチェックする
//=============================================================================
void CGame::CheckPlayerResSpawn(int nCntPlayer)
{
	if (m_pPlayer[nCntPlayer] == NULL && m_nPlayerStock[nCntPlayer] >= 1)
	{
		m_nPlayerRespawnCounter++;
		if (m_nPlayerRespawnCounter % GAME_PLAYER_RESPAWN_TIMING == 0)
		{
			m_nPlayerRespawnCounter = 0;
			m_nPlayerStock[nCntPlayer]--;
			m_pPlayer[nCntPlayer] = m_pPlayerManager[nCntPlayer]->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);

			// プレイヤーのリスポーン位置を取得
			D3DXVECTOR3 PlayerPos = INITIALIZE_D3DXVECTOR3;
			CMap *pMap = GetMap();
			if (pMap != NULL)
			{
				PlayerPos = pMap->GetPlayerRespawn(nCntPlayer)->GetPos();
			}

			// プレイヤーの位置をずらす
			m_pPlayer[nCntPlayer]->SetPos(PlayerPos);

			// エフェクトを出す
			D3DXVECTOR3 EffectPos = PlayerPos;
			CEffectManager *pEffectManager = CManager::GetBaseMode()->GetEffectManager();
			if (pEffectManager != NULL)
			{
				pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, GAME_PLAYER_SPAWN_EFFECT_IDX);
				pEffectManager->SetEffect(EffectPos, INITIALIZE_D3DXVECTOR3, GAME_PLAYER_SPAWN_EFFECT_IDX + 1);
			}

			// 音を鳴らす
			CManager::GetSound()->PlaySound(GAME_SE_PLAYER_RESPAWN_IDX);

			// UIの数字を減らす
			if (m_pUI != NULL)
			{
				m_pUI->ReCreatePlayerStock(&m_nPlayerStock[0]);
			}
		}
	}
}

//=============================================================================
// ゲームの敵の生成するタイミングかどうか判定する
//=============================================================================
void CGame::CheckEnemySpawn(int nTime)
{
	CMap *pMap = GetMap();
	if (pMap == NULL)return;

	int nNumEnemyListData = pMap->GetNumEnemyListData();
	CEnemy_ListData *pEnemyData = NULL;

	// もう敵が出ないのであればこの時点で処理終了
	if (m_nSpawnEnemyCount == nNumEnemyListData || m_State != STATE_NORMAL) return;

	// カウンター増加
	m_nGameCounter++;
	for (int nCnt = 0; nCnt < nNumEnemyListData; nCnt++)
	{// 敵を生成する数分繰り返し
		pEnemyData = pMap->GetEnemyListData(nCnt);
		if (pEnemyData->GetRespawnTime() == nTime)
		{// リスポーンするタイミングである
			EnemySpawn(pMap, pEnemyData, nCnt);
			m_nSpawnEnemyCount++;
		}
	}
}

//=============================================================================
// ゲームの敵をスポーンさせる
//=============================================================================
void CGame::EnemySpawn(CMap *pMap, CEnemy_ListData *pEnemyData, int nCnt)
{
	// 敵のリスポーン位置を取得
	D3DXVECTOR3 EnemyPos = pMap->GetEnemyRespawn(pEnemyData->GetRespawnIdx())->GetPos();

	// 敵の生成
	CreateEnemy(EnemyPos, INITIALIZE_D3DXVECTOR3, pEnemyData);

	// UIの変更
	if (m_pUI != NULL)
	{
		m_pUI->CutEnemyIcon();
	}
}


//*****************************************************************************
//
// マップを変える用関数(デバッグ用の関数込み)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームの各種カウンターを初期化する
//=============================================================================
void CGame::ResetCounter(void)
{
	// ゲーム内イベントに必要な変数を初期化
	m_nSpawnEnemyCount = 0;
	m_nGameCounter = 0;
	m_nStateCounter = 0;
	m_nEventCounter = 0;
	m_nEnemyMoveCounter = 0;
	m_HinaEvent = HINAEVENT_NORMAL;

	// ゲームリザルトに必要な変数を初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
		{
			m_nNumBreakEnemy[nCntPlayer][nCntEnemy] = 0;
		}
	}
}

//=============================================================================
// ゲームの次のマップに切り替える処理
//=============================================================================
void CGame::ChangeNextMap(void)
{
	// 現在のマップを破棄
	ReleaseMap();

	// ゲームに使用しているオブジェクトを破棄
	DeleteGameObject();

	// マップ番号を増やしマップを読み込む
	m_nMapIdx++;
	m_nStageIdx++;
	if (m_nStageIdx >= GAME_MAPSTAGE_MAX)
	{
		m_nMapIdx--;
		m_nStageIdx--;
	}
	CreateMap();

	// マップ番号の桁数を計算しておく
	CalcStageDigits();

	// プレイヤーの位置をスポーン位置を設定
	SetPlayerPosToSpawn();

	// 各種カウンター初期化
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;

	// 状態を設定
	SetState(STATE_STAGE_DISP);
}

//=============================================================================
// ゲームの前のマップに切り替える処理
//=============================================================================
void CGame::ChangePrevMap(void)
{
	// 現在のマップを破棄
	ReleaseMap();

	// ゲームに使用しているオブジェクトを破棄
	DeleteGameObject();

	// マップ番号を減らしマップを読み込む
	m_nMapIdx--;
	m_nStageIdx--;
	if (m_nStageIdx < 0)
	{
		m_nMapIdx++;
		m_nStageIdx++;
	}
	CreateMap();

	// マップ番号の桁数を計算しておく
	CalcStageDigits();

	// プレイヤーの位置をスポーン位置に設定
	SetPlayerPosToSpawn();

	// 各種カウンター初期化
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;

	// 状態を設定
	SetState(STATE_STAGE_DISP);
}

//=============================================================================
// ゲームのゲームに使用しているオブジェクトを破棄する処理
//=============================================================================
void CGame::DeleteGameObject(void)
{
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{// 処理優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ポインタが空になるまで
			pSceneNext = pScene->GetNext();
			if (GAME_DELETE_OBJECT)
			{// ゲームに使用しているオブジェクトクラスだった
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}

	// 死亡フラグチェック
	CScene::DeathCheck();
}

//=============================================================================
// ゲームのステージ番号の桁数を計算する処理
//=============================================================================
void CGame::CalcStageDigits(void)
{
	m_nNumNumberDigits = (int)log10f((float)(m_nStageIdx + 1)) + 1;	// 桁数を求める
	if (m_nNumNumberDigits <= 0) { m_nNumNumberDigits = 1; }	    // 0以下のとき1にする
}



//*****************************************************************************
//
// 生成用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームの読み込むマップファイル名のポインタを生成する
//=============================================================================
void CGame::CreateMapFilePointer(void)
{
	m_pMapFileName = new char*[m_nNumMap];
	if (m_pMapFileName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumMap; nCnt++)
		{
			m_pMapFileName[nCnt] = new char[256];
			strcpy(m_pMapFileName[nCnt], "\0");
		}
	}
}

//=============================================================================
// ゲームのカメラ生成処理
//=============================================================================
void CGame::CreateCamera(void)
{
	CCameraManager *pCameraManager = CCameraManager::Create();
	if (pCameraManager != NULL)
	{
		CGameCamera *pGameCamera = CGameCamera::Create();
		if (pGameCamera != NULL)
		{
			pCameraManager->SetCamera(pGameCamera);
		}
		SetCameraManager(pCameraManager);
	}
}

//=============================================================================
// ゲームのテクスチャ管轄クラス生成処理
//=============================================================================
void CGame::CreateTexture(int nNumTex)
{
	CTextureManager *pTextureManager = CTextureManager::Create(nNumTex);
	SetTextureManager(pTextureManager);
}

//=============================================================================
// ゲームのマップ生成処理
//=============================================================================
void CGame::CreateMap(void)
{
	CMap *pMap = CMap::Create(m_pMapFileName[m_nMapIdx]);
	SetMap(pMap);
	m_nNumEnemy = pMap->GetNumEnemyListData();
	ResetCounter();
	CEnemy::ResetSpawnCounter();
	SetPlayerPosToSpawn();
	CheckEnemySpawn(m_nGameCounter);
	ReleaseCreamMist();

	// UIを変更
	if (m_pUI != NULL)
	{
		m_pUI->ReCreateEnemyIcon(m_nNumEnemy);
		m_pUI->ReCreateStageNumber((m_nStageIdx + 1));
	}

	// アイテムの総数をリセット
	CItem::ResetNumAll();
}

//=============================================================================
// ゲームのUI生成処理
//=============================================================================
void CGame::CreateUI(void)
{
	m_pUI = CUI::Create(GetTextureManager(), 0, (m_nStageIdx + 1), &m_nPlayerStock[0], m_nNumberTexIdx);
}

//=============================================================================
// ゲームのプレイヤー生成処理
//=============================================================================
void CGame::CreatePlayer(void)
{
	// プレイヤーを生成する
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = m_pPlayerManager[nCntPlayer]->SetPlayer(INITIALIZE_D3DXVECTOR3, INITIALIZE_D3DXVECTOR3, nCntPlayer);
	}
}

//=============================================================================
// ゲームの敵生成処理
//=============================================================================
CEnemy *CGame::CreateEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CEnemy_ListData *pEnemyData)
{
	CEnemy *pEnemy = m_pEnemyManager[pEnemyData->GetEnemyType()]->SetEnemy(pos, rot, pEnemyData->GetEnemyType());

	if (pEnemy == NULL)return NULL;
	// アイテムのデータを設定する
	if (pEnemyData->GetItem() == true)
	{
		pEnemy->SetItemData(pEnemyData->GetItemType());
	}

	// AIのデータを設定する
	pEnemy->SetAIData(pEnemyData->GetAI_BulletEva(), pEnemyData->GetAI_MassEva(), pEnemyData->GetAI_DownEva());

	// スポーンエフェクトを生成する
	pEnemy->SetSpawnEffect();

	return pEnemy;
}

//=============================================================================
// ゲームのアイテムを生成する処理
//=============================================================================
CItem *CGame::CreateItem(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const int nItemType)
{
	CItem *pItem = NULL;
	int nModelIdx = 0;
	switch (nItemType)
	{
	case CItem::TYPE_STAR:
		nModelIdx = m_nItemModelIdx[0];
		pItem = CItemStar::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_GRENADE:
		nModelIdx = m_nItemModelIdx[1];
		pItem = CItemGrenade::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_1UP_TANK:
		nModelIdx = m_nItemModelIdx[2];
		pItem = CItem1up::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_SCOOP:
		nModelIdx = m_nItemModelIdx[3];
		pItem = CItemScoop::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_CLOCK:
		nModelIdx = m_nItemModelIdx[4];
		pItem = CItemClock::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	case CItem::TYPE_HELMET:
		nModelIdx = m_nItemModelIdx[5];
		pItem = CItemHelmet::Create(pos, rot, nItemType, nModelIdx, GetModelCreate()->GetMesh(nModelIdx),
			GetModelCreate()->GetBuffMat(nModelIdx), GetModelCreate()->GetNumMat(nModelIdx),
			GetModelCreate()->GetTexture(nModelIdx));
		break;
	}

	return pItem;
}

//=============================================================================
// ゲームのゲームオーバーロゴ表示用ポリゴンを生成する
//=============================================================================
void CGame::CreateGameOverLogo(void)
{
	m_pGameOverLogo = CScene2D::Create(m_GameOverPolyData.pos, m_GameOverPolyData.col,
		m_GameOverPolyData.fWidth, m_GameOverPolyData.fHeight, GAME_GAMEOVERLOGO_PRIORITY);
	if (m_pGameOverLogo != NULL, GetTextureManager() != NULL)
	{
		m_pGameOverLogo->BindTexture(GetTextureManager()->GetTexture(m_GameOverPolyData.nTexIdx));
	}
}

//=============================================================================
// ゲームのステージ背景用ポリゴンを生成する
//=============================================================================
void CGame::CreateStageBg(void)
{
	m_pStageBg = CScene2D::Create(m_StageBgData.pos, m_StageBgData.col, m_StageBgData.fWidth,
		m_StageBgData.fHeight, GAME_STAGEPOLYGON_PRIORITY);
}

//=============================================================================
// ゲームのステージロゴ表示用ポリゴンを生成する
//=============================================================================
void CGame::CreateStageLogo(void)
{
	m_pStageLogo = CScene2D::Create(m_StageLogoData.pos, m_StageLogoData.col,
		m_StageLogoData.fWidth, m_StageLogoData.fHeight, GAME_STAGEPOLYGON_PRIORITY);
	if (m_pStageLogo != NULL && GetTextureManager() != NULL)
	{
		m_pStageLogo->BindTexture(GetTextureManager()->GetTexture(m_StageLogoData.nTexIdx));
	}
}

//=============================================================================
// ゲームのステージ番号表示用ポリゴンを生成する
//=============================================================================
void CGame::CreateStageNumber(void)
{
	int nStageNumber = 0;
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
	}
	D3DXVECTOR3 NumberInterVal = D3DXVECTOR3(m_StageNumberData.fWidth + (m_StageNumberData.fWidth * GAME_NUMBER_INTERVAL_CUT), 0.0f, 0.0f);
	m_pStageNumber = CNumber::Create(m_StageNumberData.pos, m_StageNumberData.col, m_StageNumberData.fWidth,
		m_StageNumberData.fHeight, NumberInterVal, pTexture, (m_nStageIdx + 1), CNumberPolygon::STATE_NONE,
		3, 0.0f, GAME_STAGEPOLYGON_PRIORITY);
}

//=============================================================================
// ゲームの桜の花びらを生成する処理
//=============================================================================
void CGame::CreateBlossoms(void)
{
	// 各種値の計算
	// 移動量
	D3DXVECTOR3 Move = INITIALIZE_D3DXVECTOR3;
	Move.x = (float)((rand() % (m_CherryBlossomsData.nMoveXMax - m_CherryBlossomsData.nMoveXMin)) + m_CherryBlossomsData.nMoveXMin);
	Move.x *= -1.0f;
	Move.y = (float)((rand() % (m_CherryBlossomsData.nMoveYMax - m_CherryBlossomsData.nMoveYMin)) + m_CherryBlossomsData.nMoveYMin);

	// 角度の回転スピード
	float fAngleSpeed = (float)(rand() % (m_CherryBlossomsData.nAngleSpeedMax - m_CherryBlossomsData.nAngleSpeedMin) + m_CherryBlossomsData.nAngleSpeedMin);

	// 向きの回転スピード
	float fRotSpeed = (float)(rand() % (m_CherryBlossomsData.nRotSpeedMax - m_CherryBlossomsData.nRotSpeedMin) + m_CherryBlossomsData.nRotSpeedMin);

	// 幅
	float fWidth = (float)((rand() % (m_CherryBlossomsData.nWidthMax - m_CherryBlossomsData.nWidthMin)) + m_CherryBlossomsData.nWidthMin);

	// 高さ
	float fHeight = (float)((rand() % (m_CherryBlossomsData.nHeightMax - m_CherryBlossomsData.nHeightMin)) + m_CherryBlossomsData.nHeightMin);

	// 花びらポリゴン生成
	CBlossoms *pBlossoms = CBlossoms::Create(Move, D3DXToRadian(fAngleSpeed), D3DXToRadian(fRotSpeed), GAME_BLOSSOMS_POS_INI, GAME_BLOSSOMS_COL_INI, fWidth, fHeight);
	if (pBlossoms != NULL && GetTextureManager() != NULL)
	{
		pBlossoms->BindTexture(GetTextureManager()->GetTexture(m_CherryBlossomsData.nTexIdx));
	}
}

//=============================================================================
// ゲームのひなあられを生成する処理
//=============================================================================
void CGame::CreateHinaarare(void)
{
	// ひなあられを出す位置をマス目から計算
	D3DXVECTOR3 HinaararePos = INITIALIZE_D3DXVECTOR3;
	int nMassX = rand() % MASS_BLOCK_X;
	int nMassZ = rand() % MASS_BLOCK_Z;
	HinaararePos.x = (nMassX * MASS_SIZE_X) + MASS_SIZE_X_HALF - (MASS_BLOCK_X * MASS_SIZE_X_HALF);
	HinaararePos.z = (nMassZ * MASS_SIZE_Z) + MASS_SIZE_Z_HALF - (MASS_BLOCK_Z * MASS_SIZE_Z_HALF);
	HinaararePos.y = GAME_HINAARARE_POSY;

	// ひなあられに使用するモデルを設定
	int nModelIdx = m_HinarareData.nModelIdx + (rand() % GAME_HINAARARE_MODEL_NUM);

	// ひなあられを生成
	CModelCreate *pModelCreate = GetModelCreate();
	if (pModelCreate == NULL) { return; }
	CHinaarare::Create(HinaararePos, INITIALIZE_D3DXVECTOR3, pModelCreate->GetMesh(nModelIdx),
		pModelCreate->GetBuffMat(nModelIdx), pModelCreate->GetNumMat(nModelIdx),
		pModelCreate->GetTexture(nModelIdx));
}

//=============================================================================
// ゲームのゲーム内リザルトに必要なものを生成する処理
//=============================================================================
void CGame::CreateGameResult(void)
{
	// 倒した全ての敵の数を初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_nNumAllBreakEnemy[nCntPlayer] = 0;
	}

	// 倒した全ての敵の数を計算
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
		{
			m_nNumAllBreakEnemy[nCntPlayer] += m_nNumBreakEnemy[nCntPlayer][nCntEnemy];
		}
	}

	// ボーナスを付与していない状態に
	m_bAddBonus = false;

	// 背景を作成
	CreateGameResult_Bg();

	// ステージ番号表示用ポリゴンを作成
	CreateGameResult_Stage();

	// ハイスコア用ポリゴンを作成
	CreateGameResult_HighScore();

	// プレイヤースコア用ポリゴンを作成
	CreateGameResult_PlayerScore();

	// 敵のスコア用ロゴポリゴンを作成
	CreateGameResult_EnemyScore_Logo();

	// トータルスコア用ロゴポリゴンを作成
	CreateGameResult_TortalScore_Logo();

	// 進行度を戻しておく
	m_GameResultAddvance = GAMERESULT_NONE;
	m_nStateCounter = 0;
	m_bNextGameResult = true;
}

//=============================================================================
// ゲームのゲーム内リザルト用背景を生成する処理
//=============================================================================
void CGame::CreateGameResult_Bg(void)
{
	if (m_pGameResultBg == NULL)
	{
		m_pGameResultBg = CScene2D::Create(GAME_GAMERESULT_BG_POS_INI, GAME_GAMERESULT_BG_COL_INI,
			GAME_GAMERESULT_BG_WIDTH_INI, GAME_GAMERESULT_BG_HEIGHT_INI, GAME_GAMERESULT_BG_PRIORITY);
	}
}

//=============================================================================
// ゲームのゲーム内リザルトステージ番号表示用ポリゴンを生成する処理
//=============================================================================
void CGame::CreateGameResult_Stage(void)
{
	// ロゴポリゴン生成
	if (m_pGameResultStage == NULL)
	{
		m_pGameResultStage = CScene2D::Create(GAME_GAMERESULT_STAGELOGO_POS_INI, GAME_GAMERESULT_STAGELOGO_COL_INI,
			GAME_GAMERESULT_STAGELOGO_WIDTH_INI, GAME_GAMERESULT_STAGELOGO_HEIGHT_INI, GAME_GAMERESULT_STAGELOGO_PRIORITY);
		if (m_pGameResultStage != NULL)
		{
			m_pGameResultStage->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_STAGELOGO_TEXIDX));
		}
	}

	// 番号表示用ポリゴン生成
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
	}
	if (m_pGameResultStageNumber == NULL)
	{
		m_pGameResultStageNumber = CNumber::Create(GAME_GAMERESULT_STAGENUMBER_POS_INI, GAME_GAMERESULT_STAGENUMBER_COL_INI,
			GAME_GAMERESULT_STAGENUMBER_WIDTH_INI, GAME_GAMERESULT_STAGENUMBER_HEIGHT_INI, INITIALIZE_D3DXVECTOR3,
			pTexture, (m_nStageIdx + 1), CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_STAGENUMBER_PRIORITY);
	}
}

//=============================================================================
// ゲームのハイスコアを生成する処理
//=============================================================================
void CGame::CreateGameResult_HighScore(void)
{
	// ロゴポリゴン生成
	if (m_pHighScoreLogo == NULL)
	{
		m_pHighScoreLogo = CScene2D::Create(GAME_GAMERESULT_HIGHSCORELOGO_POS_INI, GAME_GAMERESULT_HIGHSCORELOGO_COL_INI,
			GAME_GAMERESULT_HIGHSCORELOGO_WIDTH_INI, GAME_GAMERESULT_HIGHSCORELOGO_HEIGHT_INI, GAME_GAMERESULT_HIGHSCORELOGO_PRIORITY);
		if (m_pHighScoreLogo != NULL)
		{
			m_pHighScoreLogo->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_HIGHSCORELOGO_TEXIDX));
		}
	}

	// 番号表示用ポリゴン生成
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
	}
	if (m_pHighScore == NULL)
	{
		m_pHighScore = CNumber::Create(GAME_GAMERESULT_HIGHSCORENUMBER_POS_INI, GAME_GAMERESULT_HIGHSCORENUMBER_COL_INI,
			GAME_GAMERESULT_HIGHSCORENUMBER_WIDTH_INI, GAME_GAMERESULT_HIGHSCORENUMBER_HEIGHT_INI, GAME_GAMERESULT_HIGHSCORENUMBER_INTERVAL_INI,
			pTexture, CTitle::GetHighScore(), CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_HIGHSCORENUMBER_PRIORITY);
	}
}

//=============================================================================
// ゲームのプレイヤースコアを生成する処理
//=============================================================================
void CGame::CreateGameResult_PlayerScore(void)
{
	// 繰り返す回数を設定
	int nCountMax = MAX_NUM_PLAYER;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{
		nCountMax = MAX_NUM_PLAYER - 1;
	}

	// ポリゴン生成
	D3DXVECTOR3 PlayerLogoPos = GAME_GAMERESULT_PLAYERLOGO_POS_INI;
	D3DXVECTOR3 PlayerNumberPos = GAME_GAMERESULT_PLAYERNUMBER_POS_INI;
	for (int nCnt = 0; nCnt < nCountMax; nCnt++)
	{
		// プレイヤーのスコアを計算
		m_nScore[nCnt] += m_nNumBreakEnemy[nCnt][CEnemy::TYPE_NORMAL] * ENEMY_SCORE_NORMAL;
		m_nScore[nCnt] += m_nNumBreakEnemy[nCnt][CEnemy::TYPE_ARMORE] * ENEMY_SCORE_ARMORE;
		m_nScore[nCnt] += m_nNumBreakEnemy[nCnt][CEnemy::TYPE_FAST] * ENEMY_SCORE_FAST;
		m_nScore[nCnt] += m_nNumBreakEnemy[nCnt][CEnemy::TYPE_HEAVY] * ENEMY_SCORE_HEAVY;

		// ロゴポリゴン生成
		if (m_apPlayerLogo[nCnt] == NULL)
		{
			m_apPlayerLogo[nCnt] = CScene2D::Create(PlayerLogoPos, GAME_GAMERESULT_PLAYERLOGO_COL_INI,
				GAME_GAMERESULT_PLAYERLOGO_WIDTH_INI, GAME_GAMERESULT_PLAYERLOGO_HEIGHT_INI, GAME_GAMERESULT_PLAYERLOGO_PRIORITY);
			if (m_apPlayerLogo[nCnt] != NULL)
			{
				m_apPlayerLogo[nCnt]->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_PLAYERLOGO_TEXIDX + nCnt));
			}
		}

		// 番号表示用ポリゴン生成
		LPDIRECT3DTEXTURE9 pTexture = NULL;
		if (GetTextureManager() != NULL)
		{
			pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
		}
		if (m_apScore[nCnt] == NULL)
		{
			m_apScore[nCnt] = CNumber::Create(PlayerNumberPos, GAME_GAMERESULT_PLAYERNUMBER_COL_INI,
				GAME_GAMERESULT_PLAYERNUMBER_WIDTH_INI, GAME_GAMERESULT_PLAYERNUMBER_HEIGHT_INI, GAME_GAMERESULT_PLAYERNUMBER_INTERVAL_INI,
				pTexture, m_nScore[nCnt], CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_PLAYERNUMBER_PRIORITY);
		}
		PlayerLogoPos.x = SCREEN_WIDTH / 2.0f + GAME_GAMERESULT_PLAYERLOGO_POS_INI.x;
		PlayerNumberPos.x = SCREEN_WIDTH / 2.0f + GAME_GAMERESULT_PLAYERNUMBER_POS_INI.x;
	}
}

//=============================================================================
// ゲームの敵のスコア用ロゴポリゴンを生成する処理
//=============================================================================
void CGame::CreateGameResult_EnemyScore_Logo(void)
{
	// 繰り返す回数を設定
	int nCountMax = MAX_NUM_PLAYER;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{
		nCountMax = MAX_NUM_PLAYER - 1;
	}

	// ポリゴンを生成
	D3DXVECTOR3 EnemyArrowPos = GAME_GAMERESULT_ENEMYARROW_POS_INI;
	D3DXVECTOR3 EnemyScorePointLogoPos = GAME_GAMERESULT_ENEMYPOINTLOGO_POS_INI;
	for (int nCnt = 0; nCnt < nCountMax; nCnt++)
	{
		for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
		{// 敵の種類の数だけ繰り返し
			// 矢印ポリゴン生成
			if (m_apEnemyScoreArrow[nCnt][nCntEnemy] == NULL)
			{
				m_apEnemyScoreArrow[nCnt][nCntEnemy] = CScene2D::Create(EnemyArrowPos, GAME_GAMERESULT_ENEMYARROW_COL_INI,
					GAME_GAMERESULT_ENEMYARROW_WIDTH_INI, GAME_GAMERESULT_ENEMYARROW_HEIGHT_INI, GAME_GAMERESULT_ENEMYARROW_PRIORITY);
				if (m_apEnemyScoreArrow[nCnt][nCntEnemy] != NULL)
				{
					m_apEnemyScoreArrow[nCnt][nCntEnemy]->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_ENEMYARROW_TEXIDX + nCnt));
				}
			}
			EnemyArrowPos.y += GAME_GAMERESULT_ENEMYARROW_INTERVAL_INI.y;

			// ポイントポリゴン生成
			if (m_apEnemyScorePointLogo[nCnt][nCntEnemy] == NULL)
			{
				m_apEnemyScorePointLogo[nCnt][nCntEnemy] = CScene2D::Create(EnemyScorePointLogoPos, GAME_GAMERESULT_ENEMYPOINTLOGO_COL_INI,
					GAME_GAMERESULT_ENEMYPOINTLOGO_WIDTH_INI, GAME_GAMERESULT_ENEMYPOINTLOGO_HEIGHT_INI, GAME_GAMERESULT_ENEMYPOINTLOGO_PRIORITY);
				if (m_apEnemyScorePointLogo[nCnt][nCntEnemy] != NULL)
				{
					m_apEnemyScorePointLogo[nCnt][nCntEnemy]->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_ENEMYPOINTLOGO_TEXIDX));
				}
			}
			EnemyScorePointLogoPos.y += GAME_GAMERESULT_ENEMYPOINTLOGO_INTERVAL_INI.y;
		}
		EnemyArrowPos.x = (SCREEN_WIDTH / 2.0f + GAME_GAMERESULT_ENEMYARROW_INTERVAL_INI.x);
		EnemyArrowPos.y = GAME_GAMERESULT_ENEMYARROW_POS_INI.y;
		EnemyScorePointLogoPos.x = (SCREEN_WIDTH - GAME_GAMERESULT_ENEMYPOINTLOGO_INTERVAL_INI.x);
		EnemyScorePointLogoPos.y = GAME_GAMERESULT_ENEMYPOINTLOGO_POS_INI.y;
	}

	// 敵アイコンポリゴンを生成
	D3DXVECTOR3 EnemyIconPos = GAME_GAMERESULT_ENEMYICON_POS_INI;
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{// 敵の種類の数だけ繰り返し
		if (m_apEnemyScoreIcon[nCntEnemy] == NULL)
		{
			m_apEnemyScoreIcon[nCntEnemy] = CScene2D::Create(EnemyIconPos, GAME_GAMERESULT_ENEMYICON_COL_INI,
				GAME_GAMERESULT_ENEMYICON_WIDTH_INI, GAME_GAMERESULT_ENEMYICON_HEIGHT_INI, GAME_GAMERESULT_ENEMYICON_PRIORITY);
			if (m_apEnemyScoreIcon[nCntEnemy] != NULL)
			{
				m_apEnemyScoreIcon[nCntEnemy]->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_ENEMYICON_TEXIDX + nCntEnemy));
			}
		}
		EnemyIconPos += GAME_GAMERESULT_ENEMYICON_INTERVAL_INI;
	}
}

//=============================================================================
// ゲームの敵のスコア用数字ポリゴンを生成する処理
//=============================================================================
void CGame::CreateGameResult_EnemyScore_Number(void)
{
	// 繰り返す回数を設定
	int nCountMax = MAX_NUM_PLAYER;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{
		nCountMax = MAX_NUM_PLAYER - 1;
	}

	int nEnemyScore[CEnemy::TYPE_MAX] = { ENEMY_SCORE_NORMAL, ENEMY_SCORE_ARMORE ,ENEMY_SCORE_FAST ,ENEMY_SCORE_HEAVY };
	D3DXVECTOR3 EnemyScorePos = GAME_GAMERESULT_ENEMYSCORE_POS_INI;
	D3DXVECTOR3 EnemyScoreInterval = D3DXVECTOR3(GAME_GAMERESULT_ENEMYSCORE_INTERVAL_INI.x, 0.0f, 0.0f);
	D3DXVECTOR3 EnemyBreakPos = GAME_GAMERESULT_ENEMYBREAK_POS_INI;
	D3DXVECTOR3 EnemyBreakInterval = D3DXVECTOR3(GAME_GAMERESULT_ENEMYBREAK_INTERVAL_INI.x, 0.0f, 0.0f);
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
	}
	for (int nCnt = 0; nCnt < nCountMax; nCnt++)
	{
		// スコアポリゴンを生成
		if (m_apEnemyScore[nCnt][m_GameResultAddvance] == NULL)
		{
			EnemyScorePos.y += m_GameResultAddvance * GAME_GAMERESULT_ENEMYSCORE_INTERVAL_INI.y;
			m_apEnemyScore[nCnt][m_GameResultAddvance] = CNumber::Create(EnemyScorePos,
				GAME_GAMERESULT_ENEMYSCORE_COL_INI,
				GAME_GAMERESULT_ENEMYSCORE_WIDTH_INI, GAME_GAMERESULT_ENEMYSCORE_HEIGHT_INI,
				EnemyScoreInterval, pTexture, 0,
				CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_ENEMYSCORE_PRIORITY);
		}
		EnemyScorePos.x = SCREEN_WIDTH - GAME_GAMERESULT_ENEMYSCORE_POS_INI.x;
		EnemyScorePos.y = GAME_GAMERESULT_ENEMYSCORE_POS_INI.y;

		// 敵を倒した数ポリゴンを生成
		if (m_apNumBreakEnemy[nCnt][m_GameResultAddvance] == NULL)
		{
			EnemyBreakPos.y += m_GameResultAddvance * GAME_GAMERESULT_ENEMYBREAK_INTERVAL_INI.y;
			m_apNumBreakEnemy[nCnt][m_GameResultAddvance] = CNumber::Create(EnemyBreakPos,
				GAME_GAMERESULT_ENEMYBREAK_COL_INI,
				GAME_GAMERESULT_ENEMYBREAK_WIDTH_INI, GAME_GAMERESULT_ENEMYBREAK_HEIGHT_INI,
				EnemyBreakInterval, pTexture, 0,
				CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_ENEMYBREAK_PRIORITY);
		}
		EnemyBreakPos.x = SCREEN_WIDTH - GAME_GAMERESULT_ENEMYBREAK_POS_INI.x - EnemyBreakInterval.x;
		EnemyBreakPos.y = GAME_GAMERESULT_ENEMYBREAK_POS_INI.y;
	}
}

//=============================================================================
// ゲームのトータルスコア用ロゴポリゴンを生成する処理
//=============================================================================
void CGame::CreateGameResult_TortalScore_Logo(void)
{
	// ロゴポリゴン生成
	if (m_pTortalLogo == NULL)
	{
		m_pTortalLogo = CScene2D::Create(GAME_GAMERESULT_TOTALLOGO_POS_INI, GAME_GAMERESULT_TOTALLOGO_COL_INI,
			GAME_GAMERESULT_TOTALLOGO_WIDTH_INI, GAME_GAMERESULT_TOTALLOGO_HEIGHT_INI, GAME_GAMERESULT_TOTALLOGO_PRIORITY);
		if (m_pTortalLogo != NULL)
		{
			m_pTortalLogo->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_TOTALLOGO_TEXIDX));
		}
	}

	// ラインポリゴン生成
	if (m_pTortalLine == NULL)
	{
		m_pTortalLine = CScene2D::Create(GAME_GAMERESULT_TOTALLINE_POS_INI, GAME_GAMERESULT_TOTALLINE_COL_INI,
			GAME_GAMERESULT_TOTALLINE_WIDTH_INI, GAME_GAMERESULT_TOTALLINE_HEIGHT_INI, GAME_GAMERESULT_TOTALLINE_PRIORITY);
	}
}

//=============================================================================
// ゲームのトータルスコア用数字ポリゴンを生成する処理
//=============================================================================
void CGame::CreateGameResult_TortalScore_Number(void)
{
	// 繰り返す回数を設定
	int nCountMax = MAX_NUM_PLAYER;
	if (CTitle::GetGameMode() == CTitle::GAMEMODE_LOCAL1P)
	{
		nCountMax = MAX_NUM_PLAYER - 1;
	}

	// ポリゴンを生成
	D3DXVECTOR3 TortalScorePos = GAME_GAMERESULT_TOTALSCORE_POS_INI;
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
	}
	for (int nCnt = 0; nCnt < nCountMax; nCnt++)
	{
		if (m_apNumAllBreakEnemy[nCnt] == NULL)
		{
			m_apNumAllBreakEnemy[nCnt] = CNumber::Create(TortalScorePos, GAME_GAMERESULT_TOTALSCORE_COL_INI,
				GAME_GAMERESULT_TOTALSCORE_WIDTH_INI, GAME_GAMERESULT_TOTALSCORE_HEIGHT_INI,
				GAME_GAMERESULT_TOTALSCORE_INTERVAL_INI, pTexture, m_nNumAllBreakEnemy[nCnt],
				CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_TOTALSCORE_PRIORITY);
		}
		TortalScorePos.x = SCREEN_WIDTH - GAME_GAMERESULT_TOTALSCORE_POS_INI.x - GAME_GAMERESULT_TOTALSCORE_INTERVAL_INI.x;
	}
}

//=============================================================================
// ゲームのボーナススコアを生成する処理
//=============================================================================
void CGame::CreateGameResult_BonusScore(void)
{
	// 敵を倒した数が同じならば処理しない
	if (m_nNumAllBreakEnemy[0] == m_nNumAllBreakEnemy[MAX_NUM_PLAYER - 1]) { return; }

	// 敵を倒した数が多い方にボーナスを付与する
	D3DXVECTOR3 BonusScorePosAdd = INITIALIZE_D3DXVECTOR3;
	if (m_nNumAllBreakEnemy[0] < m_nNumAllBreakEnemy[MAX_NUM_PLAYER - 1])
	{
		m_nScore[MAX_NUM_PLAYER - 1] += GAME_BONUS_SCORE;
		BonusScorePosAdd.x = GAME_GAMERESULT_BONUS_ADDPOS;
	}
	else
	{
		m_nScore[0] += GAME_BONUS_SCORE;
	}

	// 表示しているプレイヤースコアを変動
	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		if (m_apScore[nCnt] != NULL)
		{
			m_apScore[nCnt]->SetNumber(m_nScore[nCnt]);
		}
	}

	// ロゴポリゴン生成
	if (m_pBonusScoreLogo == NULL)
	{
		m_pBonusScoreLogo = CScene2D::Create(GAME_GAMERESULT_BONUSSCORELOGO_POS_INI + BonusScorePosAdd, GAME_GAMERESULT_BONUSSCORELOGO_COL_INI,
			GAME_GAMERESULT_BONUSSCORELOGO_WIDTH_INI, GAME_GAMERESULT_BONUSSCORELOGO_HEIGHT_INI, GAME_GAMERESULT_BONUSSCORELOGO_PRIORITY);
		if (m_pBonusScoreLogo != NULL)
		{
			m_pBonusScoreLogo->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_BONUSSCORELOGO_TEXIDX));
		}
	}

	// ポイントポリゴン生成
	if (m_pBonusPointLogo == NULL)
	{
		m_pBonusPointLogo = CScene2D::Create(GAME_GAMERESULT_BONUSPOINTLOGO_POS_INI + BonusScorePosAdd, GAME_GAMERESULT_BONUSPOINTLOGO_COL_INI,
			GAME_GAMERESULT_BONUSPOINTLOGO_WIDTH_INI, GAME_GAMERESULT_BONUSPOINTLOGO_HEIGHT_INI, GAME_GAMERESULT_BONUSPOINTLOGO_PRIORITY);
		if (m_pBonusPointLogo != NULL)
		{
			m_pBonusPointLogo->BindTexture(GetTextureManager()->GetTexture(GAME_GAMERESULT_BONUSPOINTLOGO_TEXIDX));
		}
	}

	// 数字ポリゴンを生成
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (GetTextureManager() != NULL)
	{
		pTexture = GetTextureManager()->GetTexture(m_nNumberTexIdx);
	}
	if (m_pBonus == NULL)
	{
		m_pBonus = CNumber::Create(GAME_GAMERESULT_BONUSSCORE_POS_INI + BonusScorePosAdd, GAME_GAMERESULT_BONUSSCORE_COL_INI,
			GAME_GAMERESULT_BONUSSCORE_WIDTH_INI, GAME_GAMERESULT_BONUSSCORE_HEIGHT_INI,
			GAME_GAMERESULT_BONUSSCORE_INTERVAL_INI, pTexture, GAME_BONUS_SCORE,
			CNumberPolygon::STATE_NONE, 3, 0.0f, GAME_GAMERESULT_BONUSSCORE_PRIORITY);
	}

	// ボーナスを付与した状態に
	m_bAddBonus = true;

	// 音を鳴らす
	CManager::GetSound()->PlaySound(GAME_SE_BONUS_IDX);
}

//=============================================================================
// ゲームのポーズクラスを生成する
//=============================================================================
void CGame::CreatePause(int nIdxPlayer)
{
	if (m_pPause != NULL) { return; }
	m_pPause = CPause::Create(GetTextureManager());

	// ポーズクラスを生成
	if (m_pPause != NULL)
	{
		m_pPause->SetOpenId(nIdxPlayer);
	}

	// ローカル2人プレイ以外ならこの先処理しない
	if (CTitle::GetGameMode() != CTitle::GAMEMODE_LOCAL2P) { return; }

	// どちらがポーズを開いたか表示する用ポリゴン生成
	if (m_pPausePlayerNumber == NULL)
	{
		m_pPausePlayerNumber = CScene2D::Create(GAME_PAUSE_PLAYERNUMBER_POS_INI, GAME_PAUSE_PLAYERNUMBER_COL_INI,
			GAME_PAUSE_PLAYERNUMBER_WIDTH_INI, GAME_PAUSE_PLAYERNUMBER_HEIGHT_INI, GAME_PAUSE_PLAYERNUMBER_PRIORITY);
		if (m_pPausePlayerNumber != NULL)
		{
			m_pPausePlayerNumber->BindTexture(GetTextureManager()->GetTexture(GAME_PAUSE_PLAYERNUMBER_TEXIDX + nIdxPlayer));
		}
	}
}

//=============================================================================
// ゲームのポーズを押されてない側のポリゴンを生成する
//=============================================================================
void CGame::CreateNotPause(void)
{
	// 黒背景を生成
	m_pNotPauseBlackBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
		GAME_PAUSE_NOT_BLACKBG_COL_INI, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, GAME_PAUSE_NOT_BLACKBG_PRIORITY);

	// ロゴポリゴンを生成
	m_pNotPause = CScene2D::Create(GAME_PAUSE_NOT_POS_INI, GAME_PAUSE_NOT_COL_INI, GAME_PAUSE_NOT_WIDTH_INI,
		GAME_PAUSE_NOT_HEIGHT_INI, GAME_PAUSE_NOT_PRIORITY);
	if (m_pNotPause != NULL)
	{
		m_pNotPause->BindTexture(GetTextureManager()->GetTexture(GAME_PAUSE_NOT_TEXIDX));
	}
}

//=============================================================================
// ゲームのイベント開始ロゴを生成する
//=============================================================================
void CGame::CreateEventStartLogo(void)
{
	if (m_pEventStartLogo == NULL)
	{
		// テクスチャU座標はランダムで計算
		float fStartTexU = (rand() % GAME_MAPEVENT_STARTLOGO_TEXU_SPLIT) * (1.0f / GAME_MAPEVENT_STARTLOGO_TEXU_SPLIT);

		// テクスチャV座標は次のイベントによって設定
		float fStartTexV = (1.0f / GAME_MAPEVENT_STARTLOGO_TEXV_SPLIT) * (m_nNextEvent - 1);

		m_pEventStartLogo = CScene2D::Create(GAME_MAPEVENT_STARTLOGO_POS, GAME_MAPEVENT_STARTLOGO_COL,
			GAME_MAPEVENT_STARTLOGO_WIDTH, GAME_MAPEVENT_STARTLOGO_HEIGHT, GAME_MAPEVENT_STARTLOGO_PRIORITY, 0.0f,
			fStartTexU, fStartTexV, (1.0f / GAME_MAPEVENT_STARTLOGO_TEXU_SPLIT),
			(1.0f / GAME_MAPEVENT_STARTLOGO_TEXV_SPLIT));
		if (m_pEventStartLogo != NULL)
		{
			m_pEventStartLogo->BindTexture(GetTextureManager()->GetTexture(GAME_MAPEVENT_STARTLOGO_TEXIDX));
		}
	}
}

//=============================================================================
// ゲームのクリーム靄を生成する
//=============================================================================
void CGame::CreateMist(void)
{
	if (m_pCreamMist == NULL)
	{// 生成されていなければ
		m_pCreamMist = CCreamMist::Create(GAME_CREAMMIST_POS_INI, GAME_CREAMMIST_COL_INI,
			GAME_CREAMMIST_WIDTH_INI, GAME_CREAMMIST_HEIGHT_INI, GAME_CREAMMIST_PRIORITY);
		if (m_pCreamMist != NULL)
		{
			m_pCreamMist->BindTexture(GetTextureManager()->GetTexture(GAME_CREAMMIST_TEXIDX));
		}
	}
	else
	{// 生成されていれば
		D3DXCOLOR col = m_pCreamMist->GetCol();
		col.a += GAME_CREAMMIST_ALPHA_UP;
		if (col.a >= GAME_CREAMMIST_ALPHA_MAX)
		{
			col.a = GAME_CREAMMIST_ALPHA_MAX;
		}
		m_pCreamMist->SetCol(col);
		m_pCreamMist->SetVtxBuffCol();
	}

	m_nMistCounter = 0;
}



//*****************************************************************************
//
// 開放用関数
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームの読み込むマップファイル名のポインタを開放する
//=============================================================================
void CGame::ReleaseMapFilePointer(void)
{
	if (m_pMapFileName != NULL)
	{
		for (int nCnt = 0; nCnt < m_nNumMap; nCnt++)
		{
			delete[] m_pMapFileName[nCnt];
			m_pMapFileName[nCnt] = NULL;
		}

		delete[] m_pMapFileName;
		m_pMapFileName = NULL;
	}
}

//=============================================================================
// ゲームのUIクラスを開放する
//=============================================================================
void CGame::ReleaseUI(void)
{
	if (m_pUI != NULL)
	{
		m_pUI->Uninit();
		delete m_pUI;
		m_pUI = NULL;
	}
}

//=============================================================================
// ゲームのプレイヤークラスを開放する
//=============================================================================
void CGame::ReleasePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_pPlayer[nCntPlayer] != NULL)
		{
			m_pPlayer[nCntPlayer]->Uninit();
			m_pPlayer[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// ゲームのプレイヤー管轄クラスを開放する
//=============================================================================
void CGame::ReleasePlayerManager(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		if (m_pPlayerManager[nCntPlayer] != NULL)
		{
			m_pPlayerManager[nCntPlayer]->Uninit();
			delete m_pPlayerManager[nCntPlayer];
			m_pPlayerManager[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// ゲームの敵データ管轄クラスを開放する
//=============================================================================
void CGame::ReleaseEnemyManager(void)
{
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		if (m_pEnemyManager[nCntEnemy] != NULL)
		{
			m_pEnemyManager[nCntEnemy]->Uninit();
			delete m_pEnemyManager[nCntEnemy];
			m_pEnemyManager[nCntEnemy] = NULL;
		}
	}
}

//=============================================================================
// ゲームのゲームオーバーロゴ表示用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseGameOverLogo(void)
{
	if (m_pGameOverLogo != NULL)
	{
		m_pGameOverLogo->Uninit();
		m_pGameOverLogo = NULL;
	}
}

//=============================================================================
// ゲームのステージ背景表示用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseStageBg(void)
{
	if (m_pStageBg != NULL)
	{
		m_pStageBg->Uninit();
		m_pStageBg = NULL;
	}
}

//=============================================================================
// ゲームのステージロゴ表示用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseStageLogo(void)
{
	if (m_pStageLogo != NULL)
	{
		m_pStageLogo->Uninit();
		m_pStageLogo = NULL;
	}
}

//=============================================================================
// ゲームのステージ番号表示用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseStageNumber(void)
{
	if (m_pStageNumber != NULL)
	{
		m_pStageNumber->Uninit();
		m_pStageNumber = NULL;
	}
}

//=============================================================================
// ゲームのステージ番号表示用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseGameResult(void)
{
	ReleaseGameResult_Bg();
	ReleaseGameResult_Stage();
	ReleaseGameResult_HighScore();
	ReleaseGameResult_PlayerScore();
	ReleaseGameResult_EnemyScore();
	ReleaseGameResult_TortalScore();
	ReleaseGameResult_BonusScore();
}

//=============================================================================
// ゲームのゲームリザルト背景用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseGameResult_Bg(void)
{
	if (m_pGameResultBg != NULL)
	{
		m_pGameResultBg->Uninit();
		m_pGameResultBg = NULL;
	}
}

//=============================================================================
// ゲームのゲーム内リザルトステージ表示用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseGameResult_Stage(void)
{
	// ステージロゴ
	if (m_pGameResultStage != NULL)
	{
		m_pGameResultStage->Uninit();
		m_pGameResultStage = NULL;
	}

	// ステージ番号表示用ポリゴン
	if (m_pGameResultStageNumber != NULL)
	{
		m_pGameResultStageNumber->Uninit();
		m_pGameResultStageNumber = NULL;
	}
}

//=============================================================================
// ゲームのハイスコア用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseGameResult_HighScore(void)
{
	// 数字ポリゴン
	if (m_pHighScore != NULL)
	{
		if (m_pHighScore != NULL)
		{
			m_pHighScore->Uninit();
			m_pHighScore = NULL;
		}
	}

	// ハイスコアロゴ
	if (m_pHighScoreLogo != NULL)
	{
		m_pHighScoreLogo->Uninit();
		m_pHighScoreLogo = NULL;
	}
}

//=============================================================================
// ゲームのプレイヤースコア用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseGameResult_PlayerScore(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{// プレイヤーの総数分繰り返し
	    // 数字ポリゴン
		if (m_apScore[nCntPlayer] != NULL)
		{
			m_apScore[nCntPlayer]->Uninit();
			m_apScore[nCntPlayer] = NULL;
		}

		// プレイヤーロゴ
		if (m_apPlayerLogo[nCntPlayer] != NULL)
		{
			m_apPlayerLogo[nCntPlayer]->Uninit();
			m_apPlayerLogo[nCntPlayer] = NULL;
		}
	}
}

//=============================================================================
// ゲームのエネミースコア用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseGameResult_EnemyScore(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{// プレイヤーの総数分繰り返し
		for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
		{// 敵の種類分繰り返し
			// 敵を倒した数ポリゴン
			if (m_apNumBreakEnemy[nCntPlayer][nCntEnemy] != NULL)
			{
				m_apNumBreakEnemy[nCntPlayer][nCntEnemy]->Uninit();
				m_apNumBreakEnemy[nCntPlayer][nCntEnemy] = NULL;
			}

			// 敵のスコア表示用ポリゴン
			if (m_apEnemyScore[nCntPlayer][nCntEnemy] != NULL)
			{
				m_apEnemyScore[nCntPlayer][nCntEnemy]->Uninit();
				m_apEnemyScore[nCntPlayer][nCntEnemy] = NULL;
			}

			// ポイントロゴ表示用ポリゴン
			if (m_apEnemyScorePointLogo[nCntPlayer][nCntEnemy] != NULL)
			{
				m_apEnemyScorePointLogo[nCntPlayer][nCntEnemy]->Uninit();
				m_apEnemyScorePointLogo[nCntPlayer][nCntEnemy] = NULL;
			}

			// スコア矢印表示用ポリゴン
			if (m_apEnemyScoreArrow[nCntPlayer][nCntEnemy] != NULL)
			{
				m_apEnemyScoreArrow[nCntPlayer][nCntEnemy]->Uninit();
				m_apEnemyScoreArrow[nCntPlayer][nCntEnemy] = NULL;
			}
		}
	}

	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{// 敵の種類分繰り返し
		// 敵アイコン表示用ポリゴン
		if (m_apEnemyScoreIcon[nCntEnemy] != NULL)
		{
			m_apEnemyScoreIcon[nCntEnemy]->Uninit();
			m_apEnemyScoreIcon[nCntEnemy] = NULL;
		}
	}
}

//=============================================================================
// ゲームのトータルスコア用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseGameResult_TortalScore(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{// プレイヤーの総数分繰り返し
	    // 数字ポリゴン
		if (m_apNumAllBreakEnemy[nCntPlayer] != NULL)
		{
			m_apNumAllBreakEnemy[nCntPlayer]->Uninit();
			m_apNumAllBreakEnemy[nCntPlayer] = NULL;
		}
	}

	// トータルロゴ用ポリゴン
	if (m_pTortalLogo != NULL)
	{
		m_pTortalLogo->Uninit();
		m_pTortalLogo = NULL;
	}

	// トータルライン用ポリゴン
	if (m_pTortalLine != NULL)
	{
		m_pTortalLine->Uninit();
		m_pTortalLine = NULL;
	}
}

//=============================================================================
// ゲームのボーナススコア用ポリゴンを開放する
//=============================================================================
void CGame::ReleaseGameResult_BonusScore(void)
{
	// 数字ポリゴン
	if (m_pBonus != NULL)
	{
		m_pBonus->Uninit();
		m_pBonus = NULL;
	}

	// ボーナススコアポイントロゴ表示用ポリゴン
	if (m_pBonusPointLogo != NULL)
	{
		m_pBonusPointLogo->Uninit();
		m_pBonusPointLogo = NULL;
	}

	// ボーナススコアロゴ表示用ポリゴン
	if (m_pBonusScoreLogo != NULL)
	{
		m_pBonusScoreLogo->Uninit();
		m_pBonusScoreLogo = NULL;
	}
}

//=============================================================================
// ゲームのポーズクラスを開放する
//=============================================================================
void CGame::ReleasePause(void)
{
	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;
	}

	if (m_pPausePlayerNumber != NULL)
	{
		m_pPausePlayerNumber->Uninit();
		m_pPausePlayerNumber = NULL;
	}
}

//=============================================================================
// ゲームのポーズを押されてない側のポリゴンを開放する
//=============================================================================
void CGame::ReleaseNotPause(void)
{
	// 文字ポリゴン開放
	if (m_pNotPause != NULL)
	{
		m_pNotPause->Uninit();
		m_pNotPause = NULL;
	}

	// 黒背景開放
	if (m_pNotPauseBlackBg != NULL)
	{
		m_pNotPauseBlackBg->Uninit();
		m_pNotPauseBlackBg = NULL;
	}
}


//=============================================================================
// ゲームのポーズを押されてない側のポリゴンを開放する
//=============================================================================
void CGame::ReleaseEventStartLogo(void)
{
	if (m_pEventStartLogo != NULL)
	{
		m_pEventStartLogo->Uninit();
		m_pEventStartLogo = NULL;
	}
}

//=============================================================================
// ゲームのクリーム靄を開放する
//=============================================================================
void CGame::ReleaseCreamMist(void)
{
	if (m_pCreamMist != NULL)
	{
		m_pCreamMist->Uninit();
		m_pCreamMist = NULL;
	}
}



//*****************************************************************************
//
// スクリプト読み込み用関数(マップイベント用にスクリプトを作るならここに！！)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームのシステム情報を読み込む
//=============================================================================
void CGame::LoadSystem(void)
{
	char aStr[256] = "\0";
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(GAME_SYSTEM_FILENAME);
	if (pFileLoader != NULL)
	{
		strcpy(aStr, pFileLoader->GetString(aStr));
		if (CFunctionLib::Memcmp(aStr, SCRIPT) == 0)
		{// 読み込み開始の合図だった
			LoadSystemScript(pFileLoader, aStr);
		}

		// メモリの開放
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
// ゲームのシステム情報をファイルから読み込む
//=============================================================================
void CGame::LoadSystemScript(CFileLoader *pFileLoader, char *pStr)
{
	int nCntMap = 0;
	int nCntTex = 0;
	int nCntModel = 0;
	int nCntItemModel = 0;
	int nCntEnemy = 0;
	int nCntPlayer = 0;

	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, NUM_TEXTURE) == 0)
		{// 読み込むテクスチャの数だった
			CreateTexture(CFunctionLib::ReadInt(pStr, NUM_TEXTURE));
		}
		else if (CFunctionLib::Memcmp(pStr, TEXTURE_FILENAME) == 0)
		{// 読み込むテクスチャのファイル名だった
			LoadTexFileName(pStr, nCntTex);
			nCntTex++;
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_MODEL) == 0)
		{// 読み込むモデルの数だった
			int nNumModel = CFunctionLib::ReadInt(pStr, NUM_MODEL);
			CModelCreate *pModelCreate = CModelCreate::Create(nNumModel);
			SetModelCreate(pModelCreate);
		}
		else if (CFunctionLib::Memcmp(pStr, MODEL_FILENAME) == 0)
		{// 読み込むモデルのファイル名だった
			LoadModel(pStr, nCntModel);
			nCntModel++;
		}
		else if (CFunctionLib::Memcmp(pStr, EFFECT_FILENAME) == 0)
		{// 読み込むエフェクトのファイル名だった
			LoadEffectFileName(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, NUM_MAP) == 0)
		{// 読み込むマップの数だった
			m_nNumMap = CFunctionLib::ReadInt(pStr, NUM_MAP);
			CreateMapFilePointer();
		}
		else if (CFunctionLib::Memcmp(pStr, MAP_FILENAME) == 0)
		{// 読み込むマップファイル名だった
			LoadMapFileName(pStr, nCntMap);
			nCntMap++;
		}
		else if (CFunctionLib::Memcmp(pStr, ITEMEVENT_FILENAME) == 0)
		{// アイテムイベント用スクリプトファイル名だった
			LoadItemEvent(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, NUMBER_TEX_IDX) == 0)
		{// 数字の使用するテクスチャ番号だった
			m_nNumberTexIdx = CFunctionLib::ReadInt(pStr, NUMBER_TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, BULLET_MODEL_IDX) == 0)
		{// 弾が使用するモデルの番号だった
			m_nBulletModelIdx = CFunctionLib::ReadInt(pStr, BULLET_MODEL_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, ITEM_MODEL_IDX) == 0)
		{// アイテムが使用するモデルの番号だった
			m_nItemModelIdx[nCntItemModel] = CFunctionLib::ReadInt(pStr, ITEM_MODEL_IDX);
			nCntItemModel++;
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYERDATA_FILENAME) == 0)
		{// プレイヤーデータのスクリプトファイル名だった
			LoadPlayerFileName(pStr, nCntPlayer);
			nCntPlayer++;
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYER_STOCK_INI) == 0)
		{// プレイヤーの残機数初期値だった
			m_nPlayerStockIni = CFunctionLib::ReadInt(pStr, PLAYER_STOCK_INI);
		}
		else if (CFunctionLib::Memcmp(pStr, PLAYER_STOCK_MAX) == 0)
		{// プレイヤーの残機数最大値だった
			m_nPlayerStockMax = CFunctionLib::ReadInt(pStr, PLAYER_STOCK_MAX);
		}
		else if (CFunctionLib::Memcmp(pStr, ENEMY_FILENAME) == 0)
		{// 敵データのスクリプトファイル名だった
			LoadEnemyFileName(pStr, nCntEnemy);
			nCntEnemy++;
		}
		else if (CFunctionLib::Memcmp(pStr, STAGEBGSET) == 0)
		{// ステージ背景用ポリゴン情報だった
			LoadStageBg(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, STAGELOGOSET) == 0)
		{// ステージロゴ用ポリゴン情報だった
			LoadStageLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, STAGENUMBERSET) == 0)
		{// ステージ番号表示用ポリゴン情報だった
			LoadStageNumber(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, GAMEOVERPOLYGONSET) == 0)
		{// ゲームオーバーロゴポリゴン情報だった
			LoadGameOverLogo(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, HINAMATSURI_FILENAME) == 0)
		{// ひな祭りイベント用スクリプトファイル名だった
			LoadHinamatsuriEvent(pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			break;
		}
	}
}

//=============================================================================
// ゲームの読み込むテクスチャファイル名を読み込む
//=============================================================================
void CGame::LoadTexFileName(char *pStr, int nCntTex)
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	char aTexFileName[256] = "\0";
	strcpy(aTexFileName, CFunctionLib::ReadString(pStr, aTexFileName, TEXTURE_FILENAME));

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), aTexFileName, &pTexture);

	// テクスチャ管轄クラスにポインタを設定する
	CTextureManager *pTextureManager = GetTextureManager();
	pTextureManager->SetTexture(pTexture, nCntTex);
	pTextureManager->SetFileName(pStr, nCntTex);
}

//=============================================================================
// ゲームの読み込むアイテムモデルを読み込む
//=============================================================================
void CGame::LoadModel(char *pStr, int nCntModel)
{
	// モデルのファイルパス名を読み取る
	LPD3DXMESH pMesh = NULL;
	LPD3DXBUFFER pBuffMat = NULL;
	DWORD nNumMat = 0;
	char aModelFileName[256] = "\0";
	strcpy(aModelFileName, CFunctionLib::ReadString(pStr, aModelFileName, MODEL_FILENAME));

	// xファイルの読み込み
	D3DXLoadMeshFromX(aModelFileName, D3DXMESH_SYSTEMMEM, CManager::GetRenderer()->GetDevice(), NULL,
		&pBuffMat, NULL, &nNumMat, &pMesh);

	// モデル管轄クラスに値を設定する
	CModelCreate *pModelCreate = GetModelCreate();
	if (pModelCreate == NULL)return;
	pModelCreate->SetMesh(pMesh, nCntModel);
	pModelCreate->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);
	pModelCreate->SetFileName(pStr, nCntModel);
}

//=============================================================================
// ゲームの読み込むマップファイル名を読み込む
//=============================================================================
void CGame::LoadMapFileName(char *pStr, int nCntMap)
{
	strcpy(m_pMapFileName[nCntMap], CFunctionLib::ReadString(pStr, m_pMapFileName[nCntMap], MAP_FILENAME));
}

//=============================================================================
// ゲームの読み込むエフェクトファイル名を読み込む
//=============================================================================
void CGame::LoadEffectFileName(char *pStr)
{
	char aEffectFileName[256] = "\0";
	strcpy(aEffectFileName, CFunctionLib::ReadString(pStr, aEffectFileName, EFFECT_FILENAME));

	// エフェクト管轄クラスを生成
	CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
	SetEffectManager(pEffectManager);
}

//=============================================================================
// ゲームの読み込むプレイヤーデータファイル名を読み込む
//=============================================================================
void CGame::LoadPlayerFileName(char *pStr, int nCntPlayer)
{
	char aPlayerFileName[256] = "\0";
	strcpy(aPlayerFileName, CFunctionLib::ReadString(pStr, aPlayerFileName, PLAYERDATA_FILENAME));

	for (int nCnt = 0; nCnt < MAX_NUM_PLAYER; nCnt++)
	{
		if (nCntPlayer == CCharaSelect::GetPlayerNumber(nCnt))
		{
			m_pPlayerManager[nCnt] = CPlayerManager::Create(aPlayerFileName);
		}
	}
}

//=============================================================================
// ゲームの読み込む敵データファイル名を読み込む
//=============================================================================
void CGame::LoadEnemyFileName(char *pStr, int nCntEnemy)
{
	char aEnemyFileName[256] = "\0";
	strcpy(aEnemyFileName, CFunctionLib::ReadString(pStr, aEnemyFileName, ENEMY_FILENAME));

	// キャラクター管轄クラスを作成
	m_pEnemyManager[nCntEnemy] = CCharacterManager::Create(aEnemyFileName);
}

//=============================================================================
// ゲームのステージ背景ポリゴン情報を読み込む
//=============================================================================
void CGame::LoadStageBg(CFileLoader *pFileLoader, char *pStr)
{
	// データ初期化
	m_StageBgData.pos = GAME_STAGEBG_POS_INI;
	m_StageBgData.col = GAME_STAGEBG_COL_INI;
	m_StageBgData.fWidth = GAME_STAGEBG_WIDTH_INI;
	m_StageBgData.fHeight = GAME_STAGEBG_HEIGHT_INI;

	// データ読み込み
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			m_StageBgData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_StageBgData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_StageBgData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_StageBgData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_STAGEBGSET) == 0)
		{// ステージ背景ポリゴン情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// ゲームのステージロゴポリゴン情報を読み込む
//=============================================================================
void CGame::LoadStageLogo(CFileLoader *pFileLoader, char *pStr)
{
	// データ初期化
	m_StageLogoData.nTexIdx = 0;
	m_StageLogoData.pos = GAME_STAGELOGO_POS_INI;
	m_StageLogoData.col = GAME_STAGELOGO_COL_INI;
	m_StageLogoData.fWidth = GAME_STAGELOGO_WIDTH_INI;
	m_StageLogoData.fHeight = GAME_STAGELOGO_HEIGHT_INI;

	// データ読み込み
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			m_StageLogoData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			m_StageLogoData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_StageLogoData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_StageLogoData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_StageLogoData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_STAGELOGOSET) == 0)
		{// ステージロゴポリゴン情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// ゲームのステージ番号表示用ポリゴン情報を読み込む
//=============================================================================
void CGame::LoadStageNumber(CFileLoader *pFileLoader, char *pStr)
{
	// データ初期化
	m_StageNumberData.pos = GAME_STAGENUMBER_POS_INI;
	m_StageNumberData.col = GAME_STAGENUMBER_COL_INI;
	m_StageNumberData.fWidth = GAME_STAGENUMBER_WIDTH_INI;
	m_StageNumberData.fHeight = GAME_STAGENUMBER_HEIGHT_INI;

	// データ読み込み
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			m_StageNumberData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_StageNumberData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_StageNumberData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_StageNumberData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_STAGENUMBERSET) == 0)
		{// ステージ番号表示用ポリゴン情報終了の合図だった
			break;
		}
	}
}

//=============================================================================
// ゲームのゲームオーバーロゴポリゴン情報を読み込む
//=============================================================================
void CGame::LoadGameOverLogo(CFileLoader *pFileLoader, char *pStr)
{
	// データ初期化
	m_GameOverPolyData.nTexIdx = 0;
	m_GameOverPolyData.pos = GAME_GAMEOVERLOGO_POS_INI;
	m_GameOverPolyData.col = GAME_GAMEOVERLOGO_COL_INI;
	m_GameOverPolyData.fWidth = GAME_GAMEOVERLOGO_WIDTH_INI;
	m_GameOverPolyData.fHeight = GAME_GAMEOVERLOGO_HEIGHT_INI;

	// データ読み込み
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			m_GameOverPolyData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, POS) == 0)
		{// ポリゴンの位置だった
			m_GameOverPolyData.pos = CFunctionLib::ReadVector3(pStr, POS);
		}
		else if (CFunctionLib::Memcmp(pStr, COL) == 0)
		{// ポリゴンの色だった
			m_GameOverPolyData.col = CFunctionLib::ReadVector4(pStr, COL);
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_GameOverPolyData.fWidth = CFunctionLib::ReadFloat(pStr, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_GameOverPolyData.fHeight = CFunctionLib::ReadFloat(pStr, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_GAMEOVERPOLYGONSET) == 0)
		{// ゲームオーバーロゴポリゴン情報終了の合図だった
			break;
		}
	}
}


//*****************************************************************************
//
// アイテムイベント用スクリプト読み込み処理
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームのアイテムイベントスクリプトを読み込む
//=============================================================================
void CGame::LoadItemEvent(char *pStr)
{
	char aFileName[256] = "\0";
	strcpy(aFileName, CFunctionLib::ReadString(pStr, aFileName, ITEMEVENT_FILENAME));
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(aFileName);
	if (pFileLoader != NULL)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// 読み込み開始の合図だった
			LoadItemEventScript(pFileLoader, pStr);
		}

		// メモリの開放
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
// ゲームのアイテムイベント情報をファイルから読み込む
//=============================================================================
void CGame::LoadItemEventScript(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SHAKESET) == 0)
		{// カメラ揺れイベント情報読み込み開始の合図だった
			LoadItemEvent_Shake(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, ENEMY_STOP) == 0)
		{// 敵をどれくらい止めるか情報だった
			m_ItemEventData.nStop = CFunctionLib::ReadInt(pStr, ENEMY_STOP);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			break;
		}
	}
}

//=============================================================================
// ゲームのカメラ揺れイベント情報をファイルから読み込む
//=============================================================================
void CGame::LoadItemEvent_Shake(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TIME) == 0)
		{// 揺らす時間だった
			m_ItemEventData.nShakeTime = CFunctionLib::ReadInt(pStr, TIME);
		}
		else if (CFunctionLib::Memcmp(pStr, VALUE) == 0)
		{// どれくらい揺らすかだった
			m_ItemEventData.fShake = CFunctionLib::ReadFloat(pStr, VALUE);
		}
		else if (CFunctionLib::Memcmp(pStr, CUT) == 0)
		{// どれくらい揺れを減らすかだった
			m_ItemEventData.fShakeCut = CFunctionLib::ReadFloat(pStr, CUT);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SHAKESET) == 0)
		{// 揺れイベント終了の合図だった
			break;
		}
	}
}


//*****************************************************************************
//
// ひな祭りイベント用スクリプト読み込み処理
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームのひな祭りイベントスクリプトを読み込む
//=============================================================================
void CGame::LoadHinamatsuriEvent(char *pStr)
{
	char aFileName[256] = "\0";
	strcpy(aFileName, CFunctionLib::ReadString(pStr, aFileName, HINAMATSURI_FILENAME));
	CFileLoader *pFileLoader = NULL;
	pFileLoader = CFileLoader::Create(aFileName);
	if (pFileLoader != NULL)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, SCRIPT) == 0)
		{// 読み込み開始の合図だった
			LoadHinamatsuriEventScript(pFileLoader, pStr);
		}

		// メモリの開放
		if (pFileLoader != NULL)
		{
			pFileLoader->Uninit();
			delete pFileLoader;
			pFileLoader = NULL;
		}
	}
}

//=============================================================================
// ゲームのひな祭りイベント情報をファイルから読み込む
//=============================================================================
void CGame::LoadHinamatsuriEventScript(CFileLoader *pFileLoader, char *pStr)
{
	while (1)
	{// ループ開始
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, EVENT_RANDOM) == 0)
		{// イベントランダム評価値だった
			m_nRandomEventEva = CFunctionLib::ReadInt(pStr, EVENT_RANDOM);
		}
		else if (CFunctionLib::Memcmp(pStr, CHERRYBLOSSOMSSET) == 0)
		{// 桜の花びら情報だった
			LoadCherryBlossomsData(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, HINAARARESET) == 0)
		{// ひなあられ情報だった
			LoadHinaarareData(pFileLoader, pStr);
		}
		else if (CFunctionLib::Memcmp(pStr, END_SCRIPT) == 0)
		{// スクリプトファイル終了の合図だった
			break;
		}
	}
}

//=============================================================================
// ゲームの桜の花びらデータ情報を読み込む
//=============================================================================
void CGame::LoadCherryBlossomsData(CFileLoader *pFileLoader, char *pStr)
{
	// データ初期化
	m_CherryBlossomsData.nTime = GAME_BLOSSOMS_TIME_INI;
	m_CherryBlossomsData.nLap = GAME_BLOSSOMS_LAP_INI;
	m_CherryBlossomsData.nTexIdx = 0;
	m_CherryBlossomsData.nMoveXMax = GAME_BLOSSOMS_MOVEX_MAX_INI;
	m_CherryBlossomsData.nMoveXMin = GAME_BLOSSOMS_MOVEX_MIN_INI;
	m_CherryBlossomsData.nMoveYMax = GAME_BLOSSOMS_MOVEY_MAX_INI;
	m_CherryBlossomsData.nMoveYMin = GAME_BLOSSOMS_MOVEY_MIN_INI;
	m_CherryBlossomsData.nWidthMax = GAME_BLOSSOMS_WIDTH_MAX_INI;
	m_CherryBlossomsData.nWidthMin = GAME_BLOSSOMS_WIDTH_MIN_INI;
	m_CherryBlossomsData.nHeightMax = GAME_BLOSSOMS_HEIGHT_MAX_INI;
	m_CherryBlossomsData.nHeightMin = GAME_BLOSSOMS_HEIGHT_MIN_INI;
	m_CherryBlossomsData.nAngleSpeedMax = GAME_BLOSSOMS_ANGLESPEED_MAX_INI;
	m_CherryBlossomsData.nAngleSpeedMin = GAME_BLOSSOMS_ANGLESPEED_MIN_INI;
	m_CherryBlossomsData.nRotSpeedMax = GAME_BLOSSOMS_ROTSPEED_MAX_INI;
	m_CherryBlossomsData.nRotSpeedMin = GAME_BLOSSOMS_ROTSPEED_MIN_INI;

	// データ読み込み
	int nWord = 0;
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, TIME) == 0)
		{// 桜の花びらが降り注ぐ時間
			m_CherryBlossomsData.nTime = CFunctionLib::ReadInt(pStr, TIME);
		}
		else if (CFunctionLib::Memcmp(pStr, LAP) == 0)
		{// 桜の花びらを1フレーム毎にどれくらい出すか
			m_CherryBlossomsData.nLap = CFunctionLib::ReadInt(pStr, LAP);
		}
		else if (CFunctionLib::Memcmp(pStr, TEX_IDX) == 0)
		{// 使用するテクスチャの番号だった
			m_CherryBlossomsData.nTexIdx = CFunctionLib::ReadInt(pStr, TEX_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, MOVEX) == 0)
		{// X方向の移動量だった
			m_CherryBlossomsData.nMoveXMax = CFunctionLib::ReadInt(pStr, MOVEX);
			pStr = CFunctionLib::HeadPutout(pStr, MOVEX);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nMoveXMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, MOVEY) == 0)
		{// Y方向の移動量だった
			m_CherryBlossomsData.nMoveYMax = CFunctionLib::ReadInt(pStr, MOVEY);
			pStr = CFunctionLib::HeadPutout(pStr, MOVEY);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nMoveYMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, WIDTH) == 0)
		{// ポリゴンの幅だった
			m_CherryBlossomsData.nWidthMax = CFunctionLib::ReadInt(pStr, WIDTH);
			pStr = CFunctionLib::HeadPutout(pStr, WIDTH);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nWidthMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, HEIGHT) == 0)
		{// ポリゴンの高さだった
			m_CherryBlossomsData.nHeightMax = CFunctionLib::ReadInt(pStr, HEIGHT);
			pStr = CFunctionLib::HeadPutout(pStr, HEIGHT);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nHeightMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, ANGLESPEED) == 0)
		{// 角度を回転させるスピードだった
			m_CherryBlossomsData.nRotSpeedMax = CFunctionLib::ReadInt(pStr, ANGLESPEED);
			pStr = CFunctionLib::HeadPutout(pStr, ANGLESPEED);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nRotSpeedMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, ROTSPEED) == 0)
		{// 向きを回転させるスピードだった
			m_CherryBlossomsData.nRotSpeedMax = CFunctionLib::ReadInt(pStr, ROTSPEED);
			pStr = CFunctionLib::HeadPutout(pStr, ROTSPEED);
			nWord = CFunctionLib::PopString(pStr, "");
			pStr += nWord;
			m_CherryBlossomsData.nRotSpeedMin = CFunctionLib::ReadInt(pStr, "");
		}
		else if (CFunctionLib::Memcmp(pStr, END_CHERRYBLOSSOMSSET) == 0)
		{// 桜の花びらデータ終了の合図だった
			break;
		}
	}
}

//=============================================================================
// ゲームのひなあられデータ情報を読み込む
//=============================================================================
void CGame::LoadHinaarareData(CFileLoader *pFileLoader, char *pStr)
{
	// データ初期化
	m_HinarareData.nModelIdx = GAME_HINAARARE_MODELIDX;
	m_HinarareData.nTime = GAME_HINAARARE_TIME;
	m_HinarareData.nAppear = GAME_HINAARARE_APPEAR;

	// データ読み込み
	int nWord = 0;
	while (1)
	{
		strcpy(pStr, pFileLoader->GetString(pStr));
		if (CFunctionLib::Memcmp(pStr, MODEL_IDX) == 0)
		{// ひなあられが使用するモデルの番号
			m_HinarareData.nModelIdx = CFunctionLib::ReadInt(pStr, MODEL_IDX);
		}
		else if (CFunctionLib::Memcmp(pStr, TIME) == 0)
		{// ひなあられが降り注ぐ時間
			m_HinarareData.nTime = CFunctionLib::ReadInt(pStr, TIME);
		}
		else if (CFunctionLib::Memcmp(pStr, APPEAR) == 0)
		{// ひなあられを出す間隔
			m_HinarareData.nAppear = CFunctionLib::ReadInt(pStr, APPEAR);
		}
		else if (CFunctionLib::Memcmp(pStr, END_HINAARARESET) == 0)
		{// ひなあられデータ終了の合図だった
			break;
		}
	}
}


//*****************************************************************************
//
// 設定、取得等色々関数(外部との窓口も含めてここに書いてます)
// Auther : Hodaka Niwa
//
//*****************************************************************************
//=============================================================================
// ゲームの変数を初期化する
//=============================================================================
void CGame::ClearVariable(void)
{
	m_State = STATE_STAGE_DISP;
	m_pUI = NULL;
	m_pMapFileName = NULL;
	m_nStateCounter = 0;
	m_nMapIdx = 0;
	m_nStageIdx = 0;
	m_nNumMap = 0;
	m_nGameCounter = 0;
	m_nSpawnEnemyCount = 0;
	m_nNumEnemy = 0;
	m_pGameOverLogo = NULL;
	m_pStageBg = NULL;
	m_pStageLogo = NULL;
	m_pStageNumber = NULL;
	m_nNumberTexIdx = 0;
	m_nNumNumberDigits = NULL;
	m_nPlayerRespawnCounter = 0;
	m_nNumDeleteBlock = 0;
	strcpy(m_aDeleteBlock, "\0");
	m_nNumDeleteEnemy = 0;
	strcpy(m_aDeleteEnemy, "\0");
	m_bDeletePlayerFlag = 0;
	m_bHitBulletFlag = false;
	m_bEnemyMove = true;
	m_nEnemyMoveCounter = 0;
	m_HinaEvent = HINAEVENT_NORMAL;
	m_nEventCounter = 0;
	m_nNotEventCounter = 0;
	m_nRandomEventEva = 0;
	m_nPlayerStockIni = 0;
	m_nPlayerStockMax = 0;
	m_pEventStartLogo = NULL;
	m_nNextEvent = 0;

	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_pPlayer[nCntPlayer] = NULL;
		m_pPlayerManager[nCntPlayer] = NULL;
		m_nPlayerStock[nCntPlayer] = 0;
	}
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		m_pEnemyManager[nCntEnemy] = NULL;
	}

	// アイテムドロップイベント
	m_pItem = NULL;
	m_pItemCylinder = NULL;
	m_ItemDropPos = INITIALIZE_D3DXVECTOR3;

	// ゲーム内リザルト用データ
	m_GameResultAddvance = GAMERESULT_NONE;

	// 背景用
	m_pGameResultBg = NULL;

	// ステージ番号表示用
	m_pGameResultStage = NULL;
	m_pGameResultStageNumber = NULL;

	// ハイスコア用
	m_pHighScore = NULL;
	m_pHighScoreLogo = NULL;

	// プレイヤースコア用
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_nScore[nCntPlayer] = 0;
		m_apScore[nCntPlayer] = NULL;
		m_apPlayerLogo[nCntPlayer] = NULL;
	}

	// エネミースコア用
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
		{
			m_nNumBreakEnemy[nCntPlayer][nCntEnemy] = 0;
			m_apNumBreakEnemy[nCntPlayer][nCntEnemy] = NULL;
			m_apEnemyScore[nCntPlayer][nCntEnemy] = NULL;
			m_apEnemyScorePointLogo[nCntPlayer][nCntEnemy] = NULL;
			m_apEnemyScoreArrow[nCntPlayer][nCntEnemy] = NULL;
		}
	}
	for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
	{
		m_apEnemyScoreIcon[nCntEnemy] = NULL;
	}

	// トータルスコア用
	for (int nCntPlayer = NULL; nCntPlayer < MAX_NUM_PLAYER; nCntPlayer++)
	{
		m_nNumAllBreakEnemy[nCntPlayer] = 0;
		m_apNumAllBreakEnemy[nCntPlayer] = NULL;
		m_apPlayerLogo[nCntPlayer] = NULL;
	}
	m_pTortalLogo = NULL;
	m_pTortalLine = NULL;

	// ボーナススコア用
	m_pBonus = NULL;
	m_pBonusPointLogo = NULL;
	m_pBonusScoreLogo = NULL;

	// ポーズ用
	m_bPauseOpen = false;
	m_pPause = NULL;
	m_pPausePlayerNumber = NULL;
	m_pNotPause = NULL;
	m_pNotPauseBlackBg = NULL;

	// クリーム靄用
	m_pCreamMist = NULL;
	m_nMistCounter = 0;
}

//=============================================================================
// ゲームの弾モデルを設定する処理
//=============================================================================
void CGame::SetBulletModel(CBullet *pBullet)
{
	pBullet->BindModel(GetModelCreate()->GetMesh(m_nBulletModelIdx), GetModelCreate()->GetBuffMat(m_nBulletModelIdx),
		GetModelCreate()->GetNumMat(m_nBulletModelIdx), GetModelCreate()->GetTexture(m_nBulletModelIdx));
}

//=============================================================================
// ゲームの状態を設定する
//=============================================================================
void CGame::SetState(const STATE state)
{
	m_StateOld = m_State;
	m_State = state;
	m_nStateCounter = 0;
}

//=============================================================================
// ゲームのプレイヤークラスへのポインタを設定する
//=============================================================================
void CGame::SetPlayer(CPlayer *pPlayer, const int nIdx)
{
	m_pPlayer[nIdx] = pPlayer;
}

//=============================================================================
// ゲームの出現する敵の数を設定する
//=============================================================================
void CGame::SetNumEnemy(const int nNumEnemy)
{
	m_nNumEnemy = nNumEnemy;
}

//=============================================================================
// ゲームの状態を取得する
//=============================================================================
CGame::STATE CGame::GetState(void)
{
	return m_State;
}

//=============================================================================
// ゲームのプレイヤークラスへのポインタを取得する
//=============================================================================
CPlayer *CGame::GetPlayer(const int nIdx)
{
	return m_pPlayer[nIdx];
}

//=============================================================================
// ゲームの出現する敵の数を取得する
//=============================================================================
int CGame::GetNumEnemy(void)
{
	return m_nNumEnemy;
}

//=============================================================================
// ゲームの敵を動かせるかどうか取得する
//=============================================================================
bool CGame::GetEnemyMove(void)
{
	return m_bEnemyMove;
}

//=============================================================================
// ゲームのステージ番号を取得する
//=============================================================================
int CGame::GetStageIdx(void)
{
	return m_nStageIdx;
}