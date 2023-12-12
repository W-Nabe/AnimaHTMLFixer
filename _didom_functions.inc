<?php
/*
 * config.csvを読み込む。
 *
 * @return config.csvの連想配列
 */

require_once("./vendor/autoload.php");
use DiDom\Document;

if(!isset($view)) $view = getCurrentView(); // view名決定。デフォルトはindex
$config = loadConfigCsv(); // config.csvを読み込む。
$seotext = loadSeoTextCsv(); // seotext.csvを読み込む。
$link = loadLinkCsv(); // link.csvを読み込む。
$meta = loadMetaCsv(); // meta.csvを読み込む。

function loadConfigCsv() {
  $csv = array();
  $handle = fopen("./anima/config/config.csv", "r");
  if (!$handle) exit;

  fgetcsv($handle); // タイトル行をスキップ
  while ($data = fgetcsv($handle)) $csv += array($data[0] => array("isuse" => $data[1] == "TRUE", "value" => $data[2]));

  fclose($handle);
  return $csv;
}

function loadViewIncfile($dom) {
  global $view;
  global $config;

  $filename = 'anima/include/'.$view.'.inc';
  if (file_exists($filename))  require_once($filename);

  return;
}

/*
 * {$view}.htmlの存在をチェックし、ファイル名を返す
 *
 * @return $viewfile
 */
function getViewFileName() {
  global $view;
  if (file_exists("./anima/view/".$view.".html")) {
    $viewfile = $view;
    return $viewfile;

  } elseif(file_exists("./anima/view/".$view."-sp.html")) {
    $viewfile = $view."-sp";
    return $viewfile;

  } elseif(file_exists("./anima/view/".$view."-all-breakpoints.html")) {
    $viewfile = $view."-all-breakpoints";
    return $viewfile;

  } elseif(file_exists("./anima/view/".$view."-sp-all-breakpoints.html")) {
    $viewfile = $view."-sp-all-breakpoints";
    return $viewfile;

  }
}

/*
 * seotext.csvを読み込む。
 *
 * @return seotext.csvの連想配列
 */
function loadSeoTextCsv() {
  $csv = array();
  $handle = fopen("./anima/config/seotext.csv", "r");
  if (!$handle) exit;

  fgetcsv($handle); // タイトル行をスキップ
  while ($data = fgetcsv($handle)) {
    if ($data[0]) $csv[$data[0]] = $data[1];
  }

  fclose($handle);
  return $csv;
}

/*
 * link.csvを読み込む。
 *
 * @return link.csvの連想配列
 */
function loadLinkCsv() {
  $csv = array();
  $handle = fopen("./anima/config/link.csv", "r");
  if (!$handle) exit;

  fgetcsv($handle); // タイトル行をスキップ
  while ($data = fgetcsv($handle)) $csv[$data[0]] = array("link" => $data[1], "target" => $data[2]);

  fclose($handle);
  return $csv;
}

/*
 * meta.csvを読み込む。
 * ページURLが現在のviewのもののみ。
 *
 * @return meta.csvの連想配列
 */
function loadMetaCsv() {
  global $view;
  $csv = array();
  $handle = fopen("./anima/config/meta.csv", "r");
  if (!$handle) exit;

  $target = $view;
  fgetcsv($handle); // タイトル行をスキップ
  while ($data = fgetcsv($handle)) {
    if ($data[0] == $target) {
      $csv["title"] = $data[1];
      $csv["description"] = $data[2];
      $csv["keywords"] = $data[3];
      break;
    }
  }

  fclose($handle);
  return $csv;
}

/*
 * <body>直下に<div id="loading"></div>を追加する。
 *
 * @param $dom ドキュメントのDOM
 */
function appendLoading($dom) {
  $dom->first("body")->style()->setProperty('opacity', '0');
  $loading = $dom->createElement("div", null, ["id" => "loading"]);
  $innerHtml = '
  <div class="loading-img-wrap"><img class="loading-img bg" src="anima/images/loading.png"
  srcset="anima/images/loading.png 1x, anima/images/loading@2x.png 2x">
    <div class="progress">
      <div><img class="loading-img" src="anima/images/loading.png"
          srcset="anima/images/loading.png 1x, anima/images/loading@2x.png 2x"></div>
    </div>
  </div>
  ';
  $loading->setInnerHtml($innerHtml);
  $ref = $dom->first("body")->child(0);
  $dom->first("body")->insertBefore($loading, $ref);
}

/*
 * <div class="screen">直下に<div class="design-check"></div>を追加する。
 *
 * @param $dom ドキュメントのDOM
 */
function appendDesignCheck($dom) {
  global $view;
  global $config;
  $spbreakpoint = $config["spbreakpoint"];
  $sp = htmlspecialchars($_COOKIE['windowWidth']) < $spbreakpoint["value"] ? '-sp' : '';
  $designcheck = $dom->createElement("div", null, ["id" => "designcheck"]);
  $designcheck->style()->setProperty('background-image', "url('./anima/images/designcheck/{$view}{$sp}.png')");
  $designcheckBtn = $dom->createElement("div", null, ["id" => "designcheck-btn"]);
  $ref = $dom->first(".screen")->child(0);
  $dom->first(".screen")->insertBefore($designcheck, $ref);
  $dom->first(".screen")->insertBefore($designcheckBtn, $ref);
}

/*
 * 要素は「src」属性を持っていなければならない。
 *
 * @param $elemsWithSrc 処理する要素の配列
 */
function appendViewPrefixToSrc($elemsWithSrc)
{
  foreach ($elemsWithSrc as $elem) $elem->attr('src', "anima/view/{$elem->attr('src')}");
}

/*
 * すべての要素の「href値に「view」を追加する。
 * 要素は「href」属性を持っていなければならない。
 *
 * @param $elmsWithHref 処理する要素の配列
 */
function appendViewPrefixToHref($elemsWithHref)
{
  foreach ($elemsWithHref as $elem) $elem->attr("href", "anima/view/{$elem->attr('href')}");
}

/*
 * 使用しない「.screen」を削除する。
 *
 * @param $dom ドキュメントのDOM
 */
function removeUnusedScreen($dom)
{
  global $config;
  $spbreakpoint = $config["spbreakpoint"];
  if (!$spbreakpoint["isuse"]) return;
  $target = htmlspecialchars($_COOKIE['windowWidth']) < $spbreakpoint["value"] ? '[class*="pc-wrap"]' : '[class*="sp-wrap"]';
  $dom->first($target)->parent()->parent()->remove();
}

/*
 * view内の無用なタグを削除する。
 *
 * @param $dom ドキュメントのDOM
 * @return string 無用なタグを削除したHTML
 */
function removeUselessHeadOfView($dom) {
  $res = $dom->format()->html();
  $res = substr($res, strlen("<!DOCTYPE html>\n<html>\n  <head>\n"));

  $res = str_replace([
    '<!--<meta name=description content="This site was generated with Anima. www.animaapp.com"/>-->',
    '<!-- <link rel="shortcut icon" type=image/png href="https://animaproject.s3.amazonaws.com/home/favicon.png" /> -->',
    '<link rel="shortcut icon" type="image/png" href="https://animaproject.s3.amazonaws.com/home/favicon.png">'
  ], "", $res);

  return $res;
}

/*
 * 指定された要素をsection要素に変換する。
 *
 * @param $elems 対象要素の配列
 */
function toSection($elems) {
  foreach ($elems as $elem) {
    $section = new Didom\Element('section', null, $elem->attributes());
    $section->setInnerHtml($elem->innerHtml());
    $elem->replace($section);
  }
}

/*
 * p要素をdiv要素に変換する。
 *
 * @param $ps 対象pの配列
 */
function changePToDiv($ps) {
  foreach ($ps as $div) {
    $elem = new Didom\Element('div', null, $div->attributes());
    $elem->setInnerHtml($div->innerHtml());

    $div->replace($elem);
  }
}

/*
 * h1要素をdiv要素に変換する。
 *
 * @param $h1s 対象h1の配列
 */
function changeH1ToDiv($h1s) {
  foreach ($h1s as $div) {
    $elem = new Didom\Element('div', null, $div->attributes());
    $elem->setInnerHtml($div->innerHtml());

    $div->replace($elem);
  }
}

/*
 * 指定されたdiv要素をクラスで指定された要素に変換する。
 * 変換できるのはh1〜6およびpのみ。
 *
 * @param $divs 対象divの配列
 */
function changeDivToHOrPSpecifiedByClass($divs) {
  foreach ($divs as $div) {
    $elem = new Didom\Element(getHOrPFromClass($div), null, $div->attributes());
    $elem->setInnerHtml(getStrDivOfChildToSpan($div));

    $div->replace($elem);
  }
}

/*
 * <div>内の<div>を<span>に変換した文字列を得る。
 *
 * @param $div 対象div
 * @return string <div>を<span>に変換した文字列
 */
function getStrDivOfChildToSpan($div) {
  $res = $div->innerHtml();
  $res = str_replace("<div", "<span", $res);
  $res = str_replace("</div>", "</span>", $res);
  return $res;
}

/*
 * 指定された要素のIDを設定する。
 * IDは要素のクラス文字列「-id_foobar_-」で指定されているものとする。
 * 「foobar」がIDとなる。
 *
 * 例: <div class="button original-button-id_foobar_-99999">
 *
 * @param $elems 対象要素の配列
 */
function assignId($elems) {
  foreach ($elems as $elem) {
    // 要素のクラスから「-id_foobar_-」を持つクラス文字列を得る。複数ある場合は最初のものとする。
    $class = preg_grep("/-id_[^_]+_-/", $elem->classes()->getAll())[0];

    // 「-id_foobar_-」から「foobar」を得る。
    preg_match("/-id_([^_]+)_-/", $class, $a);

    $elem->attr("id", $a[1]); // 要素のIDを設定する。「-id_foobar_-」が複数ある場合は最初のものとする。
  }
}

/*
 * 指定されたimg要素をクラスで指定された要素で囲う
 * 囲えるのはh1〜6およびpのみ。
 *
 * @param $imgs 対象imgの配列
 */
function wrapImgWithHOrPSpecifiedByClass($imgs) {
  foreach ($imgs as $img) {
    $elem = new Didom\Element(getHOrPFromClass($img));
    $elem->appendChild($img);
    $img->replace($elem);
  }
}

/*
 * 指定された要素のクラスで指定されたタグ名を得る。
 * 指定できるのはh1〜6およびpのみ。
 *
 * @param $elm 対象要素
 */
function getHOrPFromClass($elem) {
  // 要素のクラスから/^(h[1-6]|p)-txt-/を持つクラス文字列を得る。複数ある場合は最初のものとする。
  $class = preg_grep("/^(h[1-6]|p)-txt-/", $elem->classes()->getAll())[0];

  preg_match("/^(h[1-6]|p)-txt-/", $class, $a);
  return $a[1];
}

/*
 * 指定された<img>に、クラスで指定されたalt属性を持たせる。
 * alt文字列はCSVファイルに指定されているものとする。
 * @param $imgs 対象imgの配列
 */
function appendAltToImgFromClass($imgs) {
  global $seotext;

  foreach ($imgs as $img) {
    // <img>のクラスから/txt-\d\d\d\d/を持つクラス文字列を得る。複数ある場合は最初のものとする。
    $class = preg_grep("/(\b|-)txt-\d\d\d\d-/", $img->classes()->getAll())[0];

    preg_match("/(\b|-)(txt-\d\d\d\d)-/", $class, $a);
    $img->attr("alt", $seotext[$a[2]]);
  }
}

/*
 * 指定された要素を<a>で囲む。
 * リンク先はCSVファイルに指定されているものとする。
 * @param $elms 対象要素の配列
 */
function wrapA($elms) {
  global $link;

  foreach ($elms as $elm) {
    // 要素のクラスから/link-\d\d\d\d/を持つクラス文字列を得る。複数ある場合は最初のものとする。
    $class = preg_grep("/(\b|-)link-\w+-/", $elm->classes()->getAll())[0];

    preg_match("/(\b|-)(link-\w+)-/", $class, $a);
    if (!$l = $link[$a[2]]) continue;

    $a = new Didom\Element("a", null, ["href" => $l["link"]]);
    $a->appendChild($elm);
    if ($l["target"]) $a->attr("target", $l["target"]);
    $elm->replace($a);
  }
}

/*
 * 指定された<img>のsrcをdata-originalにする。
 *
 * @param $img 処理する<img>の配列
 */
function changeImgSrcToDataOriginal($imgs) {
  foreach ($imgs as $img) {
    $img->attr("data-original", $img->attr("src"));
    $img->attr("src", "./anima/images/dummy.png");
  }
}

/*
 * 指定された<img>のdata-originalをsrcにする。
 *
 * @param $img 処理する<img>の配列
 */
function changeImgDataOriginalToSrc($imgs) {
  foreach ($imgs as $img) {
    $img->attr("src", $img->attr("data-original"));
    $img->removeAttribute("data-original");
  }
}

/*
 * 現在のURLを得る。
 * 但し末尾の#ハッシュ値は消す。
 * https://blog.codecamp.jp/php-request-url
 *
 * @return string 現在のURL
 */
function getCurrentUrlWithoutHash() {
  preg_match("/^([^#]+)#?/", $_SERVER["REQUEST_URI"], $a);
  $path = $a[1];
  return (empty($_SERVER['HTTPS']) ? 'http://' : 'https://') . $_SERVER['HTTP_HOST'] . $path;
}

/*
 * その他HTMLコードを<body>の直後に追加する。
 *
 * @param $dom ドキュメントのDOM
 */
function insertBodyTop($dom) {
  global $config;
  $html = new Document("./anima/config/{$config["insert_body_top"]["value"]}", true);
  $dom->first("body")->insertBefore($html->first("body")->child(0), $dom->first("body")->child(0));
}

/*
 * その他HTMLコードを<body>の直前に追加する。
 *
 * @param $dom ドキュメントのDOM
 */
function insertBodyBottom($dom) {
  global $config;
  $html = new Document("./anima/config/{$config["insert_body_bottom"]["value"]}", true);
  $dom->first("body")->insertBefore($html->first("body")->child(0), $dom->first("body")->child(0));
}

/*
 * BOWNOW用埋め込みコードを#bownowembedに追加する。
 *
 * @param $dom ドキュメントのDOM
 */
function insertBownow($dom) {
  global $config;
  if (!($bownowembed = $dom->first("#bownowembed"))) return;

  $bownowembedcode = file_get_contents("./anima/config/{$config["bownow"]["value"]}");
  $bownowembed->setInnerHtml($bownowembedcode);
}

/*
 * view名を得る。
 * デフォルトはindex。
 *
 * @return string view名
 */
function getCurrentView() {
  if (isset($_GET['view'])) {
    $res = htmlspecialchars($_GET['view']);
    return $res;
  }
  if (preg_match("/(\w+)\.html/", htmlspecialchars($_SERVER["REQUEST_URI"]), $a)) return $a[1];
  return "index";
}


/*
 * Animate.css用処理
 * 子要素をdiv.wrap要素で囲う
 *
 * @param $elms 対象要素の配列
 */
function wrapChildDivForAnimate($elms) {
  foreach ($elms as $elm) {
    $attributes = ['class' => 'wrap'];
    $divChild = new Didom\Element("div", null, $attributes);
    $divChild->appendChild($elm->children());

    $attributes = ['class' => $elm->first('*')->getAttribute('class')];
    $div = new Didom\Element("div", null, $attributes);
    $div->appendChild($divChild);
    $elm->replace($div);
  }
}


/*
 * Animate.css用処理
 * 子要素をspan.wrap要素で囲う
 *
 * @param $elms 対象要素の配列
 */
function wrapChildspanForAnimate($elms) {
  foreach ($elms as $elm) {
    $attributes = ['class' => 'wrap'];
    $spanChild = new Didom\Element("span", null, $attributes);
    $spanChild->appendChild($elm->children());

    $attributes = ['class' => $elm->first('*')->getAttribute('class')];
    $span = new Didom\Element("span", null, $attributes);
    $span->appendChild($spanChild);
    $elm->replace($span);
  }
}


/*
 * replace-viewimg/img/に画像が存在する場合、changeImgSrcToReplaceImg()で各imgのsrc属性を置き換える
 *
 * @param $img 処理する<img>の配列
 */
function replaceImgsSrc($dom) {
  $path = './anima/images/replace-viewimg/img/';
  $replaceImgs = glob($path.'*');
  foreach ($replaceImgs as $src) {
    $src = str_replace($path, '', $src);
    changeImgSrcToReplaceImg($dom->find('img[src*="'.$src.'"]'), $src, $path);
  }
}


/*
 * 指定された<img>のsrcをreplace-viewimg/img/の画像に置き換える
 *
 * @param $img 処理する<img>の配列
 */
function changeImgSrcToReplaceImg($imgs, $src, $path) {
  foreach ($imgs as $img) {
    $img->attr("src", $path.$src);
  }
}
