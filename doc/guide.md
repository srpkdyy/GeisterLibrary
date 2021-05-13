# プレイヤの開発手順
## 仕様
プレイヤはplayer.cppをコンパイルして動的ライブラリとして生成されます．  
生成される動的ライブラリは，各ツールの実行時にリンクされます．  
そのため，実際には特定の関数が呼び出されることで動作します．  
呼び出される関数は以下の2つです．
- std::string decideRed();：初期配置の決定
- std::string decideHand(std::string res);：着手の決定

これらは実際にはその内部でさらにPLAYER_CLASSで指定したクラス（デフォルトはRandomPlayer）の所定のメンバ関数を呼んでいます．  
また，Player.cppではPlayer/all.hppをインクルードしており，同ファイルにはmake実行時にPlayer/以下の*.hppファイルがすべてインクルードされます．  

## 開発手順
新規にプレイヤを開発する基本的な手順は，
1. Player/以下に拡張子.hppのファイルを作成（ファイル名は任意）
2. 作成したファイル内にPlayerクラスを継承したクラスを作成（クラス名は任意）
3. 作成したクラス内に次の2つのメンバ関数を実装
   - std::string decideRed();
   - std::string decideHand(std::string res);
4. 次のコマンドでビルド
    ```
    make -j PC={作成したクラス名}
    ```
  
となります．
また，ビルドコマンドの引数に
```
PN={任意の文字列}
```
を追加することで，生成されるファイル名を指定することができます．  
コードの変更ごとに別名をつけることで簡易的にプレイヤの管理が可能です．

## サンプルプレイヤ
現在，サンプルプレイヤとして，以下の2つが用意してあります．  
これらのコードも参考にしてください．

 名前 | 特徴
 --- | ---
 randomPlayer | 可能な手からランダムに手を決定します．
 chototsuPlayer | 猪突戦法と呼ばれる戦法に従います．