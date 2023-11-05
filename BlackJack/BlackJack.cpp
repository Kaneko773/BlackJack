// BlackJack.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

//ポイントを判断して加算する関数
//
#include <iostream>
#include <ctime>
#include <cstdlib>
using namespace std;

class Status;

const int MARK = 4;
const int NUMBER = 13;

enum Mark {
    spade = 1,
    heart = 2,
    diamond = 3,
    clover = 4,
};
enum Number {
    reset = 0,
    ace = 1,
    two = 2,
    three = 3,
    four = 4,
    five = 5,
    six = 6,
    seven = 7,
    eight = 8,
    nine = 9,
    ten = 10,
    jack = 11,
    queen = 12,
    king = 13,
};

class Card {
public:
    //コンストラクタ
    Card() {
        _mark = spade;
        _number = ace;
    }
    //random用のコンストラクタ
    Card(Mark mark, Number number) {
        _mark = mark;
        _number = number;
    }

    void operator=(const Card& other) {
        _mark = other._mark;
        _number = other._number;
    }

public:
    Mark get_mark() {
        return _mark;
    }
    Number get_Number() {
        return _number;
    }

private:
    Mark _mark;
    Number _number;
};

Card MakeRandomCard();
void PlayerScoreCalculation(Status& player);//点数計算
void DealerScoreCalculation(Status& dealer);
void CardShow(Status other, int Index);//カード情報表示
void GeneralInformationShow(Status& player, Status& dealer);//全体情報表示

class Status {
public:
    //コンストラクタ
    Status() {
        _pcard = new Card[11];
        for (int i = 0; i < 11; ++i) {
            _pcard[i] = MakeRandomCard();
        }
        _outwardlyNum = 1;
        _score = 0;
    }
    //コピーコンストラクタ
    Status(const Status& other) {
        _pcard = new Card[11];
        for (int i = 0; i < 11; ++i) {
            _pcard[i] = other._pcard[i];
        }
        _outwardlyNum = other._outwardlyNum;
        _score = other._score;
    }
    //デストラクタ
    ~Status() {
        if (_pcard != NULL) {
            delete[] _pcard;
            _pcard = NULL;
        }
    }

public:
    Card get_pcard(int Index) {
        return _pcard[Index];
    }
    int get_outwardlyNum() {
        return _outwardlyNum;
    }
    void set_outwardlyNum() {
         ++_outwardlyNum;
    }
    int get_score() {
        return _score;
    }
    void set_score(Number point) {
        point == reset ? _score = 0 : _score += point;
    }

private:
    Card* _pcard;
    int _outwardlyNum;
    int _score;
};

int main()
{
    std::srand(time(NULL));

    Status player;
    Status dealer;
    bool Pburst = false;//プレイヤーのバースト判定
    bool Dburst = false;//ディーラーのバースト判定
    int input = 0;//入力用
    
    //player.set_outwardlyNum();//表向きの枚数＋１

    //プレイヤーの処理
    do {
        player.set_outwardlyNum();//表向きの枚数＋１
        GeneralInformationShow(player, dealer);

        if (player.get_score() > 21) {
            cout << "バーストしました" << endl << endl;
            Pburst = true;
            input = 1;
            continue;
        }

        cout << "1:スタンド  1以外の数:ヒット   ＞  " << flush;
        cin >> input;
    } while (input != 1);

    //ディーラーの処理
    if (!Pburst) {
        do {
            dealer.set_outwardlyNum();//表向きの枚数＋１
            GeneralInformationShow(player, dealer);
        } while (dealer.get_score() < 17);
        //ディーラーのバースト判定
        if (dealer.get_score() > 21)Dburst = true;
    }

    //結果表示
    if ((!Pburst && !Dburst && player.get_score() > dealer.get_score()) || Dburst) {
        cout << "プレイヤーの勝ちです" << endl;
    }
    else if (player.get_score() == dealer.get_score()) {
        cout << "引き分けです" << endl;
    }
    else {
        cout << "プレイヤーの負けです" << endl;
    }
}

Card MakeRandomCard() 
{
    //カードの使用状況
    static int cardUsageStatus[MARK][NUMBER] = { 0 };//初期化は全カードを未使用状態にする
    
    int mark = 0;
    int number = 0;
    
    //使用済みのカードを使えないようにする
    do {
        mark = rand() % MARK + 1;
        number = rand() % NUMBER + 1;
    } while (cardUsageStatus[mark - 1][number - 1] == 1);
    
    //使用済みにする
    cardUsageStatus[mark - 1][number - 1] = 1;
    
    Card random((Mark)mark, (Number)number);

    return random;
}

void PlayerScoreCalculation(Status& player)
{
    for (int i = 0; i < player.get_outwardlyNum(); ++i) {
        if (player.get_pcard(i).get_Number() == ace) {
            if (player.get_score() + 11 > 21) {
                player.set_score(player.get_pcard(i).get_Number());
            }
            else {
                player.set_score(jack);
            }
        }
        else if (two <= player.get_pcard(i).get_Number()
            && player.get_pcard(i).get_Number() <= ten) {
            player.set_score(player.get_pcard(i).get_Number());
        }
        else if (jack <= player.get_pcard(i).get_Number()
            && player.get_pcard(i).get_Number() <= king) {
            player.set_score(ten);
        }
    }
}

void DealerScoreCalculation(Status& dealer)
{
    for (int i = 0; i < dealer.get_outwardlyNum(); ++i) {
        if (dealer.get_pcard(i).get_Number() == ace) {
            if (dealer.get_score() + 11 > 21) {
                dealer.set_score(dealer.get_pcard(i).get_Number());
            }
            else {
                dealer.set_score(jack);
            }
        }
        else if (two <= dealer.get_pcard(i).get_Number()
            && dealer.get_pcard(i).get_Number() <= ten) {
            dealer.set_score(dealer.get_pcard(i).get_Number());
        }
        else if (jack <= dealer.get_pcard(i).get_Number()
            && dealer.get_pcard(i).get_Number() <= king) {
            dealer.set_score(ten);
        }
    }
}

void CardShow(Status other, int Index) 
{

    switch (other.get_pcard(Index).get_mark()) {
    case spade: printf("スペード   : "); break;
    case heart: printf("ハート     : "); break;
    case diamond: printf("ダイヤ     : "); break;
    case clover: printf("クローバー : "); break;
    }

    switch (other.get_pcard(Index).get_Number()) {
    case ace: printf("A"); break;
    case two: printf("2"); break;
    case three: printf("3"); break;
    case four: printf("4"); break;
    case five: printf("5"); break;
    case six: printf("6"); break;
    case seven: printf("7"); break;
    case eight: printf("8"); break;
    case nine: printf("9"); break;
    case ten: printf("10"); break;
    case jack: printf("J"); break;
    case queen: printf("Q"); break;
    case king: printf("K"); break;
    }
    cout << endl;
}

void GeneralInformationShow(Status& player, Status& dealer)
{
    system("cls");

    //得点をリセット
    player.set_score(reset);
    dealer.set_score(reset);

    cout << "自分の手札" << endl;
    for (int i = 0; i < player.get_outwardlyNum(); ++i) {
        CardShow(player, i);
    }
    PlayerScoreCalculation(player);
    cout << "点数 : " << player.get_score() << endl << endl;


    cout << "ディーラーの手札" << endl;
    for (int i = 0; i < dealer.get_outwardlyNum(); ++i) {
        CardShow(dealer, i);
    }
    DealerScoreCalculation(dealer);
    cout << "点数 : " << dealer.get_score() << endl << endl;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
