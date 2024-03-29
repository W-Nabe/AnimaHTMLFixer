<?php
/*
 * © LEANSYSTEM
 * Contact: nabe@lean-system.co.jp
 *
 * Figma・XDでのAnimaの使用方法やプログラムについてのご質問、ご相談などありましたらお気軽にお問い合わせください。
 * 
 * -----
 * 
 * config.csvを読み込む。
 *
 * @return config.csvの連想配列
 * 
 */

require_once("vendor/autoload.php");
use DiDom\Document;
use DiDom\Element;

function loadConfigCsv() {
  $csv = array();
  $handle = fopen(__DIR__ ."/anima/config/config.csv", "r");
  if (!$handle) exit;

  fgetcsv($handle); // タイトル行をスキップ
  while ($data = fgetcsv($handle)) $csv += array($data[0] => array("isuse" => $data[1] == "TRUE", "value" => $data[2]));

  fclose($handle);
  return $csv;
}

/*
 * ルートディレクトリの.htmlの存在をチェックし、存在したら処理を終わる
 */
function checkRootHTMLFile() {
  global $view;
  if (file_exists(__DIR__ ."/".$view.".html")) {
    echo(file_get_contents(__DIR__ ."/".$view.".html", true));
    exit;
  }
}

/*
 * {$view}.htmlの存在をチェックし、ファイル名を返す
 *
 * @return $viewfile
 */
function getViewFileName() {
  global $view;
  if (file_exists(__DIR__ ."/anima/view/".$view.".html")) {
    $viewfile = $view;
    return $viewfile;

  } elseif(file_exists(__DIR__ ."/anima/view/".$view."-sp.html")) {
    $viewfile = $view."-sp";
    return $viewfile;

  } elseif(file_exists(__DIR__ ."/anima/view/".$view."-all-breakpoints.html")) {
    $viewfile = $view."-all-breakpoints";
    return $viewfile;

  } elseif(file_exists(__DIR__ ."/anima/view/".$view."-sp-all-breakpoints.html")) {
    $viewfile = $view."-sp-all-breakpoints";
    return $viewfile;

  } else {
    if (file_exists(__DIR__ ."/404.html")) {
      echo(file_get_contents(__DIR__ ."/404.html", true));
    } else {
      echo "ページが存在しません。";
    }
    exit;
  }
}

/*
 * seotext.csvを読み込む。
 *
 * @return seotext.csvの連想配列
 */
function loadSeoCsv() {
  $csv = array();
  $handle = fopen(__DIR__ ."/anima/config/seo.csv", "r");
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
  $handle = fopen(__DIR__ ."/anima/config/link.csv", "r");
  if (!$handle) exit;

  fgetcsv($handle); // タイトル行をスキップ
  while ($data = fgetcsv($handle)) {
    $csv[$data[0]] = array();
    $csv[$data[0]]["link"] = $data[1];
    $csv[$data[0]]["target"] = $data[2];
    if (isset($data[3]) && $data[3] !== 'undefined') $csv[$data[0]]["class"] = $data[3];
  }

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
  $handle = fopen(__DIR__ ."/anima/config/meta.csv", "r");
  if (!$handle) exit;

  $target = $view;
  fgetcsv($handle); // タイトル行をスキップ
  while ($data = fgetcsv($handle)) {
    if ($data[0] == $target) {
      $csv["key"] = $data[0];
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
 * 現在表示中のMeta情報が存在するか確認
 */
function checkMetaExist($meta) {
  global $view;
  if(!isset($meta["key"])) {
    $target = $view;
    $checkTarget = "<br>チェック対象：".$target;
    echo '<div class="anima-alert">meta情報エラー：titleタグ、metaタグ情報が記載されていません。「anima/config/meta.csv」ファイルにチェック対象の行があるか確認してください。'.$checkTarget.'</div>';
  }
}

/*
 * <body>直下に<div id="loading"></div>を追加する。
 *
 * @param $dom ドキュメントのDOM
 */
function appendLoading($dom) {
  global $theme_uri;
  $dom->first("body")->style()->setProperty('opacity', '0');
  $loading = $dom->createElement("div", null, ["id" => "loading"]);
  $innerHtml = '
  <div class="loading-img-wrap"><img class="loading-img bg" src="'.$theme_uri.'/anima/images/loading.png"
  srcset="'.$theme_uri.'/anima/images/loading.png 1x, '.$theme_uri.'/anima/images/loading@2x.png 2x">
    <div class="progress">
      <div><img class="loading-img" src="'.$theme_uri.'/anima/images/loading.png"
          srcset="'.$theme_uri.'/anima/images/loading.png 1x, '.$theme_uri.'/anima/images/loading@2x.png 2x"></div>
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
  global $theme_uri;
  global $view;
  global $config;
  $spbreakpoint = $config["spbreakpoint"];
  $sp = htmlspecialchars($_COOKIE['windowWidth']) < $spbreakpoint["value"] ? '-sp' : '';
  $designcheck = $dom->createElement("div", null, ["id" => "designcheck"]);
  $designcheck->style()->setProperty('background-image', "url('{$theme_uri}/anima/images/designcheck/{$view}{$sp}.png')");
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
  global $theme_uri;
  foreach ($elemsWithSrc as $elem) $elem->attr('src', "{$theme_uri}/anima/view/{$elem->attr('src')}");
}

/*
 * すべての要素の「href値に「view」を追加する。
 * 要素は「href」属性を持っていなければならない。
 *
 * @param $elmsWithHref 処理する要素の配列
 */
function appendViewPrefixToHref($elemsWithHref)
{
  global $theme_uri;
  foreach ($elemsWithHref as $elem) $elem->attr("href", "{$theme_uri}/anima/view/{$elem->attr('href')}");
}

/*
 * 使用しない「.screen」を削除し、content-pc・content-spを追加する。
 *
 * @param $dom ドキュメントのDOM
 */
function removeUnusedScreen($dom)
{
  global $config;
  $spbreakpoint = $config["spbreakpoint"];
  if (!$spbreakpoint["isuse"]) return;
  
  $spWraps = $dom->find('[class*="sp-wrap"]');
  $pcWraps = $dom->find('[class*="pc-wrap"]');

  if($_COOKIE['windowWidth'] < $spbreakpoint["value"]) {
    // スマホ表示
    if(count($spWraps) > 0) {
      $currentClasses = $spWraps[0]->parent()->getAttribute("class");
      $spWraps[0]->parent()->setAttribute("class", "$currentClasses content-sp");
    }
    if(count($pcWraps) > 0) {
      $className = $pcWraps[0]->parent()->parent()->getAttribute("class");
      if($className == "container-center-horizontal") {
        $dom->first('[class*="pc-wrap"]')->parent()->parent()->remove();
      } else {
        $dom->first('[class*="pc-wrap"]')->parent()->remove();
      }
    }
  } else {
    // パソコン表示
    if(count($pcWraps) > 0) {
      $currentClasses = $pcWraps[0]->parent()->getAttribute("class");
      $pcWraps[0]->parent()->setAttribute("class", "$currentClasses content-pc");
    }
    if(count($spWraps) > 0) {
      $className = $spWraps[0]->parent()->parent()->getAttribute("class");
      if($className == "container-center-horizontal") {
        $dom->first('[class*="sp-wrap"]')->parent()->parent()->remove();
      } else {
        $dom->first('[class*="sp-wrap"]')->parent()->remove();
      }
    }
  }
}

/*
 * container-center-horizontalが無い場合、挿入
 *
 * @param $dom ドキュメントのDOM
 * @return $dom ドキュメントのDOM
 */
function addContainerCenterHorizontal($dom) {
  global $config;
  $domHTML = $dom->format()->html();

  // container-center-horizontalが無い場合、挿入
  if(strpos($domHTML, 'container-center-horizontal') === false) {
    $pattern = '/<input type="hidden" id="anPageName" name="page" value="([^"]+)">/';
    preg_match($pattern, $domHTML, $matches);
    $inputHidden = $matches[0];
    $domHTML = str_replace($inputHidden, $inputHidden.'<div class="container-center-horizontal">', $domHTML);
    $domHTML = str_replace("</body>","</div></body>", $domHTML);

    $dom = new Document($domHTML);
  }
  return $dom;
}

/*
 * view内の不要なタグを削除する。
 *
 * @param $dom ドキュメントのDOM
 * @return string 不要なタグを削除したHTML
 */
function output($dom) {
  global $config;
  $output = $dom->format()->html();

  // bodyタグの直後、body終了タグの直前に挿入
  $bodyTop = file_get_contents("./anima/config/insert_body_top.txt", true);
  $bodyBottom = file_get_contents("./anima/config/insert_body_bottom.txt", true);
  $output = str_replace("</body>",$bodyBottom."</body>", $output);
  preg_match('/<body[^>]*>/', $output, $bodytag);
  $output = str_replace($bodytag[0],$bodytag[0].$bodyTop, $output); 

  // 整形
  $output = substr($output, strlen("<!DOCTYPE html>\n<html>\n  <head>\n"));

  // 不要な要素を削除
  $output = str_replace([
    '<!--<meta name=description content="This site was generated with Anima. www.animaapp.com"/>-->',
    '<!-- <link rel="shortcut icon" type=image/png href="https://animaproject.s3.amazonaws.com/home/favicon.png" /> -->',
    '<link rel="shortcut icon" type="image/png" href="https://animaproject.s3.amazonaws.com/home/favicon.png">'
  ], "", $output);

  return $output;
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
 * IDは要素のクラス文字列「id-***」「id_***」で指定されているものとする。
 * 「***」の部分は、次のアンダースコアまたは、ハイフン、（無ければ末尾まで）を取得し「***」がIDとなる。
 * 「id_***」が複数ある場合は最初のものとする。
 *
 * 対象要素例:
 *   <div class="button original-button-id_foobar_-99999">
 *   <div class="button id_foobarA">
 *   <div class="id-foobarB">
 *   <div class="id-foobarC-09123">
 *   <div class="id-foobar09123">
 * 実行後例:
 *   <div class="button original-button-id_foobar_-99999" id="foobar">
 *   <div class="button id_foobarA" id="foobarA">
 *   <div class="id-foobarB" id="foobarB">
 *   <div class="id-foobarC-09123" id="foobarC">
 *   <div class="id-foobar09123" id="foobar09123">
 *
 * @param $elems 対象要素の配列
 */
function assignId($elems) {
  foreach ($elems as $elem) {
    $classes = $elem->classes()->getAll();
    $class = preg_grep("/(id|id_)?[0-9a-zA-Z_-]+(-|_)?$/", $classes)[0];

    if ($class) {
      preg_match('/id[-_](.*?)(?:[-_]|$)/', $class, $a);
      $idname = $a[1];
      $elem->attr("id", $idname);
    } else {
        $checkTarget = "<br>チェック対象：";
      foreach ($classes as $class) {
        $checkTarget = $checkTarget.$class."<br>"; 
      }
      echo '<div class="anima-alert">命名規則エラー：「id-****」「id_****」のようにidの後にハイフン、またはアンダースコアをつけてください。Animaでエクスポートするデザインのグループ名・レイヤー名を確認してください。'.$checkTarget.'</div>';
    }
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
  $class = preg_grep("/^(h[1-6]|p)-/", $elem->classes()->getAll())[0];

  preg_match("/^(h[1-6]|p)-/", $class, $a);
  return $a[1];
}

/*
 * <img>のalt属性を空にする
 */
function removeAltToImg($imgs) {
  foreach ($imgs as $img) {
    $img->attr("alt", '');
  }
}

/*
 * 指定された<img>に、クラスで指定されたalt属性を持たせる。
 * alt文字列はCSVファイルに指定されているものとする。
 * @param $imgs 対象imgの配列
 */
function appendAltToImgFromClass($imgs) {
  global $seo;

  foreach ($imgs as $img) {
    // <img>のクラスから/txt-.+/を持つクラス文字列を得る。複数ある場合は最初のものとする。
    $classes = $img->classes()->getAll();
    $matched_classes = preg_grep("/(\b|-)seo-.+/", $classes);
    
    if (!empty($matched_classes)) {
      $class = reset($matched_classes);
      preg_match("/(\\b|-)seo-[^\-]+/", $class, $a);
      $key = trim($a[0], "-");
      
      if (isset($seo[$key])) {
        $img->attr("alt", $seo[$key]);
      } else {
        $target = $key;
        $checkTarget = "<br>チェック対象：".$target;
        echo '<div class="anima-alert">SEOテキストエラー：SEOテキスト情報が記載されていません。「anima/config/seo.csv」ファイルにチェック対象の行があるか確認してください。'.$checkTarget.'</div>';
        return;
      }
    }
  }
}

/*
 * 指定されたタグ配下に、テキストを含む隠しspan要素を挿入する。
 * @param $tags 対象tagの配列
 */
function appendSeotxtToTagFromClass($tags, $dom) {
  global $seo;

  foreach ($tags as $tag) {
    $classes = $tag->classes()->getAll();
    $matched_classes = preg_grep("/(\b|-)seo-.+/", $classes);
    
    if (!empty($matched_classes)) {
      $class = reset($matched_classes);
      preg_match("/(\\b|-)seo-[^\-]+/", $class, $a);
      $key = trim($a[0], "-");
      
      if (isset($seo[$key])) {
        $span = $dom->createElement('span', $seo[$key]);
        $span->setAttribute('class', 'hidden-txt');
        $tag->appendChild($span);
      } else {
        $target = $key;
        $checkTarget = "<br>チェック対象：".$target;
        echo '<div class="anima-alert">SEOテキストエラー：SEOテキスト情報が記載されていません。「anima/config/seo.csv」ファイルにチェック対象の行があるか確認してください。'.$checkTarget.'</div>';
        return;
      }
    }
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
    $class = preg_grep("/(\b|-)link-\w+/", $elm->classes()->getAll())[0];

    preg_match("/(\b|-)(link-\w+)/", $class, $a);
    if (isset($link[$a[2]])) {
      if (!$l = $link[$a[2]]) continue;

      $a = new Didom\Element("a", null, ["href" => $l["link"]]);
      $a->appendChild($elm);
      if ($l["target"]) $a->attr("target", $l["target"]);
      if (isset($l["class"]) && $l["class"] !== 'undefined')  $a->attr("class", $l["class"]);
      $elm->replace($a);
    } else {
      $target = $a[2];
      $checkTarget = "<br>チェック対象：".$target;
      echo '<div class="anima-alert">LINKエラー：ハイパーリンク情報が記載されていません。「anima/config/link.csv」ファイルにチェック対象の行があるか確認してください。'.$checkTarget.'</div>';
      return;
    }
  }
}

/*
 * 指定された<img>のsrcをdata-originalにする。
 *
 * @param $img 処理する<img>の配列
 */
function changeImgSrcToDataOriginal($imgs) {
  global $theme_uri;
  foreach ($imgs as $img) {
    $img->attr("data-original", $img->attr("src"));
    $img->attr("src", $theme_uri."/anima/images/dummy.png");
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
    if ($elm->tagName() == "img") {
      $divChild->appendChild($elm);
    } else {
      $divChild->appendChild($elm->children());
    };

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
    if ($elm->tagName() == "img") {
      $spanChild->appendChild($elm);
    } else {
      $spanChild->appendChild($elm->children());
    };

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
  global $theme_uri;
  $path = __DIR__ .'/anima/images/replace-viewimg/img/';
  $replacePath = $theme_uri.'/anima/images/replace-viewimg/img/';
  $replaceImgs = glob($path.'*');
  foreach ($replaceImgs as $src) {
    $src = str_replace($path, '/', $src);
    changeImgSrcToReplaceImg($dom->find('img[src*="'.$src.'"]'), $src, $replacePath);
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


/*
 * replace-viewcss/にCSSが存在する場合、changeCssHrefToReplaceCss()で各linkタグのhref属性を置き換える
 *
 * @param $img 処理する<img>の配列
 */
function replaceCssHref($dom) {
  global $theme_uri;
  $path = __DIR__ .'/anima/css/replace-viewcss/';
  $replacePath = $theme_uri.'/anima/css/replace-viewcss/';
  $replaceCss = glob($path.'*');
  foreach ($replaceCss as $css) {
    $file = str_replace($path, '', $css);
    changeCssHrefToReplaceCss($dom->find('link[href*="'.$file.'"]'), $file, $theme_uri);
  }
}

/*
 * 指定された<link>のhrefをreplace-viewcss/のCSSに置き換える
 *
 * @param $css 処理する<link>の配列
 */
function changeCssHrefToReplaceCss($csses, $file, $path) {
  foreach ($csses as $css) {
    $css->attr("href", $path.'/anima/css/replace-viewcss/'.$file);
  }
}


/*
 * replace-svg/にSVGが存在する場合、changeSvgSrcToReplaceSvg()で各imgタグを置き換える
 * 
 * @param $dom ドキュメントのDOM
*/
function replaceSvg($dom) {
  global $theme_uri;
  $path = __DIR__ .'/anima/images/replace-svg/';
  $replacePath = $theme_uri.'/anima/images/replace-svg/';
  $replaceSvgs = glob($path.'*');
  foreach ($replaceSvgs as $src) {
    $src = str_replace($path, '/', $src);
    if (file_exists($path.$src)) {
      $svg = file_get_contents($path.$src);
      $filename = pathinfo($src, PATHINFO_FILENAME);
      changeSvgSrcToReplaceSvg($dom->find('img[src*="'.$filename.'"]'), $svg);
    }
  }
}

/*
 * 指定された<img>をSVGデータに置き換える
 * 
 * @param $imgs 処理する<img>の配列
 * @param $svg SVGデータ
*/  
function changeSvgSrcToReplaceSvg($imgs, $svg) {
  foreach ($imgs as $img) {
    $svgElm = new Didom\Element('svg');
    $svgElm->setInnerHtml($svg);

    // imgタグの属性をコピー 
    foreach ($img->attributes() as $name => $value) {
      if ($name !== 'src') {
        $svgElm->setAttribute($name, $value);
      }
    }

    // 元のクラス名を取得
    $originalClasses = $img->getAttribute('class');

    // 元のクラス名があればスペースで区切って追加
    if ($originalClasses) {
      $svgElm->setAttribute('class', $originalClasses . ' anim-svg');
    } else {
      $svgElm->setAttribute('class', 'anim-svg');  
    }
    
    $img->replace($svgElm);
  }
}


/**
 * 要素内のテキストを1文字ずつSpanで囲む
 * 
 * @param $elems 対象の要素配列 
 */
function wrapEachTextWithSpan($elems) {
  foreach ($elems as $elem) {
    $index = 1;
  
    ob_start();
    echo($elem->html());
    $html = ob_get_clean();
    
    $dom = new DOMDocument;
    @$dom->loadHTML(mb_convert_encoding($html, 'HTML-ENTITIES', 'UTF-8'));
    $xpath = new DOMXPath($dom);

    foreach ($xpath->query('//text()') as $textNode) {
        $frag = $dom->createDocumentFragment();
        $text = $textNode->nodeValue;
        $length = mb_strlen($text);
        for ($i = 0; $i < $length; $i++) {
            $char = mb_substr($text, $i, 1);
            if (trim($char) !== '') {
                $span = $dom->createElement('span', htmlspecialchars($char));
                $span->setAttribute('class', 'txt');
                $frag->appendChild($span);
            } else {
                $frag->appendChild($dom->createTextNode($char));
            }
        }
        $textNode->parentNode->replaceChild($frag, $textNode);
    }

    $elements = $dom->getElementsByTagName('span');

    foreach ($elements as $element) {
        if ($element->getAttribute('class') == 'txt') {
            $element->setAttribute('class', 'txt-' . $index);
            $index++;
        }
    }

    $html = $dom->saveHTML();
    
    $html = preg_replace('/<!DOCTYPE html PUBLIC "-\/\/W3C\/\/DTD HTML 4.0 Transitional\/\/EN" "http:\/\/www.w3.org\/TR\/REC-html40\/loose.dtd">/', '', $html);
    $html = preg_replace('/<html><body>/', '', $html);
    $html = preg_replace('/<\/body><\/html>/', '', $html);

    $html = new Document($html);
    $elem->replace($html->first("[class=".$elem->getAttribute('class')."]"));
  }
}

/**
 * ストローク付きテキストを複製してデザイン修正
 * 
 * @param $elems 対象の要素配列 
 */
function copyStrokeTxt($elems) {
  foreach ($elems as $elem) {
    $class = $elem->getAttribute('class');
    $pos = strpos($class, "stroketxt");
    if($pos !== false && strpos($class, "px", $pos + 9) !== false) {
      $px = (int)substr($class, $pos + 9, 2) * 2;
      $elem->setAttribute('style', 'z-index: 0; -webkit-text-stroke-width:'.$px.'px;');
    } else {
      $elem->setAttribute('style', 'z-index: 0;');
    }
    $elem->parent()->appendChild($elem);
    $elem->setAttribute('style', '-webkit-text-stroke: 0px; text-shadow: none; z-index: 1;');
  }
}


/*
 * Barba.js用の属性を追加する
 * 
 * @param $dom ドキュメントのDOM
 */
function addBarba($dom) {

  // bodyにdata-barba="wrapper"を追加
  $dom->first("body")->attr("data-barba", "wrapper");

  // .screenの子要素にdata-barba="container"とdata-barba-namespaceを追加
  global $view;
  $containers = $dom->find(".container-center-horizontal");
  foreach ($containers as $container) {
    $container->attr("data-barba", "container");
    $container->attr("data-barba-namespace", $view); 
  }

  appendBarbaInitScript($dom);
  moveLinkToTopOfContainer($dom);

}

/*
 * Barba.jsの初期化スクリプトをbody終了タグの直前に追加する
 *
 * @param $dom ドキュメントのDOM
 */  
function appendBarbaInitScript($dom) {
  global $theme_uri;
  global $config;

  $barbaLoading = $config['barba_loading']["isuse"];

  $scriptBarbaCore = $dom->createElement("script");
  $scriptBarbaCore->attr("src", "https://unpkg.com/@barba/core");

  $scriptBarbaProcess = $dom->createElement("script");

  $processScript = <<<EOD
    // 初回ページ別javascript追加
    var view = document.querySelector('[data-barba="container"]').getAttribute('data-barba-namespace');
    var script = document.createElement('script');
    var barbaLoading = "$barbaLoading";
    script.src = "{$theme_uri}/anima/js/page/"+view+".js";
    script.defer = true;
    document.querySelector('[data-barba="container"]').appendChild(script);
    barba.init({
      prevent: ({ el }) => el.classList && el.classList.contains('no-barba'),
      transitions: [{
        name: 'default-transition',
        leave() {
          /* ---ページ遷移直前の処理--- */
          
          // ローディング画面表示
          if(barbaLoading == "1") {
            document.getElementById('loading').style.opacity = '1';
            document.getElementById('loading').style.transitionProperty = 'opacity';
            document.getElementById('loading').style.transitionDuration = '0ms'
            document.getElementById('loading').style.transitionDelay = '0ms'
          }
          
          // スクロール初期化
          window.scrollTo(0, 0);
        },
        beforeEnter(data) {
          /* ---ページ遷移直前の処理--- */
          const nextEl = data.next.container;
          if (nextEl) { //Just a little check to make sure we don't run this on an error
            console.log(nextEl);
            var linkCode = $(nextEl).find('link[type="text/css"]').clone();
            console.log(linkCode);
            var existingLinks = $('head').find('link[type="text/css"]').filter(function() {
              return $(this).attr('href') === linkCode.attr('href');
            });
            
            if (existingLinks.length === 0) {
              $('head').append(linkCode);
          
              // ローディング画面表示
              document.getElementById('loading').style.opacity = '1';
              document.getElementById('loading').style.transitionProperty = 'opacity';
              document.getElementById('loading').style.transitionDuration = '0ms'
              document.getElementById('loading').style.transitionDelay = '0ms'
            }
          }
        },
        enter() {
          /* ---ページ遷移後の処理--- */
          
          // ファーストビューのアニメーション要素非表示
          $('[class*="firstview"] [class*="anim-"]').css('display', 'none');
          
          setTimeout(function () {
            
            // ページ別javascript追加・中のfunction実行
            view = document.querySelector('[data-barba="container"]').getAttribute('data-barba-namespace');
            script = document.createElement('script');
            script.src = "{$theme_uri}/anima/js/page/"+view+".js";
            script.defer = true;
            document.querySelector('[data-barba="container"]').appendChild(script);
            
            // ローディング画面非表示
            document.getElementById('loading').style.opacity = '0';
            document.getElementById('loading').style.transitionProperty = 'opacity';
            document.getElementById('loading').style.transitionDuration = '200ms'
            
            // 各デバイスの幅を判別・高さ調整
            is_pc = false; // -------PC表示識別
            is_tablet = false; // ---タブレット表示識別
            is_sp = false; // -------スマホ表示識別
            is_smallsp = false; // --375px未満のスマホ表示識別
            checkBreakPoint();
            
            // アニメーションリセット
            var instShowOnScroll = new ShowOnScroll();
            instShowOnScroll.init();
            $('[class*="firstview"] [class*="anim-"]').css('display', '').addClass("anim-showed");
            
            // 画像遅延読み込み初期化
            let checkExistDummyImgInterval = null; 
            function checkExistDummyImg() {
              const dummyImgs = document.querySelectorAll('img[src*="dummy.png"]');
              if (dummyImgs.length > 0) {
                clearInterval(checkExistDummyImgInterval); 
                imgDelayLoad();
                checkExistDummyImgInterval = setInterval(checkExistDummyImg, 100);
              } // else {
                //console.log('No dummy image found. Continuing search...'); 
              //}
            }
            function startCheckExistDummyImg() {
              checkExistDummyImgInterval = setInterval(checkExistDummyImg, 100);
              setTimeout(() => {
                clearInterval(checkExistDummyImgInterval);
              }, 10000); 
            }
            startCheckExistDummyImg();
          }, 250);
        }
      }]
    });  
  EOD;

  $scriptBarbaProcess->setInnerHtml($processScript);

  $body = $dom->find("body")[0];
  $body->appendChild($scriptBarbaCore);
  $body->appendChild($scriptBarbaProcess);

}

/*
 * link要素を.container-center-horizontalの先頭に移動する
 */  
function moveLinkToTopOfContainer($dom) {
  global $view, $theme_uri;

  $container = $dom->first('.container-center-horizontal');
  $link = $dom->first('link[rel="stylesheet"]');
  $link->setAttribute("data-anima-css", $view);
  
  $container->prependChild($link);
  $link->remove(); 

  // viewのCSSがあれば追加
  if (file_exists(__DIR__ ."/anima/css/page/{$view}.css")) {
    $css = $dom->createElement("link");
    $css->attr("rel", "stylesheet");
    $css->attr("type", "text/css");
    $css->attr("data-page-css", $view);
    $css->attr("href", $theme_uri."/anima/css/page/{$view}.css");
    $container->prependChild($css);
  }
}



/*
 * inc-で命名されたclass名配下のコンテンツとincファイルの内容とを置き換える
 * @param $incs 対象inc-クラスの配列
 */
function replaceIncContent($incs) {
  foreach ($incs as $inc) {
    // 対象要素のクラスから/inc-.+/を持つクラス文字列を得る。複数ある場合は最初のものとする。
    $classes = $inc->classes()->getAll();
    $matched_classes = preg_grep("/(\b|-)inc-.+/", $classes);
    
    if (!empty($matched_classes)) {
      $class = reset($matched_classes);
      preg_match("/(\\b|-)inc-[^\-]+/", $class, $a);
      $key = str_replace('inc-', '', trim($a[0], "-"));

      $file = __DIR__ .'/anima/include/'.$key.'.inc';
      if (file_exists($file)) {
        ob_start();
        include($file);
        $append_dom = ob_get_clean();
        $inc->setInnerHtml($append_dom);
        $inc->setAttribute("id", $key);
      }
    }
  }
}