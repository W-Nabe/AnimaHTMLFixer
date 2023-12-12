<?php
/*
 * GETパラメータ「view」によってレンダーするファイルを切り替え、DIDOMでHTMLを書き換え出力する
 * _didom_functionsの関数を使用し、HTMLコードを最終的に生成するファイル
 *
 * Animaでエクスポートしたファイルを、anima/view配下に置く
 *
 * @author 永田実 nagata@lean-system.co.jp
 */

require_once("./_didom_functions.inc");
global $view;
?>

<?= file_get_contents("./anima/config/insert_head_top.txt") // その他挿入するHTMLコード?>

<?php // 共通JSの読み込み ?>
<script src="https://code.jquery.com/jquery-3.6.3.min.js" integrity="sha256-pvPw+upLPUjgMXY0G+8O0xUf+/Im1MZjXxxgOcBQBXU=" crossorigin="anonymous" defer></script>
<script src="./anima/js/variable.php" defer></script>
<script src="./anima/js/script.js" defer></script>

<?php // VIEWに応じたJSの読み込み ?>
<?php if (file_exists("./anima/js/{$view}.js")): ?>
  <script src="./anima/js/<?= $view ?>.js" defer></script>
<?php endif; ?>

<?php // 共通CSS ?>
<link rel="stylesheet" type="text/css" href="./anima/css/variable.php">
<link rel="stylesheet" type="text/css" href="./anima/css/animate.css">
<link rel="stylesheet" type="text/css" href="./anima/css/style.css">

<?php // VIEWに応じたCSSの読み込み ?>
<?php if (file_exists("./anima/css/{$view}.css")): ?>
  <link rel="stylesheet" type="text/css" href="./anima/css/<?= $view ?>.css">
<?php endif; ?>

<link rel="stylesheet" type="text/css" href="./anima/css/media/tablet.css" media="screen and (max-width: <?= ($config["tabletbreakpoint"]["value"] - 1); ?>px)">
<link rel="stylesheet" type="text/css" href="./anima/css/media/smallsp.css" media="screen and (max-width: <?= ($config["smallspbreakpoint"]["value"] - 1); ?>px)">

<?php // アイコン ?>
<link rel="icon" type="image/x-icon" href="./anima/images/<?= $config["favicon"]["value"] ?>">
<link rel="apple-touch-icon" href="./anima/images/<?= $config["touchicon"]["value"] ?>" sizes="180x180">
<link rel="icon" type="image/png" href="./anima/images/<?= $config["touchicon"]["value"] ?>" sizes="192x192">

<?php // タイトル ?>
<title><?= $meta["title"] ?></title>

<?php // OGP情報 ?>
<meta property="og:site_name" content="<?= $config["sitetitle"]["value"] ?>">
<meta name="description" content="<?= $meta["description"] ?>">
<meta name="og:description" content="<?= $meta["description"] ?>">
<meta name="keywords" content="<?= $meta["keywords"] ?>">
<meta property="og:url" content="<?= getCurrentUrlWithoutHash() ?>">
<meta name="twitter:card" content="summary_large_image">
<meta property="og:locale" content="ja_JP">
<meta property="og:image" content="./anima/images/<?= $config["ogp_image_common"]["value"] ?>">

<?= file_get_contents("./anima/config/insert_head_bottom.txt") // その他挿入するHTMLコード ?>

<?php
// DOM操作用ライブラリ (https://zenn.dev/yu_no/articles/8f180991dcca62)
require_once("./vendor/autoload.php");
use DiDom\Document;
use DiDom\Element;

// viewをDOMとして読み込む
$dom = new Document("./anima/view/".getViewFileName().".html", true);

appendLoading($dom); // <div id="loading"></div>を追加する。
appendViewPrefixToSrc($dom->find("img")); // imgのsrcに「view」を追加する。
appendViewPrefixToHref($dom->find("link[rel='stylesheet']")); // cssのhrefに「view」を追加する。
removeUnusedScreen($dom); // 使用しない「.screen」を削除する。
if($config["designcheck"]["isuse"]) appendDesignCheck($dom); // <div class="design-check"></div>を追加する。

// pc-wrap, sp-wrap直下にあるdiv要素をsection要素に変更する。
toSection($dom->find("div[class^='pc-wrap-'] > div"));
toSection($dom->find("div[class^='sp-wrap-'] > div"));

assignId($dom->find("*[class*='-id_']")); // IDを付ける。

// <p>タグがあったら、「<div>」に変換する。
changePToDiv($dom->find("p"));
// <h1>タグがあったら、「<div>」に変換する。
changeH1ToDiv($dom->find("h1"));

// <div>のクラスに「(h[1-6]|p)」があったら、「<h[1-6]|p>」に変換する。
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='h1-']"));
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='h2-']"));
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='h3-']"));
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='h4-']"));
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='h5-']"));
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='h6-']"));
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='p-']"));

// <img>のクラスに「(h[1-6]|p)」があったら、「<h[1-6]|p>」で囲う。
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='h1-']"));
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='h2-']"));
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='h3-']"));
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='h4-']"));
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='h5-']"));
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='h6-']"));
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='p-']"));

removeAltToImg($dom->find("img")); // <img>のalt属性を空にする
appendAltToImgFromClass($dom->find("img[class*='seo-']")); // クラスで指定されたalt属性を持たせる。
appendSeotxtToTagFromClass($dom->find("[class*='seo-']:not(img)"), $dom); // SEOテキスト挿入
wrapA($dom->find("*[class*='link-']")); // <a>で囲む。
wrapChildSpanForAnimate($dom->find("[class*='anim-wipe']")); // 子要素を<span class="wrap">で囲む。

// XD・Figmaで2倍サイズで出力されない画像の対処：replace-viewimg/imgに画像があったら各imgタグのsrcを置き換える
replaceImgsSrc($dom);
// XD・Figmaで出力されたCSSの対処：replace-viewcss/にCSSがあったら各styleタグのcssを置き換える
replaceCssHref($dom);

// 指定された<img>のsrcをdata-originalにする。
// 但し「div[class^='pc-wrap'] div[class*='slick']」及び「div[class^='sp-wrap'] div[class*='slick']」配下を除く。
/* not 疑似セレクタが動作しない？
changeImgSrcToDataOriginal($dom->find(
  "img:not(div[class^='pc-wrap'] div[class*='slick'] img, div[class^='sp-wrap'] div[class*='slick'] img)")
);
*/
changeImgSrcToDataOriginal($dom->find("div[class^='pc-wrap'] img"));
changeImgSrcToDataOriginal($dom->find("div[class^='sp-wrap'] img"));
changeImgDataOriginalToSrc($dom->find("div[class^='pc-wrap'] div[class*='slick'] img"));
changeImgDataOriginalToSrc($dom->find("div[class^='sp-wrap'] div[class*='slick'] img"));

loadViewIncfile($dom);

// DOMをhtmlとして表示
echo output($dom);
