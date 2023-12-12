# Animaのエクスポートファイルを

FigmaやXDのデザインをHTMLコードとしてエクスポートしてくれるプラグイン「Anima」。
Animaを使用すると、WebページをHTML/CSSコーディングをせずに作成できますが、一般公開するまでには、CSSの微調整・SEO対策・リンク設定などを手動で行う必要があります。
このリポジトリは、その作業をできるだけ簡単にできるように作成しました。

フロントデザインを行うコーダーの作業が少しでも楽になりますように。

## 中身は何をしているのか

このプログラムは、PHPライブラリ「Didom」を使用して、AnimaからエクスポートされるHTMLファイルを書き換えてWebページとして表示します。
PHPでclass名を元にHTMLコードを書き換えるので、FigmaやXDのアートボード名・レイヤー名・グループ名・構造が重要になります。

## サンプル
FigmaやXDのファイルサンプルをご希望の方は、ぜひLEANSYSTEMにお問い合わせください。

https://lean-system.co.jp/

## 設置方法
+ Figma・XDのアートボード名・レイヤー名・グループ名・構造を整理する
+ Animaプラグインを使用して、HTMLとしてプロジェクトにSyncさせ、Animaのプレビューページに行く
+ Animaのプレビューページで「Export Code」ボタンを押し、「HTML」「Styling：Absolute Position」「Length unit：PX」の設定でソースをダウンロードする
+ ダウンロードしたファイルを解凍し、中身を「./anima/view」ディレクトリに全て入れる
+ 全ファイルをサーバーに置き、「https://****.jp/」にアクセスする。その他ページは「https://****.jp/page1.html」でアクセス可能です。

## その他設定
+ 「./anima/config」ディレクトリにあるconfig.csvでサイトの名前、ブレイクポイントなどの設定ができます。
+ 「./anima/config」ディレクトリにあるseo、link、metaのcsvファイルを書き換えることで、命名規則に従ってハイパーリンク・画像へのalt属性追加・meta情報を設定できます。
+ 「./images」ディレクトリにfavicon、タッチアイコン、loading画像などを設置します。
+ 独自のCSSやjavascriptを挿入したい場合は、パソコン版のアートボード名に従って対応するファイルを作成してください。トップページの例：「./anima/css/index.css」「./anima/js/index.js」

## FigmaやXDでの命名規則
### アートボード
パソコン版の表示用のアートボードと、スマホ版の表示用のアートボードを用意してください。以下は例です。

- トップページ：index　　トップページ（スマホ）：index-sp
- ページ１：page1　　ページ１（スマホ）：page1-sp
- お問い合わせ：contact　　お問い合わせ（スマホ）：contact-sp

### トップ階層のレイヤーグループ名
- パソコン用のアートボードは、pc-wrapというグループを作成し、その配下に全てのレイヤーを入れてください。
- スマホ用のアートボードは、sp-wrapというグループを作成し、その配下に全てのレイヤーを入れてください。

### パソコン版 追尾ヘッダー用レイヤーグループ名
- pc-wrapと同じ階層に、fixed-headerというグループを作成し、その配下はサンプルに則って作成してください。

### スマホ版 追尾ヘッダー・メニュー用レイヤーグループ
- 追尾ヘッダーは、sp-wrapと同じ階層に、fixed-header-spというグループを作成し、その配下はサンプルに則って作成してください。
- メニューは、sp-wrapと同じ階層に、fixed-sp-header-menuというグループを作成し、その配下はサンプルに則って作成してください。
