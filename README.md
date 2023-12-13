# Animaエクスポートファイルの公開作業を単純化するプログラム

FigmaやXDのデザインをHTMLコードとしてエクスポートしてくれるプラグイン「Anima」。
できるだけ、コーディング作業をせずにWebページとして公開できるようにこのプラグインに行き着ついた方もいらっしゃると思います。

確かにAnimaを使用することでHTML/CSSコーディングをせずにWebページを作成できますが、一般公開するようなクオリティにするには、CSSの微調整・SEO対策・リンク設定などを手動で行う必要があります。
このリポジトリは、その作業をできるだけ簡単にできるように作成しました。

※前提としてPHPを使用できるサーバーが必要です。

## 何をしているのか

このプログラムは、PHPライブラリ「Didom」を使用して、AnimaからエクスポートされるHTMLファイルを書き換えてWebページとして表示します。
表示にあたってPHPでclass名を元にHTMLコードを書き換えるので、FigmaやXDのアートボード名・レイヤー名・グループ名・構造が重要になります。

パソコン表示・タブレット表示（パソコン表示を縮小させる）・スマホ表示に対応しています。
FigmaやXDでは、パソコンデザイン・スマホデザインを用意してください。

## サンプル
### Figma
https://www.figma.com/file/Zixywb9dXMlZIokve54ljH/  
ご不明点がございましたら、お問い合わせください。

### Adobe XD
準備中です。XDのファイルサンプルをご希望の方は、ぜひLEANSYSTEMにお問い合わせください。  
https://lean-system.co.jp/

### サンプルWebページ
https://lean-system.co.jp/_watanabe/leancoding/

## 設置方法
+ Figma・XDのアートボード名・レイヤー名・グループ名・構造を整理する
+ Animaプラグインを使用して、HTMLとしてプロジェクトにSyncさせ、Animaのプレビューページに行く
+ Animaのプレビューページで「Export Code」ボタンを押し、「HTML」「Styling：Absolute Position」「Length unit：PX」の設定でソースをダウンロードする
+ ダウンロードしたファイルを解凍し、中身を「./anima/view」ディレクトリに全て入れる
+ 全ファイルをサーバーに置き、「https://＊＊＊＊.jp/」にアクセスする。その他ページは「https://＊＊＊＊.jp/page1.html」でアクセス可能です。

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

### アニメーション
Animaのプラグインでもアニメーションを追加することは可能ですが、「./anima/css/animate.css」のクラス名に基づいてレイヤー名に「anim-fadeIn」などを追加すると、手軽にアニメーションさせることができます。  
アニメーションの種類：https://animate.style/