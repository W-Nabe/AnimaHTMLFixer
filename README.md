# AnimaHTMLFixer

Animaエクスポートファイルの公開作業を単純化するプログラムです。

このプログラムは、PHPライブラリ「Didom」を使用して、AnimaからエクスポートされるHTMLファイルを独自の命名規則に従い書き換えてWebページとして表示します。  

https://www.animaapp.com/  

FigmaやXDのデザインをHTMLコードとしてエクスポートしてくれるプラグイン「Anima」。  
できるだけ、コーディング作業をせずにWebページとして公開できるようにこのプラグインに行き着ついた方もいらっしゃると思います。

確かにAnimaを使用することでHTML/CSSコーディングをせずにWebページを作成できますが、一般公開するようなクオリティに仕上げるまでには、CSSの微調整・SEO対策・リンク設定などを手動で行う必要があります。  
AnimaHTMLFixerは、その作業をできるだけ簡単にできるように作成しました。

前提としてPHPを使用できるサーバーが必要です。
表示にあたってPHPでclass名を元にHTMLコードを書き換えるため、FigmaやXDのフレーム（アートボード）名・レイヤー名・グループ名・構造に命名規則があります。

パソコン表示・タブレット表示（パソコン表示を縮小させる）・スマホ表示に対応しています。  
FigmaやXDでは、パソコンとスマホのデザインをご用意ください。

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
- pc-wrapと同じ階層に、fixed-headerというグループを作成し、その配下はサンプル（後日更新）に則って作成してください。

### スマホ版 追尾ヘッダー・メニュー用レイヤーグループ
- 追尾ヘッダーは、sp-wrapと同じ階層に、fixed-header-spというグループを作成し、その配下はサンプル（後日更新）に則って作成してください。
- メニューは、sp-wrapと同じ階層に、fixed-sp-header-menuというグループを作成し、その配下はサンプル（後日更新）に則って作成してください。

### リンク
レイヤーやグループ名に「link-＊＊＊」という名前を付け、「./anima/link.csv」にリンク先を記述してください。  
3列目に_blankと付けることで、新しいタブで開かせるリンクにすることができます。

### アニメーション
Animaのプラグインでもアニメーションを追加することは可能ですが、「./anima/css/animate.css」のクラス名に基づいてレイヤー名に「anim-fadeIn」などを追加すると、手軽にアニメーションさせることができます。  
アニメーションの種類：https://animate.style/

### 画像のALT属性・非表示SEO用テキスト
命名規則：seo-＊＊＊  
グループ名や画像としてエクスポートするレイヤー名に「seo-***」という名前を付け、「./anima/seo.csv」に対応する文字列を記述してください。

### h1~h6、p要素
命名規則：h1-, h2-, h3-, h4-, h5-, h6-, p-  
グループ名・レイヤー名の先頭に「h1-」「h2-」「h3-」「h4-」「h5-」「h6-」「p-」と付けることで、対応した要素（＜h1＞タグなど）が作成されます。

### 要素の置き換え
命名規則：inc-＊＊＊  
特定のエリアにGoogleMapやYoutubeなどを埋め込みたいときに使用します。  
例：Figma上でグループ名を「inc-youtube」とし、AnimaHTMLFixerの「/anima/include/」ディレクトリに「inc-」以降の文字列「youtube」をファイル名とした「youtube.inc」ファイルを配置し、ファイルの中にyoutubeのiframe埋め込みタグを記述することで「inc-youtube」の中身の要素と自動で置き換えられます。

## Figma上で起こった問題
### 画像やレイアウトが正しく表示されない
すべてのレイヤーの制約を以下に設定してください。  
![image](https://github.com/W-Nabe/Anima-WebPage/assets/9455153/6b7378c4-63ca-43ed-ad0e-fbb469b4e6c0)

### 特定のグループが画像になってしまう
（原因が現在わかっていませんが）Figma上の設定が悪さをしていますので、グループを解除してや中に入っているコンテンツをもう一度、ひとつひとつ配置しなおすことで問題回避できました。

## LEANSYSTEMへのご依頼
もし、上記の手順などを一括でLEANSYSTEMへご依頼されたい場合は、お気軽に公式サイトからチャット等でご連絡ください。  
https://lean-system.co.jp/
