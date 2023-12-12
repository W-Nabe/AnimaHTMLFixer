<?php
/*
 * config.csvからjs作成
 *
 * @author 渡辺大暁 nab@lean-system.co.jp
 */

$config = loadConfigCsv(); // config.csvを読み込む
header('Content-Type: application/x-javascript; charset=utf-8');
?>

const MAX_BREAKPOINT = <?= ($config["maxpcbreakpoint"]["value"]); ?>;  // 最大ウィンドウサイズ
const TABLET_BREAKPOINT = <?= ($config["tabletbreakpoint"]["value"]); ?>; // パソコンとタブレットのブレイクポイント
const SP_BREAKPOINT = <?= ($config["spbreakpoint"]["value"]); ?>; // タブレットとスマホのブレイクポイント
const SMALLSP_BREAKPOINT = <?= ($config["smallspbreakpoint"]["value"]); ?>; // タブレットとスマホのブレイクポイント
const MINSP_BREAKPOINT = <?= ($config["minspbreakpoint"]["value"]); ?>; // 最小スマホサイズ
const RATIO_SP_TABLET = <?= (($config["spbreakpoint"]["value"]) / ($config["tabletbreakpoint"]["value"])); ?>;
const RATIO_MINSP_SMALLSP = <?= (($config["minspbreakpoint"]["value"]) / ($config["smallspbreakpoint"]["value"])); ?>;
const LOADINGSPEED = <?= ($config["loading_speed"]["value"])/1000; ?>; // ロード画面の速さ（ミリ秒）
const ISUSE_SLICK = <?= ($config["jquery_slick"]["isuse"]) ?>; // SLICK使用・不使用
const urlHash = location.hash;  //URLのハッシュ値を取得


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