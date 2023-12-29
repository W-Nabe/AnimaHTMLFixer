# include機能の使い方

1. figma上で「inc-＊＊＊」というグループを作成する（＊＊＊は、ハイフンを含まない英数字の文字列）
1. 「/anima/include」ディレクトリに、＊＊＊.incファイルを作成する
1. ＊＊＊.incファイルに埋め込みたい、表示したいHTMLコード・CSSなどを記述する

注意：javascriptを使用する埋め込みタグなどは、barba.jsを有効している場合正常に動かなくなる可能性があります。barba.jsを有効している場合「/anima/config/insert_body_bottom.txt」などに記述してください。barba.jsを有効にしていない場合は、incファイルに書き込んで問題ありません。