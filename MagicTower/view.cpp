#include "view.h"
#include "player.h"
#include "role.h"
#include "shop_item.h"
#include "character.h" //人物触发事件
#include "tool.h"
#include <QKeyEvent>

extern Player player;
extern Role parameter;
extern Tools tools;
extern ShopItem items;
extern int map[14][14][1];

View::View(QWidget *parent) : QGraphicsView(parent) {         // Initialize the status to "welcome"
    status = "welcome";
    next_step = 0;
}

View::~View() {         // Empty the status string when quit
    status = "";
    next_step = 0;
}

void View::keyPressEvent(QKeyEvent * event) {         // This function will autorun when you press a key
    if (status == "main") {         // Read key and move player
        keyMain(event);
        action();         // Different actions according to Nextstep's num
    } else if (status == "tip" || status == "book" || status == "help") {         // One click then back to main
        SetStatus("main");
        emit change();
    } else if (status == "ending" || status == "fail")         // One click then quit
        emit quit();
    else if (status == "like")         // One click then if fail then quit else back to main
        keyLike(event);
    else if (status == "shop")         // Read key and decide which stuff player bought
        keyShop(event);
    else if (status == "welcome")         // Read key and choose to start a new game or load
        keyWelcome(event);
    else if (status == "select")         // Read key and choose sex
        keySelect(event);
    else if (status == "lecture")
        keylecture(event);
    else if (status == "prof1")
        keyprof1(event);
    else if (status == "prof2")
        keyprof2(event);
    else if (status == "prof3")
        keyprof3(event);
    else if (status == "eat")
        keyeat(event);
    else if (status == "study")
        keystudy(event);
    else if (status == "sleep")
        keysleep(event);
    else if (status == "movie")
        keymovie(event);
}

void View::keyMain(QKeyEvent * event) {
    switch (event->key()) {
        case Qt::Key_Up:        // Move up
            if (access(player.GetPosx(), player.GetPosy()-1))
                emit move(0, -1);
            player.SetToward(1);
            break;
        case Qt::Key_Down:      // Move down
            if (access(player.GetPosx(), player.GetPosy()+1))
                emit move(0, +1);
            player.SetToward(3);
            #include <QKeyEvent>
        break;
        case Qt::Key_Left:      // Move left
            if (access(player.GetPosx()-1, player.GetPosy()))
                emit move(-1, 0);
            player.SetToward(2);
            break;
        case Qt::Key_Right:     // Move right
            if (access(player.GetPosx()+1, player.GetPosy()))
                emit move(+1, 0);
            player.SetToward(4);
            break;
        case Qt::Key_S:         // Save data
            emit events("save");
            break;
        case Qt::Key_L:         // Load data
            emit events("load");
            break;
        case Qt::Key_Q:         // Quit
            emit events("quit");
            break;
        case Qt::Key_H:         // Get help menu
            SetStatus("help");
            emit events("help");
            break;
        default:
            break;
    }
}
void View::keyShop(QKeyEvent *event){};
/*
void View::keyShop(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_1:     // Hp + 30 , Money - 50
            if (player.GetMoney() >= 50) {
                player.SetMoney(player.GetMoney() - 50);
                player.SetHp(player.GetHp()+30);
            }
            emit events("shop");
            break;
        case Qt::Key_2:     // Attack + 2, Money - 50
            if (player.GetMoney() >= 50) {
                player.SetMoney(player.GetMoney() - 50);
                player.SetAttack(player.GetAttack()+2);
            }
            emit events("shop");
            break;
        case Qt::Key_3:     // Defend + 1, Money - 50
            if (player.GetMoney() >= 50) {
                player.SetMoney(player.GetMoney() - 50);
                player.SetDefend(player.GetDefend()+1);
            }
            emit events("shop");
            break;
        case Qt::Key_4:     // YellowKey + 1, Money - 40
            if (player.GetMoney() >= 40) {
                player.SetMoney(player.GetMoney() - 40);
                keys.SetYellow(keys.GetYellow()+1);
            }
            emit events("shop");
            break;
        case Qt::Key_5:     // BlueKey + 1, Money - 120
            if (player.GetMoney() >= 120) {
                player.SetMoney(player.GetMoney() - 120);
                keys.SetBlue(keys.GetBlue()+1);
            }
            emit events("shop");
            break;
        case Qt::Key_6:     // RedKey + 1, Money - 300
            if (player.GetMoney() > 300) {
                player.SetMoney(player.GetMoney() - 300);
                keys.SetRed(keys.GetRed()+1);
            }
            emit events("shop");
            break;
        case Qt::Key_Q:     // Back to main
            SetStatus("main");
            emit change();
            break;
        default:
            break;
    }
}
*/

void View::keyWelcome(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_1:     // Start a new game
            emit events("new");
            break;
        case Qt::Key_2:     // Load
            SetStatus("main");
            emit events("load");
            emit change();
            break;
        case Qt::Key_3:     // Quit
            emit events("quit");
            emit change();
            break;
        default:
            break;
    }
}

void View::keySelect(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_1:     // You are a handsome boy!
            SetStatus("main");
            emit events("boy");
            emit change();
            break;
        case Qt::Key_2:     // You are a beautiful girl~
            SetStatus("main");
            emit events("girl");
            emit change();
            break;
        case Qt::Key_3:     // What!? Who are you!? GET OUT!
            emit events("quit");
            break;
        default:
            break;
    }
}

void View::keyLike(QKeyEvent *event) {
    if ( parameter.GetLike() == 100) {
        SetStatus("love");
        emit events("love");
    } else {
        SetStatus("main");
        emit change();
    }
}

void View::action() {
    switch (next_step) {
        case 3:
            SetStatus("lecture");
            parameter.SetEnergy(parameter.GetEnerge()-10);
            player.SetIQ(player.GetIQ()+5);
            player.SetEQ(player.GetEQ()+5);
        break;

        case 4:
            SetStatus("prof1");
            parameter.SetEnergy(parameter.GetEnerge()-15);
            player.SetIQ(player.GetIQ()+2);
            player.SetEQ(player.GetEQ()+2);
            player.SetCharm(player.GetCharm()+2);
            parameter.SetMoney(parameter.GetMoney()+20);
        break;

        case 5:
            SetStatus("prof2");
            parameter.SetEnergy(parameter.GetEnerge()-15);
            player.SetIQ(player.GetIQ()+10);
            player.SetCharm(player.GetCharm()+2);
            parameter.SetMoney(parameter.GetMoney()+10);
        break;

        case 6:
            SetStatus("prof1");
            parameter.SetEnergy(parameter.GetEnerge()-1);
            player.SetIQ(player.GetIQ()+2);
            player.SetEQ(player.GetEQ()+2);
            player.SetCharm(player.GetCharm()+10);
        break;

        case 7:
            SetStatus("eat");
            parameter.SetEnergy(parameter.GetEnerge()+15);
            parameter.SetMoney(parameter.GetMoney()-20);
            parameter.SetEat(parameter.GetEat()+1);
        break;

        case 8:
            SetStatus("study");
            parameter.SetEnergy(parameter.GetEnerge()-10);
            player.SetIQ(player.GetIQ()+3);
            player.SetEQ(player.GetEQ()+1);
            player.SetCharm(player.GetCharm()+1);
        break;

        case 9:
            SetStatus("sleep");
            parameter.SetDay(parameter.GetDay()+1);
            parameter.SetMoney(player.GetMoney()+100);

            if(parameter.GetEat()==0)
            {
                if (player.GetSex()==0){parameter.SetEnergy(40);}
                else{parameter.SetEnergy(35);}
            }
            else
            {
                parameter.SetEat(parameter.GetEat()-1);
                if (player.GetSex()==0){parameter.SetEnergy(50);}
                else{parameter.SetEnergy(45);}
            }

        break;

        case 10:
            SetStatus("movie");
        break;




        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        /*
        case 23:

            SetStatus("");
            emit fight(next_step);
            break;
        */
        case 27:
            // Sorceress's tip
            SetStatus("tip");
            emit events("tip2");
            break;
        case 28:
            player.SetMoney(player.GetMoney()+100);
            break;

        case 51:
            // Shopping
            SetStatus("shop");
            emit events("shop");
            break;
        case 80:
            // Upstairs
            player.SetPlace(player.GetPlace()+1);
            break;
        case 90:
            // downstairs
            player.SetPlace(player.GetPlace()-1);
            break;
        case 99:
            // End of the game
            SetStatus("ending");
            emit events("ending");
            break;
    }
    if (status == "main")
        emit change();
}

int View::access(int x, int y) {          // Check if map(x,y) is access or not
    int tmp = map[x][y][player.GetPlace()];
    next_step = tmp;
    if (tmp >= 500) {
        map[x][y][player.GetPlace()] = 0;
        return 1;
    } else
        return 0;
}

