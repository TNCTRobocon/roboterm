# ロボコン用組み込みシェル

用途: 通信及びデバック試験機構

# 動作環境
* c99が動く環境。

# 特徴

* ポインタをファイルとみなす疑似ファイルシステムを持つ
* 静的なメモリ確保により、mallocが利用できない環境でも動作する
  + 内部的にメモリープールを持つ
# デモ

Ubuntu等の環境で次のコマンドを実行してビルド環境を整える。
```bash
sudo apt update
sudo apt upgrade -y
sudo apt install build-essential git cmake
```

githubからダウンロードして、ビルドする。
```bash
git clone <このリポジトリのURL>
cd roboterm
mkdir build
cmake ..
make
./roboterm
```

# 著作権

terurin及び豊田高専ロボコンAが著作権を有する。
非商用用途での利用及び改造を許可する。
また、利用により何らかの障害等が生じても何ら責任を追わない。