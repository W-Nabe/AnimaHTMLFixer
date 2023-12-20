<?php
/*
 * © LEANSYSTEM
 * Contact: nabe@lean-system.co.jp
 *
 * Figma・XDでのAnimaの使用方法やプログラムについてのご質問、ご相談などありましたらお気軽にお問い合わせください。
 *
 * ------------
 *
 * GETパラメータ「view」によってレンダーするファイルを切り替え、DIDOMでHTMLを書き換え出力する
 * _didom_functionsの関数を使用し、HTMLコードを最終的に生成するファイル
 *
 * Animaでエクスポートしたファイルを、anima/view配下に置く
 *
 */

require_once("./_didom_functions.inc");
global $view;
?>

<?= file_get_contents("./anima/config/insert_head_top.txt") // その他挿入するHTMLコード?>

<?php // 共通JSの読み込み ?>
<script src="https://code.jquery.com/jquery-3.6.3.min.js" integrity="sha256-pvPw+upLPUjgMXY0G+8O0xUf+/Im1MZjXxxgOcBQBXU=" crossorigin="anonymous" defer></script>
<script src="https://cdnjs.cloudflare.com/ajax/libs/jquery-easing/1.4.1/jquery.easing.min.js" integrity="sha512-0QbL0ph8Tc8g5bLhfVzSqxe9GERORsKhIn1IrpxDAgUsbBGz/V7iSav2zzW325XGd1OMLdL4UiqRJj702IeqnQ==" crossorigin="anonymous" referrerpolicy="no-referrer" defer></script>
<script src="./anima/js/variable.php" defer></script>
<script src="./anima/js/script.js" defer></script>

<?php // VIEWに応じたJSの読み込み ?>
<?php if (file_exists("./anima/js/page/{$view}.js")): ?>
  <script src="./anima/js/page/<?= $view ?>.js" defer></script>
<?php endif; ?>

<?php // 共通CSS ?>
<link rel="stylesheet" type="text/css" href="./anima/css/variable.php">
<link rel="stylesheet" type="text/css" href="./anima/css/animate.css">
<link rel="stylesheet" type="text/css" href="./anima/css/style.css">

<?php // VIEWに応じたCSSの読み込み ?>
<?php if (file_exists("./anima/css/page/{$view}.css")): ?>
  <link rel="stylesheet" type="text/css" href="./anima/css/page/<?= $view ?>.css">
<?php endif; ?>

<link rel="stylesheet" type="text/css" href="./anima/css/_tablet.css" media="screen and (max-width: <?= ($config["tabletbreakpoint"]["value"] - 1); ?>px)">
<link rel="stylesheet" type="text/css" href="./anima/css/_smallsp.css" media="screen and (max-width: <?= ($config["smallspbreakpoint"]["value"] - 1); ?>px)">

<?php // アイコン ?>
<link rel="icon" type="image/x-icon" href="./anima/images/<?= $config["favicon"]["value"] ?>">
<link rel="apple-touch-icon" href="./anima/images/<?= $config["touchicon"]["value"] ?>" sizes="180x180">
<link rel="icon" type="image/png" href="./anima/images/<?= $config["touchicon"]["value"] ?>" sizes="192x192">

<?php // タイトル ?>
<title><?= $meta["title"] ?? "" ?></title>

<?php // OGP情報 ?>
<meta property="og:site_name" content="<?= $config["sitetitle"]["value"] ?>">
<meta name="description" content="<?= $meta["description"] ?? "" ?>">
<meta name="og:description" content="<?= $meta["description"] ?? "" ?>">
<meta name="keywords" content="<?= $meta["keywords"] ?? "" ?>">
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



/* 前処理 */
/* ------------------------------- */
appendLoading($dom); // <div id="loading"></div>を追加する
appendViewPrefixToSrc($dom->find("img")); // imgのsrcに「view」を追加する
appendViewPrefixToHref($dom->find("link[rel='stylesheet']")); // cssのhrefに「view」を追加する
removeUnusedScreen($dom); // 使用しない「.screen」を削除する
if($config["designcheck"]["isuse"]) appendDesignCheck($dom); // <div class="design-check"></div>を追加する
toSection($dom->find("div[class^='pc-wrap-'] > div")); // pc-wrap直下にあるdiv要素をsection要素に変更する
toSection($dom->find("div[class^='sp-wrap-'] > div")); // sp-wrap直下にあるdiv要素をsection要素に変更する
changePToDiv($dom->find("p")); // <p>タグがあったら、「<div>」に変換する
changeH1ToDiv($dom->find("h1")); // <h1>タグがあったら、「<div>」に変換する
removeAltToImg($dom->find("img")); // <img>のalt属性を空にする



/* 命名規則処理 */
/* ---------------- */
// figma命名規則：-id_***_
assignId($dom->find("[class*='id_']")); // 要素にIDを付ける。
assignId($dom->find("[class*='id-']")); // 要素にIDを付ける。

// figma命名規則：h1-（<div>のクラスに「(h[1-6]|p)」があったら、「<h[1-6]|p>」に変換する）
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='h1-']"));
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='h2-']"));
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='h3-']"));
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='h4-']"));
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='h5-']"));
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='h6-']"));
changeDivToHOrPSpecifiedByClass($dom->find("div[class^='p-']"));

// figma命名規則：h1-（<img>のクラスに「(h[1-6]|p)」があったら、「<h[1-6]|p>」で囲う）
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='h1-']"));
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='h2-']"));
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='h3-']"));
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='h4-']"));
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='h5-']"));
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='h6-']"));
wrapImgWithHOrPSpecifiedByClass($dom->find("img[class^='p-']"));

// figma命名規則：seo-***
appendAltToImgFromClass($dom->find("img[class*='seo-']")); // クラスで指定されたalt属性を持たせる（alt属性の値は/anima/config/seo.csvに記述）
appendSeotxtToTagFromClass($dom->find("[class*='seo-']:not(img)"), $dom); // SEOテキスト挿入（テキストは/anima/config/seo.csvに記述）

// figma命名規則：link-***
wrapA($dom->find("*[class*='link-']")); // リンクにする（リンク先は/anima/config/link.csvに記述）

// figma命名規則：svg-***
replaceSvg($dom); // SVGを埋め込みに置き換える（埋め込み用SVGファイルは/anima/images/replace-svg/にAnimaでエクスポートされたimgファイルと同じファイル名で、拡張子はsvgにして配置）

// figma命名規則：anim-***
// アニメーションを追加する（***は、https://animate.style/の画面右側のリストの各アニメーション名を使用 例：anim-bounceIn）

// figma命名規則：anim-wrap
wrapChildSpanForAnimate($dom->find("[class*='anim-wipe']")); // 子要素を<span class="wrap">で囲む。

// figma命名規則：anim-text-enter
wrapEachTextWithSpan($dom->find('[class*="anim-text-enter"]')); // anim-text-enterクラスの要素内のテキストを1文字ずつbで囲む

// figma命名規則：inc-***
replaceIncContent($dom->find("*[class*='inc-']")); // /anima/include/***.incファイルを読み込みinc-クラスがついている要素の配下のコンテンツと置き換える


/* 後処理 */
/* ---------------- */
replaceImgsSrc($dom); // replace-viewimg/imgに同じファイル名の画像があったら各imgタグのsrcを置き換える（XD・Figmaで2倍サイズで出力されない画像に使用）
replaceCssHref($dom); // replace-viewcss/globals.css、replace-viewcss/styleguide.cssがあったら置き換える（フォントの修正などに使用）
changeImgSrcToDataOriginal($dom->find("div[class^='pc-wrap'] img")); // .pc-wrap配下の<img>のsrcを遅延読み込みのためdata-originalにする
changeImgSrcToDataOriginal($dom->find("div[class^='sp-wrap'] img")); // .sp-wrap配下の<img>のsrcを遅延読み込みのためdata-originalにする