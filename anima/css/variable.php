<?php
/*
 * config.csvからCSS作成
 *
 * © LEANSYSTEM
 * Contact: nabe@lean-system.co.jp
 *
 * Figma・XDでのAnimaの使用方法やプログラムについてのご質問、ご相談などありましたらお気軽にお問い合わせください。
 * 
 */

$config = loadConfigCsv(); // config.csvを読み込む
header('Content-Type: text/css; charset=utf-8');
?>
@charset "UTF-8";

:root {
  --maxpcbreakpoint: <?= ($config["maxpcbreakpoint"]["value"]); ?>px;
  --tabletbreakpoint: <?= ($config["tabletbreakpoint"]["value"]); ?>px;
  --spbreakpoint: <?= ($config["spbreakpoint"]["value"]); ?>px;
  --smallspbreakpoint: <?= ($config["smallspbreakpoint"]["value"]); ?>px;
  --minspbreakpoint: <?= ($config["minspbreakpoint"]["value"]); ?>px;
  --ratio-sp-tablet: <?= (($config["spbreakpoint"]["value"]) / ($config["tabletbreakpoint"]["value"])); ?>;
  --ratio-minsp-smallsp: <?= (($config["minspbreakpoint"]["value"]) / ($config["smallspbreakpoint"]["value"])); ?>;
  --loading-speed: <?= ($config["loading_speed"]["value"])/1000; ?>s;
  --loading-background: <?= ($config["loading_background"]["value"]); ?>;
}


<?php
/*
 * config.csvを読み込む。
 *
 * @return config.csvの連想配列
 */
function loadConfigCsv() {
  $csv = array();
  $handle = fopen("../config/config.csv", "r");
  if (!$handle) exit;

  fgetcsv($handle); // タイトル行をスキップ
  while ($data = fgetcsv($handle)) $csv += array($data[0] => array('isuse' => $data[1] == "TRUE", 'value' => $data[2]));

  fclose($handle);
  return $csv;
}