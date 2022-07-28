# saize_solver

## What's this ?

- サイゼの間違い探しを画像処理でやる
- お店で携帯で撮った写真からやる

入力画像
<img src="saize.png" width="300">

結果画像
<img src="output.png" width="300">

# Usage

```bash
git clone https://github.com/Yutarotaro/saize_solver.git && cd saize_solver
docker build -t opencv .
docker run -it -v $(pwd):/src -e DISPLAY=host.docker.internal opencv

#(in docker)
cd /src
mkdir build && cd build
cmake ..
make -j4
./main ../saize.png
```
